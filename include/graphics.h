#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <stdint.h>
#include <size_t.h>


typedef struct GraphicsMode_t {
	uint16_t Width;
	uint16_t Height;
	uint8_t Chain4;
	uint8_t Bpp;
	void * FrameBufferPtr;
	size_t FrameBufferSize;
} GraphicsMode;


// clear screen
// modePtr - current graphics mode (can be NULL)

int Gfx_ClearScreen(GraphicsMode * modePtr);

// Set pixel at coordinates x, y
// modePtr - current graphics mode (can be NULL)

int Gfx_SetPixel(GraphicsMode * modePtr, int16_t x, int16_t y, uint32_t color);

// Draw line from coordinates x0, y0 to x1, y1
// modePtr - current graphics mode (can be NULL)

int Gfx_DrawLine(GraphicsMode * modePtr
	, int16_t x0, int16_t y0
	, int16_t x1, int16_t y1
	, uint32_t color);

// Draw rectangle at coordinates x, y
// modePtr - current graphics mode (can be NULL)

int Gfx_DrawRectangle(GraphicsMode * modePtr
	, int16_t x, int16_t y
	, uint16_t width, uint16_t height
	, uint32_t color);

// Draw and fill rectangle at coordinates x, y
// modePtr - current graphics mode (can be NULL)

int Gfx_FillRectangle(GraphicsMode * modePtr
	, int16_t x, int16_t y
	, uint16_t width, uint16_t height
	, uint32_t color);


#endif
