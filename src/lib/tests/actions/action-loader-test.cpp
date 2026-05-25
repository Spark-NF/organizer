#include <QJsonArray>
#include <QJsonObject>
#include <catch.h>
#include <catch2/generators/catch_generators.hpp>
#include "actions/action-loader.h"
#include "actions/action.h"
#include "actions/delete-action.h"
#include "actions/hard-link-action.h"
#include "actions/move-action.h"
#include "actions/multiple-action.h"
#include "actions/process-action.h"
#include "actions/rename-action.h"
#include "actions/shortcut-action.h"
#include "actions/symbolic-link-action.h"
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

		SECTION("Hard link action")
		{
			QJsonObject data {
				{ "type", "hardlink" },
				{ "dest", "dir/" },
			};

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<HardLinkAction>(action) != nullptr);
		}

		SECTION("Symbolic link action")
		{
			QJsonObject data {
				{ "type", "symlink" },
				{ "dest", "dir/" },
			};

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<SymbolicLinkAction>(action) != nullptr);
		}

		SECTION("Shortcut action")
		{
			QJsonObject data {
				{ "type", "shortcut" },
				{ "dest", "dir/" },
			};

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<ShortcutAction>(action) != nullptr);
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
			const bool args = GENERATE(false, true);

			DYNAMIC_SECTION((args ? "With args" : "Without args"))
			{
				QJsonObject data {
					{ "type", "process" },
					{ "cmd", "magick" },
					{ "args", (args ? QJsonArray { "{path}" } : QJsonArray {}) },
				};

				QSharedPointer<Action> action = ActionLoader::load(data);
				REQUIRE(action != nullptr);
				REQUIRE(action.dynamicCast<ProcessAction>() != nullptr);
			}
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
