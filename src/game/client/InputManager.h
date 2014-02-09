#include "shared\Key.h"

#include "sfml\Graphics.hpp"

class InputManager
{
public:
	short int m_iSampledInput = KEY_NONE;
	short int m_iPressed = KEY_NONE;

	virtual Key TranslateKeyCode(sf::Keyboard::Key eKey)
	{
		switch (eKey)
		{
		case sf::Keyboard::Up:
			return KEY_UP;
		case sf::Keyboard::Right:
			return KEY_RIGHT;
		case sf::Keyboard::Down:
			return KEY_DOWN;
		case sf::Keyboard::Left:
			return KEY_LEFT;
		default:
			return KEY_NONE;
		}
	}

	virtual void KeyPress(Key iKey)
	{
		m_iSampledInput |= iKey;
		m_iPressed |= iKey;
	}

	virtual void KeyRelease(Key iKey)
	{
		m_iPressed &= ~iKey;
	}

	virtual short int SampleInput()
	{
		short int iInputSample = m_iSampledInput;
		m_iSampledInput = m_iPressed;
		return iInputSample;
	}
};