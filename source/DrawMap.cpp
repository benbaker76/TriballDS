#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "DrawMap.h"
#include "Globals.h"


void drawMap()
{

// the idea of this piece of code 'will' be that you pass the X,Y coord of player (0-511) and the scroll possition will be updated to match
// and also the enemies and other sprites will be affected also. This will have to eventually return 2 offsets (0-256) for the scroll (though
// they could be read from the H/VOFS, so that perhaps we can use our own sprite draw code to only show onscreen sprites? Hmmm
// Yep - this is a tricky bit for me

// also, scrolling messes up the idea of multiple players on screen (or does it?)


	REG_BG1HOFS_SUB = (int)g_scrollPos.X;
	REG_BG1VOFS_SUB = (int)g_scrollPos.Y;
	
//	g_levelX = XCoord;
//	g_levelY = 320;
}

void drawQuad(float quadSize, int textureSize, int quadFlags)
{
	quadSize /= 2.0;
	quadSize += g_texelSize.Width;

	GFX_TEX_COORD = (TEXTURE_PACK(quadFlags & QUADFLAGS_HFLIP ? inttot16(textureSize) : 0, quadFlags & QUADFLAGS_VFLIP ? 0 : inttot16(textureSize)));
	glVertex3v16(floattov16(-quadSize), floattov16(-quadSize), 0 );

	GFX_TEX_COORD = (TEXTURE_PACK(quadFlags & QUADFLAGS_HFLIP ? 0 : inttot16(textureSize), quadFlags & QUADFLAGS_VFLIP ? 0 : inttot16(textureSize)));
	glVertex3v16(floattov16(quadSize), floattov16(-quadSize), 0 );

	GFX_TEX_COORD = (TEXTURE_PACK(quadFlags & QUADFLAGS_HFLIP ? 0 : inttot16(textureSize), quadFlags & QUADFLAGS_VFLIP ? inttot16(textureSize) : 0));
	glVertex3v16(floattov16(quadSize), floattov16(quadSize), 0 );

	GFX_TEX_COORD = (TEXTURE_PACK(quadFlags & QUADFLAGS_HFLIP ? inttot16(textureSize) : 0, quadFlags & QUADFLAGS_VFLIP ? inttot16(textureSize) : 0));
	glVertex3v16(floattov16(-quadSize), floattov16(quadSize), 0);
}

void drawB2Poly(Poly* poly)
{
	glBegin(GL_TRIANGLE_STRIP);
	
	glPushMatrix();
	
	// The Box2d poly's are drawn from origin so lets translate it to draw in position
	glTranslatef(poly->BodyDef->position.x * SCALE, poly->BodyDef->position.y * SCALE, -1 + g_texelSize.Width);
	
	for(int i=0; i<poly->PolyDef->vertexCount; i++)
	{
		glVertex3v16(floattov16((float)(poly->PolyDef->vertices[i].x * SCALE)), floattov16((float)(poly->PolyDef->vertices[i].y * SCALE)), 0);
		
		// Every 3rd verticie we need to draw back 3 to complete a triangle
		if(i % 2 == 0)
			glVertex3v16(floattov16((float)(poly->PolyDef->vertices[i-2].x * SCALE)), floattov16((float)(poly->PolyDef->vertices[i-2].y * SCALE)), 0);
	}
	
	// complete last triangle
	glVertex3v16(floattov16((float)(poly->PolyDef->vertices[0].x * SCALE)), floattov16((float)(poly->PolyDef->vertices[0].y * SCALE)), 0);
	
	glPopMatrix(1);	
	
	glEnd();
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
	
	for(int i=0; i<PLATFORMCOUNT; i++)
		drawB2Poly(g_platformArray[i]);
	
	glBegin(GL_QUAD);
	
	glBindTexture(TEXTURE_BALL01, g_textureIDS[TEXTURE_BALL01]);
	glColorTable(GL_RGB256, g_palAddress[0]);
	
	for(int i=0; i<BALLCOUNT; i++)
	{	
		b2Vec2 position = g_spriteArray[i].Body->GetOriginPosition();
		float rotation = g_spriteArray[i].Body->GetRotation();
		
		glPushMatrix();
		glTranslatef(position.x * SCALE, position.y * SCALE, -1 + g_texelSize.Width);
		//glRotatef(rotation * (180 / PI), 0.0f, 0.0f, 1.0f);
		glRotatef(degreesToAngle(rotation), 0.0f, 0.0f, 1.0f);
		drawQuad(0.4f, 32, QUADFLAGS_NONE);
		glPopMatrix(1);
	}
	
	glBindTexture(TEXTURE_LEVEL01_1, g_textureIDS[TEXTURE_LEVEL01_1]);
	glColorTable(GL_RGB256, g_palAddress[1]);
	
	/* glPushMatrix();
	glTranslatef(-6.0f, 6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP | QUADFLAGS_VFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(-6.0f, 2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(-2.0f, 6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_VFLIP);
	glPopMatrix(1); */
	
	glPushMatrix();
	glTranslatef(-2.0f, 2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_NONE);
	glPopMatrix(1);
	
	glBindTexture(TEXTURE_LEVEL01_2, g_textureIDS[TEXTURE_LEVEL01_2]);
	
	/* glPushMatrix();
	glTranslatef(6.0f, 6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP | QUADFLAGS_VFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(6.0f, 2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(2.0f, 6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_VFLIP);
	glPopMatrix(1); */
	
	glPushMatrix();
	glTranslatef(2.0f, 2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_NONE);
	glPopMatrix(1);
	
	glBindTexture(TEXTURE_LEVEL01_3, g_textureIDS[TEXTURE_LEVEL01_3]);
	
	/* glPushMatrix();
	glTranslatef(-6.0f, -6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP | QUADFLAGS_VFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(-6.0f, -2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(-2.0f, -6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_VFLIP);
	glPopMatrix(1); */
	
	glPushMatrix();
	glTranslatef(-2.0f, -2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_NONE);
	glPopMatrix(1);
	
	glBindTexture(TEXTURE_LEVEL01_4, g_textureIDS[TEXTURE_LEVEL01_4]);
	
	/* glPushMatrix();
	glTranslatef(6.0f, -6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP | QUADFLAGS_VFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(6.0f, -2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_HFLIP);
	glPopMatrix(1);
	
	glPushMatrix();
	glTranslatef(2.0f, -6.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_VFLIP);
	glPopMatrix(1); */
	
	glPushMatrix();
	glTranslatef(2.0f, -2.0f, -1);
	glRotatef(0, 0.0f, 0.0f, 0.0f);
	drawQuad(4.0f, 256, QUADFLAGS_NONE);
	glPopMatrix(1);
	
	glEnd();
}