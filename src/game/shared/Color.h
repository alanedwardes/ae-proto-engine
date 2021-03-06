#pragma once

// Colour Channel: 0 - 255
typedef unsigned char chan;

struct Color
{
	Color() { }

	Color(chan cR, chan cB, chan cG, chan cA = 255)
	{
		r = cR, b = cB, g = cG, a = cA;
	}

	chan r = 255;
	chan b = 255;
	chan g = 255;
	chan a = 255;
};