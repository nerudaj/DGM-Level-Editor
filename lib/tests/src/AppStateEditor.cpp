#include <catch.hpp>
#include <filesystem>

#include "../include/FileApiMock.hpp"
#include "../include/EditorMock.hpp"
#include "../include/YesNoCancelDialogMock.hpp"
#include <include/AppStateEditor.hpp>
#include <include/ShortcutEngine.hpp>

class AppStateEditorTestable : public AppStateEditor
{
public:
	std::optional<std::string> getNewSavePathTest()
	{
		return getNewSavePath();
	}

	void mockUnsavedChanges()
	{
		unsavedChanges = true;
	}

	void handleExitTest(YesNoCancelDialogInterface& dialog)
	{
		handleExit(dialog);
	}

	void injectEditorMock(std::unique_ptr<EditorInterface> mock)
	{
		editor = std::move(mock);
	}

	AppStateEditorTestable(
		dgm::App& app,
		cfg::Ini& ini,
		const std::string& rootDir,
		std::unique_ptr<FileApiInterface> fileApi,
		std::unique_ptr<ShortcutEngineInterface> shortcutEngine)
		: AppStateEditor(app, ini, rootDir, std::move(fileApi), std::move(shortcutEngine))
	{
		//editor = std::make_unique<EditorMock>();
	}

	/*	~AppStateEditorTestable()
		{
			//AppStateEditor::~AppStateEditor();
		}*/
};

class DgmAppTestable : public dgm::App
{
public:
	bool exitCalled() const
	{
		return numberOfStatesToPop > 0;
	}

	AppStateEditorTestable& getState()
	{
		return dynamic_cast<AppStateEditorTestable&>(topState());
	}

	DgmAppTestable(dgm::Window& window) : dgm::App(window) {}
};

static std::string getRootPath()
{
	auto path = std::filesystem::current_path();
	return (path / ".." / ".." / "..").generic_string();
}

TEST_CASE("getNewSavePath", "[AppStateEditor]")
{
	dgm::Window window;
	dgm::App app(window);
	cfg::Ini ini;
	std::string rootDir = getRootPath();
	auto fileApiMock = std::make_unique<FileApiMock>();
	auto shortcutEngine = std::make_unique<ShortcutEngine>();

	SECTION("Returns nullopt on user cancel")
	{
		fileApiMock->userCancelled = true;
		auto appStateEditor = AppStateEditorTestable(
			app,
			ini,
			rootDir,
			std::move(fileApiMock),
			std::move(shortcutEngine));
		REQUIRE(not appStateEditor.getNewSavePathTest().has_value());
	}

	SECTION("Appends .lvd if not specified")
	{
		fileApiMock->mockFileName = "name";
		auto appStateEditor = AppStateEditorTestable(
			app,
			ini,
			rootDir,
			std::move(fileApiMock),
			std::move(shortcutEngine));
		REQUIRE(appStateEditor.getNewSavePathTest().value() == "name.lvd");
	}

	SECTION("Does nothing if .lvd is specified")
	{
		fileApiMock->mockFileName = "name.lvd";
		auto appStateEditor = AppStateEditorTestable(
			app,
			ini,
			rootDir,
			std::move(fileApiMock),
			std::move(shortcutEngine));
		REQUIRE(appStateEditor.getNewSavePathTest().value() == "name.lvd");
	}
}

TEST_CASE("handleExit", "[AppStateEditor]")
{
	dgm::Window window;
	DgmAppTestable app(window);
	cfg::Ini ini;
	std::string rootDir = getRootPath();
	auto fileApiMock = std::make_unique<FileApiMock>();
	auto shortcutEngine = std::make_unique<ShortcutEngine>();
	EditorMockState editorMockState;
	auto editorMock = std::make_unique<EditorMock>(&editorMockState);

	SECTION("Asks for confirmation with unsaved changes")
	{
		app.pushState<AppStateEditorTestable>(
			ini,
			rootDir,
			std::move(fileApiMock),
			std::move(shortcutEngine));
		app.getState().injectEditorMock(std::move(editorMock));
		app.getState().mockUnsavedChanges();

		auto dialog = YesNoCancelDialogMock();
		dialog.userConfirmed = true;

		app.getState().handleExitTest(dialog);

		REQUIRE(app.exitCalled());
		REQUIRE(editorMockState.saveToFileCallCounter == 1);
	}

	SECTION("Does not save if user rejects")
	{
		app.pushState<AppStateEditorTestable>(
			ini,
			rootDir,
			std::move(fileApiMock),
			std::move(shortcutEngine));
		app.getState().injectEditorMock(std::move(editorMock));
		app.getState().mockUnsavedChanges();

		auto dialog = YesNoCancelDialogMock();
		dialog.userConfirmed = false;

		app.getState().handleExitTest(dialog);

		REQUIRE(app.exitCalled());
		REQUIRE(editorMockState.saveToFileCallCounter == 0);
	}

	SECTION("Does not exit if user cancels")
	{
		app.pushState<AppStateEditorTestable>(
				ini,
				rootDir,
				std::move(fileApiMock),
				std::move(shortcutEngine));
		app.getState().injectEditorMock(std::move(editorMock));
		app.getState().mockUnsavedChanges();

		auto dialog = YesNoCancelDialogMock();
		dialog.userCancelled = true;

		app.getState().handleExitTest(dialog);

		REQUIRE(not app.exitCalled());
		REQUIRE(editorMockState.saveToFileCallCounter == 0);
	}
}
