#pragma once
#include "IElement.h"
#include <vector>
#include <string>

using namespace std;

/**
 * Ninja class: IElement
 *
 * Serves as main character in the game. As common attributes
 * are shared among game elements Ninja is the one who shall
 * find a way to destroy the Holy Symbol by moving, throwing
 * Shurikens and using secret paths.
 */
class Ninja : public IElement
{
private:

	string				m_Name;
	//m_DirectionPriorities which contains the priorities
	vector<direction>	m_DirectionPriorities;

	//m_FacingDirection which contains the actual facing direction
	direction			m_FacingDirection;
	
	//If Flipped Modulo %2 == 0 meaning not flipped -> needed because of the possibiliy of multiple
	// 'M's on the map. (Easier to deal with it on Engine side.
	int					m_IsFlipped;
	mode				m_CurrentMode;
	int					m_NoOfShurikens;
		
public:
	//Constructors
	Ninja();
	Ninja(string, char, elementType, position);
	Ninja(const Ninja&);

	//Setting priority direction
	void setDirection(direction);
	//Getting a direction vector
	direction getActualDirection() const;
	vector<direction> getDirectionPriorities() const;
	void setFlippedDirection();
	bool isFlippedDirectionPriority();
	void setMode(mode);
	mode getMode();
	int getNoOfShuriken();

	//Behaviour modifiers
	void throwShuriken();
	void gainShuriken();
	void move(position);
	void breakObject(IElement*, bool);
	void destroyYourself(bool) override;
	void reverseCurrentDirectionPriority();
};

