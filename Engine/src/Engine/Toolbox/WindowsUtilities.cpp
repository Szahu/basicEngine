#include "EGpch.h"
#include "WindowsUtilities.h"

#include <windows.h>
#include <Commdlg.h>

#include <atlstr.h>

const std::string Engine::WindowsUtilities::GetFileOpen()
{
	OPENFILENAME ofn;
	char szFile[256];

	HINSTANCE hInstance;
	HINSTANCE hPrevInstance; 
	LPSTR lpCmdLine; 
	int nCmdShow;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = LPWSTR(szFile);
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = LPWSTR("All\0*.*\0Text\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);

	// Now simpley display the file name 
	return CW2A(ofn.lpstrFile);
}
