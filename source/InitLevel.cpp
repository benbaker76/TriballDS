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


void initLevel()
{
	// position of scroll area
	g_scrollPos.X = 0;
	g_scrollPos.Y = 0;
	
	// box2d update vars
	timeStep = 1.0f / 60.0f;
	iterations = 2;
	
	g_cameraPos.X = 0;
	g_cameraPos.Y = 0;
	g_cameraPos.Z = 1.0f;
	
	g_cameraStart.X = 0;
	g_cameraStart.Y = 0;
	g_cameraStart.Z = 1.0f;

	g_cameraEnd.X = 0;
	g_cameraEnd.Y = 0;
	g_cameraEnd.Z = 1.0f;
	
	// test defines for now! ( -40 to 40 for some reason??)
	
	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(1.0f, 40.0f);
	g_boxDef->density = 0.0f;
	g_boxDef->friction = 1.0f;
	g_boxDef->restitution = 0.8F;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(-40.8f, 0.0f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);	
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(40.8f, 0.0f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);
	
	
	g_platformArray[0]->PolyDef = new b2PolyDef();	// left
	g_platformArray[0]->PolyDef->vertexCount = 7;
	g_platformArray[0]->PolyDef->vertices[0].Set(-14.0f, 2.0f);
	g_platformArray[0]->PolyDef->vertices[1].Set(-16.0f, 1.0f);
	g_platformArray[0]->PolyDef->vertices[2].Set(-16.0f, -1.0f);
	g_platformArray[0]->PolyDef->vertices[3].Set(-14.0f, -2.5f);
	g_platformArray[0]->PolyDef->vertices[4].Set( 4.1f, -2.5f);
	g_platformArray[0]->PolyDef->vertices[5].Set( 4.0f, 1.0f);
	g_platformArray[0]->PolyDef->vertices[6].Set( 3.0f, 2.2f);

	g_platformArray[0]->PolyDef->density = 0.0f;
	g_platformArray[0]->PolyDef->friction = 1.0f;
	g_platformArray[0]->BodyDef = new b2BodyDef();
	g_platformArray[0]->BodyDef->position.Set(14.0f, -2.4f);
	g_platformArray[0]->BodyDef->AddShape(g_platformArray[0]->PolyDef);
	g_world->CreateBody(g_platformArray[0]->BodyDef);
	
	g_platformArray[1]->PolyDef = new b2PolyDef();	// pit base
	g_platformArray[1]->PolyDef->vertexCount = 5;
	g_platformArray[1]->PolyDef->vertices[0].Set( -10.0f, 0.0f);
	g_platformArray[1]->PolyDef->vertices[1].Set( -10.0f, -4.5f);
	g_platformArray[1]->PolyDef->vertices[2].Set( -8.0f, -5.8f);
	g_platformArray[1]->PolyDef->vertices[3].Set( 4.0f, -5.7f);
	g_platformArray[1]->PolyDef->vertices[4].Set( 4.0f, -4.0f);
	g_platformArray[1]->PolyDef->vertices[5].Set( -8.0f, -3.0f); // WHY DOES THIS CRASH IT
//	g_platformArray[1]->PolyDef->vertices[6].Set( 3.0f, 2.2f);

	g_platformArray[1]->PolyDef->density = 0.0f;
	g_platformArray[1]->PolyDef->friction = 1.0f;
	g_platformArray[1]->BodyDef = new b2BodyDef();
	g_platformArray[1]->BodyDef->position.Set(24.0f, -4.4f);
	g_platformArray[1]->BodyDef->AddShape(g_platformArray[1]->PolyDef);
	g_world->CreateBody(g_platformArray[1]->BodyDef);	
	
	g_platformArray[2]->PolyDef = new b2PolyDef();	// little plat
	g_platformArray[2]->PolyDef->vertexCount = 8;
	g_platformArray[2]->PolyDef->vertices[0].Set( -8.0f, 0.0f);
	g_platformArray[2]->PolyDef->vertices[1].Set( -12.0f, -1.0f);
	g_platformArray[2]->PolyDef->vertices[2].Set( -12.0f, -3.0f);
	g_platformArray[2]->PolyDef->vertices[3].Set( -8.0f, -5.0f);	
	g_platformArray[2]->PolyDef->vertices[4].Set( -3.0f, -5.0f);	
	g_platformArray[2]->PolyDef->vertices[5].Set( 1.0f, -3.0f);
	g_platformArray[2]->PolyDef->vertices[6].Set( 1.0f, -1.0f);	
	g_platformArray[2]->PolyDef->vertices[7].Set( -3.0f, 0.0f);

	g_platformArray[2]->PolyDef->density = 0.0f;
	g_platformArray[2]->PolyDef->friction = 1.0f;
	g_platformArray[2]->BodyDef = new b2BodyDef();
	g_platformArray[2]->BodyDef->position.Set(19.0f, -19.0f);
	g_platformArray[2]->BodyDef->AddShape(g_platformArray[2]->PolyDef);
	g_world->CreateBody(g_platformArray[2]->BodyDef);	

	g_platformArray[3]->PolyDef = new b2PolyDef();	// bongo base
	g_platformArray[3]->PolyDef->vertexCount = 4;
	g_platformArray[3]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[3]->PolyDef->vertices[1].Set( 5.0f, -5.0f);
	g_platformArray[3]->PolyDef->vertices[2].Set( 10.0f,-3.0f);
	g_platformArray[3]->PolyDef->vertices[3].Set( 6.0f, 4.0f);	
	g_platformArray[3]->PolyDef->density = 0.0f;
	g_platformArray[3]->PolyDef->friction = 1.0f;
	g_platformArray[3]->BodyDef = new b2BodyDef();
	g_platformArray[3]->BodyDef->position.Set(30.0f, 8.0f);
	g_platformArray[3]->BodyDef->AddShape(g_platformArray[3]->PolyDef);
	g_world->CreateBody(g_platformArray[3]->BodyDef);		
	g_platformArray[4]->PolyDef = new b2PolyDef();	// bongo top
	g_platformArray[4]->PolyDef->vertexCount = 3;
	g_platformArray[4]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[4]->PolyDef->vertices[1].Set( 5.0f, 4.0f);
	g_platformArray[4]->PolyDef->vertices[2].Set( 2.5f, 4.0f);
	g_platformArray[4]->PolyDef->restitution = 2.8F;
	g_platformArray[4]->PolyDef->density = 0.0f;
	g_platformArray[4]->PolyDef->friction = 1.0f;
	g_platformArray[4]->BodyDef = new b2BodyDef();
	g_platformArray[4]->BodyDef->position.Set(30.0f, 8.0f);
	g_platformArray[4]->BodyDef->AddShape(g_platformArray[4]->PolyDef);
	g_world->CreateBody(g_platformArray[4]->BodyDef);		

	g_platformArray[5]->PolyDef = new b2PolyDef();	// plat under bongo
	g_platformArray[5]->PolyDef->vertexCount = 4;
	g_platformArray[5]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[5]->PolyDef->vertices[1].Set( 0.0f, -12.0f);
	g_platformArray[5]->PolyDef->vertices[2].Set( 13.0f,-12.0f);
	g_platformArray[5]->PolyDef->vertices[3].Set( 13.0f, 0.0f);	
	g_platformArray[5]->PolyDef->density = 0.0f;
	g_platformArray[5]->PolyDef->friction = 1.0f;
	g_platformArray[5]->BodyDef = new b2BodyDef();
	g_platformArray[5]->BodyDef->position.Set(27.0f, 2.0f);
	g_platformArray[5]->BodyDef->AddShape(g_platformArray[5]->PolyDef);
	g_world->CreateBody(g_platformArray[5]->BodyDef);	
}

// INIT PLAYER AND RANDOM BALLS (FOR NOW)
void initPlayer()
{
for(int i=1; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = (rand() % 128); //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_spriteArray[i].Y =  192 - ((rand() % 100)); //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_spriteArray[i].Type = BALLTYPE_EVILBALL;
		
		g_spriteArray[i].CircleDef = new b2CircleDef();
		g_spriteArray[i].BodyDef =  new b2BodyDef();
		
		g_spriteArray[i].CircleDef->radius = 32 / 2 * SCALE; 
		g_spriteArray[i].CircleDef->density = 1.0F; 
		g_spriteArray[i].CircleDef->friction = 1.0F; 
		g_spriteArray[i].CircleDef->restitution = 0.4F; 

		g_spriteArray[i].BodyDef->position.Set(g_spriteArray[i].X * SCALE, g_spriteArray[i].Y * SCALE);
		g_spriteArray[i].BodyDef->AddShape(g_spriteArray[i].CircleDef);
		
		g_spriteArray[i].Body = g_world->CreateBody(g_spriteArray[i].BodyDef);
	}
	
	// INIT PLAYER
	g_spriteArray[0].Action = ACTION_NONE;	
	g_spriteArray[0].X = 128;
	g_spriteArray[0].Y = 120;
	g_spriteArray[0].Type = BALLTYPE_PLAYER;
	
	g_spriteArray[0].CircleDef = new b2CircleDef();
	g_spriteArray[0].BodyDef =  new b2BodyDef();
	
	g_spriteArray[0].CircleDef->radius = 24 / 2 * SCALE; 
	g_spriteArray[0].CircleDef->density = 1.0F; 
	g_spriteArray[0].CircleDef->friction = 1.0F; 
	g_spriteArray[0].CircleDef->restitution = 0.0F; 

	g_spriteArray[0].BodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
	g_spriteArray[0].BodyDef->allowSleep = false;
	g_spriteArray[0].BodyDef->preventRotation = false;
	g_spriteArray[0].BodyDef->angularDamping = 0.04f;
	g_spriteArray[0].BodyDef->linearDamping = 0.00f;

	g_spriteArray[0].BodyDef->AddShape(g_spriteArray[0].CircleDef);
	
	g_spriteArray[0].Body = g_world->CreateBody(g_spriteArray[0].BodyDef);

}

//	INIT BOX2D ENGINE FOR LEVEL
void initBox2D()
{
	g_worldAABB = new b2AABB();
//	g_worldAABB->minVertex.Set(SCREEN_WIDTH * -SCALE, SCREEN_HEIGHT * -SCALE);
//	g_worldAABB->maxVertex.Set(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);
	g_worldAABB->minVertex.Set(-40, -40);
	g_worldAABB->maxVertex.Set(40,40);	
	g_gravity = new b2Vec2(0.0f, -15.0f);
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(40.0f, 0.5f);
	g_groundBoxDef->density = 0.0f;
	g_groundBoxDef->friction = 1.0f;
	g_groundBoxDef->restitution = 0.2F;	

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -40.0f);
	g_groundBodyDef->AddShape(g_groundBoxDef);

	g_world->CreateBody(g_groundBodyDef);
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, 40.0f);
	g_groundBodyDef->AddShape(g_groundBoxDef);

	g_world->CreateBody(g_groundBodyDef);
}