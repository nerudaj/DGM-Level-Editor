#pragma once

#include <json.hpp>
#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "LayerProperty.hpp"

class LayerTextureHandler {
public:
	// Get tgui texture image
	virtual tgui::Texture getTextureRect(LayerProperty* prop) const = 0;

	virtual void init(nlohmann::json& config) = 0;

	virtual ~LayerTextureHandler() {}
};

class LayerTextureHandlerTile : public LayerTextureHandler {
private:
	sf::Texture texture;
	dgm::Clip clip;

public:
	sf::Texture& getSfmlTexture() {
		return texture;
	}

	const sf::IntRect& getTextureRect(LayerPropertyTile& prop) const {
		return clip.getFrame(prop.get("id"));
	}

	const sf::Vector2u& getTextureRectSize() const {
		return clip.getFrameSize();
	}

	virtual tgui::Texture getTextureRect(LayerProperty* prop) const override;

	virtual void init(nlohmann::json& config) override;
};