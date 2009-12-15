#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <nds.h>
#include "Box2D.h"

// Having this struct inside here means it won't make a copy each time you #include it

struct Sprite							// define the elements that construct our 'balls'
{
	b2CircleDef* CircleDef;
	b2BodyDef* BodyDef;
	b2Body* Body;
	
	float X;
	float Y;
	float XSpeed;
	float YSpeed;
	int Angle;
	int Status;
	int Type;
	int Action;
};

struct Rect
{
	Rect() : X(0), Y(0), Width(0), Height(0) {}
	Rect(int x, int y, int width, int height) : X(x), Y(y), Width(width), Height(height) {}
	int X;
	int Y;
	int Width;
	int Height;
};

struct RectF
{
	RectF() : X(0), Y(0), Width(0), Height(0) {}
	RectF(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height) {}
	float X;
	float Y;
	float Width;
	float Height;
};

struct PointF
{
	PointF() : X(0), Y(0) {}
	PointF(float x, float y) : X(x), Y(y) {}
	float X;
	float Y;
};

struct Point
{
	Point() : X(0), Y(0) {}
	Point(int x, int y) : X(x), Y(y) {}
	int X;
	int Y;
};

struct Vector2
{
	Vector2() : X(0), Y(0) {}
	Vector2(int x, int y) : X(x), Y(y) {}
	float X;
	float Y;
};

struct Vector3
{
	Vector3() : X(0), Y(0), Z(0) {}
	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
	float X;
	float Y;
	float Z;
};

struct Size
{
	Size() : Width(0), Height(0) {}
	Size(int width, int height) : Width(width), Height(height) {}
	int Width;
	int Height;
};

struct SizeF
{
	SizeF() : Width(0), Height(0) {}
	SizeF(float width, float height) : Width(width), Height(height) {}
	float Width;
	float Height;
};


enum QuadFlags
{
	QUADFLAGS_NONE = BIT(0),
	QUADFLAGS_HFLIP = BIT(1),
	QUADFLAGS_VFLIP = BIT(2)
};

#define BG0_MAP_BASE				31
#define BG0_MAP_BASE_SUB			31

#define BG0_TILE_BASE				0
#define BG0_TILE_BASE_SUB			0

#define SCREEN_WIDTH				256
#define SCREEN_HEIGHT				192

#define LEVEL_WIDTH					512
#define LEVEL_HEIGHT				512
	
	// Defines for the physics effects
#define 	ACCEL				0.35F			// Horizontal acceleration on forced movement		( *1.5 when turning)
#define		MAXACCEL			16.0F			// Maximum horizontal speed
#define		TURNSPEED			2.5F			// multiple of ACCEL when turning (should only work when on platform)
#define		FRICTION			0.1f
#define 	ROTSPEED			0.2f			// Speed in which to increase rotation when moving on platform
#define		ROTMAX				8.0f			// Maximum 'controlled' rotation speed

// THESE ARE NOT USED CURRENTLY
#define		BOUNCE_X_DEADEN		1.55F			// how much to deaden a horzontal bounce
#define		JUMPSPEED			24.0F			// initial speed of a jump (negetive)				( > higher jump)
#define		GRAVITY				0.18F			// force of gravity									(affects jump and bounce)
#define		BOUNCEFACTOR		0				// used to reverse a vertical drop for a bounce		( < for more bounces)
#define		BOUNCEFACTORAMOUNT	1.55F			// the amount of bounce to allow on a vertical drop	( > for smaller bounce)
#define		MAXYSPEED			4.5F			// maximum speed you can drop
#define		ROLLSPEEDLIMIT		1F				// speed at which a roll works of a platform
#define		ROLLSPEED			0.15F			// amount ball speed is affected by a roll


	// Defines for ball properties
#define		BALLSCROLLX			72				// closeness to sides to enable scrolling
#define		BALLSCROLLY			80				// closeness to top/bot to enable scrolling
#define		BALLSIZE			24				// Size of balls
#define		BALLOFFSET			4				// Ball offset of balls
#define		BALLCOUNT			10				// Number of balls

	// Defines for player (ball) status
	
#define		BALLSTATUS_NORMAL				0
#define		BALLSTATUS_JUMPING				1
#define		BALLSTATUS_FALLING				2
#define		BALLSTATUS_GROUNDTOUCH			3

	//	Defines for the balls type
	
#define		BALLTYPE_NULL		0
#define		BALLTYPE_PLAYER		1
#define		BALLTYPE_EVILBALL	2

	// Defines for ball actions
#define		ACTION_NONE			0
#define		ACTION_MOVELEFT		1
#define		ACTION_MOVERIGHT	2
#define		ACTION_SLOW			3
#define		ACTION_JUMP			4

	// Collision data (these are the decypted values from colmaps)
	
#define		PLATFORM			2				// the highest value of a decryted colmap platform

#define		BLANK				0
#define		SOLID				1
#define		JUMPTHROUGH			2
#define		CRUMBLER			3
#define		FALLER				4

#define		DEADLY				64

#define		SCALE				0.1F

#define		VELX				5.0F
#define		VELY				5.0F
#define		IMPX				0.01F
#define		IMPY				0.01F

#define		TEXTURECOUNT			8

#define		TEXTURE_BALL01			0
#define		TEXTURE_BALL02			1
#define		TEXTURE_BALL03			2
#define		TEXTURE_BALL04			3
#define		TEXTURE_LEVEL01_1		4
#define		TEXTURE_LEVEL01_2		5
#define		TEXTURE_LEVEL01_3		6
#define		TEXTURE_LEVEL01_4		7

#endif

// This means that g_playerBall; is defined externally (in Globals.cpp)

extern Sprite g_spriteArray[];
extern Vector2 g_scrollPos;
extern Vector3 g_cameraPos;
extern Vector3 g_cameraStart;
extern Vector3 g_cameraEnd;

extern SizeF g_texelSize;

extern int g_frameCount;

// Box2d vars
extern float timeStep;
extern int iterations;

extern bool g_jumpTrap;
extern bool g_reJump;

extern Sprite g_colSprite1;
extern Sprite g_colSprite2;

extern b2AABB* g_worldAABB;
extern b2World* g_world;
extern b2Vec2* g_gravity;
extern b2BoxDef* g_groundBoxDef;
extern b2BodyDef* g_groundBodyDef;
extern b2BoxDef* g_boxDef;
extern b2BodyDef* g_bodyDef;
extern b2Body* g_body;

extern b2PolyDef* g_plat;

extern int g_textureIDS[];
extern int g_palAddress[];

