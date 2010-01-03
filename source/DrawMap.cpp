#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "level01_tex_bin.h"
#include "level01_pal_bin.h"
#include "InitVideo.h"
#include "DrawMap.h"
#include "Globals.h"
#include "Text.h"
#include "Easing.h"

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

void drawMap()
{
	glColorTable(GL_RGB256, g_palAddress[1]);
	
	for(int y=0; y<g_levelGridSize.Height; y++)
	{
		for(int x=0; x<g_levelGridSize.Width; x++)
		{
			int textureId = x + y * g_levelGridSize.Width;
			
			glBindTexture(0, g_textureIDS[textureId]);
					
			static RectF quadRect;
			
			getLevelQuad(x, y, &quadRect);
			
			glPushMatrix();
			glTranslatef(quadRect.X, quadRect.Y, -1);
			//glTranslatef(quadRect.Left, quadRect.Top, -1);
			glRotatef(0, 0.0f, 0.0f, 0.0f);
			drawQuad(quadRect.Width, g_levelTextureSize, QUADFLAGS_NONE);
			//drawQuad(quadRect.Right - quadRect.Left, g_levelTextureSize, QUADFLAGS_NONE);
			glPopMatrix(1);
		}
	}
}

void drawGLScene()
{
	//move it away from the camera
	glTranslate3f32(0, 0, floattof32(-1));
			
	//glRotateZ(rotateZ);
	
	//glRotatef(xrot,1.0f,0.0f,0.0f);
	//glRotatef(yrot,0.0f,1.0f,0.0f);
	//glRotatef(zrot,0.0f,0.0f,1.0f);

	glNormal(NORMAL_PACK(0, inttov10(-1), 0));
	
	//for(int i=0; i<PLATFORMCOUNT; i++)
	//	drawB2Poly(g_platformArray[i]);
	
	glBegin(GL_QUAD);
	
	glColorTable(GL_RGB256, g_palAddress[0]);
	
	for(int register i=0; i<BALLCOUNT; i++)
	{	
	
		if (i==0)
			glBindTexture(0, g_textureIDS[TEXTURE_BALL04]);
		else if (i==1)
			glBindTexture(0, NULL); // g_textureIDS[TEXTURE_BOX01]
		else
			glBindTexture(0, g_textureIDS[TEXTURE_BALL01]);
	
		b2Vec2 position = g_spriteArray[i].Body->GetOriginPosition();
		float rotation = g_spriteArray[i].Body->GetRotation();
		
		glPushMatrix();
		glTranslatef(position.x * SCALE, position.y * SCALE, -1 + 0.01F);
		glRotatef(rotation * (180 / PI), 0.0f, 0.0f, 1.0f);
	//	glRotatef(degreesToAngle(rotation), 0.0f, 0.0f, 1.0f); // this does not work?
		drawQuad(0.4f, 32, QUADFLAGS_NONE);
		glPopMatrix(1);
	}

	// Update trail display
	glBindTexture(0, g_textureIDS[TEXTURE_BALL05]);
	
	int drawPos = g_trailPos;
	float drawScale = 0.4f;
	float scaleStep = drawScale / (TRAILAMOUNT - 1);

	for (int register i=0; i<TRAILAMOUNT; i++)
	{
		int alphaValue = (((float)(TRAILAMOUNT - i) / (float) TRAILAMOUNT) * 30.0F) + 1;
		glPolyFmt(POLY_ALPHA(alphaValue) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);

		glBegin(GL_QUAD);
		
		drawPos += TRAILINTERVAL - (i / TRAILINTERVAL);	// the (i/ti) is to pull the end sections closer (a kludge)
		if (drawPos > (TRAILINTERVAL * TRAILAMOUNT) - 1) drawPos = drawPos - (TRAILINTERVAL * TRAILAMOUNT);
		glPushMatrix();
		glTranslatef(TrailPoints[drawPos].X, TrailPoints[drawPos].Y, -1 + 0.01F);
		glRotatef(TrailPoints[drawPos].Rot, 0.0f, 0.0f, 1.0f);
		drawQuad(drawScale -= scaleStep, 32, QUADFLAGS_NONE);
		glPopMatrix(1);

		glEnd();
	}
	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);
	
	glBegin(GL_QUAD);
	
	drawMap();
	
	
	/* static RectF viewRect;
	
	viewRect.X = g_cameraPos.X;
	viewRect.Y = g_cameraPos.Y;
	viewRect.Width = 4;
	viewRect.Height = 4;
	
	glBindTexture(0, g_textureIDS[TEXTURE_BALL01]);
	glColorTable(GL_RGB256, g_palAddress[0]);
	
	glPushMatrix();
	glTranslatef(viewRect.X, viewRect.Y, -1 + g_texelSize.Width);
	drawQuad(viewRect.Width, 32, QUADFLAGS_NONE);
	glPopMatrix(1); */
	
	glEnd();
}

void updateCamera()
{
	char buffer[20];
	
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	
	if(g_frameCount++ == 10)
	{
		g_frameCount = 0;
		
		g_cameraStart.X = g_cameraPos.X;
		g_cameraStart.Y = g_cameraPos.Y;
		
		g_cameraEnd.X = position.x / 10.0f;
		g_cameraEnd.Y = position.y / 10.0f;
	}
	
	Vector2 cameraDist(abs((g_cameraEnd.X - g_cameraStart.X) * 30), abs((g_cameraEnd.Y - g_cameraStart.Y) * 30));

	sprintf(buffer, "Cam X %d  ",(int)cameraDist.X);
	DrawString(buffer, 0, 10, true);

	float largest;
	
	if (cameraDist.X >= cameraDist.Y)
		largest = cameraDist.X;
	else
		largest = cameraDist.Y;
	
	if (largest > 10)
		largest = 10;
	
	g_cameraStart.Z = g_cameraPos.Z;
	g_cameraEnd.Z = (largest / 10) + 0.1f;

	g_cameraPos.X = Cubic.EaseOut(g_frameCount, g_cameraStart.X, g_cameraEnd.X - g_cameraStart.X, 100);
	g_cameraPos.Y = Cubic.EaseOut(g_frameCount, g_cameraStart.Y, g_cameraEnd.Y - g_cameraStart.Y, 100);
	g_cameraPos.Z = Cubic.EaseOut(1, g_cameraStart.Z, g_cameraEnd.Z - g_cameraStart.Z, 100);
	
	if(g_cameraPos.X > (g_levelGridSize.Width / 2 + 1.0F))
	{
		g_cameraPos.X = (g_levelGridSize.Width / 2 + 1.0F);
	}
	if(g_cameraPos.X < -(g_levelGridSize.Width / 2 + 1.0F))
	{
		g_cameraPos.X = -(g_levelGridSize.Width / 2 + 1.0F);
	}
	if(g_cameraPos.Y > (g_levelGridSize.Height / 2 - 0.2F)) // 1.8
	{
		g_cameraPos.Y = (g_levelGridSize.Height / 2 - 0.2F);
	}
	if(g_cameraPos.Y < -(g_levelGridSize.Height / 2 - 0.2F))
	{
		g_cameraPos.Y = -(g_levelGridSize.Height / 2 - 0.2F);
	}
	
	sprintf(buffer, "CamPos X:%02.02f Y:%02.02f Z:%02.02f", (float)g_cameraPos.X, (float)g_cameraPos.Y, (float)g_cameraPos.Z);
	DrawString(buffer, 0, 12, true);

	glLoadIdentity();
	
	//gluLookAt(	0, 0,8,		//camera possition
	//			0, 0, 0.0,		//look at
	//			0.0, 1.0, 0.0);		//up

	gluLookAt(	g_cameraPos.X, g_cameraPos.Y, g_cameraPos.Z,		//camera possition
				g_cameraPos.X, g_cameraPos.Y, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
}
