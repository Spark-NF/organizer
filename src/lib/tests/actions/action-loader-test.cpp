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
		std::shared_ptr<Action> action = ActionLoader::load(QJsonObject());
		REQUIRE(action == nullptr);
	}

	SECTION("Unknown type")
	{
		QJsonObject data {
			{ "type", "unknown" },
		};

		std::shared_ptr<Action> action = ActionLoader::load(data);
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

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<RenameAction>(action) != nullptr);
		}

		SECTION("Move action")
		{
			QJsonObject data {
				{ "type", "move" },
				{ "dest", "dir/" },
			};

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<MoveAction>(action) != nullptr);
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

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<DeleteAction>(action) != nullptr);
		}

		SECTION("Trash action")
		{
			QJsonObject data {
				{ "type", "trash" },
			};

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<TrashAction>(action) != nullptr);
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

				std::shared_ptr<Action> action = ActionLoader::load(data);
				REQUIRE(action != nullptr);
				REQUIRE(std::dynamic_pointer_cast<ProcessAction>(action) != nullptr);
			}
		}

		SECTION("Multiple action")
		{
			QJsonObject data {
				{ "type", "multiple" },
				{ "actions", QJsonArray { QJsonObject { { "type", "trash" } } } },
			};

			std::shared_ptr<Action> action = ActionLoader::load(data);
			REQUIRE(action != nullptr);
			REQUIRE(std::dynamic_pointer_cast<MultipleAction>(action) != nullptr);
		}
	}
}
