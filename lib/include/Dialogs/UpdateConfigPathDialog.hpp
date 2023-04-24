#pragma once

#include "include/Dialogs/DialogBase.hpp"
#include "include/Interfaces/FileApiInterface.hpp"
#include "include/Utilities/GC.hpp"

class UpdateConfigPathDialog final : public DialogInterface
{
public:
    UpdateConfigPathDialog(GC<Gui> gui, GC<FileApiInterface> fileApi);

public:
    [[nodiscard]] std::string getConfigPath() const;

private:
    virtual void customOpenCode() override;
};