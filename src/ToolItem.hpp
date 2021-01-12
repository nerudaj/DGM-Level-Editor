#pragma once

#include "Tool.hpp"

class ItemToolProperty : public ImageToolProperty {
	// Dìdí se pøes ImageToolProperty.
	virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) override;

public:
	virtual bool isEmpty() override;
	virtual void clear() override;
};

class ToolItem : public Tool {
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
	ItemToolProperty itemProperty;

	unsigned penValue = 0;
	sf::Vector2i penDownPos;
	sf::Vector2i penPos;

	EditMode editMode = EditMode::ModeDraw;
	bool dragging = false;

	virtual void buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) override;

	void changeEditMode(EditMode mode);

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