#include <catch.hpp>
#include <include/Utilities/ProcessCreator.hpp>

#include "TestHelpers/TestAssets.hpp"

TEST_CASE("[ProcessCreator]")
{
	ProcessCreator creator;

	SECTION("Correctly executes existing batfile")
	{
		auto result = creator.Exec(Process::PROCESS_PATH.string(), "param1 param2");
		REQUIRE(result.has_value());
	}

	SECTION("Returns error message if batfile doesn't exist")
	{
		auto result = creator.Exec("nonexistent.bat", "param1");

		REQUIRE_FALSE(result.has_value());
		// There are extra whitespaces, so direct comparison is not ideal
		REQUIRE(result.error().find("The system cannot find the file specified.") != std::string::npos);
	}
}
