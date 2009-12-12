/*
Copyright (c) 2006 Henry Strickland & Ryan Seto
              2007 Tobias Weyand (modifications and extensions)

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

        (* http://www.opensource.org/licenses/mit-license.php *)
*/

#ifndef _FIXED_H_
#define _FIXED_H_

#include <stdio.h>

#define FIXED_BP        16
#define FIXED_MAX       ((1<<(32-FIXED_BP-1))-1)
#define FIXED_MIN       (-(1<<(32-FIXED_BP-1)))
#define FIXED_EPSILON   (Fixed(0.00002f))

class Fixed {

private:

	int	g; // the guts

const static int BP= FIXED_BP;  // how many low bits are right of Binary Point
const static int BP2= BP*2;  // how many low bits are right of Binary Point
const static int BPhalf= BP/2;  // how many low bits are right of Binary Point

double STEP();  // smallest step we can represent

	// for private construction via guts
  	enum FixedRaw { RAW };
	Fixed(FixedRaw, int guts);


public:

  
	Fixed();
	Fixed(const Fixed& a);
	Fixed(float a);
	Fixed(double a);
	Fixed(int a);
	Fixed(long a);
	Fixed& operator =(const Fixed& a);
	Fixed& operator =(float a);
	Fixed& operator =(double a);
	Fixed& operator =(int a);
	Fixed& operator =(long a);

	operator float();
	operator double();
	operator int();
	operator long();
  operator unsigned short();

	Fixed operator +() const;
	Fixed operator -() const;

	Fixed operator +(const Fixed& a) const;
	Fixed operator -(const Fixed& a) const;
#if 1
	// more acurate, using long long
	Fixed operator *(const Fixed& a) const;
#else
	// faster, but with only half as many bits right of binary point
	Fixed operator *(const Fixed& a) const;
#endif
	Fixed operator /(const Fixed& a) const;

	Fixed operator *(int a) const;

	Fixed operator +(float a) const;
	Fixed operator -(float a) const;
	Fixed operator *(float a) const;
	Fixed operator /(float a) const;

	Fixed operator +(double a) const;
	Fixed operator -(double a) const;
	Fixed operator *(double a) const;
	Fixed operator /(double a) const;

	Fixed& operator +=(Fixed a);
	Fixed& operator -=(Fixed a);
	Fixed& operator *=(Fixed a);
	Fixed& operator /=(Fixed a);

	Fixed& operator +=(int a);
	Fixed& operator -=(int a);
	Fixed& operator *=(int a);
	Fixed& operator /=(int a);

	Fixed& operator +=(long a);
	Fixed& operator -=(long a);
	Fixed& operator *=(long a);
	Fixed& operator /=(long a);

	Fixed& operator +=(float a);
	Fixed& operator -=(float a);
	Fixed& operator *=(float a);
	Fixed& operator /=(float a);

	Fixed& operator +=(double a);
	Fixed& operator -=(double a);
	Fixed& operator *=(double a);
	Fixed& operator /=(double a);

	bool operator ==(const Fixed& a) const;
	bool operator !=(const Fixed& a) const;
	bool operator <=(const Fixed& a) const;
	bool operator >=(const Fixed& a) const;
	bool operator  <(const Fixed& a) const;
	bool operator  >(const Fixed& a) const;

	bool operator ==(float a) const;
	bool operator !=(float a) const;
	bool operator <=(float a) const;
	bool operator >=(float a) const;
	bool operator  <(float a) const;
	bool operator  >(float a) const;

	bool operator ==(double a) const;
	bool operator !=(double a) const;
	bool operator <=(double a) const;
	bool operator >=(double a) const;
	bool operator  <(double a) const;
	bool operator  >(double a) const;
 
	bool operator  >(int a) const;
	bool operator  <(int a) const;

  Fixed abs();
  Fixed sqrt();
};

inline Fixed operator +(float a, const Fixed& b) { return Fixed(a)+b; }
inline Fixed operator -(float a, const Fixed& b) { return Fixed(a)-b; }
inline Fixed operator *(float a, const Fixed& b) { return Fixed(a)*b; }
Fixed operator /(float a, const Fixed& b);

inline bool operator ==(float a, const Fixed& b) { return Fixed(a) == b; }
inline bool operator !=(float a, const Fixed& b) { return Fixed(a) != b; }
inline bool operator <=(float a, const Fixed& b) { return Fixed(a) <= b; }
inline bool operator >=(float a, const Fixed& b) { return Fixed(a) >= b; }
inline bool operator  <(float a, const Fixed& b) { return Fixed(a)  < b; }
inline bool operator  >(float a, const Fixed& b) { return Fixed(a)  > b; }



inline Fixed operator +(double a, const Fixed& b) { return Fixed(a)+b; }
inline Fixed operator -(double a, const Fixed& b) { return Fixed(a)-b; }
inline Fixed operator *(double a, const Fixed& b) { return Fixed(a)*b; }
inline Fixed operator /(double a, const Fixed& b) { return Fixed(a)/b; }

inline bool operator ==(double a, const Fixed& b) { return Fixed(a) == b; }
inline bool operator !=(double a, const Fixed& b) { return Fixed(a) != b; }
inline bool operator <=(double a, const Fixed& b) { return Fixed(a) <= b; }
inline bool operator >=(double a, const Fixed& b) { return Fixed(a) >= b; }
inline bool operator  <(double a, const Fixed& b) { return Fixed(a)  < b; }
inline bool operator  >(double a, const Fixed& b) { return Fixed(a)  > b; }

int& operator +=(int& a, const Fixed& b) ;
int& operator -=(int& a, const Fixed& b) ;
int& operator *=(int& a, const Fixed& b) ;
int& operator /=(int& a, const Fixed& b) ;

long& operator +=(long& a, const Fixed& b) ;
long& operator -=(long& a, const Fixed& b) ;
long& operator *=(long& a, const Fixed& b) ;
long& operator /=(long& a, const Fixed& b) ;

float& operator +=(float& a, const Fixed& b) ;
float& operator -=(float& a, const Fixed& b) ;
float& operator *=(float& a, const Fixed& b) ;
float& operator /=(float& a, const Fixed& b) ;

double& operator +=(double& a, const Fixed& b) ;
double& operator -=(double& a, const Fixed& b) ;
double& operator *=(double& a, const Fixed& b) ;
double& operator /=(double& a, const Fixed& b) ;

Fixed abs(Fixed f);

#endif
