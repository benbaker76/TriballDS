#ifndef __EASING_H__
#define __EASING_H__

#define PI 3.1415926536

enum EaseCategory
{
	EaseCat_Linear,
	EaseCat_Quad,
	EaseCat_Cubic,
	EaseCat_Quart,
	EaseCat_Quint,
	EaseCat_Sine,
	EaseCat_Expo,
	EaseCat_Circ,
	EaseCat_Elastic,
	EaseCat_Back,
	EaseCat_Bounce
};

enum EaseType
{
	EaseType_None,
	EaseType_In,
	EaseType_Out,
	EaseType_InOut
};

class CBack
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseIn(float t, float b, float c, float d, float s);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d, float s);
	static float EaseInOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d, float s);
};

class CBounce
{
public:
	static float EaseOut(float t, float b, float c, float d);
	static float EaseIn(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CCirc
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CCubic
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CElastic
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseIn(float t, float b, float c, float d, float a, float p);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d, float a, float p);
	static float EaseInOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d, float a, float p);
};

class CExpo
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CLinear
{
public:
	static float EaseNone(float t, float b, float c, float d);
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CQuad
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CQuart
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CQuint
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

class CSine
{
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
	static float EaseInOut(float t, float b, float c, float d);
};

extern CBack Back;
extern CBounce Bounce;
extern CCirc Circ;
extern CCubic Cubic;
extern CElastic Elastic;
extern CExpo Expo;
extern CLinear Linear;
extern CQuad Quad;
extern CQuart Quart;
extern CQuint Quint;
extern CSine Sine;

#endif
