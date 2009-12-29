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
	g_gravity = new b2Vec2(0.0f, -55.0f);
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(g_levelGridSize.Width * 10.0f, 1.0f);
	g_groundBoxDef->density = 0.0f;
	g_groundBoxDef->friction = 1.0f;
	g_groundBoxDef->restitution = 0.2F;	

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -(g_levelGridSize.Height * 10.0f - 0.5F));
	g_groundBodyDef->AddShape(g_groundBoxDef);

	g_groundBody = g_world->CreateBody(g_groundBodyDef);
	
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
	
	g_platformArray[0] = new Poly();
	g_platformArray[0]->PolyDef = new b2PolyDef();	// left
	g_platformArray[0]->PolyDef->vertexCount = 7;
	g_platformArray[0]->PolyDef->vertices[0].Set(-10.0f, 2.0f);
	g_platformArray[0]->PolyDef->vertices[1].Set(-16.0f, 1.0f);
	g_platformArray[0]->PolyDef->vertices[2].Set(-16.0f, -1.0f);
	g_platformArray[0]->PolyDef->vertices[3].Set(-14.0f, -2.5f);
	g_platformArray[0]->PolyDef->vertices[4].Set( 25.0f, -2.5f);
	g_platformArray[0]->PolyDef->vertices[5].Set( 25.0f, 3.3f);
	g_platformArray[0]->PolyDef->vertices[6].Set( 10.0f, 2.8f);

	g_platformArray[0]->PolyDef->density = 0.0f;
	g_platformArray[0]->PolyDef->friction = 1.0f;
	g_platformArray[0]->BodyDef = new b2BodyDef();
	g_platformArray[0]->BodyDef->position.Set(15.0f, -2.4f);
	g_platformArray[0]->BodyDef->AddShape(g_platformArray[0]->PolyDef);
	g_platformArray[0]->Body = g_world->CreateBody(g_platformArray[0]->BodyDef);
	
	
	g_platformArray[2] = new Poly();
	g_platformArray[2]->PolyDef = new b2PolyDef();	// little plat
	g_platformArray[2]->PolyDef->vertexCount = 8;
	g_platformArray[2]->PolyDef->vertices[0].Set( -9.0f, 0.0f);
	g_platformArray[2]->PolyDef->vertices[1].Set( -12.0f, -1.0f);
	g_platformArray[2]->PolyDef->vertices[2].Set( -12.0f, -3.0f);
	g_platformArray[2]->PolyDef->vertices[3].Set( -8.0f, -4.0f);	
	g_platformArray[2]->PolyDef->vertices[4].Set( -2.0f, -4.0f);	
	g_platformArray[2]->PolyDef->vertices[5].Set( 3.0f, -2.0f);
	g_platformArray[2]->PolyDef->vertices[6].Set( 2.0f, -0.1f);	
	g_platformArray[2]->PolyDef->vertices[7].Set( -3.0f, 0.0f);

	g_platformArray[2]->PolyDef->density = 0.0f;
	g_platformArray[2]->PolyDef->friction = 1.0f;
	g_platformArray[2]->BodyDef = new b2BodyDef();
	g_platformArray[2]->BodyDef->position.Set(11.0f, -21.0f);
	g_platformArray[2]->BodyDef->AddShape(g_platformArray[2]->PolyDef);
	g_platformArray[2]->Body = g_world->CreateBody(g_platformArray[2]->BodyDef);

	g_platformArray[3] = new Poly();
	g_platformArray[3]->PolyDef = new b2PolyDef();	// bongo base
	g_platformArray[3]->PolyDef->vertexCount = 4;
	g_platformArray[3]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[3]->PolyDef->vertices[1].Set( 5.0f, -5.0f);
	g_platformArray[3]->PolyDef->vertices[2].Set( 10.0f,-3.0f);
	g_platformArray[3]->PolyDef->vertices[3].Set( 6.0f, 4.0f);	
	g_platformArray[3]->PolyDef->density = 0.0f;
	g_platformArray[3]->PolyDef->friction = 1.0f;
	g_platformArray[3]->BodyDef = new b2BodyDef();
	g_platformArray[3]->BodyDef->position.Set(30.0f, -15.0f);
	g_platformArray[3]->BodyDef->AddShape(g_platformArray[3]->PolyDef);
	g_platformArray[3]->Body = g_world->CreateBody(g_platformArray[3]->BodyDef);
	
	g_platformArray[4] = new Poly();
	g_platformArray[4]->PolyDef = new b2PolyDef();	// bongo top
	g_platformArray[4]->PolyDef->vertexCount = 3;
	g_platformArray[4]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[4]->PolyDef->vertices[1].Set( 5.0f, 4.0f);
	g_platformArray[4]->PolyDef->vertices[2].Set( 2.5f, 4.0f);
	g_platformArray[4]->PolyDef->restitution = 2.8F;
	g_platformArray[4]->PolyDef->density = 0.0f;
	g_platformArray[4]->PolyDef->friction = 1.0f;
	g_platformArray[4]->BodyDef = new b2BodyDef();
	g_platformArray[4]->BodyDef->position.Set(30.0f, -15.0f);
	g_platformArray[4]->BodyDef->AddShape(g_platformArray[4]->PolyDef);
	g_platformArray[4]->Body = g_world->CreateBody(g_platformArray[4]->BodyDef);
	
	g_platformArray[5] = new Poly();
	g_platformArray[5]->PolyDef = new b2PolyDef();	// plat under bongo
	g_platformArray[5]->PolyDef->vertexCount = 4;
	g_platformArray[5]->PolyDef->vertices[0].Set( 0.0f, -0.5f);
	g_platformArray[5]->PolyDef->vertices[1].Set( 0.0f, -4.0f);
	g_platformArray[5]->PolyDef->vertices[2].Set( 13.0f,-4.0f);
	g_platformArray[5]->PolyDef->vertices[3].Set( 13.0f, 1.0f);	
	g_platformArray[5]->PolyDef->density = 0.0f;
	g_platformArray[5]->PolyDef->friction = 1.0f;
	g_platformArray[5]->BodyDef = new b2BodyDef();
	g_platformArray[5]->BodyDef->position.Set(27.0f, -20.5f);
	g_platformArray[5]->BodyDef->AddShape(g_platformArray[5]->PolyDef);
	g_platformArray[5]->Body = g_world->CreateBody(g_platformArray[5]->BodyDef);
}

// INIT PLAYER AND RANDOM BALLS (FOR NOW)
void initPlayer()
{
	for(int i=2; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = (rand() % 30)+200; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
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

		if(i == 0) // If we are the main player create a collision circle around it so detection is earlier than the visible character
		{
			g_spriteArray[i].ColCircleDef = new b2CircleDef();
			g_spriteArray[i].ColBodyDef =  new b2BodyDef();
		
			g_spriteArray[i].ColCircleDef->radius = 34 / 2 * SCALE; 
			g_spriteArray[i].ColCircleDef->density = 1.0F; 
			g_spriteArray[i].ColCircleDef->friction = 1.0F; 
			g_spriteArray[i].ColCircleDef->restitution = 0.4F;
		
			g_spriteArray[i].ColBodyDef->position.Set(g_spriteArray[i].X * SCALE, g_spriteArray[i].Y * SCALE);
			g_spriteArray[i].ColBodyDef->AddShape(g_spriteArray[i].ColCircleDef);
		
			g_spriteArray[i].ColBody = g_world->CreateBody(g_spriteArray[i].ColBodyDef);
		}
		else
			g_spriteArray[i].ColBody = g_spriteArray[i].Body; // We are an enemy so just make the collision body the main body to save CPU time
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
	g_spriteArray[0].CircleDef->restitution = 0.0F; 

	g_spriteArray[0].BodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
	g_spriteArray[0].BodyDef->allowSleep = false;
	g_spriteArray[0].BodyDef->preventRotation = false;
	g_spriteArray[0].BodyDef->angularDamping = 0.04f;
	g_spriteArray[0].BodyDef->linearDamping = 0.00f;

	g_spriteArray[0].BodyDef->AddShape(g_spriteArray[0].CircleDef);
	
	g_spriteArray[0].Body = g_world->CreateBody(g_spriteArray[0].BodyDef);
	
	// INIT PLAYER COLLISION
	g_spriteArray[0].ColCircleDef = new b2CircleDef();
	g_spriteArray[0].ColBodyDef =  new b2BodyDef();
	
	g_spriteArray[0].ColCircleDef->radius = 34 / 2 * SCALE; 
	g_spriteArray[0].ColCircleDef->density = 1.0F; 
	g_spriteArray[0].ColCircleDef->friction = 1.0F; 
	g_spriteArray[0].ColCircleDef->restitution = 0.0F;

	g_spriteArray[0].ColBodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
	g_spriteArray[0].ColBodyDef->allowSleep = false;
	g_spriteArray[0].ColBodyDef->preventRotation = false;
	g_spriteArray[0].ColBodyDef->angularDamping = 0.04f;
	g_spriteArray[0].ColBodyDef->linearDamping = 0.00f;

	g_spriteArray[0].ColBodyDef->AddShape(g_spriteArray[0].ColCircleDef);
	
	g_spriteArray[0].ColBody = g_world->CreateBody(g_spriteArray[0].ColBodyDef);

		// me playing with a box

		g_spriteArray[1].Action = ACTION_NONE;	
		g_spriteArray[1].X = 65; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_spriteArray[1].Y = 25; //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_spriteArray[1].Type = BALLTYPE_OBJECT;
		
		g_spriteArray[1].PolyDef = new b2PolyDef();	
		g_spriteArray[1].BodyDef =  new b2BodyDef();
		// when jumped on, drops into the platform
		g_spriteArray[1].PolyDef->vertexCount = 4;
		g_spriteArray[1].PolyDef->vertices[0].Set( -1.6f, 1.6f);
		g_spriteArray[1].PolyDef->vertices[1].Set( -1.6f, -1.6f);
		g_spriteArray[1].PolyDef->vertices[2].Set( 1.6f,-1.6f);
		g_spriteArray[1].PolyDef->vertices[3].Set( 1.6f, 1.6f);		
		g_spriteArray[1].PolyDef->density = 0.1F; 
		g_spriteArray[1].PolyDef->friction = 0.05F; 
		g_spriteArray[1].PolyDef->restitution = 0.7F; 

		g_spriteArray[1].BodyDef->position.Set(g_spriteArray[1].X * SCALE, g_spriteArray[1].Y * SCALE);
		g_spriteArray[1].BodyDef->AddShape(g_spriteArray[1].PolyDef);
		
		g_spriteArray[1].Body = g_world->CreateBody(g_spriteArray[1].BodyDef);



}




//	INIT BOX2D ENGINE FOR LEVEL
void initBox2D()
{

}