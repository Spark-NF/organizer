#include <QFile>
#include <catch.h>
#include "actions/process-action.h"


TEST_CASE("ProcessAction")
{
	SECTION("Not found")
	{
		ProcessAction action("this_program_does_not_exist", {});

		QFile file("test.jpg");
		REQUIRE(action.execute(file) == false);
	}

	SECTION("Execute")
	{
		#if defined(Q_OS_WINDOWS)
			const QString command = "cmd";
			const QStringList args { "/C", "echo test" };
		#else
			const QString command = "sh";
			const QStringList args { "-c", "echo test" };
		#endif

		ProcessAction action(command, args);

		QFile file("test.jpg");
		REQUIRE(action.execute(file) == true);
	}
}
