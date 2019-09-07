@echo off

robocopy 3rdParty\SFML\bin vsbuild\Release sfml-audio-2.dll sfml-graphics-2.dll sfml-network-2.dll sfml-system-2.dll sfml-window-2.dll openal32.dll
robocopy 3rdParty\SFML\bin vsbuild\Debug sfml-audio-d-2.dll sfml-graphics-d-2.dll sfml-network-d-2.dll sfml-system-d-2.dll sfml-window-d-2.dll openal32.dll
robocopy 3rdParty\TGUI\bin vsbuild\Release tgui.dll
robocopy 3rdParty\TGUI\bin vsbuild\Debug tgui-d.dll