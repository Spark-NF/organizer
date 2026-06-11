#include <QFile>
#include <catch.h>
#include "conditions/comparators/glob-comparator.h"
#include "conditions/comparators/regex-comparator.h"
#include "conditions/process-condition.h"
#include "media.h"


TEST_CASE("ProcessCondition")
{
	Media media(QFile("dummy.txt"));

	#if defined(Q_OS_WINDOWS)
		const QString sh = "cmd";
		auto makeArgs = [](const QString &cmd) { return QStringList { "/C", cmd }; };
	#else
		const QString sh = "sh";
		auto makeArgs = [](const QString &cmd) { return QStringList { "-c", cmd }; };
	#endif

	SECTION("Without comparator")
	{
		SECTION("Exit code 0")
		{
			ProcessCondition cond(sh, makeArgs("exit 0"), 5000);
			REQUIRE(cond.match(media) == true);
		}

		SECTION("Non-zero exit code")
		{
			ProcessCondition cond(sh, makeArgs("exit 1"), 5000);
			REQUIRE(cond.match(media) == false);
		}
	}

	SECTION("With comparator")
	{
		SECTION("Match")
		{
			auto comparator = std::make_shared<GlobComparator>("yes");
			ProcessCondition cond(sh, makeArgs("printf yes"), 5000, comparator);
			REQUIRE(cond.match(media) == true);
		}

		SECTION("No match")
		{
			auto comparator = std::make_shared<GlobComparator>("yes");
			ProcessCondition cond(sh, makeArgs("printf no"), 5000, comparator);
			REQUIRE(cond.match(media) == false);
		}

		SECTION("Stdout is trimmed")
		{
			auto comparator = std::make_shared<GlobComparator>("yes");
			ProcessCondition cond(sh, makeArgs("echo yes"), 5000, comparator);
			REQUIRE(cond.match(media) == true);
		}

		SECTION("Exit code is irrelevant")
		{
			auto comparator = std::make_shared<GlobComparator>("yes");
			#if defined(Q_OS_WINDOWS)
				ProcessCondition cond(sh, makeArgs("echo yes & exit /b 1"), 5000, comparator);
			#else
				ProcessCondition cond(sh, makeArgs("printf yes; exit 1"), 5000, comparator);
			#endif
			REQUIRE(cond.match(media) == true);
		}
	}
}
