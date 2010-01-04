#include "DrawMap.h"
#include "Globals.h"

void trailUpdate()					// Update the trail (call every vblank)
{
	b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
	TrailPoints[g_trailPos].X = position.x * SCALE;			// do SCALE here as it is quicker to do once here
	TrailPoints[g_trailPos].Y = position.y * SCALE;			// than for all in the draw loop
//	TrailPoints[g_trailPos].Rot = g_spriteArray[0].Body->GetRotation() * (180 / PI);	// same for rotation (if needed)
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

void drawTrail()
{
	glBindTexture(0, g_textureIDS[TEXTURE_TRAIL]);
	glColorTable(GL_RGB32_A3, g_palAddress[PALETTE_TRAIL]); // were using 32 colors with 5 bits for alpha so edges can be alpha blended out
	
	// Set color to blue
	glColor3b(0, 0, 255);

	int drawPos = g_trailPos;
	float drawScale = 0.4f;
	float scaleStep = drawScale / (TRAILAMOUNT - 1);
	float alphaStep = 8 / TRAILAMOUNT;
	float alphaValue = 8;
	b2Vec2 pos = g_spriteArray[0].Body->GetOriginPosition();
	float x1 = pos.x;
	float y1 = pos.y;

	for (int register i=0; i<TRAILAMOUNT; i++)
	{
		// Set the alpha value so it will alpha blend out more towards the end of the trail
		//int alphaValue = (((float)(TRAILAMOUNT - i) / (float) TRAILAMOUNT) * 30.0F) + 1;		// just trying to take the calcs out of the loop
		//glPolyFmt(POLY_ALPHA(alphaValue) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);

		glPolyFmt(POLY_ALPHA((alphaValue -= alphaStep)) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);
	
		glBegin(GL_QUAD);
		
		drawPos += TRAILINTERVAL;  // - (i / TRAILINTERVAL);	// the (i/ti) is to pull the end sections closer (a kludge)
		if (drawPos > (TRAILINTERVAL * TRAILAMOUNT) - 1) drawPos = drawPos - (TRAILINTERVAL * TRAILAMOUNT);
		
	//	double n1 = sqrt(x1 * x1 + y1 * y1), n2 = sqrt(TrailPoints[drawPos].X * TrailPoints[drawPos].X + TrailPoints[drawPos].Y * TrailPoints[drawPos].Y);
	//	double a1 = acos((x1 * TrailPoints[drawPos].X + y1 * TrailPoints[drawPos].Y)/(n1 * n2)) * 180 / PI;	
		
		double a1 = 180-atan2(TrailPoints[drawPos].X - x1,TrailPoints[drawPos].Y - y1) * 180 / PI; 
		
		x1 = TrailPoints[drawPos].X;
		y1 = TrailPoints[drawPos].Y;
		
		glPushMatrix();
		glTranslatef(TrailPoints[drawPos].X, TrailPoints[drawPos].Y, -1 + 0.01F);
		glRotatef(a1, 0.0f, 0.0f, 1.0f);
		
		drawQuad(drawScale, drawScale, 32, QUADFLAGS_NONE);
		
		drawScale -= scaleStep;
		
		glPopMatrix(1);

		glEnd();
	}
	
	// Set color back to white
	glColor3b(255, 255, 255);
	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);
}