// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"

DWORD WINAPI HackThread(HMODULE hModule) {
	AllocConsole(); // create console on thread start;
	FILE* f;
	freopen_s(&f, "CONOUT$", "w" ,stdout);

	std::printf("AC INTERNAL v0.1\n\n");

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL); // gives you address .exe address of module somehow
	uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10F4F4);


	bool bHealth = false, bAmmo = false, bGrenades = false, bWaitTime = false, bArmour = false;




	// Hack loop

	while(true) {

		if(GetAsyncKeyState(VK_END) & 1) {
			break;
		}

		if(GetAsyncKeyState(VK_NUMPAD1) & 1) {
			bHealth = !bHealth;
			std::printf("Infinite health is now: %hhd\n", bHealth);
		}

		if(GetAsyncKeyState(VK_NUMPAD2) & 1) {
			bAmmo = !bAmmo;
			std::printf("Infinite ammo is now: %hhd\n", bAmmo);
		}

		if(GetAsyncKeyState(VK_NUMPAD3) & 1) {
			bGrenades = !bGrenades;
			std::printf("Infinite Grenades is now: %hhd\n", bGrenades);
		}


		if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
			bWaitTime = !bWaitTime;
			std::printf("No gun wait time is now: %hhd\n", bWaitTime);
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
			bArmour = !bArmour;
			std::printf("Infinite armour is now: %hhd\n", bArmour);
		}

		// continous loop

		if(bHealth) { // health offset = 0xF8
			*(int*)(*localPlayerPtr + 0xF8) = 1337;
		}

		if(bAmmo) { // 3 level pointer to current weapon ammo
			uintptr_t ammoAddr = mem::FindDMMAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 });
			int* ammo = (int*)ammoAddr;
			*ammo = 1337;

		}

		if(bGrenades) { // 0x0158
			*(int*)(*localPlayerPtr + 0x158) = 1337;
		}

		if(bWaitTime) { //0x0178
			*(int*)(*localPlayerPtr + 0x178) = 0;

		}

		if(bArmour) { //  0x00FC
			*(int*)(*localPlayerPtr + 0xFC) = 1337;
		}
		

		Sleep(5);
		
	}

	// cleanup && eject
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule,0);
	return 0;



	
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: // ONLY CREATE THREAD ON ATTACH (inject)
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

