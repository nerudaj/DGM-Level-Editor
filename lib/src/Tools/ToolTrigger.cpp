#include "include/Tools/ToolTrigger.hpp"
#include "include/JsonHelper.hpp"
#include "include/LogConsole.hpp"

/* Helpers */
std::size_t ToolTrigger::getTriggerFromPosition(const sf::Vector2i& pos) const
{
	for (unsigned i = 0; i < triggers.size(); i++)
	{
		const sf::Vector2i tpos(triggers[i].x, triggers[i].y);
		if (triggers[i].areaType == PenType::Circle)
		{
			if (dgm::Math::vectorSize(sf::Vector2f(pos - tpos)) < triggers[i].radius)
			{
				return i;
			}
		}
		else if (triggers[i].areaType == PenType::Rectangle)
		{
			if (int(triggers[i].x) <= pos.x && pos.x < int(triggers[i].x + triggers[i].width) &&
				int(triggers[i].y) <= pos.y && pos.y < int(triggers[i].y + triggers[i].height))
			{
				return i;
			}
		}
	}

	return -1;
}

void ToolTrigger::selectItemsInArea(sf::IntRect& selectedArea)
{
	for (std::size_t i = 0; i < triggers.size(); i++)
	{
		sf::Vector2i itemCenter;
		itemCenter = triggers[i].areaType == PenType::Circle
			? sf::Vector2i(triggers[i].x, triggers[i].y)
			: sf::Vector2i(triggers[i].x + triggers[i].width / 2,
				triggers[i].y + triggers[i].height / 2);

		if (selectedArea.contains(itemCenter))
		{
			selectedItems.insert(i);
		}
	}
}

void ToolTrigger::moveSelectedTriggersTo(const sf::Vector2i& vec)
{
	const sf::Vector2i offset = vec + dragOffset;
	const sf::Vector2i forward = offset - sf::Vector2i(triggers[draggedItemId].x, triggers[draggedItemId].y);

	for (auto& id : selectedItems)
	{
		triggers[id].x = std::clamp<unsigned>(triggers[id].x + forward.x, 0, levelSize.x);
		triggers[id].y = std::clamp<unsigned>(triggers[id].y + forward.y, 0, levelSize.y);
	}
}

void ToolTrigger::updateVisForTrigger(sf::CircleShape& vis, const LevelD::Trigger& trig)
{
	vis.setOrigin(float(trig.radius), float(trig.radius));
	vis.setPosition(float(trig.x), float(trig.y));
	vis.setRadius(float(trig.radius));
}

void ToolTrigger::updateVisForTrigger(sf::RectangleShape& vis, const LevelD::Trigger& trig)
{
	vis.setPosition(float(trig.x), float(trig.y));
	vis.setSize({ float(trig.width), float(trig.height) });
}

/* Build & Draw */
void ToolTrigger::buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme)
{
	const float SIDEBAR_WIDTH = sidebar->getSize().x;
	const float OFFSET = 10.f;
	const float BUTTON_SIZE = SIDEBAR_WIDTH - 2 * OFFSET;

	auto cbtn = tgui::Button::create("Circular\nTrigger");
	cbtn->setTextSize(0);
	cbtn->setRenderer(theme.getRenderer("Button"));
	cbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
	cbtn->setPosition(OFFSET, OFFSET);
	cbtn->connect("clicked", [this, &gui, &theme] ()
 {
	 penType = PenType::Circle;
	Tool::buildSidebar(theme); // to properly highlight active button
	});
	cbtn->getRenderer()->setOpacity(penType != PenType::Circle ? 0.25f : 1.f); // highlight
	sidebar->add(cbtn);

	auto rbtn = tgui::Button::create("Rectangular\nTrigger");
	rbtn->setTextSize(0);
	rbtn->setRenderer(theme.getRenderer("Button"));
	rbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
	rbtn->setPosition(OFFSET, 2 * OFFSET + BUTTON_SIZE);
	rbtn->connect("clicked", [this, &gui, &theme] ()
 {
	 penType = PenType::Rectangle;
	Tool::buildSidebar(theme); // to properly highlight active button
	});
	rbtn->getRenderer()->setOpacity(penType == PenType::Circle ? 0.25f : 1.f); // highlight
	sidebar->add(rbtn);
}

void ToolTrigger::configure(nlohmann::json& config)
{
	triggers.clear();

	tileSize = JsonHelper::arrayToVector2u(config["toolMesh"]["texture"]["tileDimensions"]);

	// This is static, outline and fill colors of shapes are dependent on draw opacity
	circShape.setOutlineThickness(2.f);
	rectShape.setOutlineThickness(2.f);

	circMarker.setOutlineColor(sf::Color::Red);
	circMarker.setOutlineThickness(2.f);
	circMarker.setFillColor(sf::Color::Transparent);

	rectMarker.setOutlineColor(sf::Color::Red);
	rectMarker.setOutlineThickness(2.f);
	rectMarker.setFillColor(sf::Color::Transparent);

	selectMarker.setOutlineColor(sf::Color::Red);
	selectMarker.setOutlineThickness(2.f);
	selectMarker.setFillColor(sf::Color::Transparent);
}

void ToolTrigger::resize(unsigned width, unsigned height)
{
	if (levelSize.x / tileSize.x > width || levelSize.y / tileSize.y > height) return;

	const auto tilesX = levelSize.x / tileSize.x;
	const auto tilesY = levelSize.y / tileSize.y;
	auto offset = (sf::Vector2u(width, height) - sf::Vector2u(tilesX, tilesY)) / 2u;
	offset.x *= tileSize.x;
	offset.y *= tileSize.y;

	for (auto& trigger : triggers)
	{
		trigger.x += offset.x;
		trigger.y += offset.y;
	}

	levelSize.x = int(tileSize.x * width);
	levelSize.y = int(tileSize.y * height);
}

void ToolTrigger::saveTo(LevelD& lvd)
{
	lvd.triggers = triggers;
}

void ToolTrigger::loadFrom(const LevelD& lvd)
{
	resize(lvd.mesh.layerWidth, lvd.mesh.layerHeight);

	triggers = lvd.triggers;

	// Update tags
	for (const auto& trig : triggers)
	{
		PropertyTag::get().updateTag(trig.tag);
	}
}

void ToolTrigger::drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity)
{
	const float opacityFactor = opacity / 255.f;

	circShape.setFillColor(sf::Color(64, 255, 0, uint8_t(128 * opacityFactor)));
	circShape.setOutlineColor(sf::Color(64, 255, 0, uint8_t(255 * opacityFactor)));

	rectShape.setFillColor(sf::Color(128, 0, 255, uint8_t(128 * opacityFactor)));
	rectShape.setOutlineColor(sf::Color(128, 0, 255, uint8_t(255 * opacityFactor)));

	// Render all triggers, highlight selected ones
	for (std::size_t i = 0; i < triggers.size(); i++)
	{
		auto& trig = triggers[i];
		if (trig.areaType == LevelD::Trigger::AreaType::Circle)
		{
			updateVisForTrigger(circShape, trig);
			canvas->draw(circShape);

			if (selectedItems.count(i) > 0)
			{
				updateVisForTrigger(circMarker, trig);
				canvas->draw(circMarker);
			}
		}
		else if (trig.areaType == LevelD::Trigger::AreaType::Rectangle)
		{
			updateVisForTrigger(rectShape, trig);
			canvas->draw(rectShape);

			if (selectedItems.count(i) > 0)
			{
				updateVisForTrigger(rectMarker, trig);
				canvas->draw(rectMarker);
			}
		}
	}

	// If drawing, render preview for drawing
	if (drawing)
	{
		if (penType == PenType::Circle)
		{
			const float radius = dgm::Math::vectorSize(sf::Vector2f(getPenPosition() - drawStart));
			circShape.setOrigin(radius, radius);
			circShape.setPosition(sf::Vector2f(drawStart));
			circShape.setRadius(radius);
			canvas->draw(circShape);
		}
		else if (penType == PenType::Rectangle)
		{
			rectShape.setPosition(sf::Vector2f(drawStart));
			rectShape.setSize(sf::Vector2f(getPenPosition() - drawStart));
			canvas->draw(rectShape);
		}
	}

	if (selecting)
	{
		canvas->draw(selectMarker);
	}
}

sf::Vector2u ToolTrigger::getNormalizedPosition(const LevelD::Trigger& trigger)
{
	if (trigger.areaType == PenType::Rectangle)
		return sf::Vector2u(trigger.x, trigger.y) + sf::Vector2u(trigger.width, trigger.height) / 2u;
	return { trigger.x, trigger.y };
}

/* Pen stuff */
void ToolTrigger::penClicked(const sf::Vector2i& position)
{
	std::size_t id;

	if (!isValidPenPosForDrawing(position)) return;
	if ((id = getTriggerFromPosition(position)) != -1)
	{
		if (selectedItems.count(id) > 0) selectedItems.erase(id);
		else selectedItems.insert(id);
		return;
	}

	if (drawing)
	{
		LevelD::Trigger trigger;
		trigger.areaType = penType;

		if (penType == PenType::Circle)
		{
			trigger.x = drawStart.x;
			trigger.y = drawStart.y;
			trigger.radius = uint16_t(dgm::Math::vectorSize(sf::Vector2f(position - drawStart)));
		}
		else if (penType == PenType::Rectangle)
		{
			trigger.x = std::min(drawStart.x, position.x);
			trigger.y = std::min(drawStart.y, position.y);
			trigger.width = std::abs(position.x - drawStart.x);
			trigger.height = std::abs(position.y - drawStart.y);
		}

		triggers.push_back(trigger);

		Log::write("Trigger added. Nof triggers: " + std::to_string(triggers.size()));
	}
	else
	{
		drawStart = position;
	}

	drawing = !drawing;
}

void ToolTrigger::penDragStarted(const sf::Vector2i& start)
{
	// If mouse was pointing to trigger, then we're dragging
	//   If trigger was already selected, we're moving all selected
	//   If it was not, then we're moving only this one
	// Else we're selecting
	if ((draggedItemId = getTriggerFromPosition(start)) != -1)
	{
		dragging = true;
		if (selectedItems.count(draggedItemId) == 0) selectedItems.clear();
		selectedItems.insert(draggedItemId);
		dragOffset = sf::Vector2i(triggers[draggedItemId].x, triggers[draggedItemId].y) - start;
	}
	else
	{
		selecting = true;
		selectMarker.setPosition(sf::Vector2f(start));
	}
}

void ToolTrigger::penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end)
{
	selectMarker.setSize(sf::Vector2f(end - start));

	if (dragging)
	{
		moveSelectedTriggersTo(end);
	}
}

void ToolTrigger::penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end)
{
	if (selecting)
	{
		selectedItems.clear();
		sf::IntRect selectedArea(
			Helper::minVector(start, end),
			{ std::abs(start.x - end.x), std::abs(start.y - end.y) });
		selectItemsInArea(selectedArea);
	}

	dragging = false;
	selecting = false;
}

void ToolTrigger::penDragCancel(const sf::Vector2i& origin)
{
	drawing = false;
	selecting = false;
	selectedItems.clear();
}

void ToolTrigger::penDelete()
{
	for (auto itr = selectedItems.crbegin(); itr != selectedItems.crend(); itr++)
	{
		triggers.erase(triggers.begin() + *itr);
	}

	selectedItems.clear();
}

/* Properties */
std::unique_ptr<ToolProperty> ToolTrigger::getProperty() const
{
	auto pos = getPenPosition();

	std::size_t trigId = getTriggerFromPosition(pos);
	if (trigId == -1)
		return nullptr;

	auto&& result = std::make_unique<ToolTriggerProperty>();
	result->id = trigId;
	result->data = triggers[trigId];

	return std::move(result);
}

void ToolTrigger::setProperty(const ToolProperty& prop)
{
	auto&& property = dynamic_cast<const ToolTriggerProperty&>(prop);
	triggers[property.id] = property.data;
}

std::optional<GenericObject> ToolTrigger::getHighlightedObject() const
{
	auto&& prop = getProperty();
	if (!prop)
		return {};

	auto&& trigProp = dynamic_cast<ToolTriggerProperty&>(*prop);
	return GenericObject{
		.position = getNormalizedPosition(trigProp.data),
		.tag = trigProp.data.tag
	};
}

std::vector<sf::Vector2u> ToolTrigger::getPositionsOfObjectsWithTag(unsigned tag) const
{
	std::vector<sf::Vector2u> result;
	for (auto&& trigger : triggers)
	{
		if (trigger.tag == tag)
		{
			result.push_back(getNormalizedPosition(trigger));
		}
	}

	return result;
}

void ToolTriggerProperty::buildModalSpecifics(tgui::Gui& gui, tgui::Panel::Ptr& panel)
{
	auto dst = tgui::ScrollablePanel::create();
	panel->add(dst);

	constexpr bool DISABLED = false;

	unsigned row = 0;
	addOption(gui, dst, "X coordinate:", "Measured in pixels from top-left corner", data.x, row++);
	addOption(gui, dst, "Y coordinate:", "Measured in pixels from top-left corner", data.y, row++);
	if (data.areaType == LevelD::Trigger::AreaType::Circle)
	{
		addOption(gui, dst, "Radius:", "Measured in pixels", data.radius, row++);
	}
	else
	{
		addOption(gui, dst, "Width:", "Measured in pixels", data.width, row++);
		addOption(gui, dst, "Height:", "Measured in pixels", data.height, row++);
	}
	addOption(gui, dst, "Trigger type:", "How the trigger should be executed", data.type, row++);
	addOption(gui, dst, "Tag:", "Value used to group related objects", data.tag, row++, true, true);
	addOption(gui, dst, "Action ID:", "ID of action to execute", data.id, row++);
	addOption(gui, dst, "Parameter 1:", "First param of action", data.a1, row++);
	addOption(gui, dst, "Parameter 2:", "Second param of action", data.a2, row++);
	addOption(gui, dst, "Parameter 3:", "Third param of action", data.a3, row++);
	addOption(gui, dst, "Parameter 4:", "Fourth param of action", data.a4, row++);
	addOption(gui, dst, "Parameter 5:", "Fifth param of action", data.a5, row++);
	addOption(dst, "Metadata:", "Text field for custom data", data.metadata, row++);
}
