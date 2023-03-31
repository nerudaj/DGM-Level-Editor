#pragma once

#include "include/Interfaces/ProcessCreatorInterface.hpp"

class ProcessCreator final : public ProcessCreatorInterface
{
public:
	std::expected<ReturnFlag, ErrorMessage> Exec(
			std::string pathToBinary,
			std::string params) const noexcept override;
};
