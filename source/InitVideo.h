// game inits

#include "Globals.h"

void initVideo2D();
void getLevelQuad(int x, int y, RectF* rect);
void loadTextures();
int loadTexture(int textureSlot, int textureId);
int reloadTexture(int textureSlot, int textureId);
int reloadTexture(int textureId, const u8* texture, int frameNum, GL_TEXTURE_SIZE_ENUM textureSize);
void loadLevel();
void initVideo3D();
