#include "profile-loader.h"
#include "profile.h"
#include "rules/rule.h"
#include <QTemporaryFile>
#include <catch.h>


const QString basicProfile = R"0(
{
    "name": "Test profile",
    "rules": [
        {
            "name": "Test rule",
            "shortcut": "A",
			"conditions": [{
				"type": "filename",
				"filename": "match",
				"regex": true
			}],
            "actions": [{
				"type": "rename",
				"from": "(.+)",
				"to": "before_\\1"
			}]
		}
	]
}
)0";


TEST_CASE("ProfileLoader")
{
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
			file.write(basicProfile.toUtf8());
			file.close();

			QSharedPointer<Profile> profile = ProfileLoader::loadFile(file.fileName());
			REQUIRE(profile != nullptr);

			REQUIRE(profile->name() == "Test profile");
			REQUIRE(profile->rules().count() == 1);
			REQUIRE(profile->rules()[0][0]->name() == "Test rule");
		}
	}
}
