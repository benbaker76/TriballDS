#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Box2D.h"

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
	
	// ** WITHOUT THIS IT WILL CRASH ON HARDWARE **
	// ** CAN'T USE OBJECTS IN ARRAYS THAT HAVE NOT BEING INITIALIZED **
	for(int i=0; i<BALL_COUNT+10; i++)  // Need 10 more
		g_objectArray[i].Body = NULL; // Make Body NULL so uninitialized entries can be skipped
	
	loadLevel();
	
	g_worldAABB = new b2AABB();
	g_worldAABB->minVertex.Set(-100, -100);
	g_worldAABB->maxVertex.Set(100, 100);
	g_gravity = new b2Vec2(0.0f, -40.0f);								// -30 for Gravity seems to work best (better than -50)
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
/*	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(g_levelGridSize.Width * 10.0f, 1.0f);
	g_groundBoxDef->density = 0.0f;
	g_groundBoxDef->friction = 1.0f;
	g_groundBoxDef->restitution = 0.0F;	

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -(g_levelGridSize.Height * 10.0f - 0.5F));
	g_groundBodyDef->AddShape(g_groundBoxDef);
*/
// *** NO IDEA WHY THIS CAUSES PROBLEMS FOR BOX2D!
//	g_groundBody = g_world->CreateBody(g_groundBodyDef);
/*	
	b2BoxDef* ceilBoxDef = new b2BoxDef();
	ceilBoxDef->extents.Set(g_levelGridSize.Width * 10.0f, 0.5f);
	ceilBoxDef->density = 0.0f;
	ceilBoxDef->friction = 1.0f;
	ceilBoxDef->restitution = 0.2F;		
	
	b2BodyDef* ceilBodyDef = new b2BodyDef(); 
	ceilBodyDef->position.Set(0.0f, g_levelGridSize.Height * 10.0f);
	ceilBodyDef->AddShape(ceilBoxDef);

	g_world->CreateBody(ceilBodyDef);
*/	
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
/*	for(int i=12; i<BALL_COUNT; i++)
	{
		g_objectArray[i].Action = ACTION_NONE;	
		g_objectArray[i].X = (rand() % 760)-400; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_objectArray[i].Y =  192 - ((rand() % 100)); //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_objectArray[i].Type = BALLTYPE_EVILBALL;
		
		g_objectArray[i].CircleDef = new b2CircleDef();
		g_objectArray[i].BodyDef =  new b2BodyDef();
		
		g_objectArray[i].CircleDef->radius = 48 / 2 * SCALE; 
		g_objectArray[i].CircleDef->density = 1.0F; 
		g_objectArray[i].CircleDef->friction = 1.0F; 
		g_objectArray[i].CircleDef->restitution = 0.4F; 

		g_objectArray[i].BodyDef->position.Set(g_objectArray[i].X * SCALE, g_objectArray[i].Y * SCALE);
		g_objectArray[i].BodyDef->AddShape(g_objectArray[i].CircleDef);
		
		g_objectArray[i].Body = g_world->CreateBody(g_objectArray[i].BodyDef);
		g_objectArray[i].Body->m_userData = (void*) &g_objectArray[i]; // Link user data back to Object

	}
*/	
	// INIT PLAYER
	g_objectArray[0].Action = ACTION_NONE;	
	g_objectArray[0].X = 128;
	g_objectArray[0].Y = 120; // 120
	g_objectArray[0].Type = BALLTYPE_PLAYER;
	
	g_objectArray[0].CircleDef = new b2CircleDef();
	g_objectArray[0].BodyDef =  new b2BodyDef();
	
	g_objectArray[0].CircleDef->radius = 48 / 2 * SCALE; 
	g_objectArray[0].CircleDef->density = 1.0F; 
	g_objectArray[0].CircleDef->friction = 1.0F; 
	g_objectArray[0].CircleDef->restitution = 0.35F; 

	g_objectArray[0].BodyDef->position.Set(g_objectArray[0].X * SCALE, g_objectArray[0].Y * SCALE);
	g_objectArray[0].BodyDef->allowSleep = false;
	g_objectArray[0].BodyDef->preventRotation = false;
//	g_objectArray[0].BodyDef->isBullet = true; NOT PRESENT IN OUR BOX2D VERSION
	
	g_objectArray[0].BodyDef->angularDamping = 0.05f;
	g_objectArray[0].BodyDef->linearDamping = 0.00f;

	g_objectArray[0].BodyDef->AddShape(g_objectArray[0].CircleDef);
	
	g_objectArray[0].Body = g_world->CreateBody(g_objectArray[0].BodyDef);
	g_objectArray[0].Body->m_userData = (void*) &g_objectArray[0]; // Link user data back to Object

		// me playing with a box
/*
	// ------------------ BOXES ----------------
	g_objectArray[1].BoxDef = new b2BoxDef();
	g_objectArray[1].BodyDef =  new b2BodyDef();
	g_objectArray[1].X = 220;
	g_objectArray[1].Y = 260;	
	g_objectArray[1].Type = BALLTYPE_CRATE;
	g_objectArray[1].BoxDef->extents.Set(64 / 2 * SCALE, 64 / 2 * SCALE);
	g_objectArray[1].BoxDef->density = 0.3F; 
	g_objectArray[1].BoxDef->friction = 0.2F; 
	g_objectArray[1].BoxDef->restitution = 0.8F; 
	g_objectArray[1].BodyDef->position.Set(g_objectArray[1].X * SCALE, g_objectArray[1].Y * SCALE);
	g_objectArray[1].BodyDef->allowSleep = true;
	g_objectArray[1].BodyDef->preventRotation = false;
	g_objectArray[1].BodyDef->AddShape(g_objectArray[1].BoxDef);
	g_objectArray[1].Body = g_world->CreateBody(g_objectArray[1].BodyDef);
	g_objectArray[1].Body->m_userData = (void*) &g_objectArray[1]; // Link user data back to Object
	
	
	g_objectArray[2].BoxDef = new b2BoxDef();
	g_objectArray[2].BodyDef =  new b2BodyDef();
	g_objectArray[2].X = 320;
	g_objectArray[2].Y = 260;	
	g_objectArray[2].Type = BALLTYPE_CRATE;
	g_objectArray[2].BoxDef->extents.Set(64 / 2 * SCALE, 64 / 2 * SCALE);
	g_objectArray[2].BoxDef->density = 0.3F; 
	g_objectArray[2].BoxDef->friction = 0.2F; 
	g_objectArray[2].BoxDef->restitution = 0.8F; 
	g_objectArray[2].BodyDef->position.Set(g_objectArray[2].X * SCALE, g_objectArray[2].Y * SCALE);
	g_objectArray[2].BodyDef->allowSleep = true;
	g_objectArray[2].BodyDef->preventRotation = false;
	g_objectArray[2].BodyDef->AddShape(g_objectArray[2].BoxDef);
	g_objectArray[2].Body = g_world->CreateBody(g_objectArray[2].BodyDef);
	g_objectArray[2].Body->m_userData = (void*) &g_objectArray[2]; // Link user data back to Object

	// Create a joint between the two boxes
	g_jointArray[0].DistanceJointDef = new b2DistanceJointDef();
	g_jointArray[0].DistanceJointDef->body1 = g_objectArray[1].Body;
	g_jointArray[0].DistanceJointDef->body2 = g_objectArray[2].Body;
	
	// If you want to set the point positions manually.. (remember these are in world coordinates)
	//g_jointArray[0].DistanceJointDef->anchorPoint1.Set(0.0f, 0.0f);
	//g_jointArray[0].DistanceJointDef->anchorPoint2.Set(0.0f, 0.0f);
	
	// Place joins in middle of boxes
	g_jointArray[0].DistanceJointDef->anchorPoint1 = g_objectArray[1].Body->GetCenterPosition();
	g_jointArray[0].DistanceJointDef->anchorPoint2 = g_objectArray[2].Body->GetCenterPosition();
	g_jointArray[0].DistanceJointDef->collideConnected = true;
	g_jointArray[0].DistanceJoint = new b2DistanceJoint(g_jointArray[0].DistanceJointDef);
	g_jointArray[0].Joint = g_world->CreateJoint(g_jointArray[0].DistanceJointDef);
*/	
	// ------------------ VINE ----------------
/*
	// Create boxes for the vine
	
	for(int i=0; i<6; i++)
	{
		g_objectArray[i + 3].BoxDef = new b2BoxDef();
		g_objectArray[i + 3].BodyDef =  new b2BodyDef();
		g_objectArray[i + 3].X = -50;
		g_objectArray[i + 3].Y = 120 - (i * 28);	
		g_objectArray[i + 3].Type = BALLTYPE_VINE;
		g_objectArray[i + 3].BoxDef->extents.Set(8 / 2 * SCALE, 30 / 2 * SCALE);
		g_objectArray[i + 3].BoxDef->density = 1.0F; 
		g_objectArray[i + 3].BoxDef->friction = 0.0F;
		g_objectArray[i + 3].BoxDef->restitution = 0.0F;
		g_objectArray[i + 3].BodyDef->position.Set(g_objectArray[i + 3].X * SCALE, g_objectArray[i + 3].Y * SCALE);
		g_objectArray[i + 3].BodyDef->AddShape(g_objectArray[i + 3].BoxDef);
		g_objectArray[i + 3].Body = g_world->CreateBody(g_objectArray[i + 3].BodyDef);
		g_objectArray[i + 3].Body->m_userData = (void*) &g_objectArray[i + 3]; // Link user data back to Object
	}
	
	for(int i=0; i<6-1; i++)
	{
		// Revolute joins connect each box
		g_jointArray[i].RevoluteJointDef = new b2RevoluteJointDef();
		g_jointArray[i].RevoluteJointDef->body1 = g_objectArray[i + 3].Body;
		g_jointArray[i].RevoluteJointDef->body2 = g_objectArray[i + 3 + 1].Body;

		g_jointArray[i].RevoluteJointDef->anchorPoint.Set(g_objectArray[i + 3].Body->GetCenterPosition().x, g_objectArray[i + 3].Body->GetCenterPosition().y - (g_objectArray[i + 3].BoxDef->extents.y / 2.0f));
		g_jointArray[i].RevoluteJointDef->collideConnected = false;
		g_jointArray[i].RevoluteJointDef->motorSpeed = 0.0f;
		g_jointArray[i].RevoluteJointDef->motorTorque = 50.0f;		// Why the hell does this need to be so high?
		g_jointArray[i].RevoluteJointDef->enableMotor = true;
		g_jointArray[i].RevoluteJoint = new b2RevoluteJoint(g_jointArray[i].RevoluteJointDef);
		g_jointArray[i].Joint = g_world->CreateJoint(g_jointArray[i].RevoluteJointDef);
		
	}
	
	// attach to platform
	g_jointArray[6].RevoluteJointDef = new b2RevoluteJointDef();
	g_jointArray[6].RevoluteJointDef->body1 = g_objectArray[3].Body;
	g_jointArray[6].RevoluteJointDef->body2 = g_platformArray[6]->Body;

	g_jointArray[6].RevoluteJointDef->anchorPoint.Set(g_objectArray[3].Body->GetCenterPosition().x, g_objectArray[3].Body->GetCenterPosition().y + g_objectArray[3].BoxDef->extents.y);
	g_jointArray[6].RevoluteJointDef->collideConnected = false;
	g_jointArray[6].RevoluteJointDef->motorSpeed = 0.0f;
	g_jointArray[6].RevoluteJointDef->motorTorque = 50.0f;
	g_jointArray[6].RevoluteJointDef->enableMotor = true;
	g_jointArray[6].RevoluteJoint = new b2RevoluteJoint(g_jointArray[6].RevoluteJointDef);
	g_jointArray[6].Joint = g_world->CreateJoint(g_jointArray[6].RevoluteJointDef);
	
		// ------------------ VINE END ----------------
*/
	// generate a baddy (from array 10 +
	g_objectArray[10].BoxDef = new b2BoxDef();
	g_objectArray[10].BodyDef =  new b2BodyDef();
	g_objectArray[10].X = -50;
	g_objectArray[10].Y = -360;
	//g_objectArray[10].Movement = 1;
	//g_objectArray[10].Direction = 1;
	g_objectArray[10].Direction = DIRECTION_LEFT;
	g_objectArray[10].FrameCount = 0;
	g_objectArray[10].FrameNum = 0;
	g_objectArray[10].Accel = 0.2;
	g_objectArray[10].XSpeedMax = 18.0;
	g_objectArray[10].MoveMax = 200;
	g_objectArray[10].MoveMin = -200;
	g_objectArray[10].Active = TRUE;
	g_objectArray[10].Type = ENEMYTYPE_PATROL;
	g_objectArray[10].BoxDef->extents.Set(40 / 2 * SCALE, 64 / 2 * SCALE); // If Square, how to knock over?
	g_objectArray[10].BodyDef->preventRotation = false;	
	g_objectArray[10].BodyDef->allowSleep = true;
	g_objectArray[10].BoxDef->density = 1.0F; 
	g_objectArray[10].BoxDef->friction = 0.1F;
	g_objectArray[10].BoxDef->restitution = 0.3F;
	g_objectArray[10].BodyDef->position.Set(g_objectArray[10].X * SCALE, g_objectArray[10].Y * SCALE);
	g_objectArray[10].BodyDef->AddShape(g_objectArray[10].BoxDef);
	g_objectArray[10].Body = g_world->CreateBody(g_objectArray[10].BodyDef);
	g_objectArray[10].Body->m_userData = (void*) &g_objectArray[10]; // Link user data back to Object
/*	
	g_objectArray[11].BoxDef = new b2BoxDef();
	g_objectArray[11].BodyDef =  new b2BodyDef();
	g_objectArray[11].X = -300;
	g_objectArray[11].Y = 110;
	//g_objectArray[11].Movement = 1;
	//g_objectArray[11].Direction = 1;
	g_objectArray[11].Direction = DIRECTION_LEFT;
	g_objectArray[11].FrameCount = 0;
	g_objectArray[11].FrameNum = 0;
	g_objectArray[11].Accel = 0.6;
	g_objectArray[11].XSpeedMax = 20.0;
	g_objectArray[11].MoveMax = -300;
	g_objectArray[11].MoveMin = -310;
	g_objectArray[11].Active = TRUE;	
	g_objectArray[11].Type = ENEMYTYPE_PATROL;
	g_objectArray[11].BoxDef->extents.Set(40 / 2 * SCALE, 64 / 2 * SCALE); // If Square, how to knock over?
	g_objectArray[11].BodyDef->preventRotation = false;	
	g_objectArray[11].BodyDef->allowSleep = true;
	g_objectArray[11].BoxDef->density = 1.0F; 
	g_objectArray[11].BoxDef->friction = 0.1F;
	g_objectArray[11].BoxDef->restitution = 0.3F;
	g_objectArray[11].BodyDef->position.Set(g_objectArray[11].X * SCALE, g_objectArray[11].Y * SCALE);
	g_objectArray[11].BodyDef->AddShape(g_objectArray[11].BoxDef);
	g_objectArray[11].Body = g_world->CreateBody(g_objectArray[11].BodyDef);
	g_objectArray[11].Body->m_userData = (void*) &g_objectArray[11]; // Link user data back to Object
*/

	// generate a baddy (from array 10 +

	for(int i=11; i<15; i++)
	{
	g_objectArray[i].BoxDef = new b2BoxDef();
	g_objectArray[i].BodyDef =  new b2BodyDef();
	g_objectArray[i].X = (rand() & 400) - 200;
	g_objectArray[i].Y = (rand() & 200) - 300;
	g_objectArray[i].Direction = DIRECTION_LEFT;
	g_objectArray[i].FrameCount = 0;
	g_objectArray[i].FrameNum = 0;
	g_objectArray[i].Accel = 0.3f + ((rand() & 8)/10);
	g_objectArray[i].XSpeedMax = 20.0f - ((rand() & 5));
	g_objectArray[i].YSpeedMax = g_objectArray[i].XSpeedMax;
	g_objectArray[i].Active = TRUE;
	g_objectArray[i].Type = ENEMYTYPE_BEE;
	g_objectArray[i].BoxDef->extents.Set(16 / 2 * SCALE, 16 / 2 * SCALE); // If Square, how to knock over?
	g_objectArray[i].BodyDef->preventRotation = true;	
	g_objectArray[i].BodyDef->allowSleep = true;
	g_objectArray[i].BoxDef->density = 0.001F; 
	g_objectArray[i].BoxDef->friction = 0.0F;
	g_objectArray[i].BoxDef->restitution = 0.0F;
//g_objectArray[i].BoxDef->	g_objectArray[i].BodyDef->isSleeping = true;
	g_objectArray[i].BodyDef->position.Set(g_objectArray[i].X * SCALE, g_objectArray[i].Y * SCALE);
	g_objectArray[i].BodyDef->AddShape(g_objectArray[i].BoxDef);
	g_objectArray[i].Body = g_world->CreateBody(g_objectArray[i].BodyDef);
	g_objectArray[i].Body->m_userData = (void*) &g_objectArray[i]; // Link user data back to Object
	}
}

//	INIT BOX2D ENGINE FOR LEVEL
void initBox2D()
{

}

		// Example code, used and tested
	
		// The distance joint connects each box to the platform
		// If you look at the link below, they do the opposite
		// (ie. Use Revolutes joins to link to the platform and distance joints to link each box)
		// I couldn't get it to work this way but it seems fine this way?
		// http://www.box2d.org/forum/viewtopic.php?p=10691#p10691
	/*	
		g_jointArray[i].DistanceJointDef = new b2DistanceJointDef();
		g_jointArray[i].DistanceJointDef->body1 = g_objectArray[i + 3].Body;
		g_jointArray[i].DistanceJointDef->body2 = g_platformArray[6]->Body;

		g_jointArray[i].DistanceJointDef->anchorPoint1.Set(g_objectArray[i + 3].Body->GetCenterPosition().x,g_objectArray[i + 3].Body->GetCenterPosition().y - g_objectArray[i + 3].BoxDef->extents.y);
		//g_jointArray[i].DistanceJointDef->anchorPoint2.Set(g_objectArray[i + 3 + 1].Body->GetCenterPosition().x,g_objectArray[i + 3 + 1].Body->GetCenterPosition().y + g_objectArray[i + 3 + 1].BoxDef->extents.y);
		g_jointArray[i].DistanceJointDef->anchorPoint2.Set(-50 * SCALE, 110 * SCALE);
		g_jointArray[i].DistanceJointDef->collideConnected = false;
		g_jointArray[i].DistanceJoint = new b2DistanceJoint(g_jointArray[i].DistanceJointDef);
		g_jointArray[i].Joint = g_world->CreateJoint(g_jointArray[i].DistanceJointDef);
*/
/*	
	g_jointArray[9].DistanceJointDef = new b2DistanceJointDef();
	g_jointArray[9].DistanceJointDef->body1 = g_objectArray[3].Body;
	g_jointArray[9].DistanceJointDef->body2 = g_platformArray[6]->Body;

	g_jointArray[9].DistanceJointDef->anchorPoint1.Set(g_objectArray[3].Body->GetCenterPosition().x, g_objectArray[3].Body->GetCenterPosition().y + g_objectArray[3].BoxDef->extents.y);
	g_jointArray[9].DistanceJointDef->anchorPoint2.Set(-50 * SCALE, 110 * SCALE);
	g_jointArray[9].DistanceJointDef->collideConnected = false;
	g_jointArray[9].DistanceJoint = new b2DistanceJoint(g_jointArray[9].DistanceJointDef);
	g_jointArray[9].Joint = g_world->CreateJoint(g_jointArray[9].DistanceJointDef);
*/	
	// This is a nice way to attach the vine to the platform as it makes it nice and bouncy
	
	/* g_jointArray[10].PrismaticJointDef = new b2PrismaticJointDef();
	g_jointArray[10].PrismaticJointDef->body1 = g_objectArray[3].Body;
	g_jointArray[10].PrismaticJointDef->body2 = g_platformArray[6]->Body;
	g_jointArray[10].PrismaticJointDef->lowerTranslation = 0.0F;
	g_jointArray[10].PrismaticJointDef->upperTranslation = g_objectArray[3].Body->GetCenterPosition().y - 110 * SCALE;

	g_jointArray[10].PrismaticJointDef->anchorPoint.Set(g_objectArray[3].Body->GetCenterPosition().x, g_objectArray[3].Body->GetCenterPosition().y + g_objectArray[3].BoxDef->extents.y);
	g_jointArray[10].PrismaticJointDef->collideConnected = false;
	g_jointArray[10].PrismaticJoint = new b2PrismaticJoint(g_jointArray[10].PrismaticJointDef);
	g_jointArray[10].Joint = g_world->CreateJoint(g_jointArray[10].PrismaticJointDef);  */
	
	        /* b2MassData* massDatas = new b2MassData();
        massDatas->mass = g_objectArray[i + 3].Body->GetMass();
        massDatas->I = g_objectArray[i + 3].Body->GetInertia();
        g_objectArray[i + 3].BoxDef->ComputeMass(massDatas); */