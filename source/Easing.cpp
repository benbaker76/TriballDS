#include <math.h>
#include <stdlib.h>
#include "Easing.h"

CBack Back;
CBounce Bounce;
CCirc Circ;
CCubic Cubic;
CElastic Elastic;
CExpo Expo;
CLinear Linear;
CQuad Quad;
CQuart Quart;
CQuint Quint;
CSine Sine;

float CBack::EaseIn(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return EaseIn(t, b, c, d, s);
}

float CBack::EaseIn(float t, float b, float c, float d, float s)
{
	return c * (t /= d) * t * ((s + 1.0f) * t - s) + b;
}

float CBack::EaseOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return EaseOut(t, b, c, d, s);
}

float CBack::EaseOut(float t, float b, float c, float d, float s)
{
	return c * ((t = t / d - 1.0f) * t * ((s + 1.0f) * t + s) + 1.0f) + b;
}

float CBack::EaseInOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return EaseInOut(t, b, c, d, s);
}

float CBack::EaseInOut(float t, float b, float c, float d, float s)
{
	if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * (t * t * (((s *= (1.525f)) + 1) * t - s)) + b;
	return c / 2.0f * ((t -= 2.0f) * t * (((s *= (1.525f)) + 1.0f) * t + s) + 2.0f) + b;
}

float CBounce::EaseOut(float t, float b, float c, float d)
{
	if ((t /= d) < (1 / 2.75f))
		return c * (7.5625f * t * t) + b;
	else if (t < (2.0f / 2.75f))
		return c * (7.5625f * (t -= (1.5f / 2.75f)) * t + 0.75f) + b;
	else if (t < (2.5f / 2.75f))
		return c * (7.5625f * (t -= (2.25f / 2.75f)) * t + 0.9375f) + b;
	else
		return c * (7.5625f * (t -= (2.625f / 2.75f)) * t + 0.984375f) + b;
}

float CBounce::EaseIn(float t, float b, float c, float d)
{
	return c - EaseOut(d-t, 0, c, d) + b;
}

float CBounce::EaseInOut(float t, float b, float c, float d)
{
	if (t < d/2.0f) return EaseIn(t*2.0f, 0, c, d) * 0.5f + b;
	else return EaseOut(t*2.0f-d, 0, c, d) * 0.5f + c*0.5f + b;
}

float CCirc::EaseIn(float t, float b, float c, float d)
{
	return -c * ((float)sqrt(1.0f - (t/=d)*t) - 1.0f) + b;
}

float CCirc::EaseOut(float t, float b, float c, float d)
{
	return c * (float)sqrt(1.0f - (t = t / d - 1.0f) * t) + b;
}

float CCirc::EaseInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2.0f) < 1.0f) return -c / 2.0f * ((float)sqrt(1.0f - t * t) - 1.0f) + b;
	return c / 2.0f * ((float)sqrt(1.0f - (t -= 2) * t) + 1.0f) + b;
}

float CCubic::EaseIn(float t, float b, float c, float d)
{
	return c*(t/=d)*t*t + b;
}

float CCubic::EaseOut(float t, float b, float c, float d)
{
	return c*((t=t/d-1.0f)*t*t + 1.0f) + b;
}

float CCubic::EaseInOut(float t, float b, float c, float d)
{
	if ((t/=d/2.0f) < 1.0f) return c/2.0f*t*t*t + b;
	return c/2.0f*((t-=2.0f)*t*t + 2.0f) + b;
}

float CElastic::EaseIn(float t, float b, float c, float d)
{
	float a = 360;
	float p = 4;
	return EaseIn(t, b, c, d, a, p);
}

float CElastic::EaseIn(float t, float b, float c, float d, float a, float p)
{
	float s;
	if (t==0) return b;  if ((t/=d)==1.0f) return b+c;  if (p==0) p=d*0.3f;
	if (a==0 || a < abs(c)) { a=c; s=p/4.0f; }
	else s = p/(2.0f*(float)PI) * (float)asin (c/a);
	return -(a * (float)pow(2.0f, 10.0f * (t -= 1.0f)) * (float)sin((t * d - s) * (2.0f * (float)PI) / p)) + b;
}

float CElastic::EaseOut(float t, float b, float c, float d)
{
	float a = 360;
	float p = 4;
	return EaseOut(t, b, c, d, a, p);
}

float CElastic::EaseOut(float t, float b, float c, float d, float a, float p)
{
	float s;
	if (t==0) return b;  if ((t/=d)==1.0f) return b+c;  if (p==0) p=d*0.3f;
	if (a==0 || a < abs(c)) { a=c; s=p/4.0f; }
	else s = p / (2.0f * (float)PI) * (float)asin(c / a);
	return (a * (float)pow(2.0f, -10.0f * t) * (float)sin((t * d - s) * (2.0f * (float)PI) / p) + c + b);
}

float CElastic::EaseInOut(float t, float b, float c, float d)
{
	float a = 360;
	float p = 4;
	return EaseInOut(t, b, c, d, a, p);
}

float CElastic::EaseInOut(float t, float b, float c, float d, float a, float p)
{
	float s;
	if (t==0) return b;  if ((t/=d/2.0f)==2.0f) return b+c;  if (p==0) p=d*(0.3f*1.5f);
	if (a==0 || a < abs(c)) { a=c; s=p/4.0f; }
	else s = p / (2.0f * (float)PI) * (float)asin(c / a);
	if (t < 1) return -0.5f * (a * (float)pow(2.0f, 10.0f * (t -= 1.0f)) * (float)sin((t * d - s) * (2.0f * (float)PI) / p)) + b;
	return a * (float)pow(2.0f, -10.0f * (t -= 1.0f)) * (float)sin((t * d - s) * (2.0f * (float)PI) / p) * 0.5f + c + b;
}

float CExpo::EaseIn(float t, float b, float c, float d)
{
	return (t == 0) ? b : c * (float)pow(2.0f, 10.0f * (t / d - 1.0f)) + b;
}

float CExpo::EaseOut(float t, float b, float c, float d)
{
	return (t==d) ? b+c : c * (-(float)pow(2, -10.0f * t/d) + 1.0f) + b;
}

float CExpo::EaseInOut(float t, float b, float c, float d)
{
	if (t==0) return b;
	if (t==d) return b+c;
	if ((t /= d / 2.0f) < 1) return c / 2.0f * (float)pow(2.0f, 10.0f * (t - 1.0f)) + b;
	return c / 2.0f * (-(float)pow(2.0f, -10.0f * --t) + 2.0f) + b;
}

float CLinear::EaseNone(float t, float b, float c, float d)
{
	return c*t/d + b;
}

float CLinear::EaseIn(float t, float b, float c, float d)
{
	return c*t/d + b;
}

float CLinear::EaseOut(float t, float b, float c, float d)
{
	return c*t/d + b;
}

float CLinear::EaseInOut(float t, float b, float c, float d)
{
	return c*t/d + b;
}

float CQuad::EaseIn(float t, float b, float c, float d)
{
	return c*(t/=d)*t + b;
}

float CQuad::EaseOut(float t, float b, float c, float d)
{
	return -c *(t/=d)*(t-2.0f) + b;
}

float CQuad::EaseInOut(float t, float b, float c, float d)
{
	if ((t/=d/2.0f) < 1) return c/2.0f*t*t + b;
	return -c/2.0f * ((--t)*(t-2.0f) - 1.0f) + b;
}

float CQuart::EaseIn(float t, float b, float c, float d)
{
	return c*(t/=d)*t*t*t + b;
}

float CQuart::EaseOut(float t, float b, float c, float d)
{
	return -c * ((t=t/d-1.0f)*t*t*t - 1.0f) + b;
}

float CQuart::EaseInOut(float t, float b, float c, float d)
{
	if ((t/=d/2.0f) < 1) return c/2.0f*t*t*t*t + b;
	return -c/2.0f * ((t-=2.0f)*t*t*t - 2.0f) + b;
}

float CQuint::EaseIn(float t, float b, float c, float d)
{
	return c*(t/=d)*t*t*t*t + b;
}

float CQuint::EaseOut(float t, float b, float c, float d)
{
	return c*((t=t/d-1)*t*t*t*t + 1) + b;
}

float CQuint::EaseInOut(float t, float b, float c, float d)
{
	if ((t/=d/2.0f) < 1) return c/2.0f*t*t*t*t*t + b;
	return c/2.0f*((t-=2.0f)*t*t*t*t + 2.0f) + b;
}

float CSine::EaseIn(float t, float b, float c, float d)
{
	return -c * (float)cos(t / d * ((float)PI / 2.0f)) + c + b;
}

float CSine::EaseOut(float t, float b, float c, float d)
{
	return c * (float)sin(t / d * ((float)PI / 2.0f)) + b;
}

float CSine::EaseInOut(float t, float b, float c, float d)
{
	return -c / 2.0f * ((float)cos((float)PI * t / d) - 1.0f) + b;
}

