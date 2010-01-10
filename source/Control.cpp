#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "Control.h"
#include "Detect.h"
#include "Text.h"




void moveCharacter(Object *pObj)
{	
	b2Vec2 vel = pObj->Body->GetLinearVelocity();
	if (pObj->Type == OBJTYPE_PLAYER)				// PLAYER MOVEMENT
	{
		if (g_input.keysHeld & KEY_LEFT)							// Move left
			pObj->Action = ACTION_MOVELEFT;
		else if (g_input.keysHeld & KEY_RIGHT)						// Move right
			pObj->Action = ACTION_MOVERIGHT;
		else if (pObj->Status != BALLSTATUS_JUMPING)	// No Movement
			pObj->Action = ACTION_SLOW;
			
		if (g_input.keysHeld & KEY_A || g_input.keysHeld & KEY_B || g_reJump >0)			// Check for a jump and init if able
		{
			if (pObj->Status != BALLSTATUS_JUMPING && pObj->OnGround && vel.y < 20.0f)	// We could start a jump
																// vel.y SHOULD be <0.0f, but moving right increases vel.y ? ? ?
			{
				if (g_jumpTrap == FALSE || g_reJump >0 )							// ok, jump
				{
					pObj->JumpCount = 0;
					pObj->Status = BALLSTATUS_JUMPING;								// Set to jumping
					pObj->Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED ));		// set the velocity of the jump
					g_jumpTrap = TRUE;												// trap 'Jump Button' from releasing
					if (g_reJump > 0) g_reJump = 0;									// if this was caused by a buffered jump, clear it
				}
			}
			else if (g_jumpTrap == FALSE)											// if we cant jump, then check if a buffered jump is free
			{
				g_reJump = 45;														// Set the timer for a reJump (30 seems about right)
				g_jumpTrap = TRUE;													// Trap the jump again
			}	
		}
		else
		{
			g_jumpTrap = FALSE;														// not pressing jump, so clear flag
		}
	}
	else if (pObj->Type == OBJTYPE_EVILBALL)			// ENEMY BALL MOVEMENT
	{
		if(rand() % 32 == 0) // Only move enemy occasionally
		{
			int Action = rand() % 5;					// (0-4)
			if ((Action == ACTION_NEWJUMP) && (pObj->Status != BALLSTATUS_JUMPING && pObj->OnGround))
			{
				pObj->Status = BALLSTATUS_JUMPING;
				pObj->Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED ));
			}
			else if (Action != ACTION_NEWJUMP)
			{
				pObj->Action = Action;
			}
		}	
	}
}

// Update the character position
void updateCharacter(Object *pObj)
{

	b2Vec2 vel = pObj->Body->GetLinearVelocity();
	float charAVelocity = pObj->Body->GetAngularVelocity();
	int charAction = pObj->Action;
	int charStatus = pObj->Status;
	
	switch (charAction)
	{
	case ACTION_MOVELEFT:														// MOVE LEFT
		if (pObj->OnGround)
		{
			if (vel.x > 0)			// We are moving RIGHT, turn quicker
				vel.x -= ACCEL * TURNSPEED;
			else					// We are already moving LEFT
				vel.x -= ACCEL;
			if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;					// Make sure we do not move too fast
			if (charAVelocity < ROTMAX ) charAVelocity += ROTSPEED;	// Make sure we do not spin too fast
		}
		else						// trying to move LEFT while in the air
		{	
			vel.x -= ACCEL / 1.75 ;
			if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
			if (charAVelocity < ROTMAX ) charAVelocity += ROTSPEED * AIRSPIN;
		}
		break;
	case ACTION_MOVERIGHT:														// MOVE RIGHT
		if (pObj->OnGround)
		{
			if (vel.x < 0)			// We are moving LEFT, turn quicker
				vel.x += ACCEL * TURNSPEED;
			else					// We are already moving RIGHT
				vel.x += ACCEL;
			if (vel.x > MAXACCEL) vel.x = MAXACCEL;					// Make sure we do not move too fast
			if (charAVelocity > ROTMAX ) charAVelocity -= ROTSPEED;	// Make sure we do not spin too fast
		}
		else						// trying to move RIGHT while in the air
		{	
			vel.x += ACCEL / 1.75 ;
			if (vel.x > MAXACCEL) vel.x = MAXACCEL;
			if (charAVelocity > -ROTMAX ) charAVelocity -= ROTSPEED * AIRSPIN;
		}
		break;
	case ACTION_SLOW:															// SLOW DOWN
		float oldvel = vel.x;
		if (vel.x > 0.0f)
			vel.x -= FRICTION / 2;
		else
			vel.x += FRICTION / 2;
			if (oldvel > 0 && vel.x < 0) vel.x = 0;

		break;
	}
	if (charStatus == BALLSTATUS_JUMPING)										// UPDATE JUMP
	{
		// ok, we need to check if hit ceiling and jumpcount < 44 (centre of jump) and SOMEHOW retain Y position
		if (pObj->OnCeil && pObj->JumpCount <=44)
		{
		//	vel.y = 1.0f;	// This cannot work as a collision is mate that stops l/r movement, So, if this movement stop did not occur, it would work.
		//	if (charAVelocity > 0) charAVelocity = -charAVelocity;
		}
	
	
		if (vel.y >0.0f) pObj->JumpCount ++;
		if (charAction != ACTION_SLOW)											// if we are moving in a direction the we need to shorten our -
		if (pObj->OnCeil == FALSE) vel.x = vel.x / 1.03f ;							// velocity horizontally to make it more parabolic.
		if(pObj->OnGround && vel.y < 20.0f) pObj->Status = BALLSTATUS_NORMAL;	// if onGround, return to normal control
	} 						// vel.y < 0.0f = it SHOULD be but moving causes strange vel.y effects!

	// Update changes to velocities (x,y and rotation)
	pObj->Body->SetAngularVelocity(charAVelocity);	
	pObj->Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	// Reset movement if this is a Player
	if (pObj->Type == OBJTYPE_PLAYER) pObj->Action = 0;

	// this is to compensate for a slightly lower gravity
	pObj->Body->ApplyImpulse(b2Vec2(0.0, -3.0f), pObj->Body->GetCenterPosition());

}
