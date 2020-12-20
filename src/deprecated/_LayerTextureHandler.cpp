#include "LayerTextureHandler.hpp"
#include "JsonHelper.hpp"

tgui::Texture LayerTextureHandlerTile::getTextureRect(LayerProperty* prop) const {
	return tgui::Texture(texture, clip.getFrame(prop->get("id")));
}

void LayerTextureHandlerTile::init(nlohmann::json& config) {
	texture.loadFromFile(config["layerTile"]["texture"]["path"]);

	sf::Vector2u tileDims = JsonHelper::arrayToVector2u(config["layerTile"]["texture"]["tileDimensions"]);
	sf::Vector2u tileOffs = JsonHelper::arrayToVector2u(config["layerTile"]["texture"]["tileOffsets"]);
	sf::IntRect  bounds = JsonHelper::arrayToIntRect(config["layerTile"]["texture"]["boundaries"]);

	clip = dgm::Clip(tileDims, bounds, 0, tileOffs);
}
