#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "Control.h"
#include "Detect.h"
#include "Text.h"




void moveCharacter(Circle *pChar)
{	
	b2Vec2 vel = pChar->Body->GetLinearVelocity();
	if (pChar->Type == BALLTYPE_PLAYER)				// PLAYER MOVEMENT
	{
		if (g_input.keysHeld & KEY_LEFT)							// Move left
			pChar->Action = ACTION_MOVELEFT;
		else if (g_input.keysHeld & KEY_RIGHT)						// Move right
			pChar->Action = ACTION_MOVERIGHT;
		else if (pChar->Status != BALLSTATUS_JUMPING)	// No Movement
			pChar->Action = ACTION_SLOW;
			
		if (g_input.keysHeld & KEY_A || g_input.keysHeld & KEY_B || g_reJump >0)			// Check for a jump and init if able
		{
			if (pChar->Status != BALLSTATUS_JUMPING && pChar->OnGround && vel.y < 20.0f)	// We could start a jump
																// vel.y SHOULD be <0.0f, but moving right increases vel.y ? ? ?
			{
				if (g_jumpTrap == FALSE || g_reJump >0 )							// ok, jump
				{
					pChar->JumpCount = 0;
					pChar->Status = BALLSTATUS_JUMPING;								// Set to jumping
					pChar->Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED ));		// set the velocity of the jump
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
	else if (pChar->Type == BALLTYPE_EVILBALL)			// ENEMY BALL MOVEMENT
	{
		if(rand() % 32 == 0) // Only move enemy occasionally
		{
			int Action = rand() % 5;					// (0-4)
			if ((Action == ACTION_NEWJUMP) && (pChar->Status != BALLSTATUS_JUMPING && pChar->OnGround))
			{
				pChar->Status = BALLSTATUS_JUMPING;
				pChar->Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED ));
			}
			else if (Action != ACTION_NEWJUMP)
			{
				pChar->Action = Action;
			}
		}	
	}
}

// Update the character position
void updateCharacter(Circle *pChar)
{

	b2Vec2 vel = pChar->Body->GetLinearVelocity();
	float charAVelocity = pChar->Body->GetAngularVelocity();
	int charAction = pChar->Action;
	int charStatus = pChar->Status;
	
	switch (charAction)
	{
	case ACTION_MOVELEFT:														// MOVE LEFT
		if (pChar->OnGround)
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
		if (pChar->OnGround)
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
		if (pChar->OnCeil && pChar->JumpCount <=44)
		{
		//	vel.y = 1.0f;	// This cannot work as a collision is mate that stops l/r movement, So, if this movement stop did not occur, it would work.
		//	if (charAVelocity > 0) charAVelocity = -charAVelocity;
		}
	
	
		if (vel.y >0.0f) pChar->JumpCount ++;
		if (charAction != ACTION_SLOW)											// if we are moving in a direction the we need to shorten our -
		if (pChar->OnCeil == FALSE) vel.x = vel.x / 1.03f ;							// velocity horizontally to make it more parabolic.
		if(pChar->OnGround && vel.y < 20.0f) pChar->Status = BALLSTATUS_NORMAL;	// if onGround, return to normal control
	} 						// vel.y < 0.0f = it SHOULD be but moving causes strange vel.y effects!

	// Update changes to velocities (x,y and rotation)
	pChar->Body->SetAngularVelocity(charAVelocity);	
	pChar->Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	// Reset movement if this is a Player
	if (pChar->Type == BALLTYPE_PLAYER) pChar->Action = 0;

	// this is to compensate for a slightly lower gravity
	pChar->Body->ApplyImpulse(b2Vec2(0.0, -3.0f), pChar->Body->GetCenterPosition());

}
