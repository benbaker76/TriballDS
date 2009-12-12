/*
 * Copyright (c) 2007 Eric Jordan
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef B2_POLYGON_H
#define B2_POLYGON_H

#include "../Common/b2Math.h"
#include "b2Triangle.h"

class b2Polygon;

int32 TriangulatePolygon(Fixed* xv, Fixed* yv, int32 vNum, b2Triangle* results);
bool IsEar(int32 i, Fixed* xv, Fixed* yv, int32 xvLength);
int32 PolygonizeTriangles(b2Triangle* triangulated, int32 triangulatedLength, b2Polygon* polys, int32 polysLength);
int32 DecomposeConvex(b2Polygon* p, b2Polygon* results, int32 maxPolys);
b2PolyDef* DecomposeConvexAndAddTo(b2Polygon* p, b2BodyDef* bd, b2PolyDef* prototype);
b2Polygon ConvexHull(b2Vec2* v, int nVert);
b2Polygon ConvexHull(Fixed* cloudX, Fixed* cloudY, int32 nVert);
void ReversePolygon(Fixed* x, Fixed* y, int n);

class b2Polygon {
	
public:
    const static int32 maxVerticesPerPolygon = b2_maxPolyVertices;
    Fixed* x; //vertex arrays
    Fixed* y;
    int32 nVertices;
	
    b2Polygon(Fixed* _x, Fixed* _y, int32 nVert);
    b2Polygon(b2Vec2* v, int32 nVert);
	b2Polygon();
    ~b2Polygon();
	
	void MergeParallelEdges(Fixed tolerance);
    b2Vec2* GetVertexVecs();
    b2Polygon(b2Triangle& t);
    void Set(b2Polygon& p);
    bool IsConvex();
//    bool IsSimple();
    void AddTo(b2PolyDef& pd);
	
    b2Polygon* Add(b2Triangle& t);
	
};

#endif
