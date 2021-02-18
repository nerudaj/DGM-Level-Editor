#pragma once

#include "Tool.hpp"

class ToolTriggerProperty : public ToolProperty {
public:
	// Dìdí se pøes ToolProperty.
	virtual void buildModalSpecifics(tgui::Panel::Ptr& panel) override;
	virtual bool isEmpty() override;
	virtual void clear() override;

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
	sf::RectangleShape rectShape, rectMarker;
	sf::CircleShape circShape, circMarker;

	sf::Vector2u tileSize;
	sf::Vector2i levelSize;

	using PenType = LevelD::Trigger::AreaType;
	sf::Vector2i penDownPos = NULL_VECTOR;
	sf::Vector2i penPos = NULL_VECTOR;
	PenType penType = PenType::Circle;
	
	std::size_t getTriggerFromPosition(const sf::Vector2i& pos) const;

	bool isValidPenPosForDrawing(const sf::Vector2i& pos) const {
		return !(pos.x < 0 || pos.y < 0 || pos.x >= levelSize.x || pos.y >= levelSize.y);
	}

	bool isDrawing(const sf::Vector2i &downPos) const {
		return downPos != NULL_VECTOR;
	}

	/* returns true if pen is down and is being moved */
	bool isPenDragged(const sf::Vector2i &downPos) const {
		return !clickedNotDrawn(downPos);
	}

	bool clickedNotDrawn(const sf::Vector2i& downPos) const {
		return dgm::Math::vectorSize(sf::Vector2f(penPos - downPos)) < DRAW_THRESHOLD;
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