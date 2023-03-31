#include "include/Dialogs/YesNoCancelDialog.hpp"

class YesNoCancelDialogMock : public YesNoCancelDialogInterface
{
public:
	bool userCancelled = false;
	bool userConfirmed = false;

	virtual void open(const std::string, const std::string&, std::function<void(UserChoice)> completedCallback)
	{
		if (userCancelled)
			completedCallback(UserChoice::Cancelled);
		else if (userConfirmed)
			completedCallback(UserChoice::Confirmed);
		else
			completedCallback(UserChoice::Denied);
	}
};