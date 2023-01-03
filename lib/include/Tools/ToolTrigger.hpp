#pragma once

#include "include/Tools/ToolInterface.hpp"
#include "include/Tools/ToolWithDragAndSelect.hpp"
#include "include/Commands/CommandQueue.hpp"
#include "include/Utilities/DragContext.hpp"
#include "include/Tools/SidebarUserTrigger.hpp"

class ToolTriggerProperty : public ToolProperty
{
	virtual void buildModalSpecifics(tgui::Gui& gui, tgui::Panel::Ptr& panel) override;

public:
	LevelD::Trigger data;
	std::size_t id;
};

class ToolTrigger final : public ToolWithDragAndSelect
{
private:
	using super = ToolWithDragAndSelect;

public:
	[[nodiscard]]
	ToolTrigger(
		std::function<void(void)> onStateChanged,
		ShortcutEngineInterface& shortcutEngine,
		tgui::Gui& gui,
		tgui::Theme& theme,
		CommandQueue& commandQueue,
		std::function<sf::Vector2i()> getPenPosition) noexcept
		: ToolWithDragAndSelect(onStateChanged, shortcutEngine)
		, sidebarUser(gui, theme)
		, commandQueue(commandQueue)
		, getPenPosition(getPenPosition)
	{}

public: // PenUserInterface
	virtual void penClicked(const sf::Vector2i& position) override;

public: // ToolInterface
	void buildSidebar() override { sidebarUser.buildSidebar(); }
	virtual void configure(nlohmann::json& config) override;
	virtual void resize(unsigned width, unsigned height) override;
	virtual void saveTo(LevelD& lvd) const override;
	virtual void loadFrom(const LevelD& lvd) override;
	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;
	virtual ExpectedPropertyPtr getProperty(const sf::Vector2i& penPos) const override;
	virtual void setProperty(const ToolProperty& prop) override;

	virtual std::optional<GenericObject> getHighlightedObject(const sf::Vector2i& penPos) const override;
	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const override;

protected: // ToolInterface
	void buildCtxMenuInternal(tgui::MenuBar::Ptr&) override {}

protected: // ToolWithDragAndSelect
	virtual void penDragCancel(const sf::Vector2i& origin) override;
	virtual std::optional<std::size_t> getObjectIndexFromMousePos(const sf::Vector2i& pos) const final override;
	virtual sf::Vector2i getPositionOfObjectWithIndex(std::size_t index) const final override;
	virtual void selectObjectsInArea(const sf::IntRect& selectedArea) final override;
	virtual void moveSelectedObjectsTo(const sf::Vector2i& point) final override;
	virtual void createMoveCommand(
		const sf::Vector2i& src,
		const sf::Vector2i& dest) final override;
	virtual void createDeleteCommand() final override;

private:
	static sf::Vector2u getNormalizedPosition(const LevelD::Trigger& trigger);
	void updateVisForTrigger(sf::CircleShape& vis, const LevelD::Trigger& trigger);
	void updateVisForTrigger(sf::RectangleShape& vis, const LevelD::Trigger& trigger);

	[[nodiscard]]
	constexpr bool isValidPenPosForDrawing(const sf::Vector2i& pos) const noexcept
	{
		return !(pos.x < 0 || pos.y < 0 || pos.x >= levelSize.x || pos.y >= levelSize.y);
	}

private:
	using PenType = LevelD::Trigger::AreaType;

	// Visualization objects
	sf::RectangleShape rectShape, rectMarker;
	sf::CircleShape circShape, circMarker;

	// Pen interactions
	sf::Vector2i drawStart;
	bool drawing = false;

	// Actual data
	std::vector<LevelD::Trigger> triggers;
	sf::Vector2u tileSize;
	sf::Vector2i levelSize;

	// Dependencies
	CommandQueue& commandQueue;
	SidebarUserTrigger sidebarUser;
	std::function<sf::Vector2i()> getPenPosition;
};