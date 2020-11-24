#pragma once

#include <vector>
#include "Ninja.h"
#include "SecretPath.h"
#include <map>
#include <fstream>


/**
 * Map class serves as a container and renderer
 *
 * 2 dimensional storage of the map including every other
 * game participants (Ninja, Obstacles, Holy Symbol, etc.)
 * Other main function is to render it's data via console
 * among two steps.
 */
class Map
{
protected:
	int m_Length;
	int m_Width;
    vector<vector<IElement*>> m_Map;

	//Intermediate container for determining the secret start and end points
    map<int, secretPathHelper> m_SecretPaths;

public:
    Map();
    ~Map();
	/**
     * \Brief
     * Resetting variables after a map is solved
     * 
     * \Return
     * TRUE if m_Map is empty
     */
    bool reInitToEmpty();
	/**
     * \Brief
     * Rendering the symbol of each element of the 'matrix'
     */
    void render();
	
	/**
     * \Param
     * @1: IElement pointer to a game participant
     *
     * Brief
     * Addig participant to Map's m_Map
     */
    void addParticipant(IElement*);
	
	/**
     * \Param
     * @1: File path
     *
     * Brief
     * Build up the Map (and it's elements) based on the file
     *
     *\Return
     * TRUE if the map build up was successful
     */
	bool stringToMap(string);

    /**
	 * Brief
	 * Iterating through secret paths and setting their endpoint pair
	 */
	void matchSecretPaths();
	/**
     * \Param
     * @1: Character with which the participant could be determined
     *
     * Brief
     * Creating participant element according to the character symbol
     */
	void determinElement(char);

    /**
     * Brief
     * getNinja() function
     *
     *\Return
     * pointer to Ninja element
     */
    Ninja* getNinja();

    /**
    *
    * Brief
    * getLength() and getWidth() retrieve dimensions
    *
    * \Return
    * m_Length / m_Width member
    */
    int getLength();
	int getWidth();

    /**
     * \Param
     * @1: Positin of the needed element
     * 
     * Brief
     * getElement() function based on position
     *
     *\Return
     * IElement pointer to the desired element
     */
    IElement* getElement(position);

    /**
     * \Param
     * @1: field on the map which Ninja is heading towards
     *
     * Brief
     * Determining the next action what a Ninja could do during movement
     *
     *\Return
     * movementAction what to do (step on with saving (like M/B/S/N/W/E, etc.),
     * step without saving (like '*') or no stepping possibility)
     */
    movementAction getNextStep(IElement*);

    /**
     * \Param
     * @1: Pointer of the IElement object Ninja wants to head
     * @2: POinter to Ninja itself
     * Brief
     * Putting Ninja to the pointing place
     */
    void setNinjaNewPosition(IElement*, IElement*);

    /**
    *
    * Brief
    * getMap() retrieves m_Map element;
    *
    * \Return
    * m_Map
    */
    vector<vector<IElement*>>& getMap();

};

