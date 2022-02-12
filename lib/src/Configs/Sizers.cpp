#include <Windows.h>
#include <WinUser.h>

#include "include/Configs/Sizers.hpp"

unsigned Sizers::GetSystemDPI() {
	return GetDpiForSystem();
}

unsigned Sizers::GetMenuBarHeight() {
	const unsigned dpi = GetSystemDPI();
	return GetSystemMetricsForDpi(SM_CYCAPTION, dpi)
		+ GetSystemMetricsForDpi(SM_CYSIZEFRAME, dpi)
		+ GetSystemMetricsForDpi(SM_CYEDGE, dpi) * 2;
}

unsigned Sizers::GetMenuBarTextHeight() {
	return GetMenuBarHeight() / 3u;
}
