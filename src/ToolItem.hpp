#pragma once

#include "Tool.hpp"

class ItemToolProperty : public ImageToolProperty {
	// Dìdí se pøes ImageToolProperty.
	virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) override;

public:
	virtual bool isEmpty() override;
	virtual void clear() override;

	ItemToolProperty(Tool* parent) : ImageToolProperty(parent) {}
};

class ToolItem : public ToolWithSprites {
public:
	enum class EditMode : std::size_t {
		ModeDraw, ModeErase
	};

protected:
	struct ItemRenderData {
		sf::Texture texture;
		sf::IntRect clip;
		sf::Sprite sprite;
	};

	std::vector<LevelD::Thing> items;
	std::vector<ItemRenderData> renderData;
	ItemToolProperty itemProperty = ItemToolProperty(this);

	sf::Vector2i penDownPos;
	sf::Vector2i penPos;

	EditMode editMode = EditMode::ModeDraw;
	bool dragging = false;

	void changeEditMode(EditMode mode);

	virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const override {
		return tgui::Texture(renderData[spriteId].texture, renderData[spriteId].clip);
	}

	virtual unsigned getSpriteCount() const override {
		return renderData.size();
	}

public:
	virtual void configure(nlohmann::json& config) override;

	virtual void resize(unsigned width, unsigned height) override;

	virtual void saveTo(LevelD& lvd) override;

	virtual void loadFrom(const LevelD& lvd) override;

	virtual void drawTo(tgui::Canvas::Ptr& canvas) override;

	virtual void penDown() override;

	virtual void penPosition(const sf::Vector2i& position) override;

	virtual void penUp() override;

	virtual void penCancel() override;

	virtual ToolProperty& getProperty() override;

	virtual void setProperty(const ToolProperty& prop) override;

	virtual void buildCtxMenu(tgui::MenuBar::Ptr& menu) override;
};

namespace std {
	std::string to_string(ToolItem::EditMode mode);
};