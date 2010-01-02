
#include "Globals.h"

void trailUpdate()					// Update the trail (call every vblank)
{
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	TrailPoints[g_trailPos].X = position.x * SCALE;			// do SCALE here as it is quicker to do once here
	TrailPoints[g_trailPos].Y = position.y * SCALE;			// than for all in the draw loop
	TrailPoints[g_trailPos].Rot = g_spriteArray[0].Body->GetRotation() * (180 / PI);	// same for rotation (if needed)
	g_trailPos -= 1;
	if (g_trailPos < 0) g_trailPos = (TRAILINTERVAL * TRAILAMOUNT) - 1;
}

void initTrail()					// Set all trail points to player coords
{
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	for (int register i=0; i<TRAILAMOUNT * TRAILINTERVAL; i++)
	{
		TrailPoints[i].X = position.x * SCALE;			// do SCALE here as it is quicker to do once here
		TrailPoints[i].Y = position.y * SCALE;			// than for all in the draw loop
		TrailPoints[i].Rot = g_spriteArray[0].Body->GetRotation() * (180 / PI);	
	}

}