#include "actions/multiple-action.h"
#include "actions/rename-action.h"
#include <QFileInfo>
#include <catch.h>


TEST_CASE("MultipleAction")
{
	SECTION("Empty")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		MultipleAction action({});
		REQUIRE(action.execute(file) == true);
		REQUIRE(file.remove());
	}

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		MultipleAction action({
			QSharedPointer<RenameAction>::create(QRegularExpression("(.+)"), "first_\\1", false),
			QSharedPointer<RenameAction>::create(QRegularExpression("(.+)"), "second_\\1", false),
		});
		REQUIRE(action.execute(file) == true);
		REQUIRE(QFileInfo(file).fileName() == "second_first_file.bin");
		REQUIRE(file.remove());
	}
}
