#pragma once

#include <sstream>
#include <Windows.h>

inline std::vector<std::string> split(std::string input, std::string delimiter) {
	size_t pos = 0;
	std::string token;
	std::vector<std::string> tokens;
	while ((pos = input.find(delimiter)) != std::string::npos) {
		token = input.substr(0, pos);
		input.erase(0, pos + delimiter.length());
		tokens.push_back(token);
	}
	tokens.push_back(input);
	return tokens;
}

inline std::string selectFile() {
	// From: https://www.daniweb.com/programming/software-development/code/217307/a-simple-getopenfilename-example
	OPENFILENAME ofn;
	char szFile[100];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	GetOpenFileName(&ofn);
	return ofn.lpstrFile;
}

inline std::string resolve(std::string relativePath) {
	const int BUFFER_SIZE = 4096;
	TCHAR  buffer[BUFFER_SIZE] = TEXT("");
	TCHAR** lppPart = { NULL };
	if (GetFullPathName(relativePath.c_str(), BUFFER_SIZE, buffer, lppPart))
		return std::string(buffer);
	else
		throw std::runtime_error("Unable to resolve path: " + relativePath);
}