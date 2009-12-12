void moveSprite(Sprite *pSprite);

void updateSprite(Sprite* pSprite);

float rotateSprite(float fromX, float toX, int type, float oldX);

// void checkCollision(Sprite* pSprite1, Sprite* pSprite2);

// Fix the boundaries of a point so it doesn't go off the screen
void fixBoundary(Sprite* pSprite);

int scrollCheckX(int type);
int scrollCheckY(int type);