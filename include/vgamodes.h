#ifndef _VGAMODES_H
#define _VGAMODES_H

#include <stdint.h>

#include <graphics.h>


int VGA_SetGraphicsMode(uint16_t width, uint16_t height, uint8_t chain4);

// Get current graphics mode

int VGA_GetGraphicsMode(GraphicsMode * modePtr);


#endif
