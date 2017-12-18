#include <string.h>
#include <console.h>
#include <hal.h>
#include <exception.h>
#include "physicalmemorymanager.h"
#include "virtualmemorymanager.h"
#include "bootinfo.h"
#include <keyboard.h>
#include <sysapi.h>
#include <user.h>
#include <vgamodes.h>
#include <graphics.h>

// This is a dummy __main.  For some reason, gcc puts in a call to 
// __main from main, so we just include a dummy.
 
BootInfo *	_bootInfo;
 
void __main() {}

void InitialisePhysicalMemory()
{
	// Initialise the physical memory manager
	// We place the memory bit map in the next available block of memory after the kernel.
	// Note that all memory addresses here are physical memory addresses since we are dealing
	// with management of physical memory

	uint32_t memoryMapAddress = 0x100000 + _bootInfo->KernelSize;
	if (memoryMapAddress % PMM_GetBlockSize() != 0)
	{
		// Make sure that the memory map is going to be aligned on a block boundary
		memoryMapAddress = (memoryMapAddress / PMM_GetBlockSize() + 1) * PMM_GetBlockSize();
	}
	uint32_t sizeOfMemoryMap = PMM_Initialise(_bootInfo, memoryMapAddress);

	// We now need to mark various regions as unavailable
	
	// Mark first page as unavailable (so that a null pointer is invalid)
	PMM_MarkRegionAsUnavailable(0x00, PMM_GetBlockSize());

	// Mark memory used by kernel as unavailable
	PMM_MarkRegionAsUnavailable(0x100000, _bootInfo->KernelSize);

	// Mark memory used by memory map as unavailable
	PMM_MarkRegionAsUnavailable(PMM_GetMemoryMap(), sizeOfMemoryMap);

	// Reserve two blocks for the stack and make unavailable (the stack is set at 0x90000 in boot loader)
	uint32_t stackSize = PMM_GetBlockSize() * 2;
	PMM_MarkRegionAsUnavailable(_bootInfo->StackTop - stackSize, stackSize);
	
	// Reserve two block for the kernel stack and make unavailable
	PMM_MarkRegionAsUnavailable(0x80000 - stackSize, stackSize);
}
void Initialise()
{
	ConsoleClearScreen(0x1F);
	ConsoleWriteString("UODOS 32-bit Kernel. Kernel size is ");
	ConsoleWriteInt(_bootInfo->KernelSize, 10);
	ConsoleWriteString(" bytes\n");
	HAL_Initialise();
	InitialisePhysicalMemory();
	VMM_Initialise();
	KeyboardInstall(33); 
	InitialiseSysCalls();
}

void main(BootInfo * bootInfo) 
{
	_bootInfo = bootInfo;
	Initialise();

	// Uncomment out the following line when you have code that is capable of being running
	// in ring 3
	//HAL_EnterUserMode();
	
	// Uncomment out the following line to switch into VGA mode 13h
	// Note that the screen will be full of rubbish until you clear the screen
	// since it will just display what is in memory starting at 0xA0000.
	if (VGA_SetGraphicsMode(320, 200, 1) != 0) {
		GraphicsMode gfxMode = { 0 };
		if (VGA_GetGraphicsMode(&gfxMode) != 0) {
			Gfx_ClearScreen(&gfxMode);

			for (uint16_t y = 0; y < 100; y++) {
				for (uint16_t x = 0; x < 255; x++) {
					Gfx_SetPixel(&gfxMode, x, y, x);
				}
			}

			// Gfx_DrawLine(&gfxMode, 0, 120, 319, 120, 15);
			// Gfx_DrawLine(&gfxMode, 10, 122, 10, 152, 15);
			// Gfx_DrawLine(&gfxMode, 12, 122, 13, 152, 15);

			Gfx_SetPixel(&gfxMode, 319, 0, 13);
		}
	}
}
