int feetCentre(float Xcoord,float Ycoord, int Type);
int headCentre(float Xcoord,float Ycoord, int Type);
int feetLeft(float Xcoord,float Ycoord, int Type);
int feetRight(float Xcoord,float Ycoord, int Type);

int bodyRight(float Xcoord,float Ycoord, int Type);
int bodyLeft(float Xcoord,float Ycoord, int Type);

int collisionDecrypt(int colData);

//void checkCollision(Ball* pBall1, Ball* pBall2);
void fixBoundary(Sprite* pSprite);