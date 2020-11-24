#pragma once
#include "Symbol.h"

/**
 * SecretPath class: Symbol
 *
 * Special Symbol which has an other position holding it's endpoint coordinates
 */
class SecretPath :
	public Symbol
{
	position		m_EndPointPosition;

public:
	SecretPath();
	//To speed up the map scan process sete the endpoint position by default (-1,-1)
	SecretPath(char, elementType, position, position p = position(-1,-1)); 	
	SecretPath(const SecretPath&);
	void setEndpoint(position);
	position getEndpoint();
};

