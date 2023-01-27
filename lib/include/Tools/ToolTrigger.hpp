#pragma once

#include "include/Tools/ToolInterface.hpp"
#include "include/Tools/ToolWithDragAndSelect.hpp"
#include "include/Commands/CommandQueue.hpp"
#include "include/Utilities/DragContext.hpp"
#include "include/Tools/SidebarUserTrigger.hpp"
#include "include/Utilities/CoordConverter.hpp"

class ToolTriggerProperty : public ToolPropertyInterface
{
public:
	void fillEditDialog(tgui::Panel::Ptr& panel) override;

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
		GC<ShortcutEngineInterface> shortcutEngine,
		tgui::Gui& gui,
		tgui::Theme& theme,
		GC<CommandQueue> commandQueue,
		std::function<sf::Vector2i()> getPenPosition) noexcept
		: ToolWithDragAndSelect(onStateChanged, shortcutEngine)
		, sidebarUser(gui, theme)
		, commandQueue(commandQueue)
		, getPenPosition(getPenPosition)
	{}

public: // PenUserInterface
	void penClicked(const sf::Vector2i& position) override;

public: // ToolInterface
	void buildSidebar() override { sidebarUser.buildSidebar(); }
	void configure(nlohmann::json& config) override;
	void resize(unsigned width, unsigned height) override;
	void shrinkTo(TileRect const& boundingBox) override;
	void saveTo(LevelD& lvd) const override;
	void loadFrom(const LevelD& lvd) override;
	void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;
	ExpectedPropertyPtr getProperty(const sf::Vector2i& penPos) const override;
	void setProperty(const ToolPropertyInterface& prop) override;

	std::optional<GenericObject> getHighlightedObject(const sf::Vector2i& penPos) const override;
	std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const override;

	[[nodiscard]]
	std::optional<TileRect> getBoundingBox() const noexcept override;

protected: // ToolInterface
	void buildCtxMenuInternal(tgui::MenuBar::Ptr&) override;

protected: // ToolWithDragAndSelect
	void penDragCancel(const sf::Vector2i& origin) override;
	std::optional<std::size_t> getObjectIndexFromMousePos(const sf::Vector2i& pos) const final override;
	sf::Vector2i getPositionOfObjectWithIndex(std::size_t index) const final override;
	void selectObjectsInArea(const sf::IntRect& selectedArea) final override;
	void moveSelectedObjectsTo(const sf::Vector2i& point) final override;
	void createMoveCommand(
		const sf::Vector2i& src,
		const sf::Vector2i& dest) final override;
	void createDeleteCommand() final override;

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
	CoordConverter coordConverter;
	sf::Vector2i levelSize;

	// Dependencies
	GC<CommandQueue> commandQueue;
	SidebarUserTrigger sidebarUser;
	std::function<sf::Vector2i()> getPenPosition;
};