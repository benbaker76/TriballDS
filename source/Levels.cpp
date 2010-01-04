#include "Levels.h"
#include "Globals.h"

void Level1()

{
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
	g_platformArray[0]->PolyDef->restitution = 0.0f;
	g_platformArray[0]->BodyDef = new b2BodyDef();
	g_platformArray[0]->BodyDef->position.Set(15.0f, -2.4f);
	g_platformArray[0]->BodyDef->AddShape(g_platformArray[0]->PolyDef);
	g_platformArray[0]->Body = g_world->CreateBody(g_platformArray[0]->BodyDef);
	
	
	g_platformArray[1] = new Poly();
	g_platformArray[1]->PolyDef = new b2PolyDef();	// little plat
	g_platformArray[1]->PolyDef->vertexCount = 8;
	g_platformArray[1]->PolyDef->vertices[0].Set( -9.0f, 0.0f);
	g_platformArray[1]->PolyDef->vertices[1].Set( -12.0f, -1.0f);
	g_platformArray[1]->PolyDef->vertices[2].Set( -12.0f, -3.0f);
	g_platformArray[1]->PolyDef->vertices[3].Set( -8.0f, -4.0f);	
	g_platformArray[1]->PolyDef->vertices[4].Set( -2.0f, -4.0f);	
	g_platformArray[1]->PolyDef->vertices[5].Set( 3.0f, -2.0f);
	g_platformArray[1]->PolyDef->vertices[6].Set( 2.0f, -0.1f);	
	g_platformArray[1]->PolyDef->vertices[7].Set( -3.0f, 0.0f);

	g_platformArray[1]->PolyDef->density = 0.0f;
	g_platformArray[1]->PolyDef->friction = 1.0f;
	g_platformArray[1]->PolyDef->restitution = 0.0f;
	g_platformArray[1]->BodyDef = new b2BodyDef();
	g_platformArray[1]->BodyDef->position.Set(11.0f, -21.0f);
	g_platformArray[1]->BodyDef->AddShape(g_platformArray[1]->PolyDef);
	g_platformArray[1]->Body = g_world->CreateBody(g_platformArray[1]->BodyDef);

	g_platformArray[2] = new Poly();
	g_platformArray[2]->PolyDef = new b2PolyDef();	// bongo base
	g_platformArray[2]->PolyDef->vertexCount = 4;
	g_platformArray[2]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[2]->PolyDef->vertices[1].Set( 5.0f, -5.0f);
	g_platformArray[2]->PolyDef->vertices[2].Set( 10.0f,-3.0f);
	g_platformArray[2]->PolyDef->vertices[3].Set( 6.0f, 4.0f);
	
	g_platformArray[2]->PolyDef->density = 0.0f;
	g_platformArray[2]->PolyDef->friction = 1.0f;
	g_platformArray[2]->PolyDef->restitution = 0.0f;
	g_platformArray[2]->BodyDef = new b2BodyDef();
	g_platformArray[2]->BodyDef->position.Set(30.0f, -15.0f);
	g_platformArray[2]->BodyDef->AddShape(g_platformArray[2]->PolyDef);
	g_platformArray[2]->Body = g_world->CreateBody(g_platformArray[2]->BodyDef);
	
	g_platformArray[3] = new Poly();
	g_platformArray[3]->PolyDef = new b2PolyDef();	// bongo top
	g_platformArray[3]->PolyDef->vertexCount = 3;
	g_platformArray[3]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[3]->PolyDef->vertices[1].Set( 5.0f, 4.0f);
	g_platformArray[3]->PolyDef->vertices[2].Set( 2.5f, 4.0f);
	
	g_platformArray[3]->PolyDef->density = 0.0f;
	g_platformArray[3]->PolyDef->friction = 1.0f;
	g_platformArray[3]->PolyDef->restitution = 1.0f;
	g_platformArray[3]->BodyDef = new b2BodyDef();
	g_platformArray[3]->BodyDef->position.Set(30.0f, -15.0f);
	g_platformArray[3]->BodyDef->AddShape(g_platformArray[3]->PolyDef);
	g_platformArray[3]->Body = g_world->CreateBody(g_platformArray[3]->BodyDef);
	
	g_platformArray[4] = new Poly();
	g_platformArray[4]->PolyDef = new b2PolyDef();	// plat under bongo
	g_platformArray[4]->PolyDef->vertexCount = 4;
	g_platformArray[4]->PolyDef->vertices[0].Set( 0.0f, -0.5f);
	g_platformArray[4]->PolyDef->vertices[1].Set( 0.0f, -4.0f);
	g_platformArray[4]->PolyDef->vertices[2].Set( 13.0f,-4.0f);
	g_platformArray[4]->PolyDef->vertices[3].Set( 13.0f, 1.0f);
	
	g_platformArray[4]->PolyDef->density = 0.0f;
	g_platformArray[4]->PolyDef->friction = 1.0f;
	g_platformArray[4]->PolyDef->restitution = 0.0f;
	g_platformArray[4]->BodyDef = new b2BodyDef();
	g_platformArray[4]->BodyDef->position.Set(27.0f, -20.5f);
	g_platformArray[4]->BodyDef->AddShape(g_platformArray[4]->PolyDef);
	g_platformArray[4]->Body = g_world->CreateBody(g_platformArray[4]->BodyDef);
	
	g_platformArray[5] = new Poly();
	g_platformArray[5]->PolyDef = new b2PolyDef();	// mid left
	g_platformArray[5]->PolyDef->vertexCount = 4;
	g_platformArray[5]->PolyDef->vertices[0].Set( 0.0f, -0.0f);
	g_platformArray[5]->PolyDef->vertices[1].Set( 0.0f, -4.0f);
	g_platformArray[5]->PolyDef->vertices[2].Set( 13.0f,-4.0f);
	g_platformArray[5]->PolyDef->vertices[3].Set( 13.0f, 0.0f);
	
	g_platformArray[5]->PolyDef->density = 0.0f;
	g_platformArray[5]->PolyDef->friction = 1.0f;
	g_platformArray[5]->PolyDef->restitution = 0.0f;
	g_platformArray[5]->BodyDef = new b2BodyDef();
	g_platformArray[5]->BodyDef->position.Set(-27.0f, -12.1f);
	g_platformArray[5]->BodyDef->AddShape(g_platformArray[5]->PolyDef);
	g_platformArray[5]->Body = g_world->CreateBody(g_platformArray[5]->BodyDef);

	g_platformArray[6] = new Poly();
	g_platformArray[6]->PolyDef = new b2PolyDef();	// top mid
	g_platformArray[6]->PolyDef->vertexCount = 4;
	g_platformArray[6]->PolyDef->vertices[0].Set( 0.0f, -1.0f);
	g_platformArray[6]->PolyDef->vertices[1].Set( 0.0f, -3.0f);
	g_platformArray[6]->PolyDef->vertices[2].Set( 11.0f,-2.0f);
	g_platformArray[6]->PolyDef->vertices[3].Set( 11.0f, 0.0f);
	
	g_platformArray[6]->PolyDef->density = 0.0f;
	g_platformArray[6]->PolyDef->friction = 1.0f;
	g_platformArray[6]->PolyDef->restitution = 0.0f;
	g_platformArray[6]->BodyDef = new b2BodyDef();
	g_platformArray[6]->BodyDef->position.Set(-10.5f, 17.0f);
	g_platformArray[6]->BodyDef->AddShape(g_platformArray[6]->PolyDef);
	g_platformArray[6]->Body = g_world->CreateBody(g_platformArray[6]->BodyDef);

	g_platformArray[7] = new Poly();
	g_platformArray[7]->PolyDef = new b2PolyDef();	// top left
	g_platformArray[7]->PolyDef->vertexCount = 4;
	g_platformArray[7]->PolyDef->vertices[0].Set( 0.0f, -0.0f);
	g_platformArray[7]->PolyDef->vertices[1].Set( 0.0f, -6.0f);
	g_platformArray[7]->PolyDef->vertices[2].Set( 16.0f, -4.0f);
	g_platformArray[7]->PolyDef->vertices[3].Set( 16.0f, -1.0f);
	
	g_platformArray[7]->PolyDef->density = 0.0f;
	g_platformArray[7]->PolyDef->friction = 1.0f;
	g_platformArray[7]->PolyDef->restitution = 0.0f;
	g_platformArray[7]->BodyDef = new b2BodyDef();
	g_platformArray[7]->BodyDef->position.Set(-40.0f, 9.0f);
	g_platformArray[7]->BodyDef->AddShape(g_platformArray[7]->PolyDef);
	g_platformArray[7]->Body = g_world->CreateBody(g_platformArray[7]->BodyDef);	
	
	g_platformArray[8] = new Poly();
	g_platformArray[8]->PolyDef = new b2PolyDef();	// top right
	g_platformArray[8]->PolyDef->vertexCount = 4;
	g_platformArray[8]->PolyDef->vertices[0].Set( 0.0f, -0.0f);
	g_platformArray[8]->PolyDef->vertices[1].Set( 0.0f, -6.0f);
	g_platformArray[8]->PolyDef->vertices[2].Set( 30.0f, -4.0f);
	g_platformArray[8]->PolyDef->vertices[3].Set( 30.0f, 5.0f);
	
	g_platformArray[8]->PolyDef->density = 0.0f;
	g_platformArray[8]->PolyDef->friction = 1.0f;
	g_platformArray[8]->PolyDef->restitution = 0.0f;
	g_platformArray[8]->BodyDef = new b2BodyDef();
	g_platformArray[8]->BodyDef->position.Set(10.0f, 20.0f);
	g_platformArray[8]->BodyDef->AddShape(g_platformArray[8]->PolyDef);
	g_platformArray[8]->Body = g_world->CreateBody(g_platformArray[8]->BodyDef);	
	
	g_platformArray[9] = new Poly();
	g_platformArray[9]->PolyDef = new b2PolyDef();	// FLOOR
	g_platformArray[9]->PolyDef->vertexCount = 4;
	g_platformArray[9]->PolyDef->vertices[0].Set( 0.0f, 0.0f);
	g_platformArray[9]->PolyDef->vertices[1].Set( 0.0f, -2.0f);
	g_platformArray[9]->PolyDef->vertices[2].Set( 80.0f, -2.0f);
	g_platformArray[9]->PolyDef->vertices[3].Set( 80.0f, 0.0f);
	
	g_platformArray[9]->PolyDef->density = 0.0f;
	g_platformArray[9]->PolyDef->friction = 1.0f;
	g_platformArray[9]->PolyDef->restitution = 0.0f;
	g_platformArray[9]->BodyDef = new b2BodyDef();
	g_platformArray[9]->BodyDef->position.Set(-40.0f, -39.0f);
	g_platformArray[9]->BodyDef->AddShape(g_platformArray[9]->PolyDef);
	g_platformArray[9]->Body = g_world->CreateBody(g_platformArray[9]->BodyDef);		
}
