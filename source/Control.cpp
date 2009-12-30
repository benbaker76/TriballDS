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
	pChar->LandTrap = FALSE;
	
	b2Vec2 vel = pChar->Body->GetLinearVelocity();
	if (pChar->Type == BALLTYPE_PLAYER)				// PLAYER MOVEMENT
	{
		if (g_input.keysHeld & KEY_LEFT)							// Move left
			pChar->Action = ACTION_MOVELEFT;
		else if (g_input.keysHeld & KEY_RIGHT)						// Move right
			pChar->Action = ACTION_MOVERIGHT;
		else if (pChar->Status != BALLSTATUS_JUMPING)	// No Movement
			pChar->Action = ACTION_SLOW;
			
		if (g_input.keysHeld & KEY_A || g_input.keysHeld & KEY_B || g_reJump == TRUE)				// Check for a jump and init if able
		{
			if (pChar->Status != BALLSTATUS_JUMPING && pChar->OnGround && vel.y > -1.0f)	// We could start a jump
			{
				if (g_jumpTrap == FALSE || g_reJump == TRUE)								// ok, jump
				{
					pChar->Status = BALLSTATUS_JUMPING;
					pChar->LandTrap = TRUE;
					pChar->Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED ));
					g_jumpTrap = TRUE;
					if (g_reJump == TRUE) g_reJump = FALSE;
					//	pChar->Body->ApplyImpulse(b2Vec2(0.0, 250.0), pChar->Body->GetCenterPosition());
					//	pChar->Body->ApplyForce(b2Vec2(0.0f,10.0f),b2Vec2(0.0f,0.0f) );
				}
			}
			else if (g_jumpTrap == FALSE && vel.y < 1.0f)									// if we cant jump, then
			{
				g_reJump = TRUE;															// store a jump for when we can
			}
		}
		else
		{
			g_jumpTrap = FALSE;																// not pressing jump, so clear flag
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
				pChar->LandTrap = TRUE;
				pChar->Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED ));
			}
			if (Action == ACTION_NEWJUMP) Action = pChar->Action;	// retain current direction
			pChar->Action = Action;
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
	
	if (charAction == ACTION_MOVELEFT)									// MOVE LEFT
	{
		if (pChar->OnGround)
		{
			if (vel.x > 0)			// We are moving RIGHT, turn quicker
			{
				vel.x -= ACCEL * TURNSPEED;
				if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
			}
			else					// We are already moving LEFT
			{
				vel.x -= ACCEL;
				if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
			}
			if (charAVelocity < ROTMAX ) charAVelocity += ROTSPEED;
		}
		else						// we are jumping and trying to move LEFT
		{	
			vel.x -= ACCEL /2 ;
			if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
			if (charAVelocity < ROTMAX ) charAVelocity += ROTSPEED * AIRSPIN;
		}
	}
	else if (charAction == ACTION_MOVERIGHT)							// MOVE RIGHT
	{
		if (pChar->OnGround)
		{
			if (vel.x < 0)			// We are moving LEFT, turn quicker
			{
				vel.x += ACCEL * TURNSPEED;
				if (vel.x > MAXACCEL) vel.x = MAXACCEL;
			}
			else					// We are already moving RIGHT
			{
				vel.x += ACCEL;
				if (vel.x > MAXACCEL) vel.x = MAXACCEL;
			}
			if (charAVelocity > ROTMAX ) charAVelocity -= ROTSPEED;
		}
		else						// we are jumping and trying to move RIGHT
		{	
			vel.x += ACCEL /2 ;
			if (vel.x > MAXACCEL) vel.x = MAXACCEL;
			if (charAVelocity > -ROTMAX ) charAVelocity -= ROTSPEED * AIRSPIN;
		}
	}
	else if (charAction == ACTION_SLOW)									// SLOW DOWN
	{
		float oldvel = vel.x;
		if (vel.x > 0.0f)
			{
			vel.x -= FRICTION / 2;
			if (oldvel > 0 && vel.x < 0) 
				vel.x = 0;
			}
		else
			{
			vel.x += FRICTION / 2;
			if (oldvel < 0 && vel.x > 0)
				vel.x = 0;
			}
	};
	if (charStatus == BALLSTATUS_JUMPING)							// UPDATE JUMP
	{
		vel.x = vel.x / 1.02f ;				// we need to shorten our linear velocity horizontally to make it more parabolic
		// Check if we have landed!
		if(pChar->OnGround && pChar->LandTrap == FALSE && vel.y < 1.0f)		// This will not work correctly as we need to set vel.y
		{											// to 0 when landed, and doing so stops another jump,
			pChar->Status = BALLSTATUS_NORMAL;		// as a new jump is instantly killed by groundcheck
													// I have an idea that I will implement when plats are in!
		//	if (vel.y > JUMPSPEED)
		//	vel.y = 0;							// uncomment to try
		}
	} 

	// Update changes to velocities
	pChar->Body->SetAngularVelocity(charAVelocity);	
	pChar->Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	// Reset movement if Player
	if (pChar->Type == BALLTYPE_PLAYER) pChar->Action = 0;		
	// Update outer (ColBody) collision circle with new pos and rotation
	pChar->ColBody->SetCenterPosition(pChar->Body->GetCenterPosition(), pChar->Body->GetRotation());

}
// removed for now!
//	if (vel.x >= -1.1f && vel.x < 0.75f)			// ******* REMOVE THIS WHEN LEFT DRIFT IS FIXED
//	{
//	g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(0.025f, vel.y));
//	}
