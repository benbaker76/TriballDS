#include "Globals.h"
#include "Detect.h"
#include "Text.h"
#include "Enemies.h"

void updateEnemies()
{
	for(int i=10; i<ENEMY_COUNT+10; i++)
	{
		switch (g_spriteArray[i].Type)
		{
			case ENEMYTYPE_PATROL:
				enemyPatrol	(&g_spriteArray[i]);
			case ENEMYTYPE_BEES:
				enemyBees	(&g_spriteArray[i]);
		}
		
	}
}

void enemyPatrol(Circle *pChar)
/*
Ideas to add to this. If the min and max positions are set to NULL, then the patroller will walk until he finds and
edge of a platform and turn round, also, if he collides with an object, he will also turn.
if min and max are set to values he will walk them areas only, unless he falls, then the min/max becomes null and he
will find his own way.
*/

{
	b2Vec2 Evel = pChar->Body->GetLinearVelocity();
	b2Vec2 Epos = pChar->Body->GetOriginPosition();	
	float Erot = (pChar->Body->GetRotation()) * 180 / PI;
		
	if ((Erot >-30 && Erot <30) && pChar->Active == TRUE)		// between these two, we can say he has fallen over!
	{
		if (pChar->Direction == DIRECTION_RIGHT)				// right
		{
			if (pChar->XSpeed < pChar->XSpeedMax)
			{
				pChar->XSpeed += pChar->Accel;
			}
			if (Epos.x >= pChar->MoveMax * SCALE)
				pChar->Direction = DIRECTION_LEFT;
		}
		else if (pChar->Direction == DIRECTION_LEFT)			// left
		{
			if (pChar->XSpeed > -pChar->XSpeedMax)
			{
				pChar->XSpeed -= pChar->Accel;
			}
			if (Epos.x  <= pChar->MoveMin * SCALE)
				pChar->Direction = DIRECTION_RIGHT;	
		}
			if (pChar->OnGround == FALSE && Evel.y < 0.0f)
			// Stop enemy from falling over
				pChar->Body->SetCenterPosition(pChar->Body->GetCenterPosition(), 0);
			// SET VELOCTIES
			pChar->Body->SetLinearVelocity(b2Vec2(pChar->XSpeed, Evel.y));
	}
	else	// PATROLLER has fallen
	{
			pChar->Active = FALSE; 	// make him inactive
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

void enemyBees(Circle *pChar)
{

}