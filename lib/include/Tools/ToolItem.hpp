#pragma once

#include "include/Tools/ToolInterface.hpp"
#include "include/Tools/ToolWithDragAndSelect.hpp"
#include "include/Tools/SidebarUserItem.hpp"

class ItemToolProperty : public ImageToolProperty
{
	virtual void buildModalSpecifics(tgui::Gui& gui, tgui::ScrollablePanel::Ptr& panel) override;

public:
	std::size_t itemId;
	LevelD::Thing data;
};

class ToolItem final : public ToolWithDragAndSelect
{
public:
	enum class EditMode : std::size_t
	{
		ModeDraw, ModeErase
	};
	const sf::Vector2i NULL_VECTOR = sf::Vector2i(-1, -1);

public:
	[[nodiscard]]
	ToolItem(
		std::function<void(void)> onStateChanged,
		ShortcutEngineInterface& shortcutEngine,
		tgui::Gui& gui,
		tgui::Theme& theme,
		CommandQueue& commandQueue) noexcept
		: ToolWithDragAndSelect(onStateChanged, shortcutEngine)
		, sidebarUser(gui, theme)
		, commandQueue(commandQueue)
	{}

public: // PenUserInterface
	void penClicked(const sf::Vector2i& position) override;

public: // ToolInterface
	void buildSidebar() override { sidebarUser.buildSidebar(); }

	void configure(nlohmann::json& config) override;

	void resize(unsigned width, unsigned height) override;

	void saveTo(LevelD& lvd) const override;

	void loadFrom(const LevelD& lvd) override;

	void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;

	ExpectedPropertyPtr getProperty(const sf::Vector2i& penPos) const override;

	void setProperty(const ToolProperty& prop) override;

	std::optional<GenericObject> getHighlightedObject(const sf::Vector2i& penPos) const override;
	std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const override;

protected: // ToolInterface
	void buildCtxMenuInternal(tgui::MenuBar::Ptr& menu) override;

protected: // ToolWithDragAndSelect
	virtual std::optional<std::size_t> getObjectIndexFromMousePos(const sf::Vector2i& pos) const final override;
	virtual sf::Vector2i getPositionOfObjectWithIndex(std::size_t index) const final override;
	virtual void selectObjectsInArea(const sf::IntRect& selectedArea) final override;
	virtual void moveSelectedObjectsTo(const sf::Vector2i& point) final override;
	virtual void createMoveCommand(
		const sf::Vector2i& src,
		const sf::Vector2i& dest) final override;
	virtual void createDeleteCommand() final override;

protected:
	void changeEditMode(EditMode mode);

	bool isValidPenPosForDrawing(const sf::Vector2i& pos) const
	{
		return !(pos.x < 0 || pos.y < 0 || pos.x >= levelSize.x || pos.y >= levelSize.y);
	}

private:
	std::vector<LevelD::Thing> items;
	sf::Vector2u tileSize;
	sf::Vector2i levelSize;
	EditMode editMode = EditMode::ModeDraw;
	SidebarUserItem sidebarUser;
	CommandQueue& commandQueue;

};

namespace std
{
	std::string to_string(ToolItem::EditMode mode);
};