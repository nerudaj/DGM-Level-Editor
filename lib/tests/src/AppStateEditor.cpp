#include <catch.hpp>
#include <filesystem>

#include "../include/FileApiMock.hpp"
#include "include/AppStateEditor.hpp"

class AppStateEditorTestable : public AppStateEditor
{
public:
	std::optional<std::string> getNewSavePathTest()
	{
		return getNewSavePath();
	}

	AppStateEditorTestable(dgm::App& app, cfg::Ini& ini, const std::string& rootDir, std::unique_ptr<FileApiInterface> fileApi)
		: AppStateEditor(app, ini, rootDir, std::move(fileApi))
	{}

	~AppStateEditorTestable()
	{
		//AppStateEditor::~AppStateEditor();
	}
};

TEST_CASE("getNewSavePath", "[AppStateEditor]")
{
	dgm::Window window;
	dgm::App app(window);
	cfg::Ini ini;
	auto path = std::filesystem::current_path();
	std::string rootDir = (path / ".." / ".." / "..").generic_string();

	SECTION("Returns nullopt on user cancel")
	{
		auto fileApi = std::make_unique<FileApiMock>();
		fileApi->userCancelled = true;
		auto appStateEditor = AppStateEditorTestable(
			app,
			ini,
			rootDir,
			std::move(fileApi));
		REQUIRE(not appStateEditor.getNewSavePathTest().has_value());
	}

	SECTION("Appends .lvd if not specified")
	{
		auto fileApi = std::make_unique<FileApiMock>();
		fileApi->mockFileName = "name";
		auto appStateEditor = AppStateEditorTestable(
			app,
			ini,
			rootDir,
			std::move(fileApi));
		REQUIRE(appStateEditor.getNewSavePathTest().value() == "name.lvd");
	}

	SECTION("Does nothing if .lvd is specified")
	{
		auto fileApi = std::make_unique<FileApiMock>();
		fileApi->mockFileName = "name.lvd";
		auto appStateEditor = AppStateEditorTestable(
			app,
			ini,
			rootDir,
			std::move(fileApi));
		REQUIRE(appStateEditor.getNewSavePathTest().value() == "name.lvd");
	}
}