// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"
#include "proc.h"


DWORD WINAPI HackThread(HMODULE hModule) {
	//create console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "TEST CONSOLE PRINT" << std::endl;

	//get module base
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

	bool bHealth = false, bAmmo = false, bRecoil = false;

	int newValue = 1337;


	//hack loop
	while (true) {
		// key input
		if(GetAsyncKeyState(VK_END	) & 1) {
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			bHealth = !bHealth;
			std::cout << "Unlimited health is now: " << bHealth <<std::endl;
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1) { // ammo decrement memory address to change 0x637e9 (original 2 bytes FF 0E)
			bAmmo = !bAmmo;
			std::cout << "Unlimited Ammo is now: " << bAmmo << std::endl;

			if(bAmmo) {
				mem::Patch((BYTE*)(moduleBase + 0x637e9), (BYTE*)"xFF\x06", 2); // "xFF\x06" is increment in assembler
			}else {
				mem::Patch((BYTE*)(moduleBase + 0x637e9), (BYTE*)"xFF\x0E", 2);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1) { // recoil memory instruction address to mop (0x63786)
			bRecoil = !bRecoil;
			std::cout << "NoRecoil is now: " << bRecoil << std::endl;

			if(bRecoil) {
				mem::Nop((BYTE*)(moduleBase + 0x63786), 10); // nulling 10 bytes starting from this offset
			}else {
				mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2",10); // restoring original 10 bytes starting from this offset
			}
		}

		// continous write

		uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10f4f4);

		// // first way same as external;
		// uintptr_t healthAddress = mem::FindDMMAAddy(localPlayerPtr, {0xf8});
		// mem::Patch((BYTE*)healthAddress, (BYTE*)&newValue, sizeof(newValue));

		//second way
		if(localPlayerPtr) {
			if(bHealth) {
				// long way
				// uintptr_t healthAddr = mem::FindDMMAAddy((uintptr_t)localPlayerPtr, { 0xf8 });
				// int* health = (int*)healthAddr;
				// *health = 1337;
				// short way
				*(int*)(*localPlayerPtr + 0xf8) = 1337; // DEREFERENCE POINTER
			}
		}

		Sleep(10);
		
	}
	
	// cleanup & eject
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
