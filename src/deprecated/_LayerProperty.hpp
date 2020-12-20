#pragma once

#include <string>
#include <cstdint>

class LayerProperty {
public:
	virtual ~LayerProperty() {}
	virtual void set(const std::string& key, uint32_t value) =0;
	virtual uint32_t get(const std::string& key) = 0;
};

class LayerPropertyTile : public LayerProperty {
private:
	uint16_t tileId = 0;
	bool solid = false;

public:
	virtual void set(const std::string& key, uint32_t value) override;

	virtual uint32_t get(const std::string& key) override;

	LayerPropertyTile() {}
	LayerPropertyTile(uint16_t id, bool solid) : tileId(id), solid(solid) {}
};