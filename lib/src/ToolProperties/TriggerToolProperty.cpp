#include "include/ToolProperties/TriggerToolProperty.hpp"
#include "include/Dialogs/DialogBuilderHelper.hpp"

void TriggerToolProperty::fillEditDialog(
		tgui::Panel::Ptr& panel,
		FormValidatorToken& formValidatorToken)
{
	using namespace DialogBuilderHelper;

	auto dst = tgui::ScrollablePanel::create();
	panel->add(dst);

	unsigned row = 0;
	addOption(dst, formValidatorToken, "X coordinate:", "Measured in pixels from top-left corner", data.x, row++);
	addOption(dst, formValidatorToken, "Y coordinate:", "Measured in pixels from top-left corner", data.y, row++);
	if (data.areaType == LevelD::Trigger::AreaType::Circle)
	{
		addOption(dst, formValidatorToken, "Radius:", "Measured in pixels", data.radius, row++);
	}
	else
	{
		addOption(dst, formValidatorToken, "Width:", "Measured in pixels", data.width, row++);
		addOption(dst, formValidatorToken, "Height:", "Measured in pixels", data.height, row++);
	}
	addOption(dst, formValidatorToken, "Trigger type:", "How the trigger should be executed", data.type, row++);
	addOption(dst, formValidatorToken, "Tag:", "Value used to group related objects", data.tag, row++, true, true);
	addOption(dst, formValidatorToken, "Action ID:", "ID of action to execute", data.id, row++);
	addOption(dst, formValidatorToken, "Parameter 1:", "First param of action", data.a1, row++);
	addOption(dst, formValidatorToken, "Parameter 2:", "Second param of action", data.a2, row++);
	addOption(dst, formValidatorToken, "Parameter 3:", "Third param of action", data.a3, row++);
	addOption(dst, formValidatorToken, "Parameter 4:", "Fourth param of action", data.a4, row++);
	addOption(dst, formValidatorToken, "Parameter 5:", "Fifth param of action", data.a5, row++);
	addOption(dst, "Metadata:", "Text field for custom data", data.metadata, row++);
}
