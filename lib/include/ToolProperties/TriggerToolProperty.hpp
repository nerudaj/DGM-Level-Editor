#pragma once

#include "include/Interfaces/ToolPropertyInterface.hpp"

#include <TGUI/TGUI.hpp>
#include <LevelD.hpp>

#include <climits>

class TriggerToolProperty : public ToolPropertyInterface
{
public:
	void fillEditDialog(
		tgui::Panel::Ptr& panel,
		FormValidatorToken& formValidatorToken) override;

public:
	LevelD::Trigger data;
	std::size_t id;
};
