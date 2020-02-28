//
// Created by marco on 27/02/2020.
//


#include "pch.h"
#include "mem.h"


// patching memory cell with new instructions;
void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess) {

	DWORD oldProtect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);
}


// removing instrucions from memory
void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess) {

	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;

}


/*
 * FIND DYNAMIC ALLOCATED MEMORY ADDRESS
 * @param ptr is pointer to our process e.g. AssaultCube
*/
uintptr_t mem::FindDMMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t  addr = ptr;

	for (unsigned int offset : offsets) {
		BOOL rpmBool = ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), nullptr);
		if (rpmBool == false) {
			printf("SOMETHING WENT WRONG IN RPM: %lu\n", GetLastError());
		}
		addr += offset;
	}
	return addr; // returns dynamic address of actual ammo value
}


// ===================== INTERNAL =============
// patching memory cell with new instructions;
void mem::Patch(BYTE* dst, BYTE* src, unsigned int size) {

	DWORD oldProtect;
	VirtualProtect( dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy( dst, src, size);
	VirtualProtect( dst, size, oldProtect, &oldProtect);
}


// removing instrucions from memory
void mem::Nop(BYTE* dst, unsigned int size) {

	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);

}


/*
 * FIND DYNAMIC ALLOCATED MEMORY ADDRESS
 * @param ptr is pointer to our process e.g. AssaultCube
*/
uintptr_t mem::FindDMMAAddy( uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t  addr = ptr;

	for (unsigned int offset : offsets) {
		addr = *(uintptr_t*)addr;
		addr += offset;
	}
	return addr; // returns dynamic address of actual ammo value
}