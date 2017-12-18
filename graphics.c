#include <string.h>
#include <stdlib.h>

#include <vgamodes.h>
#include <graphics.h>


int Gfx_ClearScreen(GraphicsMode * modePtr)
{
	GraphicsMode mode = { 0 };

	if (modePtr == NULL) {
		if (VGA_GetGraphicsMode(&mode) == 0) {
			return 0;
		}

		modePtr = &mode;
	}

	// only linear frame buffer is supported
	if (modePtr->Chain4 != 1) {
		return 0;
	}

	memset(modePtr->FrameBufferPtr, 0, modePtr->FrameBufferSize);

	return 1;
}

int Gfx_SetPixel(GraphicsMode * modePtr, int16_t x, int16_t y, uint32_t color)
{
	GraphicsMode mode = { 0 };

	if (modePtr == NULL) {
		if (VGA_GetGraphicsMode(&mode) == 0) {
			return 0;
		}

		modePtr = &mode;
	}

	// check coordinates
	if (x >= modePtr->Width || y >= modePtr->Height
			|| x < 0 || y < 0) {
		return 0;
	}

	// only linear frame buffer is supported
	if (modePtr->Chain4 != 1) {
		return 0;
	}

	// only 8 bits per pixel are supported
	if (modePtr->Bpp != 8) {
		return 0;
	}

	*((uint8_t *)modePtr->FrameBufferPtr + (y * modePtr->Width) + x) = color;

	return 1;
}

int Gfx_DrawLine(GraphicsMode * modePtr, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
	GraphicsMode mode = { 0 };

	if (modePtr == NULL) {
		if (VGA_GetGraphicsMode(&mode) == 0) {
			return 0;
		}

		modePtr = &mode;
	}

	// check coordinates
	if ((x0 >= modePtr->Width && x1 >= modePtr->Width)
			|| (y0 >= modePtr->Height && y1 >= modePtr->Height)
			|| (x0 < 0 && x1 < 0)
			|| (y0 < 0 && y1 < 0)) {
		return 0;
	}

	// only linear frame buffer is supported
	if (modePtr->Chain4 != 1) {
		return 0;
	}

	// only 8 bits per pixel are supported
	if (modePtr->Bpp != 8) {
		return 0;
	}

	// swap start and end coordinates (y1 should be >= y0)
	if (y1 < y0) {
		int16_t t = y1;
		y1 = y0;
		y0 = t;

		t = x1;
		x1 = x0;
		x0 = t;
	}

	int width = abs(x1 - x0);
	int height = y1 - y0;
	int x = x0;
	int stepX = x1 >= x0 ? 1 : -1;

	uint8_t * ptr = (uint8_t *)modePtr->FrameBufferPtr 
		+ (y0 * modePtr->Width) 
		+ x0;

	int error = 0;
	int errorDelta;
	int errorBound;

	if (width > height) {
		errorDelta = height;
		errorBound = width;
	}
	else {
		errorDelta = width;
		errorBound = height;
	}

	for (int y = y0; y <= y1; y++) {
		do {
			if ((y >= 0 && y < modePtr->Height)
					&& (x >= 0 && x < modePtr->Width)) {
				*(ptr + x) = color;
			}

			error += errorDelta;

			int dblError = error * 2;

			if (dblError >= errorBound) {
				error -= errorBound;
				break;
			}
			
			x += stepX;
		}
		while (x != (x1 + stepX));

		ptr += modePtr->Width;
	}

	return 1;
}

int Gfx_DrawRectangle(GraphicsMode * modePtr
		, int16_t x, int16_t y
		, uint16_t width, uint16_t height
		, uint32_t color)
{
	return 1;
}

int Gfx_FillRectangle(GraphicsMode * modePtr
		, int16_t x, int16_t y
		, uint16_t width, uint16_t height
		, uint32_t color)
{
	return 1;
}
