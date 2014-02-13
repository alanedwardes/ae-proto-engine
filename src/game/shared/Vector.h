#pragma once

struct Vector
{
	Vector() { x, y, z = 0.0f; }

	Vector(float flX, float flY, float flZ)
	{
		x = flX;
		y = flY;
		z = flZ;
	};

	inline Vector operator+(Vector other)
	{
		other.x += x;
		other.y += y;
		other.z += z;
		return other;
	}

	inline Vector operator-(Vector other)
	{
		other.x -= x;
		other.y -= y;
		other.z -= z;
		return other;
	}

    inline bool operator==(Vector other)
	{
        return (x == other.x && y == other.y && z == other.z);
    }

    inline bool operator!=(Vector other)
	{
        return (x != other.x || y != other.y || z != other.z);
    }

	float x;
	float y;
	float z;
};