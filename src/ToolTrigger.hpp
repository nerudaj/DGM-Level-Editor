#pragma once

#include "Tool.hpp"

class ToolTriggerProperty : public ToolProperty {
public:
	LevelD::Trigger data;
	std::size_t id = -1;

	// Dìdí se pøes ToolProperty.
	virtual void buildModalSpecifics(tgui::Panel::Ptr& panel) override;
	virtual bool isEmpty() override { return id == -1; }
	virtual void clear() override { id = -1; }

	ToolTriggerProperty(tgui::Gui& gui, Tool* parent) : ToolProperty(gui, parent) {
		clear();
	}
};

class ToolTrigger : public Tool {
private:
	const sf::Vector2i NULL_VECTOR = sf::Vector2i(-1, -1);
	const float DRAW_THRESHOLD = 3.f;

	ToolTriggerProperty property = ToolTriggerProperty(gui, this);

	std::vector<LevelD::Trigger> triggers;
	sf::RectangleShape rectShape, rectMarker, selectMarker;
	sf::CircleShape circShape, circMarker;
	sf::Vector2i drawStart;
	bool drawing = false, selecting = false;

	sf::Vector2u tileSize;
	sf::Vector2i levelSize;

	using PenType = LevelD::Trigger::AreaType;
	PenType penType = PenType::Circle;
	
	std::set<std::size_t> selectedItems;

	std::size_t getTriggerFromPosition(const sf::Vector2i& pos) const;

	bool isValidPenPosForDrawing(const sf::Vector2i& pos) const {
		return !(pos.x < 0 || pos.y < 0 || pos.x >= levelSize.x || pos.y >= levelSize.y);
	}
	
	virtual void penClicked(const sf::Vector2i& position) override;
	virtual void penDragStarted(const sf::Vector2i& start) override;
	virtual void penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end) override;
	virtual void penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) override;
	virtual void penDragCancel(const sf::Vector2i& origin) override;

public:
	virtual void buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) override;
	virtual void configure(nlohmann::json& config) override;
	virtual void resize(unsigned width, unsigned height) override;
	virtual void saveTo(LevelD& lvd) override;
	virtual void loadFrom(const LevelD& lvd) override;
	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;
	virtual void penDelete() override;
	virtual ToolProperty& getProperty() override;
	virtual void setProperty(const ToolProperty& prop) override;

	ToolTrigger(tgui::Gui& gui) : Tool(gui) {}
};