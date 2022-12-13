#pragma once

#include "Tool.hpp"

class ItemToolProperty : public ImageToolProperty
{
	// Dìdí se pøes ImageToolProperty.
	virtual void buildModalSpecifics(tgui::Gui& gui, tgui::ScrollablePanel::Ptr& panel) override;

public:
	std::size_t itemId;
	LevelD::Thing data;
};

class ToolItem : public ToolWithSprites
{
public:
	enum class EditMode : std::size_t
	{
		ModeDraw, ModeErase
	};
	const sf::Vector2i NULL_VECTOR = sf::Vector2i(-1, -1);

protected:
	struct ItemRenderData
	{
		sf::Texture texture;
		tgui::Texture guiTexture;
		sf::IntRect clip;
		sf::Sprite sprite;
	};

	std::vector<LevelD::Thing> items;
	std::vector<ItemRenderData> renderData;
	std::set<std::size_t> selectedItems;
	sf::RectangleShape selectRect;

	sf::Vector2u tileSize;
	sf::Vector2i levelSize;

	EditMode editMode = EditMode::ModeDraw;
	bool dragging = false;
	bool selecting = false;
	std::size_t draggedItemId = 0;
	sf::Vector2i dragOffset;

	void changeEditMode(EditMode mode);

	bool isValidPenPosForDrawing(const sf::Vector2i& pos) const
	{
		return !(pos.x < 0 || pos.y < 0 || pos.x >= levelSize.x || pos.y >= levelSize.y);
	}

	virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const override
	{
		return renderData[spriteId].guiTexture;
	}

	virtual std::size_t getSpriteCount() const override
	{
		return renderData.size();
	}

	std::size_t getItemFromPosition(const sf::Vector2f& vec) const;

	void selectItemsInArea(sf::IntRect& selectedArea);

	void moveSelectedItemsTo(const sf::Vector2i& vec);

	virtual void penClicked(const sf::Vector2i& position) override;
	virtual void penDragStarted(const sf::Vector2i& start) override;
	virtual void penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end) override;
	virtual void penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) override;
	virtual void penDragCancel(const sf::Vector2i& origin) override;

public:
	virtual void configure(nlohmann::json& config) override;

	virtual void resize(unsigned width, unsigned height) override;

	virtual void saveTo(LevelD& lvd) const override;

	virtual void loadFrom(const LevelD& lvd) override;

	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;

	virtual void penDelete() override;

	virtual std::unique_ptr<ToolProperty> getProperty() const override;

	virtual void setProperty(const ToolProperty& prop) override;

	virtual void buildCtxMenu(tgui::MenuBar::Ptr& menu) override;

	virtual std::optional<GenericObject> getHighlightedObject() const override;
	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const override;

	ToolItem(
		tgui::Gui& gui,
		std::function<void(void)> onStateChanged,
		CommandQueue& commandQueue,
		ShortcutEngineInterface& shortcutEngine)
		: ToolWithSprites(gui, onStateChanged, commandQueue, shortcutEngine)
	{}
};

namespace std
{
	std::string to_string(ToolItem::EditMode mode);
};