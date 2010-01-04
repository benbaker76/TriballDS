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
	for(int i=2; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = (rand() % 760)-400; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
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
	
	g_spriteArray[0].CircleDef->radius = 32 / 2 * SCALE; 
	g_spriteArray[0].CircleDef->density = 1.0F; 
	g_spriteArray[0].CircleDef->friction = 1.0F; 
	g_spriteArray[0].CircleDef->restitution = 0.4F; 

	g_spriteArray[0].BodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
	g_spriteArray[0].BodyDef->allowSleep = false;
	g_spriteArray[0].BodyDef->preventRotation = false;
	g_spriteArray[0].BodyDef->angularDamping = 0.04f;
	g_spriteArray[0].BodyDef->linearDamping = 0.00f;

	g_spriteArray[0].BodyDef->AddShape(g_spriteArray[0].CircleDef);
	
	g_spriteArray[0].Body = g_world->CreateBody(g_spriteArray[0].BodyDef);

		// me playing with a box


	g_spriteArray[1].BoxDef = new b2BoxDef();
	g_spriteArray[1].BodyDef =  new b2BodyDef();
	g_spriteArray[1].X = 350;
	g_spriteArray[1].Y = 260;	
	g_spriteArray[1].BoxDef->extents.Set(32 / 2 * SCALE, 32 / 2 * SCALE);
	g_spriteArray[1].BoxDef->density = 0.6F; 
	g_spriteArray[1].BoxDef->friction = 0.05F; 
	g_spriteArray[1].BoxDef->restitution = 0.95F; 

	g_spriteArray[1].BodyDef->position.Set(g_spriteArray[1].X * SCALE, g_spriteArray[1].Y * SCALE);
	g_spriteArray[1].BodyDef->allowSleep = false;
	g_spriteArray[1].BodyDef->preventRotation = false;

	g_spriteArray[1].BodyDef->AddShape(g_spriteArray[1].BoxDef);
	
	g_spriteArray[1].Body = g_world->CreateBody(g_spriteArray[1].BodyDef);
	
	
	g_spriteArray[2].BoxDef = new b2BoxDef();
	g_spriteArray[2].BodyDef =  new b2BodyDef();
		g_spriteArray[2].X = -200;
		g_spriteArray[2].Y = -100;	
		g_spriteArray[2].Type = BALLTYPE_OBJECT;
	g_spriteArray[2].BoxDef->extents.Set(32 / 2 * SCALE, 32 / 2 * SCALE);
	g_spriteArray[2].BoxDef->density = 0.6F; 
	g_spriteArray[2].BoxDef->friction = 0.05F; 
	g_spriteArray[2].BoxDef->restitution = 0.95F; 

	g_spriteArray[2].BodyDef->position.Set(g_spriteArray[2].X * SCALE, g_spriteArray[2].Y * SCALE);
	g_spriteArray[2].BodyDef->allowSleep = false;
	g_spriteArray[2].BodyDef->preventRotation = false;

	g_spriteArray[2].BodyDef->AddShape(g_spriteArray[2].BoxDef);
	
	g_spriteArray[2].Body = g_world->CreateBody(g_spriteArray[2].BodyDef);	
}




//	INIT BOX2D ENGINE FOR LEVEL
void initBox2D()
{

}