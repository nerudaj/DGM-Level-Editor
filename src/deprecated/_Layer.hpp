#pragma once

#include "LayerProperty.hpp"
#include "LayerTextureHandler.hpp"
#include "LayerData.hpp"
#include "LayerTool.hpp"

class Layer {
protected:
	tgui::Canvas::Ptr& canvas;

public:
	virtual void draw() = 0;

	virtual void click(int x, int y) = 0;

	virtual LayerTool& getTool() = 0;

	virtual const LayerTool& getTool() const = 0;

	virtual void init(nlohmann::json& config) = 0;

	virtual void resize(unsigned width, unsigned height) = 0;

	Layer(tgui::Canvas::Ptr& canvas) : canvas(canvas) {}
};

class LayerTile : public Layer {
private:
	LayerTextureHandlerTile textureHandler;
	LayerDataTile data = LayerDataTile(canvas, textureHandler);
	LayerToolTile tool = LayerToolTile(textureHandler);

public:
	virtual void draw() override;

	virtual void click(int x, int y) override;

	virtual LayerTool& getTool() override {
		return tool;
	}

	virtual const LayerTool& getTool() const override {
		return tool;
	}

	virtual void init(nlohmann::json& config) override;

	virtual void resize(unsigned width, unsigned height) override;

	LayerTile(tgui::Canvas::Ptr& canvas) : Layer(canvas) {}
};