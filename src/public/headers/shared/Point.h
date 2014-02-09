#pragma once

struct Point
{
	Point() { x = 0.0f, y = 0.0f; }

	Point(float flX, float flY)
	{
		x = flX, y = flY;
	};

	inline Point operator +(Point right) { return Point(x + right.x, y + right.y); }
	inline Point operator -(Point right) { return Point(x - right.x, y - right.y); }
	inline Point operator *(Point right) { return Point(x * right.x, y * right.y); }
	inline Point operator /(Point right) { return Point(x / right.x, y / right.y); }

	inline Point operator +(float right) { return Point(x + right, y + right); }
	inline Point operator -(float right) { return Point(x - right, y - right); }
	inline Point operator *(float right) { return Point(x * right, y * right); }
	inline Point operator /(float right) { return Point(x / right, y / right); }
    
	inline bool operator ==(Point right) { return (x == right.x && y == right.y); }
	inline bool operator !=(Point right) { return (x != right.x || y != right.y); }

	inline bool operator !() { return (x == 0 && y == 0); }

	float x;
	float y;
};