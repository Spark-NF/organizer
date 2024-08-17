#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryFile>
#include <catch.h>
#include "profile-loader.h"
#include "profile.h"
#include "rules/rule.h"


TEST_CASE("ProfileLoader")
{
	QJsonObject jsonProfile {
		{ "name", "Test profile" },
		{ "rules", QJsonArray {
			QJsonObject {
				{ "name", "Test rule" },
				{ "shortcut", "A" },
				{ "conditions", QJsonArray { QJsonObject {
					{ "type", "filename" },
					{ "filename", "match" },
					{ "regex", true },
				}}},
				{ "actions", QJsonArray { QJsonObject {
					{ "type", "rename" },
					{ "from", "(.+)" },
					{ "to", "before_\\1" },
				}}},
			}
		}}
	};

	SECTION("Load file")
	{
		SECTION("File not found")
		{
			QSharedPointer<Profile> profile = ProfileLoader::loadFile("does_not_exists.json");
			REQUIRE(profile == nullptr);
		}

		SECTION("Invalid file")
		{
			QTemporaryFile file;
			file.open();
			file.write("invalid_file");
			file.close();

			QSharedPointer<Profile> profile = ProfileLoader::loadFile(file.fileName());
			REQUIRE(profile == nullptr);
		}

		SECTION("Valid file")
		{
			QTemporaryFile file;
			file.open();
			file.write(QJsonDocument(jsonProfile).toJson());
			file.close();

			QSharedPointer<Profile> profile = ProfileLoader::loadFile(file.fileName());
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().count() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}
	}

	SECTION("Load")
	{
		SECTION("Valid")
		{
			QSharedPointer<Profile> profile = ProfileLoader::load(jsonProfile);
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().count() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}

		SECTION("Ignore invalid rules")
		{
			QJsonArray rules = jsonProfile["rules"].toArray();
			rules.append(QJsonObject {
				QJsonObject {
					{ "name", "Another rule" },
					{ "shortcut", "B" },
					{ "conditions", QJsonArray() },
					{ "actions", QJsonArray() },
				}
			});
			jsonProfile["rules"] = rules;

			QSharedPointer<Profile> profile = ProfileLoader::load(jsonProfile);
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().count() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}

		SECTION("Duplicate shortcut")
		{
			QJsonArray rules = jsonProfile["rules"].toArray();
			rules.append(QJsonObject {
				QJsonObject {
					{ "name", "Another rule" },
					{ "shortcut", "A" },
					{ "conditions", QJsonArray { QJsonObject {
						{ "type", "filename" },
						{ "filename", "match" },
						{ "regex", true },
					}}},
					{ "actions", QJsonArray { QJsonObject {
						 { "type", "rename" },
						 { "from", "(.+)" },
						 { "to", "after_\\1" },
					 }}},
				}
			});
			jsonProfile["rules"] = rules;

			QSharedPointer<Profile> profile = ProfileLoader::load(jsonProfile);
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().count() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}
	}
}
