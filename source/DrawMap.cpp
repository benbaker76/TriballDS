#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "DrawMap.h"
#include "Globals.h"


void drawMap()
{

// the idea of this piece of code 'will' be that you pass the X,Y coord of player (0-511) and the scroll possition will be updated to match
// and also the enemies and other sprites will be affected also. This will have to eventually return 2 offsets (0-256) for the scroll (though
// they could be read from the H/VOFS, so that perhaps we can use our own sprite draw code to only show onscreen sprites? Hmmm
// Yep - this is a tricky bit for me

// also, scrolling messes up the idea of multiple players on screen (or does it?)


	REG_BG1HOFS_SUB = (int)g_levelX;
	
	REG_BG1VOFS_SUB = (int)g_levelY;
	
//	g_levelX = XCoord;
//	g_levelY = 320;
}