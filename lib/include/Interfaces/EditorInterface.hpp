#pragma once

#include "include/Editor/EditorState.hpp"
#include <LevelD.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <filesystem>

class EditorInterface
{
public:
    virtual void draw() = 0;

    virtual void init(
        unsigned levelWidth,
        unsigned levelHeight,
        const std::filesystem::path& configPath) = 0;

    virtual void
    handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) = 0;

    [[nodiscard]] virtual LevelD save() const = 0;

    virtual void loadFrom(
        const LevelD& lvd,
        const std::filesystem::path& pathToJsonConfig,
        bool skipInit = false) = 0;

    virtual void switchTool(EditorState state) = 0;

    virtual void resizeDialog() = 0;

    virtual void
    resize(unsigned width, unsigned height, bool isTranslationDisabled) = 0;

    virtual void shrinkToFit() = 0;

    virtual ~EditorInterface() = default;
};
