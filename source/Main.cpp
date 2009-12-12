// ------------------------
// HeadRoll Demo
// ------------------------

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

void initBox2D()
{
	g_worldAABB = new b2AABB();
	g_worldAABB->minVertex.Set(SCREEN_WIDTH * -SCALE, SCREEN_HEIGHT * -SCALE);
	g_worldAABB->maxVertex.Set(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);
	
	g_gravity = new b2Vec2(0.0f, -10.0f);
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(50.0f, 10.0f);
	g_groundBoxDef->density = 0.0f;

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -20.0f);
	g_groundBodyDef->AddShape(g_groundBoxDef);

	g_world->CreateBody(g_groundBodyDef);

	g_plat = new b2PolyDef();	// HOW THE FUCK?
	g_plat->vertexCount = 6;
	g_plat->vertices[0].Set(0.0f, 0.4f);
	g_plat->vertices[1].Set(-3.0f, 0.05f);
	g_plat->vertices[2].Set(-3.0f, -0.4f);
	g_plat->vertices[3].Set(0.0f, -0.5f);
	g_plat->vertices[4].Set(5.8f, -0.5f);
	g_plat->vertices[5].Set(5.7f, 0.7f);
	g_plat->density = 0.0f;
	g_plat->friction = 2.0f;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(9.5f, 3.4f);
	g_bodyDef->AddShape(g_plat);
	g_world->CreateBody(g_bodyDef);

	g_plat = new b2PolyDef();	// HOW THE FUCK?
	g_plat->vertexCount = 4;
	g_plat->vertices[0].Set(0.0f, 0.5f);
	g_plat->vertices[1].Set(0.0f, -0.5f);
	g_plat->vertices[2].Set(6.0f, -1.0f);
	g_plat->vertices[3].Set(6.0f, 1.5f);
	g_plat->density = 0.0f;
	g_plat->friction = 2.0f;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(20.8f, 3.6f);
	g_bodyDef->AddShape(g_plat);
	g_world->CreateBody(g_bodyDef);

	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(4.7f, 1.0f);
	g_boxDef->density = 0.0f;
	g_boxDef->friction = 1.0f;
	g_boxDef->restitution = 0.0F;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(17.5f, 0.6f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);
	
	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(1.0f, 20.0f);
	g_boxDef->density = 0.0f;
	g_boxDef->friction = 1.0f;
	g_boxDef->restitution = 0.0F;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(25.6f, 19.2f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);	
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(-2.0f, 19.2f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);	
}

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
	glEnable(GL_ANTIALIAS);
	
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
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)ball_tex_bin);
	
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
}

void drawQuad(float quadSize, int textureSize)
{
	GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(textureSize)));
	glVertex3v16(floattov16(-(quadSize / 2)), floattov16(-(quadSize / 2)), 0 );

	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(textureSize), inttot16(textureSize)));
	glVertex3v16(floattov16((quadSize / 2)), floattov16(-(quadSize / 2)), 0 );

	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(textureSize), 0));
	glVertex3v16(floattov16((quadSize / 2)), floattov16((quadSize / 2)), 0 );

	GFX_TEX_COORD = (TEXTURE_PACK(0, 0));
	glVertex3v16(floattov16(-(quadSize / 2)), floattov16((quadSize / 2)), 0 );
}

void drawGLScene()
{
	//move it away from the camera
	glTranslate3f32(0, 0, floattof32(-1));
			
	//glRotateZ(rotateZ);
	
	//glRotatef(xrot,1.0f,0.0f,0.0f);
	//glRotatef(yrot,0.0f,1.0f,0.0f);
	//glRotatef(zrot,0.0f,0.0f,1.0f);

	glMaterialf(GL_AMBIENT, RGB15(31, 31, 31));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(31, 31, 31));
	glMaterialf(GL_EMISSION, RGB15(31, 31, 31));
	
	// setup the lighting
	//glLight(0, RGB15(31,31,31) , 0, floattov10(-.5), floattov10(-.85));

	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();

	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);

	glNormal(NORMAL_PACK(0, inttov10(-1), 0));
	
	glBegin(GL_QUAD);
	
	glBindTexture(TEXTURE_BALL01, g_textureIDS[TEXTURE_BALL01]);
	glColorTable(GL_RGB256, g_palAddress[0]);
	
	for(int i=0; i<BALLCOUNT; i++)
	{	
		b2Vec2 position = g_spriteArray[i].Body->GetOriginPosition();
		float rotation = g_spriteArray[i].Body->GetRotation();
					
		g_spriteArray[i].X = (position.x / SCALE);			
		g_spriteArray[i].Y = 192 - BALLSIZE - ((float)position.y / SCALE);
		
		glPushMatrix();
		glTranslatef(position.x * SCALE, position.y * SCALE, 0);
		glRotatef(rotation * 90, 0.0f, 0.0f, 1.0f);
		drawQuad(0.3f, 32);
		glPopMatrix(1);	
	}
	
	glBindTexture(TEXTURE_LEVEL01_1, g_textureIDS[TEXTURE_LEVEL01_1]);
	glColorTable(GL_RGB256, g_palAddress[1]);
	
	glPushMatrix();
	glTranslatef(-2.0f, 2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256);
	glPopMatrix(1);
	
	glBindTexture(TEXTURE_LEVEL01_2, g_textureIDS[TEXTURE_LEVEL01_2]);
	
	glPushMatrix();
	glTranslatef(2.0f, 2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256);
	glPopMatrix(1);
	
	glBindTexture(TEXTURE_LEVEL01_3, g_textureIDS[TEXTURE_LEVEL01_3]);
	
	glPushMatrix();
	glTranslatef(-2.0f, -2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256);
	glPopMatrix(1);
	
	glBindTexture(TEXTURE_LEVEL01_4, g_textureIDS[TEXTURE_LEVEL01_4]);
	
	glPushMatrix();
	glTranslatef(2.0f, -2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256);
	glPopMatrix(1);
	
	glEnd();
}

void movePlayer()
{
	scanKeys();						// Read button data
	int held = keysHeld();			// Used to calculate if a button is down
	
	b2Vec2 vel = g_spriteArray[0].Body->GetLinearVelocity();
	
	if (held & KEY_LEFT)
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(-VELX, vel.y));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(IMPX, 0), b2Vec2(0, 0));
	}
	else if (held & KEY_RIGHT)
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(VELX, vel.y));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(-IMPX, 0), b2Vec2(0, 0));
	}
	else if (held & KEY_UP)
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, VELY));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, IMPY), b2Vec2(0, 0));
	}
	else if (held & KEY_DOWN)
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, -VELY));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, -IMPY), b2Vec2(0, 0));
	}
	else
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(0.0f, vel.y));
	}
}

int main(void)
{
	//consoleDebugInit(DebugDevice_NOCASH);
	consoleDemoInit();
	
	initBox2D();
	initVideo2D();
	initVideo3D();
	
	DrawString("HEADRUSH, whatever next?", 0, 0, true);
	DrawString("WELL!! Looking a bit better?", 0, 22, true);
	DrawString("ALPHA 0.00000000004 and a chuff", 0, 2, true);
	
	// --------------------------

	for(int i=1; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = (rand() % 100) + 140; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_spriteArray[i].Y =  192 - ((rand() % 100)); //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_spriteArray[i].Type = BALLTYPE_EVILBALL;
		
		g_spriteArray[i].CircleDef = new b2CircleDef();
		g_spriteArray[i].BodyDef =  new b2BodyDef();
		
		g_spriteArray[i].CircleDef->radius = 32 / 2 * SCALE; 
		g_spriteArray[i].CircleDef->density = 1.0F; 
		g_spriteArray[i].CircleDef->friction = 0.5F; 
		g_spriteArray[i].CircleDef->restitution = 0.8F; 

		g_spriteArray[i].BodyDef->position.Set(g_spriteArray[i].X * SCALE, g_spriteArray[i].Y * SCALE);
		g_spriteArray[i].BodyDef->AddShape(g_spriteArray[i].CircleDef);
		
		g_spriteArray[i].Body = g_world->CreateBody(g_spriteArray[i].BodyDef);
	}
	
	// INIT PLAYER
	g_spriteArray[0].Action = ACTION_NONE;	
	g_spriteArray[0].X = 128;
	g_spriteArray[0].Y = 120;
	g_spriteArray[0].Type = BALLTYPE_PLAYER;
	
	g_spriteArray[0].CircleDef = new b2CircleDef();
	g_spriteArray[0].BodyDef =  new b2BodyDef();
	
	g_spriteArray[0].CircleDef->radius = 24 / 2 * SCALE; 
	g_spriteArray[0].CircleDef->density = 0.5F; 
	g_spriteArray[0].CircleDef->friction = 0.2F; 
	g_spriteArray[0].CircleDef->restitution = 0.8F; 

	g_spriteArray[0].BodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
	g_spriteArray[0].BodyDef->AddShape(g_spriteArray[0].CircleDef);
	
	g_spriteArray[0].Body = g_world->CreateBody(g_spriteArray[0].BodyDef);

	g_levelX = 0;
	g_levelY = 0;

	float timeStep = 1.0f / 30.0f;
	int iterations = 1;
	char buffer[20];
	
	while(1)
	{
		b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
		float rotationP = g_spriteArray[0].Body->GetRotation();
	
		sprintf(buffer, "%d X ",(int)(position.x * 10));
		DrawString(buffer, 10, 21, true);
		sprintf(buffer, "%d Y ",192 - (int)(position.y * 10));
		DrawString(buffer, 16, 21, true);
		sprintf(buffer, "%d X SCRL",(int) g_levelX);
		DrawString(buffer, 0, 4, true);
		sprintf(buffer, "%d Y SCRL",(int) g_levelY);
		DrawString(buffer, 0, 5, true);	
		sprintf(buffer, "%d Rot",(int)(rotationP *100));
		DrawString(buffer, 16, 20, true);
		
		movePlayer();
	
		g_world->Step(timeStep, iterations);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		drawGLScene();
		
		glPopMatrix(1);
			
		glFlush(0);
	
		// Wait for vblank
		swiWaitForVBlank();
	}

	// Will never reach here
	return 0;
}
