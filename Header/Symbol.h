#pragma once
#include "IElement.h"

using namespace std;

/**
 * Symbol class: IElement
 * 
 * Representing the symbols which are different then Ninja
 *  - Destructible 'Shuriken' because if you step onto it it will be in the inventory and not on the map anymore
 *  - Destructible 'X' obstacle
 *  - Destructible Holy Symbol
 *  - Destructible  ' ' BlankArea
 *  - Indestructible '#' dojo wall
 *  - Indestructible 'M' mirror
 *  - Indestructible 'B' saké
 * SecretPath is a special symbol.
 */
class Symbol:
	public IElement
{
public:
	//Constructors
	Symbol();
	Symbol(char, elementType, position);
	Symbol(const Symbol&);

	void destroyYourself(bool) override;

	private:

	/**
	 * This function get param 'bool' to determine if it needs to be a 'Shuriken' or a 'BlankArea' 
	 */
	void changeToShuriken(bool);
};

