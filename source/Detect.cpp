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

b2Body* getBodyAtPoint(b2Vec2* v)
{
	// Create a small box at point
	b2AABB aabb;
	aabb.minVertex.Set(v->x - 0.001, v->y - 0.001);
	aabb.maxVertex.Set(v->x + 0.001, v->y + 0.001);
	
	// Look at the shapes intersecting this box (max.: 10)
	b2Shape* shapes[10];
	int32 count = g_world->Query(aabb, shapes, 10);

	if (shapes == NULL)
		return NULL;  // No body there...

	for (int i = 0; i < count; i++)
	{
		b2Shape* shape = shapes[i];
		b2Body* body = shape->GetBody();
		
		if(g_spriteArray[0].Body == body)
			continue;
		
		//if (!body->IsStatic())  // Don't pick static shapes
		//{
			// Ensure it is really at this point
			if (shape->TestPoint(*v))
				return body; // Return the first body found
		//}
	}

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
	pChar->OnCeil = false;
	
//	DrawString("                                ", 0, 7, true);

	for (b2ContactNode* contactNode = pChar->Body->GetContactList(); contactNode; contactNode = contactNode->next)
	{
		b2Contact* contact = contactNode->contact;
		
		if (contact->GetManifoldCount() > 0)
		{
			b2Body* body1 = contact->GetShape1()->GetBody();
			b2Body* body2 = contact->GetShape2()->GetBody();
			b2Body* body = NULL;
			
			// Which body is the player?
			if(pChar->Body == body1)
				body = body2;
			else
				body = body1;
			
			/* if(g_groundBody == body) // Collide with ground?
			{
//				DrawString("Player Collided With Ground", 0, 7, true);
				
				b2Manifold* manifold = contact->GetManifolds();
				b2ContactPoint* cp = manifold->points;
				b2Vec2 position1 = cp->position;
				b2Vec2 position2 = pChar->Body->GetOriginPosition();
				
				pChar->OnGround = (position1.y < position2.y);
				pChar->OnCeil = (position1.y > position2.y);
				
			}
			else // Which platform did it collide with?
			{ */
				for(int i=0; i<PLATFORM_COUNT; i++)
				{
					if(g_platformArray[i]->Body == body)
					{
						//static char buf[256];
						//sprintf(buf, "Player Collided With Platform %d   ", i);
						//DrawString(buf, 0, 7, true);
						
						b2Manifold* manifold = contact->GetManifolds();
						b2ContactPoint* cp = manifold->points;
						b2Vec2 position1 = cp->position;
						b2Vec2 position2 = pChar->Body->GetOriginPosition();
						
						pChar->OnGround = (position1.y < position2.y);
						pChar->OnCeil = (position1.y > position2.y);
						
						return;
					}
				}
				
				for(int i=1; i<BALL_COUNT; i++)
				{
					if(g_spriteArray[i].Body == body)
					{
						//static char buf[256];
						//sprintf(buf, "Player Collided With Platform %d   ", i);
						//DrawString(buf, 0, 7, true);
						
						b2Manifold* manifold = contact->GetManifolds();
						b2ContactPoint* cp = manifold->points;
						b2Vec2 position1 = cp->position;
						b2Vec2 position2 = pChar->Body->GetOriginPosition();
						
						pChar->OnGround = (position1.y < position2.y);
					}
				}
			//}
		}
	}
	
/*	if(pChar->OnGround)
		DrawString("Player     On Ground", 0, 8, true);
	else
		DrawString("Player Not On Ground", 0, 8, true);
*/}

void updateGroundCollision()
{
	// Get position of player
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	b2Vec2 v1(position.x - g_spriteArray[0].CircleDef->radius + 0.1f, position.y - g_spriteArray[0].CircleDef->radius - 0.1F);
	b2Vec2 v2(position.x, position.y - g_spriteArray[0].CircleDef->radius - 0.1F);
	b2Vec2 v3(position.x + g_spriteArray[0].CircleDef->radius - 0.1f , position.y - g_spriteArray[0].CircleDef->radius - 0.1F);
	
	// Bottom left of player
	b2Body* body = getBodyAtPoint(&v1);
	
	if(body != NULL)
		g_spriteArray[0].OnGround = true;
	
	// Bottom centre of player
	body = getBodyAtPoint(&v2);
	
	if(body != NULL)
		g_spriteArray[0].OnGround = true;

	// Bottom right of player
	body = getBodyAtPoint(&v3);
	
	if(body != NULL)
		g_spriteArray[0].OnGround = true;
}
