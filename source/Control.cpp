#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "Control.h"
#include "Detect.h"
#include "Text.h"
#include "Easing.h"

void movePlayer()
{
	scanKeys();						// Read button data
	int held = keysHeld();			// Used to calculate if a button is down
	
	b2Vec2 vel = g_spriteArray[0].Body->GetLinearVelocity();
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	float lVelocity = g_spriteArray[0].Body->GetAngularVelocity();
	
	char buffer[20];
	sprintf(buffer, "L VEL %d  ",(int)vel.x);
	DrawString(buffer, 0, 21, true);
	sprintf(buffer, "A VEL %d  ",(int)lVelocity);
	DrawString(buffer, 0, 20, true);	
//	g_spriteArray[0].Body->SetCenterPosition(b2Vec2(0,0));	// HOW DO I SET THE ROTATION SPEED MANUALLY??

//	bool Platform = ?? // We will need a TRUE to tell us we are on a platform to improve to control.

	if (held & KEY_LEFT)
	{

	
		if (vel.x > 0)			// We are moving RIGHT, turn quicker
		
		{
		vel.x -= ACCEL * TURNSPEED;
		if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(-IMPX, 0), b2Vec2(0, 0));
		if (lVelocity < ROTMAX ) lVelocity += ROTSPEED;
		}
		else					// We are already moving LEFT
		{
		vel.x -= ACCEL;
		if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(IMPX, 0), b2Vec2(0, 0));
		}
		if (lVelocity < ROTMAX ) lVelocity += ROTSPEED;

		
		
	}
	else if (held & KEY_RIGHT)
	{
		if (vel.x < 0)			// We are moving LEFT, turn quicker
		
		{
		vel.x += ACCEL * TURNSPEED;
		if (vel.x > MAXACCEL) vel.x = MAXACCEL;
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(-IMPX * TURNSPEED, 0), b2Vec2(0, 0));
		if (lVelocity >  -ROTMAX ) lVelocity -= ROTSPEED;
		}
		else					// We are already moving RIGHT
		{
		vel.x += ACCEL;
		if (vel.x > MAXACCEL) vel.x = MAXACCEL;
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(IMPX, 0), b2Vec2(0, 0));
		}
		if (lVelocity >  -ROTMAX ) lVelocity -= ROTSPEED;
	}
	else	// we are not moving LEFT or RIGHT
	{
	/*	if (lVelocity > 0)
			lVelocity -= FRICTION / 2;
		else
			lVelocity += FRICTION / 2;
		if (lVelocity > 0 && lVelocity <= FRICTION) lVelocity = 0;
		else if (lVelocity < 0 && lVelocity >= -FRICTION) lVelocity = 0;
*/		
		if (vel.x > 0)
			vel.x -= FRICTION;
		else
			vel.x += FRICTION;
		if (vel.x > 0 && vel.x < FRICTION) vel.x = 0;
		else if (vel.x < 0 && vel.x > -FRICTION) vel.x = 0;

	};
	g_spriteArray[0].Body->SetAngularVelocity( lVelocity );
	g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));

	// This is going to be tricky?


	if (held & KEY_UP)
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, VELY));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, IMPY), b2Vec2(0, 0));
	}
	else if (held & KEY_DOWN)
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, -VELY));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, -IMPY), b2Vec2(0, 0));
	}
	
	if ((held & KEY_L) && (g_cameraPos.Z > 0.1F))
	{
		g_cameraPos.Z -= 0.1f;

	}
	else if ((held & KEY_R) && (g_cameraPos.Z < 3.9F))
	{
		g_cameraPos.Z += 0.1f;
	}
		
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


/*	if(cameraDist.X < 10 && cameraDist.Y < 10)
	{
		g_cameraStart.Z = g_cameraPos.Z;
		g_cameraEnd.Z = 0.1F;
	}
	else if(cameraDist.X < 15 && cameraDist.Y < 15)
	{
		g_cameraStart.Z = g_cameraPos.Z;
		g_cameraEnd.Z = 0.5F;
	}
	else if(cameraDist.X >= 15 && cameraDist.Y >= 15)
	{
		g_cameraStart.Z = g_cameraPos.Z;
		g_cameraEnd.Z = 1.0F;
	}
*/	
	float largest;
	if (cameraDist.X >= cameraDist.Y) largest = cameraDist.X;
	else largest = cameraDist.Y;
	if (largest > 20) largest = 20;
		g_cameraStart.Z = g_cameraPos.Z;
		g_cameraEnd.Z = (largest / 10) + 0.1f;

	g_cameraPos.X = Cubic.EaseOut(g_frameCount, g_cameraStart.X, g_cameraEnd.X - g_cameraStart.X, 100);
	g_cameraPos.Y = Cubic.EaseOut(g_frameCount, g_cameraStart.Y, g_cameraEnd.Y - g_cameraStart.Y, 100);
	g_cameraPos.Z = Cubic.EaseOut(1, g_cameraStart.Z, g_cameraEnd.Z - g_cameraStart.Z, 100);

	glLoadIdentity();

	gluLookAt(	g_cameraPos.X, g_cameraPos.Y, g_cameraPos.Z,		//camera possition
				g_cameraPos.X, g_cameraPos.Y, 0.0,		//look at
				0.0, 1.0, 0.0);		//up		
		
}


void moveSprite(Sprite *pSprite)

// add control head to this first, only if type == player.
// if type == normal (enemy) then check and set random movement
// we do not need to pass action? Should we add action to the class?

// The problem with this is if we want to move LR and jump, we can't


{
	if (pSprite->Type == BALLTYPE_PLAYER)								// = Player control
	{
		scanKeys();						// Read button data
		int held = keysHeld();			// Used to calculate if a button is down
		
		// checks for left and right movement		(Use L/R and LEFT/RIGHT)
		
		if (held & KEY_L || held & KEY_LEFT)
		{
			pSprite->Action = ACTION_MOVELEFT;
		}
		else if (held & KEY_R || held & KEY_RIGHT)
		{
			pSprite->Action = ACTION_MOVERIGHT;
		}
		else
		{
		pSprite->Action = ACTION_SLOW;
		};
		
		// check for jump.								(USE A)
		
		// check players status for the setting of jump, and if true, do not init until false
		// if jump is held, do not allow another jump!
		if (held & KEY_A || g_reJump == TRUE)
		{
			if (g_jumpTrap == FALSE || g_reJump == TRUE)
			{	// time to jump
				if ((pSprite->Status != BALLSTATUS_JUMPING) && (pSprite->Status != BALLSTATUS_FALLING))
				{
					pSprite->Status = BALLSTATUS_JUMPING;
					pSprite->YSpeed = -JUMPSPEED;
					g_reJump = FALSE;
					g_jumpTrap = TRUE;
				}
				else if (pSprite->YSpeed > 0) //if (g_jumpTrap == FALSE)
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
	else if (pSprite->Type == BALLTYPE_EVILBALL)							// = Random control
	{
		if(rand() % 32 == 0) // Only move enemy occasionally
		{
			// rand() % 5 returns a random value from 0 to 4
			pSprite->Action = rand() % 5;
			if ((pSprite->Action == ACTION_JUMP) && (pSprite->Status != BALLSTATUS_JUMPING && pSprite->Status != BALLSTATUS_FALLING))
			{
				pSprite->Status = BALLSTATUS_JUMPING;
				pSprite->YSpeed = -JUMPSPEED;		
			}
		}
	}
	
	// Act on the 'action' of the ball
	
	switch(pSprite->Action)
	{
	case ACTION_MOVELEFT:													// LEFT
		if ((pSprite->Status == BALLSTATUS_NORMAL) && (pSprite->XSpeed > 0))		// if we are on the ground,
			pSprite->XSpeed = pSprite->XSpeed - (ACCEL * 4);						// then allow a quicker turn
		else
			pSprite->XSpeed = pSprite->XSpeed - (ACCEL * 1.5);						// else, normal turn
		if (pSprite->XSpeed < -MAXACCEL) pSprite->XSpeed = -MAXACCEL;				// stop the speed from going past maximum
		break;
	case ACTION_MOVERIGHT:													// RIGHT
		if ((pSprite->Status == BALLSTATUS_NORMAL) && (pSprite->XSpeed < 0))		// if we are on the ground,
			pSprite->XSpeed = pSprite->XSpeed + (ACCEL * 4);						// then allow a quicker turn
		else
			pSprite->XSpeed = pSprite->XSpeed + (ACCEL * 1.5);								// else, normal turn
		if (pSprite->XSpeed > MAXACCEL) pSprite->XSpeed = MAXACCEL;				// stop the speed from going past maximum
		break;
	case ACTION_SLOW:														// SLOW DOWN
		if (pSprite->XSpeed < 0)													// if speed if negative	
		{
			pSprite->XSpeed = pSprite->XSpeed + FRICTION;							// add friction to the movement
			if (pSprite->XSpeed > 0)												// if speed becomes a positive
			{
				pSprite->XSpeed = 0;												// time to stop
			}
		}
		else if (pSprite->XSpeed > 0)												// if speed is positive
		{
			pSprite->XSpeed = pSprite->XSpeed - FRICTION;							// do the reverse af above!
			if (pSprite->XSpeed < 0)
			{
				pSprite->XSpeed = 0;
			}
		}
		break;
	}
}


//----------------------------------------------------------------------------------
// updateHead the Ball
// Here we update the ball based on YSpeed and XSpeed

void updateSprite(Sprite* pSprite)
{
	float oldSpriteX = pSprite->X;
	float oldSpriteY = pSprite->Y;
	float oldLevelX = g_scrollPos.X;
	float oldXSpeed = pSprite->XSpeed;
	
	pSprite->X = pSprite->X + pSprite->XSpeed;
	float spritePreBounce = pSprite->X;
	
	// Do l/r detection
	// Right
	if (pSprite->X > oldSpriteX)	// we are moving RIGHT
	{
	
		if ((bodyRight(pSprite->X, pSprite->Y, pSprite->Type) == SOLID) || (bodyRight(pSprite->X, pSprite->Y + 8, pSprite->Type) == SOLID) || (bodyRight(pSprite->X, pSprite->Y + 16, pSprite->Type) == SOLID))
		{
			pSprite->X = oldSpriteX;
		//	int XSettle = ((int)(pSprite->X) + (int)scrollCheckX(pSprite->Type));
		//	pSprite->X = ((XSettle >> 3) << 3) - ((int)scrollCheckX(pSprite->Type));

		//	pSprite->X = oldSpriteX;
			pSprite->XSpeed = -abs(pSprite->XSpeed / BOUNCE_X_DEADEN);
		
		};
	
	}
	else if (pSprite->X < oldSpriteX)	// we are moving LEFT
	{
		
		if ((bodyLeft(pSprite->X, pSprite->Y, pSprite->Type) == SOLID) || (bodyLeft(pSprite->X, pSprite->Y + 8, pSprite->Type) == SOLID) || (bodyLeft(pSprite->X, pSprite->Y + 16, pSprite->Type) == SOLID))
		{
			pSprite->X = oldSpriteX;
			pSprite->XSpeed = abs(pSprite->XSpeed / BOUNCE_X_DEADEN);
		
		};
	
	};


/* Meeded perhaps for areas without side walls (if we use them)
	if (pSprite->X + scrollCheckX(pSprite->Type) > LEVEL_WIDTH-BALLSIZE)
	{
		pSprite->X = oldSpriteX;
		pSprite->XSpeed = -abs(pSprite->XSpeed / BOUNCE_X_DEADEN);
	}
	else if (pSprite->X + scrollCheckX(pSprite->Type) < 0)
	{
		pSprite->XSpeed = abs(pSprite->XSpeed / BOUNCE_X_DEADEN);
	}
*/
// ok, now account for the jump, this needs to check Status and see if we are jumping (or bouncing)
// but... It only really needs to work with upward movement!
	
	if ((pSprite->Status == BALLSTATUS_JUMPING) && (pSprite->YSpeed <= 0))
	{
		// jumping up!
		pSprite->Y += pSprite->YSpeed;	// y speed will be negative
		
		pSprite->YSpeed += GRAVITY;	// check if Y speed is now possitive
		
		if ( pSprite->YSpeed >= 0)
		{	// Y speed is +
			pSprite->Status = BALLSTATUS_FALLING;
		}
		else
		{	// Y speed is -
			pSprite->Status = BALLSTATUS_JUMPING;
		}
			// Check above our head
		if (headCentre(pSprite->X, pSprite->Y, pSprite->Type) == SOLID)
		{
			// We need to fall now
			pSprite->YSpeed = 0;
			pSprite->Status = BALLSTATUS_FALLING;
			pSprite->Y = oldSpriteY;
		}
	};
// ok, now we need to check if there is ground below the ball
// use 'feetCentre' to check the centre of a ball, the value is returned!
	
	if (pSprite->Status != BALLSTATUS_JUMPING)	// we already know that we are not jumping!
	{
		if ((feetCentre(pSprite->X, pSprite->Y, pSprite->Type) == BLANK) &&  ((feetRight(pSprite->X, pSprite->Y, pSprite->Type) == BLANK) || (feetRight(pSprite->X, pSprite->Y, pSprite->Type) > PLATFORM))  &&  ((feetLeft(pSprite->X, pSprite->Y, pSprite->Type) == BLANK) || (feetLeft(pSprite->X, pSprite->Y, pSprite->Type) > PLATFORM)))			// not on the floor
		{	// We are falling (ie. not on the floor)
		
			if (pSprite->YSpeed < MAXYSPEED) pSprite->YSpeed += GRAVITY;
			{
			pSprite->Y += pSprite->YSpeed;
			pSprite->Status = BALLSTATUS_FALLING;
			}
			if (feetCentre(pSprite->X, pSprite->Y, pSprite->Type) > BLANK && feetCentre(pSprite->X, pSprite->Y, pSprite->Type) <= PLATFORM)
			{	// We have hit the floor and need to stop bouncing.
				
				DrawString("GROUND", 20, 1, false);
			
				pSprite->YSpeed = 0;
				pSprite->Status = BALLSTATUS_NORMAL;
			}				
		}
		else if ( ((feetRight(pSprite->X, pSprite->Y, pSprite->Type) > BLANK) && (feetRight(pSprite->X, pSprite->Y, pSprite->Type) <= PLATFORM)) && (feetCentre(pSprite->X, pSprite->Y, pSprite->Type) == BLANK))
		{	// This checks if we are part on a platforms edge with the RIGHT of the ball, and Ypos to match

			DrawString("RIGHT ", 22, 1, false);
		
			int XPos = ((((int)pSprite->X -1) + scrollCheckX(pSprite->Type) )) & 7;
	
			int ySettle = int((pSprite->Y) + (int)scrollCheckY(pSprite->Type));
			pSprite->Y = ((ySettle >> 3) << 3) - ((int)scrollCheckY(pSprite->Type));
			
			pSprite->Y += (7-XPos);

			pSprite->XSpeed -= (GRAVITY / 3 + ( (pSprite->YSpeed) / 8));
			pSprite->YSpeed = 0;
			pSprite->Status = BALLSTATUS_NORMAL;			
			
			char buffer[20];	
			sprintf(buffer, "%d X SOF",XPos) ;
			DrawString(buffer, 0, 5, false);				
		}
		else if ( ((feetLeft(pSprite->X, pSprite->Y, pSprite->Type) > BLANK) && (feetLeft(pSprite->X, pSprite->Y, pSprite->Type) <= PLATFORM)) && (feetCentre(pSprite->X, pSprite->Y, pSprite->Type) == BLANK))
		{	// This checks if we are part on a platforms edge with the LEFT of the ball, and Ypos to match

			DrawString("LEFT  ", 22, 1, false);
			
			int XPos = ((((int)pSprite->X) + scrollCheckX(pSprite->Type) )) & 7;
			
			int ySettle = int((pSprite->Y) + (int)scrollCheckY(pSprite->Type));
			pSprite->Y = ((ySettle >> 3) << 3) - ((int)scrollCheckY(pSprite->Type));
			
			pSprite->Y += XPos;
			
			pSprite->XSpeed += (GRAVITY / 3 + ( (pSprite->YSpeed) / 8));
			pSprite->YSpeed = 0;
			pSprite->Status = BALLSTATUS_NORMAL;			
			
			char buffer[20];	
			sprintf(buffer, "%d X LOF",XPos) ;
			DrawString(buffer, 0, 6, false);
		}	
		else  //if (( ( (int)pSprite->Y + (int)g_scrollPos.Y +24) & 7) < MAXYSPEED)										// we are on the floor
		{
	//		DrawString("      ", 20, 1, false);
			// This will settle the ball to a platform, taking into count the Y level position if 
			// the ball is the player.

			int ySettle = (int)((pSprite->Y) + scrollCheckY(pSprite->Type));
			pSprite->Y = ((ySettle >> 3) << 3) - ((int)scrollCheckY(pSprite->Type));
			pSprite->Status = BALLSTATUS_NORMAL;
			
			//
			// Now we need to check our speed and if we are going right and bottom of right check = SOLID and top 2
			// !=Solid, a little bounce?
			
			if ((spritePreBounce > pSprite->X)) // && (oldXSpeed > 1.25))	// we are moving RIGHT
			{
				if ((bodyRight(spritePreBounce, pSprite->Y, pSprite->Type) != SOLID) && (bodyRight(spritePreBounce, pSprite->Y + 8, pSprite->Type) != SOLID) && (bodyRight(spritePreBounce, pSprite->Y + 16, pSprite->Type) == SOLID))
				{
					pSprite->XSpeed = oldXSpeed;
					pSprite->Y -= 0.55;
				//	pSprite->Status = BALLSTATUS_JUMPING;
				//	pSprite->YSpeed = - 2;
				}
			}
			else if ((spritePreBounce < pSprite->X)) // && (oldXSpeed < - 1.25))	// we are moving LEFT
			{
				if ((bodyLeft(spritePreBounce, pSprite->Y, pSprite->Type) != SOLID) && (bodyLeft(spritePreBounce, pSprite->Y + 8, pSprite->Type) != SOLID) && (bodyLeft(spritePreBounce, pSprite->Y + 16, pSprite->Type) == SOLID))
				{
					pSprite->XSpeed = oldXSpeed;
					pSprite->Y -= 0.55;
				//	pSprite->Status = BALLSTATUS_JUMPING;
				//	pSprite->YSpeed = - 2;
				}
			}
			
		}
	};
	//
	// ok, we need to scroll the screen if this is a player ball! (ulp)
	//
/*	if (pSprite->Type == BALLTYPE_PLAYER)
	{
		if (pSprite->X + BALLSIZE > SCREEN_WIDTH - BALLSCROLLX) 	// have we moved (right) into an area that means scroll
		{
			if (g_scrollPos.X < LEVEL_WIDTH - SCREEN_WIDTH)			// if the level X scroll is < the edge,
			{
			g_scrollPos.X = g_scrollPos.X + (pSprite->X - oldSpriteX);	// scroll level
			pSprite->X = oldSpriteX; //(SCREEN_WIDTH - BALLSCROLLX) - BALLSIZE;							// and keep player stationary
			}
			else												// if not,
			{
			g_scrollPos.X = LEVEL_WIDTH - SCREEN_WIDTH;				// let player move and keep scroll stationary
			}
		}
		else if (pSprite->X < BALLSCROLLX)							// have we moved (left) into an area that means scroll				
		{
			if (g_scrollPos.X > 0)									// are we able to scroll?
			{			
			g_scrollPos.X = g_scrollPos.X - (oldSpriteX - pSprite->X);	// if so, scroll map and
			pSprite->X = oldSpriteX;							// keep player stationary.
			if (g_scrollPos.X < 0) g_scrollPos.X = 0;
			}
			else												// otherwise,
			{
	//		g_scrollPos.X = 0;										// keep level stationary
			if (pSprite->X < 0) pSprite->X = 0;					// and allow player to move if possible
			}
		}		
		if (pSprite->Y + BALLSIZE > SCREEN_HEIGHT - BALLSCROLLY)	// these work the same for Y as they did for X
		{
			if (g_scrollPos.Y < LEVEL_HEIGHT - SCREEN_HEIGHT)
			{
			g_scrollPos.Y = g_scrollPos.Y + (pSprite->Y - oldSpriteY);
			pSprite->Y = (SCREEN_HEIGHT - BALLSCROLLY) - BALLSIZE;
			}
			else
			{
			g_scrollPos.Y = LEVEL_HEIGHT - SCREEN_HEIGHT;
			}
		}
		else if (pSprite->Y < BALLSCROLLY)
		{
			if (g_scrollPos.Y > 0)
			{
			g_scrollPos.Y = g_scrollPos.Y - (oldSpriteY - pSprite->Y);
			pSprite->Y = BALLSCROLLY; //oldSpriteY;
			if (g_scrollPos.Y < 0) g_scrollPos.Y = 0;	
			}
			else
			{
			g_scrollPos.Y = 0;
			}
		}		
	
	}
*/	
	// Now we need to rotate the head, based on our movement!
	// using rotateHead to pass, initialX, and currentX to return the angle!
	// wow!! It worked!
	
	// call rotateSprite to rotate the ball based on area moved horizontally
	
	pSprite->Angle += rotateSprite(oldSpriteX, pSprite->X, pSprite->Type, oldLevelX);
	
};

//
// Calculate rotation based on horizontal movement
//
float rotateSprite(float originalX, float currentX, int type, float oldX)	// our rotation function
{
	if (type == BALLTYPE_EVILBALL)
	{
		if (currentX < originalX)
		{
			return degreesToAngle((originalX - currentX) * 4);
		}
		else if (originalX < currentX)
		{
			return -degreesToAngle((currentX - originalX) * 4);
		}	
		return 0;
	}
	else if (type == BALLTYPE_PLAYER)
	{
		if (oldX == g_scrollPos.X)
		{
			if (currentX < originalX)
			{
				return degreesToAngle((originalX - currentX) * 4);
			}
			else if (originalX < currentX)
			{
				return -degreesToAngle((currentX - originalX) * 4);
			}	
			return 0;	
		}
		else
		{
			if (g_scrollPos.X < oldX)
			{
				return degreesToAngle((oldX - g_scrollPos.X) * 4);
			}
			else if (oldX < g_scrollPos.X)
			{
				return -degreesToAngle((g_scrollPos.X - oldX) * 4);
			}	
			return 0;	
		}
	}
	return 0;
}



//
//	Return the value of the scroll possition if this is a playerball
//
int scrollCheckX(int type)
{
	if (type == BALLTYPE_PLAYER) return g_scrollPos.X;
	else return 0;
}
int scrollCheckY(int type)
{
	if (type == BALLTYPE_PLAYER) return g_scrollPos.Y;
	else return 0;
}