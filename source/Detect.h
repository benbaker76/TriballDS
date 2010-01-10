#include "Box2d.h"

Object* getObjectAtPoint(b2Vec2* v);
void updateWorldContacts();
void updateCharacterContacts(Object *pObj);
void updateGroundCollision();