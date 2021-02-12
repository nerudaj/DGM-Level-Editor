#include "ToolTrigger.hpp"
#include "JsonHelper.hpp"
#include "LogConsole.hpp"

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

    for (const auto& trig : triggers) {
        if (trig.areaType == LevelD::Trigger::AreaType::Circle) {
            circShape.setPosition(float(trig.x), float(trig.y));
            circShape.setRadius(float(trig.radius));
            canvas->draw(circShape);
        }
        else if (trig.areaType == LevelD::Trigger::AreaType::Rectangle) {
            rectShape.setPosition(float(trig.x), float(trig.y));
            rectShape.setSize({ float(trig.width), float(trig.height) });
            canvas->draw(rectShape);
        }
    }

    if (isDrawing(penDownPos) && !clickedNotDrawn(penDownPos)) {
        if (penType == PenType::Circle) {
            circMarker.setPosition(sf::Vector2f(penDownPos));
            circMarker.setRadius(dgm::Math::vectorSize(sf::Vector2f(penPos - penDownPos)));
            canvas->draw(circMarker);
        }
        else if (penType == PenType::Rectangle) {
            rectMarker.setPosition(sf::Vector2f(penDownPos));
            rectMarker.setSize(sf::Vector2f(penPos - penDownPos));
            canvas->draw(rectMarker);
        }
    }
}

void ToolTrigger::penDown() {
    if (!isValidPenPosForDrawing(penPos)) return;
    penDownPos = penPos;
}

void ToolTrigger::penPosition(const sf::Vector2i& position) {
    penPos = position;
}

void ToolTrigger::penUp() {
    auto downPos = penDownPos;
    penDownPos = NULL_VECTOR;

    if (!isDrawing(downPos)) return;
    if (clickedNotDrawn(downPos)) return;

    LevelD::Trigger trigger;
    trigger.x = downPos.x;
    trigger.y = downPos.y;
    trigger.areaType = penType;

    if (penType == PenType::Circle) {
        trigger.radius = dgm::Math::vectorSize(sf::Vector2f(penPos - downPos));
    }
    else if (penType == PenType::Rectangle) {
        trigger.width = std::abs(penPos.x - downPos.x);
        trigger.height = std::abs(penPos.y - downPos.y);
    }

    triggers.push_back(trigger);

    Log::write("Trigger added. Nof triggers: " + std::to_string(triggers.size()));
}

void ToolTrigger::penCancel() {
    penDownPos = NULL_VECTOR;
}

void ToolTrigger::penDelete() {
    Log::write("ToolTrigger::penDelete: NOT IMPLEMENTED!");
}

ToolProperty& ToolTrigger::getProperty() {
    return property;
}

void ToolTrigger::setProperty(const ToolProperty &) {
}

void ToolTriggerProperty::buildModalSpecifics(tgui::Panel::Ptr& panel) {
    Log::write("ToolTriggerProperty::buildModalSpecifics: NOT IMPLEMENTED!");
}

bool ToolTriggerProperty::isEmpty() {
    Log::write("ToolTriggerProperty::isEmpty: NOT IMPLEMENTED!");
    return true;
}

void ToolTriggerProperty::clear() {
}
