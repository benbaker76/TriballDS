#include "DrawMap.h"
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

void drawTrail()
{
	glBindTexture(0, g_textureIDS[TEXTURE_TRAIL]);
	glColorTable(GL_RGB32_A3, g_palAddress[1]); // were using 32 colors with 5 bits for alpha so edges can be alpha blended out
	
	int drawPos = g_trailPos;
	float drawScale = 0.4f;
	float scaleStep = drawScale / (TRAILAMOUNT - 1);
	float alphaStep = 15 / TRAILAMOUNT;
	float alphaValue = 15;

	for (int register i=0; i<TRAILAMOUNT; i++)
	{
		// Set the alpha value so it will alpha blend out more towards the end of the trail
		//int alphaValue = (((float)(TRAILAMOUNT - i) / (float) TRAILAMOUNT) * 30.0F) + 1;		// just trying to take the calcs out of the loop
		glPolyFmt(POLY_ALPHA((alphaValue -= alphaStep)) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);

		glBegin(GL_QUAD);
		
		drawPos += TRAILINTERVAL - (i / TRAILINTERVAL);	// the (i/ti) is to pull the end sections closer (a kludge)
		if (drawPos > (TRAILINTERVAL * TRAILAMOUNT) - 1) drawPos = drawPos - (TRAILINTERVAL * TRAILAMOUNT);
		glPushMatrix();
		glTranslatef(TrailPoints[drawPos].X, TrailPoints[drawPos].Y, -1 + 0.01F);
		glRotatef(TrailPoints[drawPos].Rot, 0.0f, 0.0f, 1.0f);
		
		// Draw it red..
		drawQuad(drawScale -= scaleStep, 32, 255, 0, 0, QUADFLAGS_NONE);
		glPopMatrix(1);

		glEnd();
	}
	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3);
}