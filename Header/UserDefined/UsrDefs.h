#pragma once

/**
* UsrDefs.h : This file contains every additional enumerations, structs, helper-classes to Elements
*/

/**
 * Type specifier of each game elements. 'D' stands for direction modifiers
 */
enum elementType
{
	NinjaFighter,
	HolySymbol,
	Dojo,
	BreakableObstackle,
	SecretPathPoint,
	Mirror,
	Shuriken,
	Sake,
	BlankArea,
	D_SOUTH,
	D_EAST,
	D_NORTH,
	D_WEST
};

/**
 * Type specifier of the next steps. 'M' stands for move
 */
enum nextStep
{
	M_THROW,
	M_SOUTH,
	M_EAST,
	M_NORTH,
	M_WEST,
	M_GAME_OVER,
	M_LOOP
};

/**
 * Facing directions 
 */
enum direction
{
	SOUTH,
	EAST,
	NORTH,
	WEST
};

/**
 * Action To Do after/during movement
 */
enum movementAction
{
	NO_MOVEMENT,
	STEP_ON_BLANK_AREA,
	STEP_ON_SHURIKEN,
	STEP_ON_MIRROR,
	STEP_ON_SAKE,
	PRIO_SOUTH,
	PRIO_WEST,
	PRIO_NORTH,
	PRIO_EAST,
	SP
};

/**
 * Ninja modes
 */
enum mode
{
	Normal,
	Breaker
};

/*
 * SecretPathHelper for saving the secret paths after the map is set up
 */
struct secretPathHelper
{
private:
	char	m_Symbol;
	int		m_X;
	int		m_Y;
public:
	secretPathHelper() {};
	secretPathHelper(char symbol,int y,int x):m_Symbol(symbol),m_X(x),m_Y(y){}
	int getX() { return m_X; }
	int getY() { return m_Y; }
	char getSymbol() { return m_Symbol; }
};

/**
 * Struct for storing a position of each and every IElement on the map
 */
struct position 
{
private:
	int m_Y;
	int m_X;
public:
	position():m_Y(-1), m_X(-1){}
	position(int y, int x):m_Y(y), m_X(x) {}
	position(const position& p) { m_X = p.m_X; m_Y = p.m_Y; }

	//Return pos. Y
	int getX()
	{
		return m_X;
	}

	//Return pos. Y
	int getY()
	{
		return m_Y;
	}

	//Set pos. Y
	void setX(int x)
	{
		m_X = x;
	}

	//Set pos. Y
	void setY(int y)
	{
		m_Y = y;
	}
	
};

