#include "EGpch.h"
#include "WindowsUtilities.h"

#include <windows.h>
#include <Commdlg.h>

const std::string Engine::WindowsUtilities::GetFileOpen()
{
	OPENFILENAME ofn;
	char szFile[100];

	HINSTANCE hInstance;
	HINSTANCE hPrevInstance; 
	LPSTR lpCmdLine; 
	int nCmdShow;

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
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);

	// Now simpley display the file name 
	return ofn.lpstrFile;

}
