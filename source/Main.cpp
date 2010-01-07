// ------------------------
// HeadRoll Demo
// ------------------------

#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Box2D.h"

#include "font.h"
//#include "titlescreen.h"
#include "Globals.h"
#include "Control.h"
#include "Text.h"
#include "DrawMap.h"
#include "InitLevel.h"
#include "InitVideo.h"
#include "Detect.h"
#include "SpecialFX.h"
#include "Enemies.h"

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
		
	if (g_spriteArray[0].OnCeil != FALSE)
		DrawString("ON CEIL", 0, 7, true);
	else
		DrawString("NO CIEL", 0, 7, true);
		
	sprintf(buffer, "ReJump %d  JumpCount %d    ",g_reJump,g_spriteArray[0].JumpCount);
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
	initTrail();

	DrawString("Triball, A thing of ours +1!", 0, 0, true);
	DrawString("Great city of slowdown", 0, 4, true);
	DrawString("ALPHA 0.00000009 and a grubnut", 0, 2, true);

	// --------------------------
	
	while(1)
	{
		touchRead(&g_input.touch);
		scanKeys();
			
		g_input.keysHeld = keysHeld();
		g_input.keysDown = keysDown();
		g_input.keysUp = keysUp();
	
		debugText();
		
		for(register int i=0; i<BALL_COUNT; i++)
		{
			if(g_spriteArray[i].Body != NULL)
			{
				moveCharacter(&g_spriteArray[i]);
				updateCharacter(&g_spriteArray[i]);
				updateCharacterContacts(&g_spriteArray[i]);
				updateGroundCollision();
			}
		}
		
		if (g_reJump > 0) g_reJump -= 1;
		
//		b2Vec2 v(g_input.touch.px * SCALE, -g_input.touch.py * SCALE);
//		g_spriteArray[1].Body->SetCenterPosition(v, g_spriteArray[1].Body->GetRotation());
		
		updateCamera();
		
		updateEnemies();


		//updateWorldContacts();
		
		g_world->Step(timeStep, iterations);

		trailUpdate();

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


