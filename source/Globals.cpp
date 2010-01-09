#include <math.h>
#include "Globals.h"

// Here are the real global values. Place a "g_" before so you know you're referencing a global
// This is the real definition

Circle g_spriteArray[BALL_COUNT];

Trail TrailPoints[TRAIL_AMOUNT * TRAIL_INTERVAL];				// Sorry, but could not work out how to use PointF within Circle

//
// X,Y coord of game map
//
Vector2 g_scrollPos;
Vector3 g_cameraPos;
Vector3 g_cameraStart;
Vector3 g_cameraEnd;

SizeF g_texelSize;

RectF g_textureRect;

int g_frameCount;

//
// jump mechanics
//

bool g_jumpTrap = 0;
int g_reJump = 0;

// Trail position

int g_trailPos = 0;

// box2d

float timeStep;
int iterations;

b2AABB* g_worldAABB;
b2World* g_world;
b2Vec2* g_gravity;

b2BoxDef* g_groundBoxDef;
b2BodyDef* g_groundBodyDef;
//b2Body* g_groundBody;

Poly* g_platformArray[PLATFORM_COUNT];

int g_textureIDS[TEXTURE_COUNT];
int g_palAddress[PALETTE_COUNT];

float g_Zoom;

int g_levelNum;

//int m_levelTextureLoaded[LEVEL_TEXTURE_COUNT];

int g_levelQuadID[LEVEL_QUAD_COUNT];
//bool g_levelQuadLoaded[LEVEL_QUAD_COUNT];
const u8* g_levelTexture;
const u16* g_levelPalette;
Size g_levelSize;
GL_TEXTURE_SIZE_ENUM g_levelTextureSize;
Size g_levelGridSize;
SizeF g_levelQuadSize;

Input g_input;

Joint g_jointArray[JOINT_COUNT];

bool IntersectRectF(RectF* pRectA, RectF* pRectB)
{
	if(pRectA == NULL || pRectB == NULL)
		return false;
		
	float intersectLeft = max(pRectA->X, pRectB->X);
	float intersectTop = max(pRectA->Y, pRectB->Y);
	float intersectRight = min(pRectA->X + pRectA->Width, pRectB->X + pRectB->Width);
	float intersectBottom = min(pRectA->Y + pRectA->Height, pRectB->Y + pRectB->Height);
	
	return (intersectRight > intersectLeft && intersectBottom > intersectTop);
	
	//return ((pRectA->X < pRectB->X + pRectB->Width) && (pRectA->X + pRectA->Width > pRectB->X) && (pRectA->Y < pRectB->Y + pRectB->Height) && (pRectA->Y + pRectA->Height > pRectB->Y));
}

/* bool IntersectRectF(RectF* pRectA, RectF* pRectB)
{
	if(pRectA == NULL || pRectB == NULL)
		return false;

	return ((pRectA->Left < pRectB->Right) && (pRectA->Right> pRectB->Left) && (pRectA->Top < pRectB->Bottom) && (pRectA->Bottom > pRectB->Top));
} */
