#include <QFile>
#include <catch.h>
#include "actions/process-action.h"
#include "media.h"


TEST_CASE("ProcessAction")
{
	SECTION("Not found")
	{
		ProcessAction action("this_program_does_not_exist", {});

		Media media("test.jpg");
		REQUIRE(action.execute(media) == false);
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

		Media media("test.jpg");
		REQUIRE(action.execute(media) == true);
	}
}
