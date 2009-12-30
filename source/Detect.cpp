#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "ColMap.h"
#include "Control.h"
#include "Detect.h"
#include "Text.h"

b2Body* getBodyAtPoint(Point* p)
{
	// Create a small box at point
	/* b2Vec2 v = physics.screenToWorld(p->X, p->Y);
	b2AABB aabb;
	aabb.minVertex.Set(v.x - 0.001, v.y - 0.001);
	aabb.maxVertex.Set(v.x + 0.001, v.y + 0.001);
	
	// Look at the shapes intersecting this box (max.: 10)
	b2Shape* shapes[10];
	int32 count = g_world->Query(aabb, shapes, 10);

	if (shapes == NULL)
		return NULL;  // No body there...

	for (int i = 0; i < count; i++)
	{
		b2Shape* shape = shapes[i];
		b2Body* body = shape->GetBody();
		
		if (!body->IsStatic())  // Don't pick static shapes
		{
			b2Vec2 position = body->GetOriginPosition();
		
			// Ensure it is really at this point
			if (shape->TestPoint(position))
				return body; // Return the first body found
		}
	} */

	return NULL;
} 

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

void updateCharacterContacts(Circle *pChar)
{
	pChar->OnGround = false;
	
//	DrawString("                                ", 0, 7, true);

	for (b2ContactNode* contactNode = pChar->ColBody->GetContactList(); contactNode; contactNode = contactNode->next)
	{
		b2Contact* contact = contactNode->contact;
		
		if (contact->GetManifoldCount() > 0)
		{
			b2Body* body1 = contact->GetShape1()->GetBody();
			b2Body* body2 = contact->GetShape2()->GetBody();
			b2Body* body = NULL;
			
			// Which body is the player?
			if(pChar->ColBody == body1)
				body = body2;
			else
				body = body1;
			
			if(g_groundBody == body) // Collide with ground?
			{
//				DrawString("Player Collided With Ground", 0, 7, true);
				
				b2Manifold* manifold = contact->GetManifolds();
				b2ContactPoint* cp = manifold->points;
				b2Vec2 position1 = cp->position;
				b2Vec2 position2 = pChar->ColBody->GetOriginPosition();
				
				pChar->OnGround = (position1.y < position2.y);
				
			}
			else // Which platform did it collide with?
			{
				for(int i=0; i<PLATFORMCOUNT; i++)
				{
					if(g_platformArray[i]->Body == body)
					{
						//static char buf[256];
						//sprintf(buf, "Player Collided With Platform %d   ", i);
						//DrawString(buf, 0, 7, true);
						
						b2Manifold* manifold = contact->GetManifolds();
						b2ContactPoint* cp = manifold->points;
						b2Vec2 position1 = cp->position;
						b2Vec2 position2 = pChar->ColBody->GetOriginPosition();
						
						pChar->OnGround = (position1.y < position2.y);
						
						return;
					}
				}
				
				for(int i=1; i<BALLCOUNT; i++)
				{
					if(g_spriteArray[i].Body == body)
					{
						//static char buf[256];
						//sprintf(buf, "Player Collided With Platform %d   ", i);
						//DrawString(buf, 0, 7, true);
						
						b2Manifold* manifold = contact->GetManifolds();
						b2ContactPoint* cp = manifold->points;
						b2Vec2 position1 = cp->position;
						b2Vec2 position2 = pChar->ColBody->GetOriginPosition();
						
						pChar->OnGround = (position1.y < position2.y);
					}
				}
			}
		}
	}
	
/*	if(pChar->OnGround)
		DrawString("Player     On Ground", 0, 8, true);
	else
		DrawString("Player Not On Ground", 0, 8, true);
*/}