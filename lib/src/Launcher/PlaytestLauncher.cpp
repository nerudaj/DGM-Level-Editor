#include "include/Launcher/PlaytestLauncher.hpp"
#include "include/Configs/Strings.hpp"

#include <functional>
#include <optional>

PlaytestLauncher::PlaytestLauncher(
	tgui::Gui& gui,
	GC<ShortcutEngineInterface> shortcutEngine,
		const std::filesystem::path& binaryPath,
	const std::string& launchOptions) noexcept
	: gui(gui)
	, shortcutEngine(shortcutEngine)
	, binaryPath(binaryPath)
	, launchOptions(launchOptions)
{}

void PlaytestLauncher::buildContextMenu(tgui::MenuBar::Ptr menu)
{
	menu->addMenu(Strings::Launcher::CTX_MENU_NAME);

	auto addMenuItem = [this, &menu](
		auto&& label,
		std::function<void(void)> callback,
		std::optional<sf::Keyboard::Key> shortcut = {})
	{
		menu->addMenuItem(std::forward<decltype(label)>(label));
		menu->connectMenuItem(
			Strings::Launcher::CTX_MENU_NAME,
			label,
			callback);

		if (shortcut.has_value())
		{
			shortcutEngine->registerShortcut(
				"PlaytestLauncherShortcuts",
				{ false, false, shortcut.value() },
				callback);
		}
	};

	using namespace Strings::Launcher::ContextMenu;

	addMenuItem(
		PLAYTEST,
		[this] { handlePlaytestExecuted(); },
		sf::Keyboard::F5);
	addMenuItem(
		CONFIGURE_LAUNCH,
		[this] { handleConfigureLaunchOptions(); });
}

void PlaytestLauncher::handlePlaytestExecuted()
{}

void PlaytestLauncher::handleConfigureLaunchOptions()
{}
