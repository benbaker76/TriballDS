#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Box2D.h"
#include "level01_tex_bin.h"
#include "level01_pal_bin.h"
#include "ball_tex_bin.h"
#include "ball_pal_bin.h"

#include "font.h"
#include "titlescreen.h"
#include "Globals.h"
#include "Control.h"
#include "Text.h"
#include "DrawMap.h"
#include "InitVideo.h"
#include "Levels.h"

void initLevel()
{
	// position of scroll area
	g_scrollPos.X = 0;
	g_scrollPos.Y = 0;
	
	// box2d update vars
	timeStep = 1.0f / 60.0f;
	iterations = 1;
	
	g_cameraPos.X = 0;
	g_cameraPos.Y = 0;
	g_cameraPos.Z = 1.0f;
	
	g_cameraStart.X = 0;
	g_cameraStart.Y = 0;
	g_cameraStart.Z = 1.0f;

	g_cameraEnd.X = 0;
	g_cameraEnd.Y = 0;
	g_cameraEnd.Z = 1.0f;
	
	g_levelNum = 0;
	
	loadLevel();
	
	g_worldAABB = new b2AABB();
	g_worldAABB->minVertex.Set(-(g_levelGridSize.Width * 10.0F), -(g_levelGridSize.Height * 10.0F));
	g_worldAABB->maxVertex.Set(g_levelGridSize.Width * 10.0F, g_levelGridSize.Height * 10.0F);
	g_gravity = new b2Vec2(0.0f, -40.0f);								// -30 for Gravity seems to work best (better than -50)
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(g_levelGridSize.Width * 10.0f, 1.0f);
	g_groundBoxDef->density = 0.0f;
	g_groundBoxDef->friction = 1.0f;
	g_groundBoxDef->restitution = 0.0F;	

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -(g_levelGridSize.Height * 10.0f - 0.5F));
	g_groundBodyDef->AddShape(g_groundBoxDef);

// *** NO IDEA WHY THIS CAUSES PROBLEMS FOR BOX2D!
//	g_groundBody = g_world->CreateBody(g_groundBodyDef);
	
	b2BoxDef* ceilBoxDef = new b2BoxDef();
	ceilBoxDef->extents.Set(g_levelGridSize.Width * 10.0f, 0.5f);
	ceilBoxDef->density = 0.0f;
	ceilBoxDef->friction = 1.0f;
	ceilBoxDef->restitution = 0.2F;		
	
	b2BodyDef* ceilBodyDef = new b2BodyDef(); 
	ceilBodyDef->position.Set(0.0f, g_levelGridSize.Height * 10.0f);
	ceilBodyDef->AddShape(ceilBoxDef);

	g_world->CreateBody(ceilBodyDef);
	
	// test defines for now! ( -40 to 40 for some reason??)
	
	b2BoxDef* wallBoxDef = new b2BoxDef();
	wallBoxDef->extents.Set(1.0f, g_levelGridSize.Height * 10.0f);
	wallBoxDef->density = 0.0f;
	wallBoxDef->friction = 1.0f;
	wallBoxDef->restitution = 0.8F;
	
	b2BodyDef* wallBodyDef = new b2BodyDef();
	wallBodyDef->position.Set(-(g_levelGridSize.Width * 10.0f), 0.0f);
	wallBodyDef->AddShape(wallBoxDef);
	g_world->CreateBody(wallBodyDef);	
	
	wallBodyDef = new b2BodyDef();
	wallBodyDef->position.Set(g_levelGridSize.Width * 10.0f, 0.0f);
	wallBodyDef->AddShape(wallBoxDef);
	g_world->CreateBody(wallBodyDef);
	
	Level1();				// get geometry for level 1
	
}

// INIT PLAYER AND RANDOM BALLS (FOR NOW)
void initPlayer()
{
/*	for(int i=2; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = (rand() % 760)-400; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_spriteArray[i].Y =  192 - ((rand() % 100)); //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_spriteArray[i].Type = BALLTYPE_EVILBALL;
		
		g_spriteArray[i].CircleDef = new b2CircleDef();
		g_spriteArray[i].BodyDef =  new b2BodyDef();
		
		g_spriteArray[i].CircleDef->radius = 48 / 2 * SCALE; 
		g_spriteArray[i].CircleDef->density = 1.0F; 
		g_spriteArray[i].CircleDef->friction = 1.0F; 
		g_spriteArray[i].CircleDef->restitution = 0.4F; 

		g_spriteArray[i].BodyDef->position.Set(g_spriteArray[i].X * SCALE, g_spriteArray[i].Y * SCALE);
		g_spriteArray[i].BodyDef->AddShape(g_spriteArray[i].CircleDef);
		
		g_spriteArray[i].Body = g_world->CreateBody(g_spriteArray[i].BodyDef);
	}
	*/
	// INIT PLAYER
	g_spriteArray[0].Action = ACTION_NONE;	
	g_spriteArray[0].X = 128;
	g_spriteArray[0].Y = 120;
	g_spriteArray[0].Type = BALLTYPE_PLAYER;
	
	g_spriteArray[0].CircleDef = new b2CircleDef();
	g_spriteArray[0].BodyDef =  new b2BodyDef();
	
	g_spriteArray[0].CircleDef->radius = 48 / 2 * SCALE; 
	g_spriteArray[0].CircleDef->density = 1.0F; 
	g_spriteArray[0].CircleDef->friction = 1.0F; 
	g_spriteArray[0].CircleDef->restitution = 0.4F; 

	g_spriteArray[0].BodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
	g_spriteArray[0].BodyDef->allowSleep = false;
	g_spriteArray[0].BodyDef->preventRotation = false;
	g_spriteArray[0].BodyDef->angularDamping = 0.03f;
	g_spriteArray[0].BodyDef->linearDamping = 0.00f;

	g_spriteArray[0].BodyDef->AddShape(g_spriteArray[0].CircleDef);
	
	g_spriteArray[0].Body = g_world->CreateBody(g_spriteArray[0].BodyDef);

		// me playing with a box


	g_spriteArray[1].BoxDef = new b2BoxDef();
	g_spriteArray[1].BodyDef =  new b2BodyDef();
	g_spriteArray[1].X = 220;
	g_spriteArray[1].Y = 260;	
	g_spriteArray[1].Type = BALLTYPE_CRATE;
	g_spriteArray[1].BoxDef->extents.Set(64 / 2 * SCALE, 64 / 2 * SCALE);
	g_spriteArray[1].BoxDef->density = 0.3F; 
	g_spriteArray[1].BoxDef->friction = 0.05F; 
	g_spriteArray[1].BoxDef->restitution = 0.95F; 

	g_spriteArray[1].BodyDef->position.Set(g_spriteArray[1].X * SCALE, g_spriteArray[1].Y * SCALE);
	g_spriteArray[1].BodyDef->allowSleep = true;
	g_spriteArray[1].BodyDef->preventRotation = false;
	g_spriteArray[1].BodyDef->AddShape(g_spriteArray[1].BoxDef);
	g_spriteArray[1].Body = g_world->CreateBody(g_spriteArray[1].BodyDef);
	
	
	g_spriteArray[2].BoxDef = new b2BoxDef();
	g_spriteArray[2].BodyDef =  new b2BodyDef();
	g_spriteArray[2].X = 320;
	g_spriteArray[2].Y = 260;	
	g_spriteArray[2].Type = BALLTYPE_CRATE;
	g_spriteArray[2].BoxDef->extents.Set(64 / 2 * SCALE, 64 / 2 * SCALE);
	g_spriteArray[2].BoxDef->density = 0.3F; 
	g_spriteArray[2].BoxDef->friction = 0.05F; 
	g_spriteArray[2].BoxDef->restitution = 0.95F; 
	g_spriteArray[2].BodyDef->position.Set(g_spriteArray[2].X * SCALE, g_spriteArray[2].Y * SCALE);
	g_spriteArray[2].BodyDef->allowSleep = true;
	g_spriteArray[2].BodyDef->preventRotation = false;
	g_spriteArray[2].BodyDef->AddShape(g_spriteArray[2].BoxDef);
	g_spriteArray[2].Body = g_world->CreateBody(g_spriteArray[2].BodyDef);
	
	// Create a joint between the two boxes
	g_jointArray[0].DistanceJointDef = new b2DistanceJointDef();
	g_jointArray[0].DistanceJointDef->body1 = g_spriteArray[1].Body;
	g_jointArray[0].DistanceJointDef->body2 = g_spriteArray[2].Body;
	
	// If you want to set the point positions manually.. (remember these are in world coordinates)
	//g_jointArray[0].DistanceJointDef->anchorPoint1.Set(0.0f, 0.0f);
	//g_jointArray[0].DistanceJointDef->anchorPoint2.Set(0.0f, 0.0f);
	
	// Place joins in middle of boxes
	g_jointArray[0].DistanceJointDef->anchorPoint1 = g_spriteArray[1].Body->GetCenterPosition();
	g_jointArray[0].DistanceJointDef->anchorPoint2 = g_spriteArray[2].Body->GetCenterPosition();
	g_jointArray[0].DistanceJointDef->collideConnected = true;
	g_jointArray[0].DistanceJoint = new b2DistanceJoint(g_jointArray[0].DistanceJointDef);
	g_jointArray[0].Joint = g_world->CreateJoint(g_jointArray[0].DistanceJointDef);
	
	
	// ok start a rope from platform 5 (centre top)
	// create the first one

	g_spriteArray[3].BoxDef = new b2BoxDef();
	g_spriteArray[3].BodyDef =  new b2BodyDef();
	g_spriteArray[3].X = -50, g_spriteArray[3].Y = 120;	
	g_spriteArray[3].Type = BALLTYPE_VINE;
	g_spriteArray[3].BoxDef->extents.Set(15 / 2 * SCALE, 30 / 2 * SCALE);
	g_spriteArray[3].BoxDef->density = 0.1F; 
	g_spriteArray[3].BoxDef->friction = 1.0F; 
	g_spriteArray[3].BodyDef->position.Set(g_spriteArray[3].X * SCALE, g_spriteArray[3].Y * SCALE);
	g_spriteArray[3].BodyDef->AddShape(g_spriteArray[3].BoxDef);
	g_spriteArray[3].Body = g_world->CreateBody(g_spriteArray[3].BodyDef);
	
	// join it to something
	
	g_jointArray[1].DistanceJointDef = new b2DistanceJointDef();
	g_jointArray[1].DistanceJointDef->body1 = g_platformArray[6]->Body;
	g_jointArray[1].DistanceJointDef->body2 = g_spriteArray[3].Body;

	g_jointArray[1].DistanceJointDef->anchorPoint1.Set(-50 * SCALE, 140 * SCALE);
	g_jointArray[1].DistanceJointDef->anchorPoint2.Set(g_spriteArray[3].Body->GetCenterPosition().x,g_spriteArray[3].Body->GetCenterPosition().y + 1.0f);
	g_jointArray[1].DistanceJointDef->collideConnected = false;
	g_jointArray[1].DistanceJoint = new b2DistanceJoint(g_jointArray[1].DistanceJointDef);
	g_jointArray[1].Joint = g_world->CreateJoint(g_jointArray[1].DistanceJointDef);
	
	// we need a len and X,Y coords
	int vlen = 9;
	int Vx = -50;
	int Vy = 105;
	int Vj = 2;
	
	for(int i=4; i<vlen; i++)
	{
	
	g_spriteArray[i].BoxDef = new b2BoxDef();
	g_spriteArray[i].BodyDef =  new b2BodyDef();
	g_spriteArray[i].X = Vx, g_spriteArray[i].Y = Vy - ((i-4) * 20);	
	g_spriteArray[i].Type = BALLTYPE_VINE;
	g_spriteArray[i].BoxDef->extents.Set(15 / 2 * SCALE, 30 / 2 * SCALE);
	g_spriteArray[i].BoxDef->density = 0.1F; 
	g_spriteArray[i].BoxDef->friction = 1.0F; 
	g_spriteArray[i].BodyDef->position.Set(g_spriteArray[i].X * SCALE, g_spriteArray[i].Y * SCALE);
	g_spriteArray[i].BodyDef->AddShape(g_spriteArray[i].BoxDef);
	g_spriteArray[i].Body = g_world->CreateBody(g_spriteArray[i].BodyDef);

	g_jointArray[Vj].DistanceJointDef = new b2DistanceJointDef();
	g_jointArray[Vj].DistanceJointDef->body1 = g_spriteArray[i-1].Body;
	g_jointArray[Vj].DistanceJointDef->body2 = g_spriteArray[i].Body;

	g_jointArray[Vj].DistanceJointDef->anchorPoint1.Set(g_spriteArray[i-1].Body->GetCenterPosition().x,g_spriteArray[i-1].Body->GetCenterPosition().y - 0.5f);
	g_jointArray[Vj].DistanceJointDef->anchorPoint2.Set(g_spriteArray[i].Body->GetCenterPosition().x,g_spriteArray[i].Body->GetCenterPosition().y + 0.5f);
	g_jointArray[Vj].DistanceJointDef->collideConnected = false;
	g_jointArray[Vj].DistanceJoint = new b2DistanceJoint(g_jointArray[Vj].DistanceJointDef);
	g_jointArray[Vj].Joint = g_world->CreateJoint(g_jointArray[Vj].DistanceJointDef);		
	
	
	Vj ++;
	
//	Vy -= 20;
	
	}


}





//	INIT BOX2D ENGINE FOR LEVEL
void initBox2D()
{

}

/*
	g_jointArray[2].RevoluteJointDef = new b2RevoluteJointDef();

	g_jointArray[2].RevoluteJointDef->anchorPoint.Set(g_spriteArray[3].Body->GetCenterPosition().x,g_spriteArray[3].Body->GetCenterPosition().y - 1.0f);
	g_jointArray[2].RevoluteJointDef->collideConnected = false;
	g_jointArray[2].RevoluteJoint = new b2RevoluteJoint(g_jointArray[2].RevoluteJointDef);
	g_jointArray[2].Joint = g_world->CreateJoint(g_jointArray[2].RevoluteJointDef);	
*/