#include "Globals.h"

void updateEnemies()
{
	for(int i=10; i<ENEMY_COUNT+10; i++)
	{
		b2Vec2 Evel = g_spriteArray[i].Body->GetLinearVelocity();
		b2Vec2 Epos = g_spriteArray[i].Body->GetOriginPosition();
		
		float Erot = (g_spriteArray[i].Body->GetRotation()) * 180 / PI;
		
		if ((Erot >-60 && Erot <60) && g_spriteArray[i].Active == TRUE)		// between these two, we can say he has fallen over!
		{
			if (g_spriteArray[i].Movement == 1)		// right
			{
				if (g_spriteArray[i].XSpeed < g_spriteArray[i].XSpeedMax)
				{
					g_spriteArray[i].XSpeed += g_spriteArray[i].Accel;
				}
				if (Epos.x >= g_spriteArray[i].MoveMax * SCALE)
					g_spriteArray[i].Movement = 0;
			}
			else if (g_spriteArray[i].Movement == 0)		// left
			{
				if (g_spriteArray[i].XSpeed > -g_spriteArray[i].XSpeedMax)
				{
					g_spriteArray[i].XSpeed -= g_spriteArray[i].Accel;
				}
				if (Epos.x  <= g_spriteArray[i].MoveMin * SCALE)
					g_spriteArray[i].Movement = 1;
			
			}
			// SET VELOCTIES
			g_spriteArray[i].Body->SetLinearVelocity(b2Vec2(g_spriteArray[i].XSpeed, Evel.y));
	
		}
		else
		{
		//	g_spriteArray[i].Body->setFriction = 1.0F; // HELP - HOW TO CHANGE CURRENT BODIES FRICTION
			g_spriteArray[i].Active = FALSE; 
		}
	}
}