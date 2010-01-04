#include "Box2d.h"

b2Body* getBodyAtPoint(b2Vec2* v);
void updateWorldContacts();
void updateCharacterContacts(Circle *pChar);
void updateGroundCollision();