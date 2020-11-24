#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Map.h"
#include "Ninja.h"

//See desc. at m_HeatMap;
#define MAGIC_NUMBER 5

using namespace std;
/**
 * Engine class
 *
 * Singleton class to handle all the game relevant actions like start game, process map, take next action,etc. 
 */
class Engine
{
private:

	// Private constructor to prevent instancing
	Engine();

	// One and only instance pointer
	static Engine*		m_Instance;

    // Holding the absolute path of the maps
    vector<string>      m_MapList;

    //Current map
    Map                 m_CurrentMap;

    //Ninja pointer
    Ninja*              m_CurrentNinja;

    //A symbol to be saved if ninja steps on it and shall remain 'seated' (like M,S,W,N,E, B, etc.)
    Symbol              m_SavedObject;

    //Ordered list of positions where breakable obstacles are found and in theory they could be destroyed (in case Holy Symbol not found)
    //Set up and cleared / destroyed at before every movement action
    vector<position> m_BreakableObstacles;

    /**
    * List of coordinates [y][x] storing an integer value which represent the 'HeatMap'
    * and stores a counter how many time a Ninja is stepped onto the field
    * 
    * The counter for detecting the 'LOOP'.
    * If one value reaches 5 we detected a 'LOOP' because:
    *
    For example
    MAP1: This is not a 'LOOP MAP'
    - Let's assume Ninja is out of shurikens already.
    - It reaches poisition above X (+1)
    - Goes to right and changes direction and reaches the posiiton above X once more (+1)
    - On the left he gets into Breaker mode and reverse his direction and reaches posiiton above X again (+1)
    - This is 3 steps onto the same pos. and not an infinite loop
    #        #
    #     @  #
    #        #
    #        #
    #        #
    #M B    M#
    ######X###
    #     $  #
    ##########


    MAP2: This is a 'LOOP MAP' ( heatmap position value:  4 steps are needed to detect loop)
    #        #
    #     @  #
    #        #
    #        #
    #        #
    #M B    M#
    ######X###
    #     B  #
    #     @  #
    ##########

    MAP3: This is a 'LOOP MAP' ( heatmap position value:  4 steps are needed to detect loop)
    #        #
    #     @  #
    #        #
    #        #
    #        #
    #M B    M#
    ###### ###
    #     #  #
    #     @  #
    ##########

       And since there might be initially some cases I haven't noticed I raise this number 4 to 5 to be on the safe side, this is how this magic number is calculated
    */
    vector<vector<int> > m_HeatMap;

public:
    ~Engine();
    /**
     * \Brief
     * Access method to m_Instance
     *
     *\Return
       m_Instance Engine pointer
     */
	static Engine* getInstance();

    /**
    * \Brief
    * Erase a given substring from string
    */
	static void eraseSubStr(std::string& mainStr, const std::string& toErase);

	/**
    * \Brief
    * Loading map files into m_MapList
    *
    * \Return
    *  TRUE if map list file could be opened and m_MapList is not empty
    */
    bool loadMapList(string file);

    /**
    * \Brief
    * Loading one map from m_MapList and converting it to a 'Map' according to Map class
    *
    * \Return
    *  TRUE if map file could be opened and map is built up
    */
    bool processMap(string &fileName);

    /**
     * \Brief
     * Retrieves the list of maps available in string format
     *
     * \Return
     *  m_MapList
     */
    vector<string> getMaps() const;

	/**
     * \Brief
     * Retrieves current map
     * 
     * \Return
     *  m_CurrentMap;
     */
	Map& getCurrentMap();

	/**
    *********************************************
    *********************************************
    ********STATE MACHINE FUNCTIONS**************
    *********************************************
    *********************************************
    */

    /**
	 * \Brief
	 * State machine implementation
	 * Process the movement / actions of the current Ninja
	 * including destroy, throw, move to new place, etc.
	 *
	 * \Return
	 *  FALSE is no movement left
	 */
    bool move();


	/**
	 *\Param
	 *@1: the fixed coordinate (might be X-wise, might be Y)
	 *@2: the starting index offset, in case SOUT and WEST it is -1, otherwise +1
	 *@3: boolean indicating if X coordinate is the fox one or not
	 *@4: Loop direction indication (counting up, or down)
     *@5: boolean indicating if it shall discover the map for movement (true) or for destroying sg.
	 *
	 * \Brief
     * Since the iteration logic through a map is common
     * the program uses this in case of destroying a holy 
     * symbol - together with collecting possible destructible
     * objects and moving to a new position
	 *
	 * \Return
	 * TRUE only if HolySymbol is destroyed. If breakable 
     * obstacle is detected (and put into the list 
     * m_BreakableObstacles) it still returns FALSE.
     * In case the method is called with isMoveAction
     * equals to TRUE the return value changes and 
     * return true if the movement was possible.
	 */	
    bool b_DestroyHS_or_CollectDestrObj_or_Move(int fixedCoordinate, int startingIdxOffset, bool isXtheFixedCoordinate,
         bool isDecreasingIdxNeeded, bool isMoveAction);
	
    /**
	 * \Brief
	 * Try to destroy Holy Symbol via calling the 
	 * b_DestroyHolySymbol_OR_BreakableObstacle()
	 *
	 * \Return
	 *  TRUE only if HolySymbol is destroyed
	 *  if breakable obstacle is destroy it still
	 *  return FALSE
	 */
    bool b_PossibleToDestroyHolySymbol();

    /**
     * \Brief
     * If HolySymbol cannot be destroyed but Destructible
     * Objects are found we have to destroy them with priority
     * if we can (with Shuriken or Breaker mode)
     */
    void destroyDestructibles();

    /**
	 * \Brief
	 * First try to move to the actual heading direction
	 *
	 * \Return
	 * TRUE if moving to actual direction was possible
	 */
	bool possibleToMoveToActualDirection(direction);
    /**
     * \Brief
     * Moving to the next position
     *
     * \Return
     *  TRUE if moving to a new position was possible
     *  return FALSE
     */
	bool moveToNextPosition();

    /**
     * \Brief
     * If loop is detected Ninja need to move to next map
     *
     * \Return
     *  TRUE if 'LOOP' is detected
     */
    bool b_IsLoopDetected();
};