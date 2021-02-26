#include "FileApi.hpp"
#include <Windows.h>
#include <stdexcept>
#include <shlobj_core.h>
#include <locale>
#include <codecvt>

std::string FileApi::resolveAppdata() {
	PWSTR raw;
	std::wstring result;
	if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &raw) == S_OK) {
		result = std::wstring(raw);
	}
	CoTaskMemFree(raw);

	// Need to convert wstring to string
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	return converter.to_bytes(result);
}

std::string FileApi::getSaveFileName(const char *filter) {
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH - 1;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetSaveFileNameA(&ofn)) {
		throw std::runtime_error("File select dialog failed");
	}

	filename[MAX_PATH - 1] = '\0';
	return std::string(filename);
}

std::string FileApi::getOpenFileName(const char* filter) {
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH - 1;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetOpenFileNameA(&ofn)) {
		throw std::runtime_error("File select dialog failed");
	}

	filename[MAX_PATH - 1] = '\0';
	return std::string(filename);
}
