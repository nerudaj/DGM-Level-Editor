#pragma once

#include "include/Utilities/Box.hpp"
#include "include/Tools/ToolProperty.hpp"
#include "include/Tools/ToolInterface.hpp"

#include <functional>

class EditPropertyDialog final
{
public:
	EditPropertyDialog(
		tgui::Gui& gui,
		tgui::Theme& theme) noexcept
		: gui(gui)
		, theme(theme)
	{}

public:
	void open(
		Box<ToolPropertyInterface> property,
		ToolInterface& targetTool);

	void close();

public:
	static inline constexpr const char* DIALOG_ID = "EditPropertyDialog";

private:
	void buildBottomButtonBar(
		tgui::ChildWindow::Ptr& modal,
		std::function<void(void)> submit);

	tgui::Button::Ptr createButton(
		const std::string& label,
		const tgui::Layout2d& size,
		const tgui::Layout2d& position,
		std::function<void(void)> callback);

private:
	tgui::Gui& gui;
	tgui::Theme& theme;
	Box<ToolPropertyInterface> currentProperty = Box<NullToolProperty2>();
};
