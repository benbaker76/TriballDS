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
	sprintf(buffer, "%d X ",(int)(position.x * 10));
	DrawString(buffer, 10, 21, true);
	sprintf(buffer, "%d Y ",192 - (int)(position.y * 10));
	DrawString(buffer, 16, 21, true);
	sprintf(buffer, "%d Rot",(int)(rotationP *100));
	DrawString(buffer, 16, 20, true);
	
	
	b2Vec2 vel = g_spriteArray[0].Body->GetLinearVelocity();
	float aVelocity = g_spriteArray[0].Body->GetAngularVelocity();
	
	sprintf(buffer, "L VEL %d / V VEL %d    ",(int)(vel.x * 10), (int)(vel.y * 10));
	DrawString(buffer, 0, 19, true);
	sprintf(buffer, "A VEL %d  ",(int)aVelocity);
	DrawString(buffer, 0, 20, true);	
}

int main(void)
{
	consoleDebugInit(DebugDevice_NOCASH);
	//consoleDemoInit();
	
	initBox2D();
	initVideo3D();
	initVideo2D();

	initLevel();
	initPlayer();

	DrawString("HEADRUSH, whatever next?", 0, 0, true);
	DrawString("WELL!! It fucking moves LOL", 0, 23, true);
	DrawString("ALPHA 0.00000005 and a boogie", 0, 2, true);
	
	// --------------------------
	
	while(1)
	{
	
		debugText();
		
		movePlayer();
		updateCamera();
		
		//updateWorldContacts();
		updatePlayerContacts();
	
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


