#include "../Header/Symbol.h"

Symbol::Symbol():IElement()
{
}

Symbol::Symbol(char symbol, elementType type, position pos) : IElement(symbol, type, pos)
{
}

Symbol::Symbol(const Symbol& s): IElement(s.m_Symbol, s.m_ElementType, s.m_Position)
{
}

void Symbol::destroyYourself(bool withShuriken)
{
	/*This function is representing a change within Symbols
	i.e.: on the MAP[3][3] there is a destructible object like 'X'
	and if the Ninja throws a Shuriken this object will be a Shuriken.
	*/

	switch (this->getType()) 
	{
		case HolySymbol:
		case BreakableObstackle:
		case Shuriken:
			changeToShuriken(withShuriken);
			break;
		default:
			/*In case of other types they are undestructible so no need to change them*/
			break;
	}
}

void Symbol::changeToShuriken(bool withSuriken)
{
	if (withSuriken)
	{
		this->setElementType(Shuriken);
		this->setSymbol('*');
	}
	else
	{
		this->setElementType(BlankArea);
		this->setSymbol(' ');
	}
}
