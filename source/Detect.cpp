#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "ColMap.h"
#include "Control.h"
#include "Detect.h"
#include "Text.h"

//
// Check the char block at the centre of our ball
//
int feetCentre(float Xcoord,float Ycoord, int Type)
{
	int x = 0;
	int y = 0;
	int ySettle = 0;
	
	if (Type == BALLTYPE_EVILBALL)
	{
		x = (int)Xcoord + 8;
		y = (int)Ycoord + 24;
	}
	else if (Type == BALLTYPE_PLAYER)
	{

		x = (int)Xcoord + g_levelX + 8;
		y = (int)Ycoord + g_levelY + 24;		

		ySettle = (int(g_levelY + Ycoord) & 7);
	}
	int d3;
	
	if (ySettle < MAXYSPEED )
	{	
		int d1 = collisionDecrypt( bLevelData[( (y/8)*64 ) + (x/ 8) ] ); // was x-2
		int d2 = collisionDecrypt( bLevelData[((y/8)*64) + ((x + 7) / 8)] );
	
		if ((d1 == d2) && (d1 <= PLATFORM && d1 != BLANK)) d3=d1;
		else if ((d1 != BLANK && d1 <= PLATFORM)) d3=d1;
		else if ((d2 != BLANK && d2 <= PLATFORM)) d3=d2;	
		else d3 = BLANK;

		char buffer[20];
		sprintf(buffer, "%d CNTR ",d3) ;	
		DrawString(buffer, 10, 5, false);

		return d3;

	}
	return BLANK;

}
int headCentre(float Xcoord,float Ycoord, int Type)
{
	int x = 0;
	int y = 0;
	int ySettle = 0;
	
	if (Type == BALLTYPE_EVILBALL)
	{
		x = (int)Xcoord + 8;
		y = (int)Ycoord;
	}
	else if (Type == BALLTYPE_PLAYER)
	{

		x = (int)Xcoord + g_levelX + 8;
		y = (int)Ycoord + (int)g_levelY;		

		ySettle = int(g_levelY);
	}
	int d3;
	
	if ((y & 7) < MAXYSPEED )
	{	
		int d1 = collisionDecrypt( bLevelData[( (y/8)*64 ) + (x/ 8) ] ); // was x-2
		int d2 = collisionDecrypt( bLevelData[((y/8)*64) + ((x + 7) / 8)] );
	
		if ((d1 == d2) && (d1 <= PLATFORM && d1 != BLANK)) d3=d1;
		else if ((d1 != BLANK && d1 <= PLATFORM)) d3=d1;
		else if ((d2 != BLANK && d2 <= PLATFORM)) d3=d2;	
		else d3 = BLANK;

		char buffer[20];
		sprintf(buffer, "%d HDCT ",d3) ;	
		DrawString(buffer, 10, 8, false);

		return d3;

	}
	return BLANK;

}
int feetLeft(float Xcoord,float Ycoord, int Type)
{
	int x = 0;
	int y = 0;

	if (Type == BALLTYPE_EVILBALL)
	{
		x = (int)Xcoord + 1;
		y = (int)Ycoord + 24;
	}
	else if (Type == BALLTYPE_PLAYER)
	{

		x = (int)(Xcoord + g_levelX);		
		y = (int)Ycoord + g_levelY + 24;	
	}
		
		int d1 = collisionDecrypt( bLevelData[((y/8)*64) + (x / 8)] );

		char buffer[20];
		sprintf(buffer, "%d LEFT ",d1) ;	
		DrawString(buffer, 10, 8, false);

		return d1;		
}
int feetRight(float Xcoord,float Ycoord, int Type)
{
	int x = 0;
	int y = 0;

	if (Type == BALLTYPE_EVILBALL)
	{
		x = (int)Xcoord + 23;
		y = (int)Ycoord + 24;
	}
	else if (Type == BALLTYPE_PLAYER)
	{

		x = (int)Xcoord + g_levelX + 23;		
		y = (int)Ycoord + g_levelY + 24;	
	}
		
		int d1 = collisionDecrypt( bLevelData[((y/8)*64) + (x / 8)] );

		char buffer[20];
		sprintf(buffer, "%d RGHT ",d1) ;	
		DrawString(buffer, 10, 7, false);

		return d1;		
}

int bodyRight(float Xcoord,float Ycoord, int Type)
{
	int x = 0;
	int y = 0;
	
	if (Type == BALLTYPE_EVILBALL)
	{
		x = (int)Xcoord + 23;
		y = (int)Ycoord;
	}
	else if (Type == BALLTYPE_PLAYER)
	{
		x = (int)Xcoord + (int)g_levelX + 23;
		y = (int)Ycoord + (int)g_levelY;
	}
	return collisionDecrypt( bLevelData[((y/8)*64) + (x/8)] );

}
int bodyLeft(float Xcoord,float Ycoord, int Type)
{
	int x = 0;
	int y = 0;
	int ySettle = 0;
	
	if (Type == BALLTYPE_EVILBALL)
	{
		x = (int)Xcoord;
		y = (int)Ycoord;
	}
	else if (Type == BALLTYPE_PLAYER)
	{
		x = (int)Xcoord + (int)g_levelX;
		y = (int)Ycoord + (int)g_levelY;		
	}

	return collisionDecrypt( bLevelData[((y/8)*64) + (x/8)] );

}
// Calculate if there is a collision between two balls
// Pass in via pointers so values can be changed

// WHY does it make MEGA jumps???
/*void checkCollision(Ball* pSprite1, Ball* pSprite2)
{
//	if (((pSprite1->X >= pSprite2->X) && (pSprite1->X <= pSprite2->X+BALLSIZE)) && ((pSprite1->Y >= pSprite2->Y) && (pSprite1->Y <= pSprite2->Y+BALLSIZE)))
	{
		int xDist = pSprite2->X - pSprite1->X;
		int yDist = pSprite2->Y - pSprite1->Y;
		int dist = sqrt(xDist * xDist + yDist * yDist);
		float angle = atan2(yDist, xDist);
	 
		if (dist < BALLSIZE)
		{
			// Uncomment the following two lines so the balls repell each other instead of ball1 pushing ball2 away
			pSprite1->X = pSprite2->X - (BALLSIZE * cos(angle));
			pSprite1->Y = pSprite2->Y - (BALLSIZE * sin(angle));
		
			pSprite2->X = pSprite1->X + (BALLSIZE * cos(angle));
			pSprite2->Y = pSprite1->Y + (BALLSIZE * sin(angle));

		}
	}
}
*/
// Fix the boundaries of a ball so it doesn't go off the screen
void fixBoundary(Sprite* pSprite)
{
	if(pSprite->X + scrollCheckX(pSprite->Type)< 0)
		pSprite->X = 0;
	if(pSprite->Y + scrollCheckY(pSprite->Type)< 0)
		pSprite->Y = 0;
//	if(pSprite->X > LEVEL_WIDTH - BALLSIZE)
//		pSprite->X = LEVEL_WIDTH - BALLSIZE;
//	if(pSprite->Y > SCREEN_HEIGHT - BALLSIZE)
//		pSprite->Y = SCREEN_HEIGHT - BALLSIZE;
}
// Return a collision value based on the data in colmap
// 0-15		=	nothing						= 0
// 16-31	=	solid wall					= 1
// 32-47	=	jump-thru platform			= 2
// etc..
int collisionDecrypt(int colData)
{
	if (colData <=15) return BLANK;
	else if (colData <=31) return SOLID;
	else if (colData <=47) return JUMPTHROUGH;
	
	else return 0;
}