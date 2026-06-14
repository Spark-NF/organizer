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
					{ "data", "filename" },
					{ "regex", "match" },
				}}},
				{ "actions", QJsonArray { QJsonObject {
					{ "type", "rename" },
					{ "dest", "before_{filename}" },
				}}},
			}
		}}
	};

	SECTION("Load file")
	{
		SECTION("File not found")
		{
			std::shared_ptr<Profile> profile = ProfileLoader::loadFile("does_not_exists.json");
			REQUIRE(profile == nullptr);
		}

		SECTION("Invalid file")
		{
			QTemporaryFile file;
			file.open();
			file.write("invalid_file");
			file.close();

			std::shared_ptr<Profile> profile = ProfileLoader::loadFile(file.fileName());
			REQUIRE(profile == nullptr);
		}

		SECTION("Valid file")
		{
			QTemporaryFile file;
			file.open();
			file.write(QJsonDocument(jsonProfile).toJson());
			file.close();

			std::shared_ptr<Profile> profile = ProfileLoader::loadFile(file.fileName());
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().size() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}
	}

	SECTION("Load")
	{
		SECTION("Valid")
		{
			std::shared_ptr<Profile> profile = ProfileLoader::load(jsonProfile);
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().size() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}

		SECTION("Array of array for rules")
		{
			QJsonObject arrayOfArrayProfile {
				{ "name", "Test profile" },
				{ "rules", QJsonArray { jsonProfile["rules"] } }
			};

			std::shared_ptr<Profile> profile = ProfileLoader::load(arrayOfArrayProfile);
			REQUIRE(profile != nullptr);

			REQUIRE(profile->rules().size() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}

		SECTION("Unsupported version")
		{
			QJsonObject versionedProfile {
				{ "version", 999 },
				{ "name", "Test" },
				{ "rules", QJsonArray() },
			};

			QString error;
			const auto profile = ProfileLoader::load(versionedProfile, &error);
			REQUIRE(profile == nullptr);
			REQUIRE(error.contains("Unsupported profile version"));
		}

		SECTION("Bad rule fails the profile")
		{
			QJsonObject badProfile {
				{ "name", "Test" },
				{ "rules", QJsonArray {
					QJsonObject {
						{ "name", "Good rule" },
						{ "conditions", QJsonArray { QJsonObject {{ "data", "filename" }, { "glob", "*.jpg" }}}},
						{ "actions", QJsonArray { QJsonObject {{ "type", "rename" }, { "dest", "{filename}" }}}},
					},
					QJsonObject {
						{ "name", "Bad rule" },
						{ "conditions", QJsonArray { QJsonObject {{ "data", "filename" }, { "glob", "*.jpg" }}}},
						{ "actions", QJsonArray { QJsonObject {{ "type", "unknown_type" }}}},
					},
				}}
			};

			QString error;
			const auto profile = ProfileLoader::load(badProfile, &error);
			REQUIRE(profile == nullptr);
			REQUIRE(!error.isEmpty());
		}

		SECTION("Error message includes rule name")
		{
			QJsonObject badProfile {
				{ "name", "Test" },
				{ "rules", QJsonArray {
					QJsonObject {
						{ "name", "My Rule" },
						{ "conditions", QJsonArray {}},
						{ "actions", QJsonArray { QJsonObject {{ "type", "unknown_type" }}}},
					},
				}}
			};

			QString error;
			ProfileLoader::load(badProfile, &error);
			REQUIRE(error.contains("My Rule"));
		}

		SECTION("Duplicate shortcut")
		{
			QJsonArray rules = jsonProfile["rules"].toArray();
			rules.append(QJsonObject {
				QJsonObject {
					{ "name", "Another rule" },
					{ "shortcut", "A" },
					{ "conditions", QJsonArray { QJsonObject {
						{ "data", "filename" },
						{ "regex", "match" },
					}}},
					{ "actions", QJsonArray { QJsonObject {
						 { "type", "rename" },
						 { "dest", "after_{filename}" },
					 }}},
				}
			});
			jsonProfile["rules"] = rules;

			std::shared_ptr<Profile> profile = ProfileLoader::load(jsonProfile);
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().size() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}
	}
}
