#pragma once

#include <DGM\dgm.hpp>

class AppStateIngame : public dgm::AppState {
private:
	const dgm::ResourceManager &resmgr;

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateIngame(const dgm::ResourceManager &resmgr) : resmgr(resmgr) {}
};