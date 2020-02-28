//
// Created by marco on 27/02/2020.
//
#include <windows.h>
#include <vector>

namespace mem {
	// EXTERNAL
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	uintptr_t  FindDMMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>offsets);

	// INTERNAL VERSION
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
	uintptr_t  FindDMMAAddy(uintptr_t ptr, std::vector<unsigned int>offsets);
}
