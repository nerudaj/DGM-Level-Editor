#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include <json.hpp>
#include <LevelD.hpp>

#include <optional>

#include "ToolProperty.hpp"
#include "include/Commands/CommandQueue.hpp"
#include "include/Shortcuts/ShortcutEngineInterface.hpp"
#include "include/Tools/PenUserInterface.hpp"

struct GenericObject
{
	sf::Vector2u position;
	unsigned tag;
};

class ToolInterface : public PenUserInterface
{
public:
	[[nodiscard]]
	ToolInterface(
		std::function<void(void)> onStateChanged,
		ShortcutEngineInterface& shortcutEngine) noexcept
		: onStateChangedCallback(onStateChanged)
		, shortcutEngine(shortcutEngine)
	{}
	virtual ~ToolInterface() = default;

public: // Non virtual interface
	void buildCtxMenu(tgui::MenuBar::Ptr& menu);
	void destroyCtxMenu(tgui::MenuBar::Ptr& menu);

public: // Public virtual interface
	virtual void buildSidebar() = 0;

	virtual void configure(nlohmann::json& config) = 0;

	virtual void resize(unsigned width, unsigned height) = 0;

	virtual void saveTo(LevelD& lvd) const = 0;
	virtual void loadFrom(const LevelD& lvd) = 0;

	virtual void drawTo(
		tgui::Canvas::Ptr& canvas,
		uint8_t opacity) = 0;

	// Returns nullptr if no property can be returned
	[[nodiscard]]
	virtual std::unique_ptr<ToolProperty> getProperty(const sf::Vector2i& penPos) const = 0;
	virtual void setProperty(const ToolProperty& prop) = 0;

	[[nodiscard]]
	virtual std::optional<GenericObject> getHighlightedObject(const sf::Vector2i& penPos) const = 0;

	[[nodiscard]]
	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const = 0;

protected: // Protected non-virtual interface
	void addCtxMenuItem(
		tgui::MenuBar::Ptr& menu,
		const std::string& label,
		std::function<void(void)> callback,
		sf::Keyboard::Key shortcutKey);

	void signalStateChanged()
	{
		onStateChangedCallback();
	}

protected: // Protected virtual interface
	virtual void buildCtxMenuInternal(tgui::MenuBar::Ptr& menu) = 0;

protected:
	std::function<void(void)> onStateChangedCallback;
	ShortcutEngineInterface& shortcutEngine;
};

template<class T>
concept DerivedFromTool = std::derived_from<T, ToolInterface>;

namespace Helper
{
	sf::Vector2i minVector(const sf::Vector2i& a, const sf::Vector2i& b);

	sf::Vector2i maxVector(const sf::Vector2i& a, const sf::Vector2i& b);
}
