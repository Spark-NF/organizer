#include <QFile>
#include <catch.h>
#include "actions/process-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"


TEST_CASE("ProcessAction")
{
	RealFilesystem fs;

	SECTION("Not found")
	{
		ProcessAction action("this_program_does_not_exist", {}, 30000);

		Media media("test.jpg");
		REQUIRE(action.execute(media, fs) == false);
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

		ProcessAction action(command, args, 30000);

		Media media("test.jpg");
		REQUIRE(action.execute(media, fs) == true);
	}

	SECTION("Non-zero exit code")
	{
		#if defined(Q_OS_WINDOWS)
			const QString command = "cmd";
			const QStringList args { "/C", "exit 1" };
		#else
			const QString command = "sh";
			const QStringList args { "-c", "exit 1" };
		#endif

		ProcessAction action(command, args, 30000);

		Media media("test.jpg");
		QString error;
		REQUIRE(action.execute(media, fs, &error) == false);
		REQUIRE(error.contains("1"));
	}
}
