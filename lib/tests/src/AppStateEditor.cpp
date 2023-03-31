#include <catch.hpp>
#include <fakeit.hpp>
#include <filesystem>

#include <include/AppStateEditor.hpp>
#include <include/Shortcuts/ShortcutEngine.hpp>
#include "include/Dialogs/ErrorInfoDialog.hpp"

#include "Stubs/FileApiStub.hpp"
#include "TestHelpers/EditorMock.hpp"
#include "TestHelpers/YesNoCancelDialogMock.hpp"

using namespace fakeit;

class AppStateEditorTestable : public AppStateEditor
{
public:
	std::optional<std::string> getNewSavePathTest()
	{
		return getNewSavePath();
	}

	void mockUnsavedChanges()
	{
		configPath = "dummy";
		unsavedChanges = true;
	}

	void handleExitTest(YesNoCancelDialogInterface& dialog)
	{
		handleExit(dialog);
	}

	void injectEditorMock(Box<EditorInterface> mock)
	{
		editor = std::move(mock);
	}

	AppStateEditorTestable(
		dgm::App& app,
		tgui::Gui& gui,
		tgui::Theme& theme,
		cfg::Ini& ini,
		ProgramOptions options,
		GC<FileApiInterface> fileApi,
		GC<ShortcutEngineInterface> shortcutEngine,
		GC<YesNoCancelDialogInterface> yesNoDialog,
		GC<ErrorInfoDialogInterface> errorDialog)
		: AppStateEditor(
			app,
			gui,
			theme,
			ini,
			options,
			fileApi,
			shortcutEngine,
			yesNoDialog,
			errorDialog)
	{}
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

TEST_CASE("[AppStateEditor]")
{
	dgm::Window window;
	cfg::Ini ini; // Ini must be declared before app to outlive it during destruction
	DgmAppTestable app(window);
	ProgramOptions options{
		.binaryDir = "C:\\dummy",
		.rootDir = getRootPath(),
		.binaryDirHash = "Testing",
	};

	auto fileApiMock = GC<FileApiStub>();
	auto fileApiSpy = Mock<FileApiInterface>(*fileApiMock);

	auto shortcutEngine = GC<ShortcutEngine>();
	EditorMockState editorMockState;
	auto editorMock = Box<EditorMock>(&editorMockState);
	tgui::Gui gui;
	tgui::Theme theme;
	GC<YesNoCancelDialogInterface> yesNoDialog = GC<YesNoCancelDialogMock>();
	GC<ErrorInfoDialogInterface> errorInfoDialog = GC<ErrorInfoDialog>(gui, theme);

	SECTION("handleExit")
	{
		app.pushState<AppStateEditorTestable>(
			gui,
			theme,
			ini,
			options,
			fileApiMock,
			shortcutEngine,
			yesNoDialog,
			errorInfoDialog);

		SECTION("Asks for confirmation with unsaved changes")
		{
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
			app.getState().injectEditorMock(std::move(editorMock));
			app.getState().mockUnsavedChanges();

			auto dialog = YesNoCancelDialogMock();
			dialog.userCancelled = true;

			app.getState().handleExitTest(dialog);

			REQUIRE(not app.exitCalled());
			REQUIRE(editorMockState.saveToFileCallCounter == 0);
		}
	}

	SECTION("getNewSavePath")
	{
		auto appStateEditor = AppStateEditorTestable(
			app,
			gui,
			theme,
			ini,
			options,
			fileApiMock,
			shortcutEngine,
			yesNoDialog,
			errorInfoDialog);

		SECTION("Returns nullopt on user cancel")
		{
			When(Method(fileApiSpy, getSaveFileName))
				.Do([] (const char*) -> std::optional<std::string>
				{
					return {};
				});

			REQUIRE_FALSE(
				appStateEditor
					.getNewSavePathTest()
					.has_value());
		}

		SECTION("Appends .lvd if not specified")
		{
			When(Method(fileApiSpy, getSaveFileName))
				.Do([] (const char*) -> std::optional<std::string>
				{
					return "name";
				});

			REQUIRE(
				appStateEditor
					.getNewSavePathTest()
					.value() == "name.lvd");
		}

		SECTION("Does nothing if .lvd is specified")
		{
			When(Method(fileApiSpy, getSaveFileName))
				.Do([] (const char*) -> std::optional<std::string>
				{
					return "name.lvd";
				});

			REQUIRE(
				appStateEditor
					.getNewSavePathTest()
					.value() == "name.lvd");
		}
	}
}
