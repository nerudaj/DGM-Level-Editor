#pragma once

#include <filesystem>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

const std::filesystem::path MESH_TEXTURE_PATH("C:/Users/doomi/Documents/GitHub/DGM-Level-Editor/lib/tests/assets/mesh.png");
const sf::Vector2u FRAME_SIZE(32u, 32u);
const sf::Vector2u FRAME_SPACING(0u, 0u);
const sf::IntRect TEXTURE_BOUNDS(0, 0, 64, 64);
