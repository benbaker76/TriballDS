#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Box2D.h"
#include "level01_tex_bin.h"
#include "level01_pal_bin.h"
#include "ball_tex_bin.h"
#include "ball_pal_bin.h"

#include "font.h"
#include "titlescreen.h"
#include "Globals.h"
#include "Control.h"
#include "Text.h"
#include "DrawMap.h"
#include "initLevel.h"
#include "initVideo.h"


void initVideo2D()
{
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	
	vramSetBankC(VRAM_C_SUB_BG);
	
	bgInitSub(0, BgType_Text4bpp, BgSize_T_256x256, BG0_MAP_BASE_SUB, BG0_TILE_BASE_SUB);
	
	dmaCopy(fontTiles, BG_TILE_RAM_SUB(BG0_TILE_BASE_SUB), fontTilesLen);
	dmaCopy(fontPal, BG_PALETTE_SUB, fontPalLen);
	
	BG_PALETTE_SUB[0] = 0;
}

void initVideo3D()
{
	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);

	// initialize gl
	glInit();
	
	//enable textures
	glEnable(GL_TEXTURE_2D);
	
	// enable antialiasing
//	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0, 0, 0, 31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0, 0, 255, 191);
	
	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankC(VRAM_C_TEXTURE);
	vramSetBankD(VRAM_D_TEXTURE);
	//vramSetBankE(VRAM_E_TEX_PALETTE);
	//vramSetBankF(VRAM_F_TEX_PALETTE);
	//vramSetBankG(VRAM_G_TEX_PALETTE);

	glGenTextures(TEXTURECOUNT, g_textureIDS);
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL01]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin );
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL02]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin + 1024);
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL03]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin + 1024 * 2);
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL04]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin + 1024 * 3);
	
	g_palAddress[0] = gluTexLoadPal((u16*)ball_pal_bin, 256, GL_RGB256);
	
	glBindTexture(0, g_textureIDS[TEXTURE_LEVEL01_1]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin);
	
	glBindTexture(0, g_textureIDS[TEXTURE_LEVEL01_2]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin + 65536);
	
	glBindTexture(0, g_textureIDS[TEXTURE_LEVEL01_3]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin + 65536 * 2);
		
	glBindTexture(0, g_textureIDS[TEXTURE_LEVEL01_4]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin + 65536 * 3);
	
	g_palAddress[1] = gluTexLoadPal((u16*)level01_pal_bin, 256, GL_RGB256);
	
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	gluLookAt(	0.0, 0.0, 1.0,		//camera possition
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
				
	g_texelSize.Width = 1.0 / 256.0;
	g_texelSize.Height = 1.0 / 192.0;
}