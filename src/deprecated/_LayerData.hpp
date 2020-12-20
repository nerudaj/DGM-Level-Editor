#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "LayerProperty.hpp"
#include "LayerTextureHandler.hpp"
#include "BackgroundGrid.hpp"

class LayerData {
protected:
	tgui::Canvas::Ptr& canvas;

public:
	virtual void draw() =0;

	virtual void set(int x, int y, const LayerProperty& prop) = 0;

	virtual const LayerProperty &get(unsigned x, unsigned y) const = 0;

	virtual void resize(unsigned width, unsigned height) = 0;

	virtual void init() = 0;

	LayerData(tgui::Canvas::Ptr& canvas) : canvas(canvas) {}
};

class LayerDataTile : public LayerData {
private:
	BackgroundGrid background;

	LayerTextureHandlerTile& textureHandler;
	std::vector<LayerPropertyTile> data;
	sf::Vector2u dataSize = { 0, 0 };
	sf::Vector2u tileSize;

	int originOffsetX = 0;
	int originOffsetY = 0;

	unsigned getNormalizedIndex(unsigned x, unsigned y) const;

	void centerBackupInResizedData(std::vector<LayerPropertyTile>& backup, unsigned int width, unsigned int height);

	void resizeIfNecessary(int x, int y);

public:
	virtual void draw() override;

	void set(int x, int y, const LayerProperty& prop) override;

	const LayerProperty &get(unsigned x, unsigned y) const;

	void resize(unsigned width, unsigned height) override;

	virtual void init() override;

	LayerDataTile(tgui::Canvas::Ptr& canvas, LayerTextureHandlerTile &handler) : LayerData(canvas), textureHandler(handler) {}
};