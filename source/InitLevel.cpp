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
	
	// test defines for now!
	
	g_plat = new b2PolyDef();	// HOW THE FUCK?
	g_plat->vertexCount = 6;
	g_plat->vertices[0].Set(0.0f, 0.4f);
	g_plat->vertices[1].Set(-3.0f, 0.05f);
	g_plat->vertices[2].Set(-3.0f, -0.4f);
	g_plat->vertices[3].Set(0.0f, -0.5f);
	g_plat->vertices[4].Set(5.8f, -0.5f);
	g_plat->vertices[5].Set(5.7f, 0.7f);
	g_plat->density = 0.0f;
	g_plat->friction = 2.0f;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(9.5f, 3.4f);
	g_bodyDef->AddShape(g_plat);
	g_world->CreateBody(g_bodyDef);

	g_plat = new b2PolyDef();	// HOW THE FUCK?
	g_plat->vertexCount = 4;
	g_plat->vertices[0].Set(0.0f, 0.5f);
	g_plat->vertices[1].Set(0.0f, -0.5f);
	g_plat->vertices[2].Set(6.0f, -1.0f);
	g_plat->vertices[3].Set(6.0f, 1.5f);
	g_plat->density = 0.0f;
	g_plat->friction = 2.0f;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(20.8f, 3.6f);
	g_bodyDef->AddShape(g_plat);
	g_world->CreateBody(g_bodyDef);

	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(4.7f, 1.0f);
	g_boxDef->density = 0.0f;
	g_boxDef->friction = 1.0f;
	g_boxDef->restitution = 0.0F;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(17.5f, 0.6f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);
	
	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(1.0f, 20.0f);
	g_boxDef->density = 0.0f;
	g_boxDef->friction = 1.0f;
	g_boxDef->restitution = 0.0F;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(25.6f, 19.2f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);	
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(-2.0f, 19.2f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);	
}

// INIT PLAYER AND RANDOM BALLS (FOR NOW)
void initPlayer()
{
for(int i=1; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = (rand() % 100) + 140; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_spriteArray[i].Y =  192 - ((rand() % 100)); //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_spriteArray[i].Type = BALLTYPE_EVILBALL;
		
		g_spriteArray[i].CircleDef = new b2CircleDef();
		g_spriteArray[i].BodyDef =  new b2BodyDef();
		
		g_spriteArray[i].CircleDef->radius = 32 / 2 * SCALE; 
		g_spriteArray[i].CircleDef->density = 1.0F; 
		g_spriteArray[i].CircleDef->friction = 0.5F; 
		g_spriteArray[i].CircleDef->restitution = 0.8F; 

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
	g_spriteArray[0].CircleDef->restitution = 0.2F; 

	g_spriteArray[0].BodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
	g_spriteArray[0].BodyDef->AddShape(g_spriteArray[0].CircleDef);
	
	g_spriteArray[0].Body = g_world->CreateBody(g_spriteArray[0].BodyDef);

}

//	INIT BOX2D ENGINE FOR LEVEL
void initBox2D()
{
	g_worldAABB = new b2AABB();
	g_worldAABB->minVertex.Set(SCREEN_WIDTH * -SCALE, SCREEN_HEIGHT * -SCALE);
	g_worldAABB->maxVertex.Set(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);
	
	g_gravity = new b2Vec2(0.0f, -15.0f);
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(50.0f, 10.0f);
	g_groundBoxDef->density = 0.0f;

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -20.0f);
	g_groundBodyDef->AddShape(g_groundBoxDef);

	g_world->CreateBody(g_groundBodyDef);

}