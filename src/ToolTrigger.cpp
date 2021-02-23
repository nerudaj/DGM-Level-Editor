#include "ToolTrigger.hpp"
#include "JsonHelper.hpp"
#include "LogConsole.hpp"

std::size_t ToolTrigger::getTriggerFromPosition(const sf::Vector2i& pos) const {
    for (unsigned i = 0; i < triggers.size(); i++) {
        const sf::Vector2i tpos(triggers[i].x, triggers[i].y);
        if (triggers[i].areaType == PenType::Circle) {
            if (dgm::Math::vectorSize(sf::Vector2f(pos - tpos)) < triggers[i].radius) {
                return i;
            }
        }
        else if (triggers[i].areaType == PenType::Rectangle) {
            if (triggers[i].x <= pos.x && pos.x < triggers[i].x + triggers[i].width &&
                triggers[i].y <= pos.y && pos.y < triggers[i].y + triggers[i].height) {
                return i;
            }
        }
    }

    return -1;
}

void ToolTrigger::penClicked(const sf::Vector2i& position) {
    std::size_t id;

    if (!isValidPenPosForDrawing(position)) return;
    if ((id = getTriggerFromPosition(position)) != -1) {
        if (selectedItems.count(id) > 0) selectedItems.erase(id);
        else selectedItems.insert(id);
        return;
    }

    if (drawing) {
        LevelD::Trigger trigger;
        trigger.areaType = penType;

        if (penType == PenType::Circle) {
            trigger.x = drawStart.x;
            trigger.y = drawStart.y;
            trigger.radius = dgm::Math::vectorSize(sf::Vector2f(position - drawStart));
        }
        else if (penType == PenType::Rectangle) {
            trigger.x = std::min(drawStart.x, position.x);
            trigger.y = std::min(drawStart.y, position.y);
            trigger.width = std::abs(position.x - drawStart.x);
            trigger.height = std::abs(position.y - drawStart.y);
        }

        triggers.push_back(trigger);

        Log::write("Trigger added. Nof triggers: " + std::to_string(triggers.size()));
    }  else {
        drawStart = position;
    }

    drawing = !drawing;
}

void ToolTrigger::penDragStarted(const sf::Vector2i& start) {
    selecting = true;
    selectMarker.setPosition(sf::Vector2f(start));
}

void ToolTrigger::penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end) {
    selectMarker.setSize(sf::Vector2f(end - start));
}

void ToolTrigger::penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) {
    // TODO: Select everything between start and end
    selecting = false;
}

void ToolTrigger::penDragCancel(const sf::Vector2i& origin) {
    drawing = false;
    selecting = false;
    selectedItems.clear();
}

void ToolTrigger::buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) {
    const float SIDEBAR_WIDTH = sidebar->getSize().x;
    const float OFFSET = 10.f;
    const float BUTTON_SIZE = SIDEBAR_WIDTH - 2 * OFFSET;

    auto cbtn = tgui::Button::create("Circle");
    cbtn->setRenderer(theme.getRenderer("Button"));
    cbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
    cbtn->setPosition(OFFSET, OFFSET);
    cbtn->connect("clicked", [this, &gui, &theme]() {
        penType = PenType::Circle;
        Tool::buildSidebar(theme); // to properly highlight active button
    });
    cbtn->getRenderer()->setOpacity(penType != PenType::Circle ? 0.25f : 1.f); // highlight
    sidebar->add(cbtn);

    auto rbtn = tgui::Button::create("Rectangle");
    rbtn->setRenderer(theme.getRenderer("Button"));
    rbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
    rbtn->setPosition(OFFSET, 2 * OFFSET + BUTTON_SIZE);
    rbtn->connect("clicked", [this, &gui, &theme]() {
        penType = PenType::Rectangle;
        Tool::buildSidebar(theme); // to properly highlight active button
    });
    rbtn->getRenderer()->setOpacity(penType == PenType::Circle ? 0.25f : 1.f); // highlight
    sidebar->add(rbtn);
}

void ToolTrigger::configure(nlohmann::json& config) {
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

void ToolTrigger::resize(unsigned width, unsigned height) {
    levelSize.x = int(tileSize.x * width);
    levelSize.y = int(tileSize.y * height);
}

void ToolTrigger::saveTo(LevelD& lvd) {
    lvd.triggers = triggers;
}

void ToolTrigger::loadFrom(const LevelD& lvd) {
    resize(lvd.mesh.layerWidth, lvd.mesh.layerHeight);

    triggers = lvd.triggers;

    // Update tags
    for (const auto& trig : triggers) {
        PropertyTag::get().updateTag(trig.tag);
    }
}

void ToolTrigger::drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) {
    const float opacityFactor = opacity / 255.f;

    circShape.setFillColor(sf::Color(64, 255, 0, 128 * opacityFactor));
    circShape.setOutlineColor(sf::Color(64, 255, 0, 255 * opacityFactor));

    rectShape.setFillColor(sf::Color(128, 0, 255, 128 * opacityFactor));
    rectShape.setOutlineColor(sf::Color(128, 0, 255, 255 * opacityFactor));

    for (std::size_t i = 0; i < triggers.size(); i++) {
        auto& trig = triggers[i];
        if (trig.areaType == LevelD::Trigger::AreaType::Circle) {
            circShape.setOrigin(float(trig.radius), float(trig.radius));
            circShape.setPosition(float(trig.x), float(trig.y));
            circShape.setRadius(float(trig.radius));
            canvas->draw(circShape);

            if (selectedItems.count(i) > 0) {
                circMarker.setOrigin(float(trig.radius), float(trig.radius));
                circMarker.setPosition(float(trig.x), float(trig.y));
                circMarker.setRadius(float(trig.radius));
                canvas->draw(circMarker);
            }
        }
        else if (trig.areaType == LevelD::Trigger::AreaType::Rectangle) {
            rectShape.setPosition(float(trig.x), float(trig.y));
            rectShape.setSize({ float(trig.width), float(trig.height) });
            canvas->draw(rectShape);

            if (selectedItems.count(i) > 0) {
                rectMarker.setPosition(float(trig.x), float(trig.y));
                rectMarker.setSize({ float(trig.width), float(trig.height) });
                canvas->draw(rectMarker);
            }
        }
    }

    if (drawing) {
        if (penType == PenType::Circle) {
            const float radius = dgm::Math::vectorSize(sf::Vector2f(getPenPosition() - drawStart));
            circShape.setOrigin(radius, radius);
            circShape.setPosition(sf::Vector2f(drawStart));
            circShape.setRadius(radius);
            canvas->draw(circShape);
        }
        else if (penType == PenType::Rectangle) {
            rectShape.setPosition(sf::Vector2f(drawStart));
            rectShape.setSize(sf::Vector2f(getPenPosition() - drawStart));
            canvas->draw(rectShape);
        }
    }

    if (selecting) {
        canvas->draw(selectMarker);
    }
}

void ToolTrigger::penDelete() {
    for (auto itr = selectedItems.crbegin(); itr != selectedItems.crend(); itr++) {
        triggers.erase(triggers.begin() + *itr);
    }

    selectedItems.clear();
}

ToolProperty& ToolTrigger::getProperty() {
    property.clear();

    property.id = getTriggerFromPosition(Tool::getPenPosition());
    if (property.id == -1) return property;

    property.data = triggers[property.id];
    return property;
}

void ToolTrigger::setProperty(const ToolProperty &) {
    triggers[property.id] = property.data;
}

void ToolTriggerProperty::buildModalSpecifics(tgui::Panel::Ptr& panel) {
    auto dst = tgui::ScrollablePanel::create();
    panel->add(dst);

    constexpr bool DISABLED = false;

    unsigned row = 0;
    addOption(dst, "X coordinate:", "Measured in pixels from top-left corner", data.x, row++);
    addOption(dst, "Y coordinate:", "Measured in pixels from top-left corner", data.y, row++);
    if (data.areaType == LevelD::Trigger::AreaType::Circle) {
        addOption(dst, "Radius:", "Measured in pixels", data.radius, row++);
    }
    else {
        addOption(dst, "Width:", "Measured in pixels", data.width, row++);
        addOption(dst, "Height:", "Measured in pixels", data.height, row++);
    }
    addOption(dst, "Trigger type:", "How the trigger should be executed", data.type, row++);
    addOption(dst, "Tag:", "Value used to group related objects", data.tag, row++, true, true);
    addOption(dst, "Action ID:", "ID of action to execute", data.id, row++);
    addOption(dst, "Parameter 1:", "First param of action", data.a1, row++);
    addOption(dst, "Parameter 2:", "Second param of action", data.a2, row++);
    addOption(dst, "Parameter 3:", "Third param of action", data.a3, row++);
    addOption(dst, "Parameter 4:", "Fourth param of action", data.a4, row++);
    addOption(dst, "Parameter 5:", "Fifth param of action", data.a5, row++);
    addOption(dst, "Metadata:", "Text field for custom data", data.metadata, row++);
}
