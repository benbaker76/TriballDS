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

void updateWorldContacts()
{
	DrawString("               ", 0, 8, true);

	for (b2Contact* contact = g_world->GetContactList(); contact; contact = contact->GetNext())
	{
		if (contact->GetManifoldCount() > 0)
		{
			// These two bodies have collided
			
			//b2Body* body1 = contact->GetShape1()->GetBody();
			//b2Body* body2 = contact->GetShape2()->GetBody();
			
			//fprintf(stderr, "World Collision!");
			//DrawString("World Collision", 0, 8, true);
		}
	}
}

void updatePlayerContacts()
{
	g_spriteArray[0].OnGround = false;
	
	DrawString("                                ", 0, 7, true);

	for (b2ContactNode* contactNode = g_spriteArray[0].Body->GetContactList(); contactNode; contactNode = contactNode->next)
	{
		b2Contact* contact = contactNode->contact;
		
		if (contact->GetManifoldCount() > 0)
		{
			b2Body* body1 = contact->GetShape1()->GetBody();
			b2Body* body2 = contact->GetShape2()->GetBody();
			b2Body* bodyPlatform = NULL;
			
			// Which body is the player?
			if(g_spriteArray[0].Body == body1)
				bodyPlatform = body2;
			else
				bodyPlatform = body1;
			
			// Which platform did it collide with?
			for(int i=0; i<PLATFORMCOUNT; i++)
			{
				if(g_platformArray[i]->Body == bodyPlatform)
				{
					static char buf[256];
					sprintf(buf, "Player Collided With Platform %d   ", i);
					DrawString(buf, 0, 7, true);
					
					b2Manifold* manifold = contact->GetManifolds();
					b2ContactPoint* cp = manifold->points;
					b2Vec2 position1 = cp->position;
					b2Vec2 position2 = g_spriteArray[0].Body->GetOriginPosition();
					
					g_spriteArray[0].OnGround = (position1.y < position2.y);
				}
			}
		}
	}
	
	if(g_spriteArray[0].OnGround)
		DrawString("Player     On Ground", 0, 8, true);
	else
		DrawString("Player Not On Ground", 0, 8, true);
}

void movePlayer()
{
	scanKeys();						// Read button data
	int held = keysHeld();			// Used to calculate if a button is down
	
	b2Vec2 vel = g_spriteArray[0].Body->GetLinearVelocity();
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	float aVelocity = g_spriteArray[0].Body->GetAngularVelocity();
//	int contact = getBodyAtPoint(position.x, position.y+ 1.8f, true);//Under
	
	char buffer[20];
	sprintf(buffer, "L VEL %d  ",(int)vel.x);
	DrawString(buffer, 0, 21, true);
	sprintf(buffer, "A VEL %d  ",(int)aVelocity);
	DrawString(buffer, 0, 20, true);	

//	bool Platform = ?? // We will need a TRUE to tell us we are on a platform to improve to control.

	if (held & KEY_LEFT)
	{

	
		if (vel.x > 0)			// We are moving RIGHT, turn quicker
		
		{
			vel.x -= ACCEL * TURNSPEED;
			if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	//		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(-IMPX, 0), b2Vec2(0, 0));
			if (aVelocity < ROTMAX ) aVelocity += ROTSPEED;
		}
		else					// We are already moving LEFT
		{
			vel.x -= ACCEL;
			if (vel.x < -MAXACCEL) vel.x = -MAXACCEL;
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	//		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(-IMPX, 0), b2Vec2(0, 0));
		}
		if (aVelocity < ROTMAX ) aVelocity += ROTSPEED;
		g_spriteArray[0].Body->SetAngularVelocity( aVelocity );
		
	}
	else if (held & KEY_RIGHT)
	{
		if (vel.x < 0)			// We are moving LEFT, turn quicker
		
		{
			vel.x += ACCEL * TURNSPEED;
			if (vel.x > MAXACCEL) vel.x = MAXACCEL;
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	//		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(IMPX * TURNSPEED, 0), b2Vec2(0, 0));
			if (aVelocity >  -ROTMAX ) aVelocity -= ROTSPEED;
		}
		else					// We are already moving RIGHT
		{
			vel.x += ACCEL;
			if (vel.x > MAXACCEL) vel.x = MAXACCEL;
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	//		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(IMPX, 0), b2Vec2(0, 0));
		}
		if (aVelocity >  -ROTMAX ) aVelocity -= ROTSPEED;
		g_spriteArray[0].Body->SetAngularVelocity( aVelocity );
	}
	else	// we are not moving LEFT or RIGHT
	{
	/*	if (aVelocity > 0)
			aVelocity -= FRICTION / 2;
		else
			aVelocity += FRICTION / 2;
		if (aVelocity > 0 && aVelocity <= FRICTION) aVelocity = 0;
		else if (aVelocity < 0 && aVelocity >= -FRICTION) aVelocity = 0;
*/		

		// The problem with doing this is we now will not roll down an incline
		// How the hell to we work round that?

/*
		// Settle the linear velocity so that we can stop rolling
		if (vel.x > 0)
			vel.x -= FRICTION;
		else
			vel.x += FRICTION;
		if (vel.x > 0 && vel.x <= (FRICTION)) vel.x = 0;
		else if (vel.x < 0 && vel.x >= -(FRICTION)) vel.x = 0;
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
*/
	};


	// This is going to be tricky?


	if (held & KEY_UP)
	{
		if(g_spriteArray[0].OnGround)
		{
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, JUMPSPEED));
			g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, IMPY), b2Vec2(0, 0));
		}
	}
	else if (held & KEY_DOWN)
	{
		g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, -VELY));
		g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, -IMPY), b2Vec2(0, 0));
	}	
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
	
	if(g_cameraPos.X > 1)
	{
		g_cameraPos.X = 1;
	}
	if(g_cameraPos.X < -1)
	{
		g_cameraPos.X = -1;
	}
	if(g_cameraPos.Y > 1.8F)
	{
		g_cameraPos.Y = 1.8F;
	}
	if(g_cameraPos.Y < -1.8F)
	{
		g_cameraPos.Y = -1.8F;
	}
	
	sprintf(buffer, "CamPos X:%02.2f Y:%02.2f Z:%02.2f", (float)g_cameraPos.X, (float)g_cameraPos.Y, (float)g_cameraPos.Z);
	DrawString(buffer, 0, 12, true);

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
