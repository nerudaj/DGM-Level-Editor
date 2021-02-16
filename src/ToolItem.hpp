#pragma once

#include "Tool.hpp"

class ItemToolProperty : public ImageToolProperty {
	// Dìdí se pøes ImageToolProperty.
	virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) override;

public:
	std::size_t itemId;
	LevelD::Thing data;
	bool empty = true;

	virtual bool isEmpty() override {
		return empty;
	}

	virtual void clear() override {
		empty = true;
	}

	ItemToolProperty(tgui::Gui &gui, Tool* parent) : ImageToolProperty(gui, parent) {}
};

class ToolItem : public ToolWithSprites {
public:
	enum class EditMode : std::size_t {
		ModeDraw, ModeErase
	};
	const sf::Vector2i NULL_VECTOR = sf::Vector2i(-1, -1);

protected:
	struct ItemRenderData {
		sf::Texture texture;
		tgui::Texture guiTexture;
		sf::IntRect clip;
		sf::Sprite sprite;
	};

	std::vector<LevelD::Thing> items;
	std::vector<ItemRenderData> renderData;
	ItemToolProperty itemProperty = ItemToolProperty(gui, this);
	std::set<unsigned> selectedItems;

	sf::Vector2u tileSize;
	sf::Vector2i levelSize;

	EditMode editMode = EditMode::ModeDraw;
	bool dragging = false;
	bool selecting = false;
	std::size_t draggedItemId = 0;
	sf::Vector2i dragOffset;

	void changeEditMode(EditMode mode);

	bool isValidPenPosForDrawing(const sf::Vector2i& pos) const {
		return !(pos.x < 0 || pos.y < 0 || pos.x >= levelSize.x || pos.y >= levelSize.y);
	}

	virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const override {
		return renderData[spriteId].guiTexture;
	}

	virtual std::size_t getSpriteCount() const override {
		return renderData.size();
	}

	std::size_t getItemFromPosition(const sf::Vector2f &vec) const;

	/*sf::Vector2i getSelectedAreaStart() const {
		return {
			penDownPos.x < penPos.x ? penDownPos.x : penPos.x,
			penDownPos.y < penPos.y ? penDownPos.y : penPos.y
		};
	}

	sf::Vector2i getSelectedAreaSize() const {
		return {
			std::abs(penDownPos.x - penPos.x),
			std::abs(penDownPos.y - penPos.y)
		};
	}*/

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

	virtual void saveTo(LevelD& lvd) override;

	virtual void loadFrom(const LevelD& lvd) override;

	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;

	virtual void penDelete() override;

	virtual ToolProperty& getProperty() override;

	virtual void setProperty(const ToolProperty& prop) override;

	virtual void buildCtxMenu(tgui::MenuBar::Ptr& menu) override;

	ToolItem(tgui::Gui& gui) : ToolWithSprites(gui) {}
};

namespace std {
	std::string to_string(ToolItem::EditMode mode);
};