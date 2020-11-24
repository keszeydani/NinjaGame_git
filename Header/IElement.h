#pragma once
// IElement.h : 
//

#include "../Header/UserDefined/UsrDefs.h"

/**
 * IElement class
 *
 * Collection of atomic components on the map. This abstract
 * class serves as a common base for the participant in the
 * game like: Ninja, Holy symbol, Obstacles, etc. 
 */
class IElement
{
protected:
	char			m_Symbol;
	elementType		m_ElementType;
	position		m_Position;

public:
	virtual ~IElement() = default;
	//Constructor
	IElement() 
	{
		m_Symbol = '\0';
		m_ElementType = BlankArea;
		m_Position = position(-1, -1);
	};
	IElement(const char symbol, const elementType elementType, const position pos)
	{
		m_Symbol = symbol;
		m_ElementType = elementType;
		m_Position = pos;
	};
	IElement(const IElement& e)
	{
		m_Symbol = e.m_Symbol;
		m_ElementType = e.m_ElementType;
		m_Position = e.m_Position;
	};

	//Attribute modifiers
	void setElementType(elementType type) { m_ElementType = type; }
	elementType getType() { return m_ElementType; };
	void setSymbol(char symbol) { m_Symbol = symbol; }
	char getSymbol() { return m_Symbol; }
	void setPosition(position pos) { m_Position = pos; }
	position getPosition() { return m_Position; }
	/**
	 * Defining what happens during 'destroy procedure' which can be unique for each participant.
	 * Also making IElement an abstract class.
	 */
	virtual void destroyYourself(bool) = 0;
};