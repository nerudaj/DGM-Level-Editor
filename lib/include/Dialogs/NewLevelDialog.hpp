#pragma once

#include <DGM/dgm.hpp>
#include "DialogBase.hpp"

#include <optional>

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
public:
	NewLevelDialog(
		tgui::Gui& gui,
		tgui::Theme& theme,
		std::optional<std::string> const& configPath);
	NewLevelDialog(const NewLevelDialog&) = delete;
	NewLevelDialog(NewLevelDialog&&) = delete;

public:
	[[nodiscard]]
	unsigned getLevelWidth() const;

	[[nodiscard]]
	unsigned getLevelHeight() const;

	[[nodiscard]]
	std::string getConfigPath() const;

private:
	std::optional<std::string> const& configPath;

private:
	virtual void customOpenCode() override;
};

