#pragma once

#include "include/Dialogs/DialogBase.hpp"
#include "include/Interfaces/FileApiInterface.hpp"
#include "include/Utilities/GC.hpp"

class UpdateConfigPathDialog final : public DialogInterface
{
public:
	UpdateConfigPathDialog(
		tgui::Gui& gui,
		tgui::Theme& theme,
		GC<FileApiInterface> fileApi);

public:
	[[nodiscard]]
	std::string getConfigPath() const;

private:
	virtual void customOpenCode() override;
};