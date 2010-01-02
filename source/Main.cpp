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
#include "InitLevel.h"
#include "InitVideo.h"
#include "Detect.h"

void debugText()
{
	char buffer[20];
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	float rotationP = g_spriteArray[0].Body->GetRotation();
	sprintf(buffer, "X: %d,Y: %d   ",(int)(position.x * 10),192 - (int)(position.y * 10));
	DrawString(buffer, 0, 21, true);
	sprintf(buffer, "Rotation Value: %d        ",(int)(rotationP *10));
	DrawString(buffer, 0, 22, true);
	
	
	b2Vec2 vel = g_spriteArray[0].Body->GetLinearVelocity();
	float aVelocity = g_spriteArray[0].Body->GetAngularVelocity();
	
	sprintf(buffer, "X VEL %d / Y VEL %d    ",(int)(vel.x * 10), (int)(vel.y * 10));
	DrawString(buffer, 0, 19, true);
	sprintf(buffer, "A VEL %d  ",(int)aVelocity);
	DrawString(buffer, 0, 20, true);	

	sprintf(buffer, "Status %d  ",g_spriteArray[0].Status);
	DrawString(buffer, 0, 18, true);	
	
	if (g_spriteArray[0].OnGround != FALSE)
		DrawString("ON GROUND", 0, 6, true);
	else
		DrawString("NO GROUND", 0, 6, true);
		
	sprintf(buffer, "ReJump %d  ",g_reJump);
	DrawString(buffer, 0, 16, true);	
}

int main(void)
{
	consoleDebugInit(DebugDevice_NOCASH);
	//consoleDemoInit();
	
	lcdMainOnBottom();
	
	initBox2D();
	initVideo3D();
	initVideo2D();

	initLevel();
	initPlayer();

	DrawString("Triball, whatever next?", 0, 0, true);
	DrawString("Little detect problem...", 0, 4, true);
	DrawString("ALPHA 0.00000007 and an winkle", 0, 2, true);

	// --------------------------
	
	while(1)
	{
		touchRead(&g_input.touch);
		scanKeys();
			
		g_input.keysHeld = keysHeld();
		g_input.keysDown = keysDown();
		g_input.keysUp = keysUp();
	
		debugText();
		
		for(register int i=0; i<BALLCOUNT; i++)
		{
			moveCharacter(&g_spriteArray[i]);
			updateCharacter(&g_spriteArray[i]);
			updateCharacterContacts(&g_spriteArray[i]);	
		}
		
		if (g_reJump > 0) g_reJump -= 1;
		
//		b2Vec2 v(g_input.touch.px * SCALE, -g_input.touch.py * SCALE);
//		g_spriteArray[1].Body->SetCenterPosition(v, g_spriteArray[1].Body->GetRotation());
		
		updateCamera();


		//updateWorldContacts();
		
		g_world->Step(timeStep, iterations);

		// update to trail
		b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
		TrailPoints[g_trailPos].X = position.x * SCALE;			// do SCALE here as it is quicker to do once here
		TrailPoints[g_trailPos].Y = position.y * SCALE;			// than for all in the draw loop
		TrailPoints[g_trailPos].Rot = g_spriteArray[0].Body->GetRotation() * (180 / PI);	// same for rotation (if needed)
	
		g_trailPos -= 1;
		if (g_trailPos < 0) g_trailPos = (TRAILINTERVAL * TRAILAMOUNT) - 1;

		glMatrixMode(GL_MODELVIEW);			// what is this needed for, commenting out does nothing?

		glPushMatrix();
		drawGLScene();
		glPopMatrix(1);
			
		glFlush(0);

		// Wait for vblank
		swiWaitForVBlank();
		
		//loadTextures();
	}

	// Will never reach here
	return 0;
}


