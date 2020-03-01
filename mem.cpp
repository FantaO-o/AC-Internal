#include "pch.h"
#include "mem.h"

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oldProtect;
	VirtualProtect(dst, 0, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, 0, oldProtect, &oldProtect);
}
void mem::Nop(BYTE* dst, unsigned int size) {

	DWORD oldProtect;
	VirtualProtect(dst, 0, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, 0, oldProtect, &oldProtect);
	
}
uintptr_t mem::FindDMMAddy(uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;

	for(unsigned int offset:offsets) {
		addr = *(uintptr_t*)addr; //dereference
		addr += offset;
	}

	return addr;
	
}