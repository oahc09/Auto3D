#pragma once
#include "MMath.h"



using namespace Auto3D;
AUTO_API_BEGIN

struct MVector2 : public Vector2
{
public:
	void Set(float inX, float inY) { x = inX; y = inY; }
	float* GetPtr() { return &x; }
	const float* GetPtr()const { return &x; }
	MVector2() { x = 0.0f, y = 0.0f; }
	MVector2(const MVector2& vec) { x = vec.x; y = vec.y; }
	MVector2(float xPos, float yPos)  { x = xPos, y = yPos;   }
	// operators
	inline MVector2     operator-()const							{ return MVector2(-x, -y); }
	inline MVector2		operator+(const MVector2& rhs) const		{ return MVector2(x + rhs.x, y + rhs.y); }
	inline MVector2     operator-(const MVector2& rhs) const		{ return MVector2(x - rhs.x, y - rhs.y); }
	inline MVector2     operator*(const float scale) const			{ return MVector2(x * scale, y*scale); }
	inline MVector2     operator*(const MVector2& rhs) const		{ return MVector2(x*rhs.x, y*rhs.y); }
	inline MVector2     operator/(const float scale) const			{ return MVector2(x / scale, y / scale); }
	inline MVector2&    operator/=(const float scale)				{ x /= scale; y /= scale; return *this; }
	inline MVector2&	operator=(const MVector2& rhs)				{ x = rhs.x; y = rhs.y; return *this; }
	inline MVector2&    operator+=(const MVector2& rhs)				{ x += rhs.x; y += rhs.y; return *this; }
	inline MVector2&    operator-=(const MVector2& rhs)				{ x -= rhs.x; y -= rhs.y; return *this; }
	inline MVector2&    operator*=(const float scale)				{ x *= scale; y *= scale; return *this; }
	inline MVector2&    operator*=(const MVector2& rhs)				{ x *= rhs.x; y *= rhs.y; return *this; }
	const float&		operator[] (int i)const						{ assert(i >= 0 && i <= 1); return (&x)[i]; }
	float&				operator[] (int i)							{ assert(i >= 0 && i <= 1); return (&x)[i]; }
	inline bool			operator==(const MVector2& rhs) const		{ return (x == rhs.x) && (y == rhs.y); }
	inline bool			operator!=(const MVector2& rhs) const		{ return (x != rhs.x) || (y != rhs.y); }
};
inline float Dot(const Vector2& lhs, const Vector2& rhs)				{ return Auto3D::Dot(lhs, rhs); }
inline float Magnitude(const Vector2& vec)								{ return Auto3D::sqrt(Auto3D::Dot(vec, vec)); }
inline Vector2 & Normalize(const Vector2& vec)							{ return Auto3D::Normalize(vec); }
inline Vector2 Lerp(const Vector2& from, const Vector2& to, float t)	{ return Auto3D::Lerp(from, to, t); }
inline Vector2 Min(const Vector2& lhs, const Vector2& rhs)				{ return Auto3D::Min(lhs, rhs); }
inline Vector2 Max(const Vector2& lhs, const Vector2& rhs)				{ return Auto3D::Max(lhs, rhs); }
inline bool IsNormalized(const Vector2& vec)							{ return Auto3D::IsNormalized(vec); }
inline Vector2 Abs(const Vector2& v)									{ return Auto3D::Abs(v); }
inline float Angle(const Vector2& lhs, const Vector2& rhs)				{ return Auto3D::Angle(lhs, rhs); }


struct MVector3	: public Vector3
{
public:
	
};

struct MVector4 : public Vector4
{
public:
	
};

AUTO_API_END
