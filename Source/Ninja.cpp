#include "../Header/Ninja.h"

//Default consturctor
Ninja::Ninja():IElement()
{
	m_Name = "";
	for (int enumDirection = SOUTH; enumDirection != WEST; enumDirection++)
	{
		m_DirectionPriorities.push_back(static_cast<direction>(enumDirection));
	}
	m_IsFlipped = 0;
	m_CurrentMode = mode::Normal;
	m_NoOfShurikens = 3;
	m_FacingDirection = SOUTH;
}

//Constructor
Ninja::Ninja(string name, char symbol, elementType type, position pos):IElement(symbol,type,pos)
{
	m_Name = name;

	//Setting facing direction equal by default the default priority direction
	for (int enumDirection = SOUTH; enumDirection <= WEST; enumDirection++)
	{
		m_DirectionPriorities.push_back(static_cast<direction>(enumDirection));
	}
	m_IsFlipped = 0;
	m_CurrentMode = mode::Normal;
	m_NoOfShurikens = 3;
	m_FacingDirection = SOUTH;
}

//Copy constructor
Ninja::Ninja(const Ninja& n): IElement(n.m_Symbol, n.m_ElementType, n.m_Position)
{
	m_Name = n.m_Name;
	m_DirectionPriorities = n.m_DirectionPriorities;
	m_IsFlipped = n.m_IsFlipped;
	m_CurrentMode = n.m_CurrentMode;
	m_NoOfShurikens = n.m_NoOfShurikens;
	m_FacingDirection = n.m_FacingDirection;
}



//elementType Ninja::getType()
//{
//	return m_ElementType;
//}


void Ninja::setDirection(direction newDirection)
{
	m_FacingDirection = newDirection;
}

direction Ninja::getActualDirection() const
{
	return m_FacingDirection;
}

vector<direction> Ninja::getDirectionPriorities() const
{
	return m_DirectionPriorities;
}

void Ninja::setFlippedDirection()
{
	m_IsFlipped++;
}

bool Ninja::isFlippedDirectionPriority()
{
	return (m_IsFlipped % 2 == 0)? false:true;
}

void Ninja::setMode(mode currentMode)
{
	m_CurrentMode = currentMode;
}

mode Ninja::getMode()
{
	return m_CurrentMode;
}


int Ninja::getNoOfShuriken()
{
	return m_NoOfShurikens;
}

void Ninja::throwShuriken()
{
	if(m_NoOfShurikens>0)
	m_NoOfShurikens--;
}

void Ninja::gainShuriken()
{
	m_NoOfShurikens++;
}

void Ninja::move(position newPosition)
{
	m_Position = newPosition;
}

void Ninja::breakObject(IElement* element, bool withShuriken)
{
	element->destroyYourself(withShuriken);
}

void Ninja::destroyYourself(bool withShuriken)
{
	//Do nothing since Ninja cannot destroy himself - at least in this game ;)
}

void Ninja::reverseCurrentDirectionPriority()
{
	//If M is stepped it has to reverse either to normal or flipped priority.
	if (m_IsFlipped % 2 == 0)
	{
		m_FacingDirection = SOUTH;
		for (int enumDirection = SOUTH; enumDirection <= WEST; enumDirection++)
		{
			m_DirectionPriorities[enumDirection] = (direction)enumDirection;
		}
	}
	else
	{
		m_FacingDirection = WEST;
		int j = 3;
		for (int enumDirection = SOUTH; enumDirection <= WEST; enumDirection++)
		{
			m_DirectionPriorities[enumDirection] = (direction)(j);
			j--;
		}
	}
}
