#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "Control.h"
#include "Detect.h"
#include "Text.h"




void movePlayer()
{
	scanKeys();						// Read button data
	int held = keysHeld();			// Used to calculate if a button is down
	
	b2Vec2 vel = g_spriteArray[0].Body->GetLinearVelocity();

	if (held & KEY_LEFT)
	{
		g_spriteArray[0].Action = ACTION_MOVELEFT;
	}
	else if (held & KEY_RIGHT)
	{
		g_spriteArray[0].Action = ACTION_MOVERIGHT;
	}
	else if (g_spriteArray[0].Status != BALLSTATUS_JUMPING)	// we are not moving LEFT or RIGHT, we need to stop
	{
		g_spriteArray[0].Action = ACTION_SLOW;
	}
//	if (vel.x >= -1.1f && vel.x < 0.75f)			// ******* REMOVE THIS WHEN LEFT DRIFT IS FIXED
//	{
//	g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(0.025f, vel.y));
//	}

// Check for a jump and init if able
	if ((held & KEY_A || held & KEY_B) && (g_spriteArray[0].Status != BALLSTATUS_JUMPING))
	{
		g_spriteArray[0].Status = BALLSTATUS_JUMPING;
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED ));
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
		if (charStatus != BALLSTATUS_JUMPING )
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
		if (charStatus != BALLSTATUS_JUMPING )
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
	}
	if (pChar->Status == BALLSTATUS_JUMPING)							// UPDATE JUMP
	{
		vel.x = vel.x / 1.02f ;				// we need to shorten our linear velocity horizontally to make it more parabolic
		// Check if we have landed!
		if(pChar->OnGround && vel.y >= -1.00f)
		{
			pChar->Status = BALLSTATUS_NORMAL;
		//	vel.y = 0;
		}
	} 


	// Update changes to velocities
	
	pChar->Body->SetAngularVelocity(charAVelocity);	
	pChar->Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	
	
	// Update outer (ColBody) collision circle with new pos and rotation
	pChar->ColBody->SetCenterPosition(pChar->Body->GetCenterPosition(), pChar->Body->GetRotation());

}
/*
void moveCircle(Circle *pCircle)

// add control head to this first, only if type == player.
// if type == normal (enemy) then check and set random movement
// we do not need to pass action? Should we add action to the class?

// The problem with this is if we want to move LR and jump, we can't


{
	if (pCircle->Type == BALLTYPE_PLAYER)								// = Player control
	{
		scanKeys();						// Read button data
		int held = keysHeld();			// Used to calculate if a button is down
		
		// checks for left and right movement		(Use L/R and LEFT/RIGHT)
		
		if (held & KEY_L || held & KEY_LEFT)
		{
			pCircle->Action = ACTION_MOVELEFT;
		}
		else if (held & KEY_R || held & KEY_RIGHT)
		{
			pCircle->Action = ACTION_MOVERIGHT;
		}
		else
		{
		pCircle->Action = ACTION_SLOW;
		};
		
		// check for jump.								(USE A)
		
		// check players status for the setting of jump, and if true, do not init until false
		// if jump is held, do not allow another jump!
		if (held & KEY_A || g_reJump == TRUE)
		{
			if (g_jumpTrap == FALSE || g_reJump == TRUE)
			{	// time to jump
				if ((pCircle->Status != BALLSTATUS_JUMPING) && (pCircle->Status != BALLSTATUS_FALLING))
				{
					pCircle->Status = BALLSTATUS_JUMPING;
					pCircle->YSpeed = -JUMPSPEED;
					g_reJump = FALSE;
					g_jumpTrap = TRUE;
				}
				else if (pCircle->YSpeed > 0) //if (g_jumpTrap == FALSE)
		//	}
		//	else if (g_jumpTrap == FALSE && g_reJump == FALSE)
		//	{
				g_reJump = TRUE;
			}
		}
		else
		{
			g_jumpTrap = FALSE;
		}
	}
	else if (pCircle->Type == BALLTYPE_EVILBALL)							// = Random control
	{
		if(rand() % 32 == 0) // Only move enemy occasionally
		{
			// rand() % 5 returns a random value from 0 to 4
			pCircle->Action = rand() % 5;
			if ((pCircle->Action == ACTION_JUMP) && (pCircle->Status != BALLSTATUS_JUMPING && pCircle->Status != BALLSTATUS_FALLING))
			{
				pCircle->Status = BALLSTATUS_JUMPING;
				pCircle->YSpeed = -JUMPSPEED;		
			}
		}
	}
	
	// Act on the 'action' of the ball
	
	switch(pCircle->Action)
	{
	case ACTION_MOVELEFT:													// LEFT
		if ((pCircle->Status == BALLSTATUS_NORMAL) && (pCircle->XSpeed > 0))		// if we are on the ground,
			pCircle->XSpeed = pCircle->XSpeed - (ACCEL * 4);						// then allow a quicker turn
		else
			pCircle->XSpeed = pCircle->XSpeed - (ACCEL * 1.5);						// else, normal turn
		if (pCircle->XSpeed < -MAXACCEL) pCircle->XSpeed = -MAXACCEL;				// stop the speed from going past maximum
		break;
	case ACTION_MOVERIGHT:													// RIGHT
		if ((pCircle->Status == BALLSTATUS_NORMAL) && (pCircle->XSpeed < 0))		// if we are on the ground,
			pCircle->XSpeed = pCircle->XSpeed + (ACCEL * 4);						// then allow a quicker turn
		else
			pCircle->XSpeed = pCircle->XSpeed + (ACCEL * 1.5);								// else, normal turn
		if (pCircle->XSpeed > MAXACCEL) pCircle->XSpeed = MAXACCEL;				// stop the speed from going past maximum
		break;
	case ACTION_SLOW:														// SLOW DOWN
		if (pCircle->XSpeed < 0)													// if speed if negative	
		{
			pCircle->XSpeed = pCircle->XSpeed + FRICTION;							// add friction to the movement
			if (pCircle->XSpeed > 0)												// if speed becomes a positive
			{
				pCircle->XSpeed = 0;												// time to stop
			}
		}
		else if (pCircle->XSpeed > 0)												// if speed is positive
		{
			pCircle->XSpeed = pCircle->XSpeed - FRICTION;							// do the reverse af above!
			if (pCircle->XSpeed < 0)
			{
				pCircle->XSpeed = 0;
			}
		}
		break;
	}
}
*/