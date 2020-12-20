#pragma once

#include "LayerProperty.hpp"
#include "LayerTextureHandler.hpp"
#include <TGUI/TGUI.hpp>
#include <json.hpp>

class LayerTool {
protected:
	const LayerTextureHandler& textureHandler;

	virtual unsigned getPropertyCount() const = 0;

	virtual tgui::Texture getPropertyThumbnail(unsigned index) const = 0;

public:
	virtual const LayerProperty &getSelectedProperty() const = 0;

	virtual void selectProperty(unsigned index) = 0;

	virtual void init(const nlohmann::json& config) = 0;

	virtual void buildPropertySelectionModal(tgui::Gui& gui);

	LayerTool(const LayerTextureHandler& textureHandler) : textureHandler(textureHandler) {}
};

class LayerToolTile : public LayerTool {
private:
	std::vector<LayerPropertyTile> propDb;
	unsigned selectedIndex;

protected:
	virtual unsigned getPropertyCount() const override {
		return propDb.size();
	}

	virtual tgui::Texture getPropertyThumbnail(unsigned index) const {
		LayerPropertyTile searchProp(index, false);
		return textureHandler.getTextureRect(&searchProp);
	}

public:
	virtual const LayerProperty& getSelectedProperty() const override {
		return propDb[selectedIndex];
	}

	virtual void selectProperty(unsigned index) override {
		selectedIndex = index;
	}

	virtual void init(const nlohmann::json& config) override;

	LayerToolTile(const LayerTextureHandler& textureHandler) : LayerTool(textureHandler) {}
};