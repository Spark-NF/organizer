#include <QJsonArray>
#include <QJsonObject>
#include <catch.h>
#include "actions/action-loader.h"
#include "actions/action.h"
#include "actions/delete-action.h"
#include "actions/move-action.h"
#include "actions/multiple-action.h"
#include "actions/process-action.h"
#include "actions/rename-action.h"
#include "actions/trash-action.h"


TEST_CASE("ActionLoader")
{
	SECTION("Invalid")
	{
		QSharedPointer<Action> action = ActionLoader::load(QJsonObject());
		REQUIRE(action == nullptr);
	}

	SECTION("Unknown type")
	{
		QJsonObject data {
			{ "type", "unknown" },
		};

		QSharedPointer<Action> action = ActionLoader::load(data);
		REQUIRE(action == nullptr);
	}

	SECTION("Valid")
	{
		SECTION("Rename action")
		{
			QJsonObject data {
				{ "type", "rename" },
				{ "from", "a" },
				{ "to", "b" },
			};

			QSharedPointer<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(action.dynamicCast<RenameAction>() != nullptr);
		}

		SECTION("Move action")
		{
			QJsonObject data {
				{ "type", "move" },
				{ "dest", "dir/" },
			};

			QSharedPointer<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(action.dynamicCast<MoveAction>() != nullptr);
		}

		SECTION("Delete action")
		{
			QJsonObject data {
				{ "type", "delete" },
			};

			QSharedPointer<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(action.dynamicCast<DeleteAction>() != nullptr);
		}

		SECTION("Trash action")
		{
			QJsonObject data {
				{ "type", "trash" },
			};

			QSharedPointer<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(action.dynamicCast<TrashAction>() != nullptr);
		}

		SECTION("Process action")
		{
			QJsonObject data {
				{ "type", "process" },
				{ "cmd", "magick" },
				{ "args", QJsonArray { "{path}" } },
			};

			QSharedPointer<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(action.dynamicCast<ProcessAction>() != nullptr);
		}

		SECTION("Multiple action")
		{
			QJsonObject data {
				{ "type", "multiple" },
				{ "actions", QJsonArray { QJsonObject { { "type", "trash" } } } },
			};

			QSharedPointer<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(action.dynamicCast<MultipleAction>() != nullptr);
		}
	}
}
