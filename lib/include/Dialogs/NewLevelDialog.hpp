#pragma once

#include <DGM/dgm.hpp>
#include <Config.hpp>
#include "DialogBase.hpp"

/**
 *  \brief Modal window for creating new level
 *
 *  User should be able to define initial level width, height
 *  and he is also required to provide path to config file.
 *
 *  Programmer first has to open the modal window. If the selection is
 *  made and modal is confirmed, associated callback is fired. Then the
 *  getter functions can be used. Modal is closed automatically.
 */
class NewLevelDialog final : public DialogInterface
{
private:
	cfg::Ini& ini;

private:
	virtual void customOpenCode() override;

public:
	[[nodiscard]]
	unsigned getLevelWidth() const;

	[[nodiscard]]
	unsigned getLevelHeight() const;

	[[nodiscard]]
	std::string getConfigPath() const;

	NewLevelDialog(tgui::Gui& gui, cfg::Ini& ini);
	NewLevelDialog(const NewLevelDialog&) = delete;
	NewLevelDialog(NewLevelDialog&&) = delete;
};
