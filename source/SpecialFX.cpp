#include "DrawMap.h"
#include "Globals.h"

void trailUpdate()					// Update the trail (call every vblank)
{
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	TrailPoints[g_trailPos].X = position.x * SCALE;			// do SCALE here as it is quicker to do once here
	TrailPoints[g_trailPos].Y = position.y * SCALE;			// than for all in the draw loop
//	TrailPoints[g_trailPos].Rot = g_spriteArray[0].Body->GetRotation() * (180 / PI);	// same for rotation (if needed)
	g_trailPos -= 1;
	if (g_trailPos < 0) g_trailPos = (TRAIL_INTERVAL * TRAIL_AMOUNT) - 1;
}

void initTrail()					// Set all trail points to player coords
{
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	for (int register i=0; i<TRAIL_AMOUNT * TRAIL_INTERVAL; i++)
	{
		TrailPoints[i].X = position.x * SCALE;			// do SCALE here as it is quicker to do once here
		TrailPoints[i].Y = position.y * SCALE;			// than for all in the draw loop
		TrailPoints[i].Rot = g_spriteArray[0].Body->GetRotation() * (180 / PI);	
	}

}

void drawTrail()
{
	glBindTexture(0, g_textureIDS[TEXTURE_PARTICLE]);
	glColorTable(GL_RGB32_A3, g_palAddress[PALETTE_PARTICLE]); // were using 32 colors with 5 bits for alpha so edges can be alpha blended out
	
	// Set color to blue
	glColor3b(0, 0, 255);

	int drawPos = g_trailPos;
	float drawScale = g_spriteArray[0].CircleDef->radius * 2 * SCALE;
	float scaleStep = drawScale / (TRAIL_AMOUNT - 1);
	float alphaStep = 8 / TRAIL_AMOUNT;
	float alphaValue = 8;
	b2Vec2 pos = g_spriteArray[0].Body->GetOriginPosition();
	float x1 = pos.x;
	float y1 = pos.y;

	for (int register i=0; i<TRAIL_AMOUNT; i++)
	{
		glPolyFmt(POLY_ID(i + 1) | POLY_ALPHA((alphaValue -= alphaStep)) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);
	
		glBegin(GL_QUAD);
		
		drawPos += TRAIL_INTERVAL;  // - (i / TRAIL_INTERVAL);	// the (i/ti) is to pull the end sections closer (a kludge)
		if (drawPos > (TRAIL_INTERVAL * TRAIL_AMOUNT) - 1) drawPos = drawPos - (TRAIL_INTERVAL * TRAIL_AMOUNT);
		
	//	double a1 = 180-atan2(TrailPoints[drawPos].X - x1,TrailPoints[drawPos].Y - y1) * 180 / PI; 
		
	//	x1 = TrailPoints[drawPos].X;
	//	y1 = TrailPoints[drawPos].Y;
		
		glPushMatrix();
		glTranslatef(TrailPoints[drawPos].X, TrailPoints[drawPos].Y, -1 + 0.01F);
	//	glRotatef(a1, 0.0f, 0.0f, 1.0f);
		
		drawQuad(drawScale, drawScale, 32, QUADFLAGS_NONE);
		
		drawScale -= scaleStep;
		
		glPopMatrix(1);

		glEnd();
	}
	
	// Set color back to white
	glColor3b(255, 255, 255);
	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);
}