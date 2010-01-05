#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <nds.h>
#include "Box2D.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

// Having this struct inside here means it won't make a copy each time you #include it

struct Input
{
	touchPosition touch;
	int keysHeld;
	int keysDown;
	int keysUp;
};

struct Circle						// define the elements that construct our 'balls'
{
	b2PolyDef* PolyDef;
	b2CircleDef* CircleDef;
	b2BoxDef* BoxDef;
	
	b2BodyDef* BodyDef;
	b2Body* Body;
	
	float X;
	float Y;
	int Status;
	int Type;
	int Action;
	bool OnGround;
	bool OnCeil;
	
	int JumpCount;			// using for debug and testing for now

};

struct Poly
{
	b2PolyDef* PolyDef;
	b2BodyDef* BodyDef;
	b2Body* Body;
};

struct Trail
{
	float X;
	float Y;
	float Rot;
};

/* struct Rect
{
	Rect() : Left(0), Top(0), Right(0), Bottom(0) {}
	Rect(int left, int top, int right, int bottom) : Left(left), Top(top), Right(right), Bottom(bottom) {}
	int Left;
	int Top;
	int Right;
	int Bottom;
};

struct RectF
{
	RectF() : Left(0), Top(0), Right(0), Bottom(0) {}
	RectF(float left, float top, float right, float bottom) : Left(left), Top(top), Right(right), Bottom(bottom) {}
	float Left;
	float Top;
	float Right;
	float Bottom;
}; */

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

#define PI 3.1415926536

#define BG0_MAP_BASE				31
#define BG0_MAP_BASE_SUB			31

#define BG0_TILE_BASE				7
#define BG0_TILE_BASE_SUB			7

#define SCREEN_WIDTH				256
#define SCREEN_HEIGHT				192

#define LEVEL_WIDTH					512
#define LEVEL_HEIGHT				512
	
	// Defines for the physics effects
#define 	ACCEL				1.05F			// Horizontal acceleration on forced movement		( *1.5 when turning)
#define		MAXACCEL			25.0F			// Maximum horizontal speed
#define		TURNSPEED			2.6F			// multiple of ACCEL when turning (should only work when on platform)
#define		FRICTION			0.2f
#define 	ROTSPEED			0.2f			// Speed in which to increase rotation when moving on platform
#define		ROTMAX				16.0f			// Maximum 'controlled' rotation speed
#define		JUMPSPEED			45.0F			// initial speed of a jump
#define		AIRSPIN				4.5f			// multiplyer to add spin to a jumping ball

	// Defines for ball properties
#define		BALLSIZE			32				// Size of balls
#define		BALLCOUNT			3				// Number of balls

	// Defines for player (ball) status
	
#define		BALLSTATUS_NORMAL				0
#define		BALLSTATUS_JUMPING				1

	//	Defines for the balls type
	
#define		BALLTYPE_NULL		0				// non moving object
#define		BALLTYPE_PLAYER		1
#define		BALLTYPE_EVILBALL	2
#define		BALLTYPE_OBJECT		3				// General stationary object

	// Defines for ball actions
#define		ACTION_NONE			0
#define		ACTION_MOVELEFT		1
#define		ACTION_MOVERIGHT	2
#define		ACTION_SLOW			3
#define		ACTION_NEWJUMP		4

	// Define for scale factor
#define		SCALE				0.1F

	// Defines for GL level contruction
#define		LEVELTEXTURECOUNT		(4 * 4)
#define		LEVELQUADCOUNT			(4 * 4)
#define		TEXTURECOUNT			(LEVELTEXTURECOUNT + 7)

	// Defines for objects
#define		TEXTURE_BALL01			(LEVELTEXTURECOUNT + 0)
#define		TEXTURE_BALL02			(LEVELTEXTURECOUNT + 1)
#define		TEXTURE_BALL03			(LEVELTEXTURECOUNT + 2)
#define		TEXTURE_BALL04			(LEVELTEXTURECOUNT + 3)

#define		TEXTURE_PARTICLE		(LEVELTEXTURECOUNT + 4)
#define		TEXTURE_TRAIL			(LEVELTEXTURECOUNT + 5)

#define		TEXTURE_BOX01			(LEVELTEXTURECOUNT + 6)

#define		PLATFORMCOUNT			10

#define		PALETTE_BALL			0
#define		PALETTE_PARTICLE		1
#define		PALETTE_TRAIL			2
#define		PALETTE_BOX				3
#define		PALETTE_LEVEL			4

	// Defines for the Trails on the Players balls
	
#define		TRAILINTERVAL			1		// Size of the interval between trail parts
#define		TRAILAMOUNT				50		// Number of balls in the trail

#endif

// This means that g_playerBall; is defined externally (in Globals.cpp)

extern Circle g_spriteArray[];

extern Trail TrailPoints[];


extern Point TT[];			// could not work out how to use points in Circle (left books at work)


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
extern int g_reJump;

extern b2AABB* g_worldAABB;
extern b2World* g_world;
extern b2Vec2* g_gravity;

extern b2BoxDef* g_groundBoxDef;
extern b2BodyDef* g_groundBodyDef;
extern b2Body* g_groundBody;

extern Poly* g_platformArray[];

extern int g_textureIDS[];
extern int g_palAddress[];

extern int g_levelNum;

//extern int m_levelTextureLoaded[];

extern int g_levelQuadID[];
//extern bool g_levelQuadLoaded[];
extern const u8* g_levelTexture;
extern const u16* g_levelPalette;
extern Size g_levelSize;
extern int g_levelTextureSize;
extern GL_TEXTURE_SIZE_ENUM g_glTextureSize;
extern Size g_levelGridSize;
extern SizeF g_levelQuadSize;

extern Input g_input;

bool IntersectRect(Rect* pRectA, Rect* pRectB);
bool IntersectRectF(RectF* pRectA, RectF* pRectB);

extern int g_trailPos;

