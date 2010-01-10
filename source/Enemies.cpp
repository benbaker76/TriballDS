#include "Globals.h"
#include "Detect.h"
#include "Text.h"
#include "Enemies.h"

void updateEnemies()
{
	for(int i=9; i<ENEMY_COUNT+10; i++)
	{
		switch (g_objectArray[i].Type)
		{
			case OBJTYPE_PATROL:
				enemyPatrol	(&g_objectArray[i]);
				break;
			case OBJTYPE_BEE:
				enemyBee (&g_objectArray[i]);
				break;
			default:
				break;
		}
		
	}
}

void enemyPatrol(Object *pObj)
/*
Ideas to add to this. If the min and max positions are set to NULL, then the patroller will walk until he finds and
edge of a platform and turn round, also, if he collides with an object, he will also turn.
if min and max are set to values he will walk them areas only, unless he falls, then the min/max becomes null and he
will find his own way.
*/

{
	b2Vec2 Evel = pObj->Body->GetLinearVelocity();
	b2Vec2 Epos = pObj->Body->GetOriginPosition();	
	float Erot = (pObj->Body->GetRotation()) * 180 / PI;
		
	if ((Erot >-30 && Erot <30) && pObj->Active == TRUE)		// between these two, we can say he has fallen over!
	{
		if (pObj->Direction == DIRECTION_RIGHT)				// right
		{
			if (pObj->XSpeed < pObj->XSpeedMax)
			{
				pObj->XSpeed += pObj->Accel;
			}
			if (Epos.x >= pObj->MoveMax * SCALE)
				pObj->Direction = DIRECTION_LEFT;
		}
		else if (pObj->Direction == DIRECTION_LEFT)			// left
		{
			if (pObj->XSpeed > -pObj->XSpeedMax)
			{
				pObj->XSpeed -= pObj->Accel;
			}
			if (Epos.x  <= pObj->MoveMin * SCALE)
				pObj->Direction = DIRECTION_RIGHT;	
		}
			if (pObj->OnGround == FALSE && Evel.y < 0.0f)
			// Stop enemy from falling over
				pObj->Body->SetCenterPosition(pObj->Body->GetCenterPosition(), 0);
			// SET VELOCTIES
			pObj->Body->SetLinearVelocity(b2Vec2(pObj->XSpeed, Evel.y));
	}
	else	// PATROLLER has fallen
	{
			pObj->Active = FALSE; 	// make him inactive
	}

		
	// ok, we need to detect ahead of us for objects (checking left)
/*	b2Body* detected = getBodyAtPoint(&b2Vec2 (Epos.x - 2.0f, Epos.y));
		
	char buffer[20];
	
	if (detected == NULL)
	{
		sprintf(buffer, "DL NOTHING  ");
		DrawString(buffer, 24, 8, true);
	}
	else
	{
	//	int Dtype = detected.Type;
	
	// give up here, cannot return the type!!
	// need to know what object (ie. a crate etc) we have colided with
		
	}
*/
}

void enemyBee(Object *pObj)
/*
I did not want these to detect platforms so that they could move through them? I could just use GL to draw them, but they may need to
interact?
Also, they will only attack when you get too close, prior to that they will hover in a set area (attracted to something else?)

These will also need some kind of detection to go round platforms if they cannot just pass through, at the least they will need to
go round objects and other enemies, including themselves.

ALSO, they can get stuck under platforms, no idea why they then will not attract to you again? But, need to think of a way to make them go round...
*/
{
	b2Vec2 Evel = pObj->Body->GetLinearVelocity();
	b2Vec2 Epos = pObj->Body->GetOriginPosition();
	b2Vec2 Apos = g_objectArray[0].Body->GetOriginPosition();
	int Attract = pObj->Attract;
	const float TRange= 100 * SCALE;;			// Size of bounding box (i take it that using CONST means that at compile the value is calculated rather than in execution?)
	float Ax;
	float Ay;
	
	// Check if attacking player or area
	if (g_objectArray[Attract].Type != OBJTYPE_PLAYER)
	{				//----------------------------- swarm area
		Ax = g_objectArray[Attract].X * SCALE;
		Ay = g_objectArray[Attract].Y * SCALE;
		// Check if PLAYER is in range (you have to be closer than you need to be to ESCAPE)
		if ((Epos.x - TRange < Apos.x && Epos.x + TRange > Apos.x) && (Epos.y + TRange > Apos.y && Epos.y - TRange < Apos.y))
			{
			pObj->TempStore = pObj->Attract; 	// Store original value for later
			pObj->Attract = 0;					// set to first element (player)
			}
	}
	else
	{				//----------------------------- player area
		Ax = Apos.x;
		Ay = Apos.y;
		// Check if PLAYER is not in range
		if ((Epos.x - TRange * 1.5 > Apos.x || Epos.x + TRange * 1.5 < Apos.x) && (Epos.y + TRange * 1.5 < Apos.y || Epos.y - TRange * 1.5 > Apos.y))
			pObj->Attract = pObj->TempStore;	// set to original attractor
	}
	
	if (Epos.x < Ax)				// Left of attractor - move right
		{
		if (pObj->XSpeed < pObj->XSpeedMax) pObj->XSpeed += pObj->Accel;
		}
	else if (Epos.x > Ax)			// Right of attractor - move left
		{
		if (pObj->XSpeed > -pObj->XSpeedMax) pObj->XSpeed -= pObj->Accel;
		}	
	if (Epos.y < Ay)				// below attractor - move up
		{
		if (pObj->YSpeed < pObj->YSpeedMax) pObj->YSpeed += pObj->Accel;
		}
	else if (Epos.y > Ay)			// above attractor - move down
		{
		if (pObj->YSpeed > -pObj->YSpeedMax) pObj->YSpeed -= pObj->Accel;
		}

	// update movement and direction
	
	if (pObj->XSpeed > 0)
		pObj->Direction = DIRECTION_RIGHT;
	else
		pObj->Direction = DIRECTION_LEFT;
	pObj->Body->SetLinearVelocity(b2Vec2(pObj->XSpeed, pObj->YSpeed));	
}