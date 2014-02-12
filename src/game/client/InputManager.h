#include "shared\Key.h"

#include "sfml\Graphics.hpp"

class InputManager
{
public:
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

	virtual Key TranslateKeyCode(sf::Mouse::Button eButton)
	{
		switch (eButton)
		{
			case sf::Mouse::Left:
				return KEY_MOUSE1;
			case sf::Mouse::Right:
				return KEY_MOUSE2;
			case sf::Mouse::Middle:
				return KEY_MOUSE3;
			case sf::Mouse::XButton1:
				return KEY_MOUSE4;
			case sf::Mouse::XButton2:
				return KEY_MOUSE5;
			default:
				return KEY_NONE;
		}
	}

	virtual void NewMousePosition(Point poMousePos)
	{
		m_poMousePos = poMousePos;
	}

	virtual Point SampleMousePosition()
	{
		return m_poMousePos;
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
private:
	Point m_poMousePos;
	short int m_iSampledInput = KEY_NONE;
	short int m_iPressed = KEY_NONE;
};