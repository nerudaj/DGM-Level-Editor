#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include <json.hpp>

/**
 *  @brief Class representing drawing brush in editor
 */
class ToolRenderer {
public:
	/**
	 *  @brief Get number of items stored in brush
	 */
	virtual size_t getItemCount() const = 0;

	/**
	 *  @brief Get texture for certain item
	 */
	virtual const sf::Texture& getTextureForItem(size_t index) const = 0;

	/**
	 *  @brief Get clipping rect for texture of certain item
	 */
	virtual const sf::IntRect& getTextureRectForItem(size_t index) const = 0;

	/**
	 *  @brief Get constructed tgui texture of certain item
	 */
	tgui::Texture getTguiTextureForItem(size_t index) const {
		return { getTextureForItem(index), getTextureRectForItem(index) };
	}

	virtual void init(const nlohmann::json& config, const dgm::ResourceManager& resmgr) = 0;
};

/**
 *  @brief Tool for rendering tiles
 */
class ToolRendererTile : public ToolRenderer {
private:
	dgm::Clip clip;
	sf::Texture texture;
	std::vector<bool> meshMap; // whether tile contains collision

public:
	sf::Texture& getTexture() {
		return texture;
	}

	const dgm::Clip& getClip() const {
		return clip;
	}

	const std::vector<bool> &getMeshMap() const {
		return meshMap;
	}

	virtual size_t getItemCount() const override {
		return clip.getFrameCount();
	}

	virtual const sf::Texture& getTextureForItem(size_t index) const override {
		return texture;
	}

	virtual const sf::IntRect& getTextureRectForItem(size_t index) const override {
		return clip.getFrame(index);
	}

	virtual void init(const nlohmann::json& config, const dgm::ResourceManager& resmgr) override;
};

/**
 *  @brief Tool for rendering items
 */
class ToolRendererItem : public ToolRenderer {
private:
	struct ItemData {
		sf::IntRect frameBounds;
		sf::Texture& texture;
	};

	std::vector<ItemData> items;

public:
	virtual size_t getItemCount() const override {
		return items.size();
	}

	virtual const sf::Texture& getTextureForItem(size_t index) const override {
		return items[index].texture;
	}

	virtual const sf::IntRect& getTextureRectForItem(size_t index) const override {
		return items[index].frameBounds;
	}

	virtual void init(const nlohmann::json& config, const dgm::ResourceManager& resmgr) override;
};