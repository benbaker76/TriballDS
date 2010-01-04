#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Box2D.h"
#include "level01_tex_bin.h"
#include "level01_pal_bin.h"
#include "level02_tex_bin.h"
#include "level02_pal_bin.h"
#include "ball_tex_bin.h"
#include "ball_pal_bin.h"
#include "particle_tex_bin.h"
#include "particle_pal_bin.h"
#include "trail_tex_bin.h"
#include "trail_pal_bin.h"
#include "box_tex_bin.h"

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

void getLevelQuad(int x, int y, RectF* rect)
{
	rect->X = -(g_levelGridSize.Width * (g_levelQuadSize.Width / 2) - (g_levelQuadSize.Width / 2)) + x * g_levelQuadSize.Width;
	rect->Y = (g_levelGridSize.Height * (g_levelQuadSize.Height / 2) - (g_levelQuadSize.Height / 2)) - y * g_levelQuadSize.Height;
	rect->Width = g_levelQuadSize.Width;
	rect->Height = g_levelQuadSize.Height;
}

int getTextureSlot(int quadId)
{
	int textureId = g_textureIDS[g_levelQuadID[quadId]];
	uint32* addr = (uint32*) glGetTexturePointer(textureId);
	
	if(!addr)
		return -1;

	for(int i=0; i<LEVELTEXTURECOUNT; i++)
	{
		uint32* texPtr = (uint32*) glGetTexturePointer(g_textureIDS[g_levelQuadID[i]]);
		
		if(addr == texPtr)
			return i;
	}
	
	return -1;
}

void loadTextures()
{
	int textureSlot = 0;
	
	for(int y=0; y<g_levelGridSize.Height; y++)
	{
		for(int x=0; x<g_levelGridSize.Width; x++)
		{
			int quadId = x + y * g_levelGridSize.Width;
			static RectF viewRect;
			static RectF quadRect;
			
			/* viewRect.Left = g_cameraPos.X - 2;
			viewRect.Top = g_cameraPos.Y - 2;
			viewRect.Right = viewRect.Left + 2;
			viewRect.Bottom = viewRect.Top + 2; */
			
			viewRect.X = -2 + g_cameraPos.X;
			viewRect.Y = -1 + g_cameraPos.Y;
			viewRect.Width = 6;
			viewRect.Height = 4;
			
			/* viewRect.X = g_cameraPos.X;
			viewRect.Y = -1 + g_cameraPos.Y;
			viewRect.Width = 4;
			viewRect.Height = 4; */
			
			getLevelQuad(x, y, &quadRect);
			
			if(IntersectRectF(&viewRect, &quadRect))
			{
				//if(!g_levelQuadLoaded[quadId])
				//{
				
				/* int oldTextureSlot = getTextureSlot(quadId);
				
				if(oldTextureSlot != -1)
				{
					g_levelQuadID[quadId] = oldTextureSlot;
					
					continue;
				} */
				
					//g_levelQuadLoaded[quadId] = true;
					
					reloadTexture(textureSlot, quadId);
					
					if(textureSlot < LEVELTEXTURECOUNT - 1)
						textureSlot++;
				//}
			}
			else
			{
				g_levelQuadID[quadId] = -1;
			}
		}
	}
	
	//static char buf[256];
	//sprintf(buf, "%d", textureSlot);
	//fprintf(stderr, buf);
}

int loadTexture(int textureSlot, int quadId)
{
	g_levelQuadID[quadId] = textureSlot;
	int textureId = g_textureIDS[g_levelQuadID[quadId]];
	//m_levelTextureLoaded[textureSlot] = quadId;
	
	const uint8* texture = g_levelTexture + g_levelTextureSize * g_levelTextureSize * quadId;
	
	glBindTexture(0, textureId);
	return glTexImage2D(0, 0, GL_RGB256, g_glTextureSize, g_glTextureSize, 0, TEXGEN_TEXCOORD, texture);
}

int reloadTexture(int textureSlot, int quadId)
{
	g_levelQuadID[quadId] = textureSlot;
	int textureId = g_textureIDS[g_levelQuadID[quadId]];
	//m_levelTextureLoaded[textureSlot] = quadId;
	
	uint32 size = 1 << (g_glTextureSize + g_glTextureSize + 6);
	uint32* addr = (uint32*) glGetTexturePointer(textureId);
	uint32 vramTemp;
	const uint8* texture = g_levelTexture + g_levelTextureSize * g_levelTextureSize * quadId;
   
	if(!addr)
		return loadTexture(textureSlot, quadId);

	// unlock texture memory
	vramTemp = vramSetMainBanks(VRAM_A_LCD, VRAM_B_LCD, VRAM_C_LCD, VRAM_D_LCD);
	
	if(texture)
		//swiCopy((uint32*)texture, addr, size / 4 | COPY_MODE_WORD);
		dmaCopy((uint32*)texture, addr, size);
	
	vramRestoreMainBanks(vramTemp);
	
	return 1;
}

void loadLevel()
{
	switch(g_levelNum)
	{
	case 0:
		g_levelTexture = (u8*) level01_tex_bin;
		g_levelPalette = (u16*) level01_pal_bin;
		g_levelSize.Width = 512;
		g_levelSize.Height = 512;
		g_levelTextureSize = 128;
		break;
	case 1:
		g_levelTexture = (u8*) level02_tex_bin;
		g_levelPalette = (u16*) level02_pal_bin;
		g_levelSize.Width = 1024;
		g_levelSize.Height = 512;
		g_levelTextureSize = 128;
		break;
	}
	
	switch(g_levelTextureSize)
	{
	case 8:
		g_glTextureSize = TEXTURE_SIZE_8;
		break;
	case 16:
		g_glTextureSize = TEXTURE_SIZE_16;
		break;
	case 32:
		g_glTextureSize = TEXTURE_SIZE_32;
		break;
	case 64:
		g_glTextureSize = TEXTURE_SIZE_64;
		break;
	case 128:
		g_glTextureSize = TEXTURE_SIZE_128;
		break;
	case 256:
		g_glTextureSize = TEXTURE_SIZE_256;
		break;
	case 512:
		g_glTextureSize = TEXTURE_SIZE_512;
		break;
	case 1024:
		g_glTextureSize = TEXTURE_SIZE_1024;
		break;
	default:
		g_glTextureSize = TEXTURE_SIZE_128;
		break;
	}
	
	g_levelQuadSize.Width = g_levelTextureSize / 256.0F * 4.0F;
	g_levelQuadSize.Height = g_levelTextureSize / 256.0F * 4.0F;
	
	g_levelGridSize.Width = g_levelSize.Width / g_levelTextureSize;
	g_levelGridSize.Height = g_levelSize.Height / g_levelTextureSize;
	
	for(int y=0; y<g_levelGridSize.Height; y++)
	{
		for(int x=0; x<g_levelGridSize.Width; x++)
		{
			int textureId = x + y * g_levelGridSize.Width;
			
			//if(textureId < LEVELTEXTURECOUNT)
				loadTexture(textureId, textureId);
		}
	}
	
	g_palAddress[PALETTE_LEVEL] = gluTexLoadPal(g_levelPalette, 256, GL_RGB256);
}

void initVideo3D()
{
	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);
	
	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankC(VRAM_C_TEXTURE);
	vramSetBankD(VRAM_D_TEXTURE);
	//vramSetBankE(VRAM_E_TEX_PALETTE);
	//vramSetBankF(VRAM_F_TEX_PALETTE);
	//vramSetBankG(VRAM_G_TEX_PALETTE);

	// initialize gl
	glInit();
	
	//enable textures
	glEnable(GL_TEXTURE_2D);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0, 0, 0, 31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0, 0, 255, 191);
			  
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	//glOrtho(-3, 3,-2, 2, 0.1, 100);
	
	gluLookAt(	0.0, 0.0, 1.0,		//camera possition
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
				
	glLight(0, RGB15(31,31,31), 0, floattov10(-1.0), 0);
	glLight(1, RGB15(31,0,0), 0, floattov10(1) - 1, 0);
	glLight(2, RGB15(0,31,0), floattov10(-1.0), 0, 0);
	glLight(3, RGB15(0,0,31), floattov10(1.0) - 1, 0, 0);
	
	//need to set up some material properties since DS does not have them set by default
	glMaterialf(GL_AMBIENT, RGB15(16, 16, 16));
	glMaterialf(GL_DIFFUSE, RGB15(16, 16, 16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8, 8, 8));
	glMaterialf(GL_EMISSION, RGB15(16, 16, 16));
	
	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);
				
	g_texelSize.Width = 0.228 / 256.0;
	g_texelSize.Height = 0.228 / 192.0;

	glGenTextures(TEXTURECOUNT, g_textureIDS);
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL01]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin );
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL02]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin + 1024);
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL03]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin + 1024 * 2);
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL04]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin + 1024 * 3);
	
	glBindTexture(0, g_textureIDS[TEXTURE_BOX01]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)box_tex_bin + 1024 * 3);
	
	g_palAddress[PALETTE_BALL] = gluTexLoadPal((u16*)ball_pal_bin, 256, GL_RGB256);
	
	glBindTexture(0, g_textureIDS[TEXTURE_PARTICLE]);
	glTexImage2D(0, 0, GL_RGB32_A3, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD, (u8*)particle_tex_bin);
	
	g_palAddress[PALETTE_PARTICLE] = gluTexLoadPal((u16*)particle_pal_bin, 256, GL_RGB32_A3);

	glBindTexture(0, g_textureIDS[TEXTURE_TRAIL]);
	glTexImage2D(0, 0, GL_RGB32_A3, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD, (u8*)trail_tex_bin);
	
	g_palAddress[PALETTE_TRAIL] = gluTexLoadPal((u16*)trail_pal_bin, 256, GL_RGB32_A3);
	
	/* glBindTexture(0, g_textureIDS[TEXTURE_LEVEL01]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin);
	
	glBindTexture(0, g_textureIDS[TEXTURE_LEVEL02]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin + 65536);
	
	glBindTexture(0, g_textureIDS[TEXTURE_LEVEL03]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin + 65536 * 2);
		
	glBindTexture(0, g_textureIDS[TEXTURE_LEVEL04]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, (u8*)level01_tex_bin + 65536 * 3);
	
	g_palAddress[1] = gluTexLoadPal((u16*)level01_pal_bin, 256, GL_RGB256); */
}