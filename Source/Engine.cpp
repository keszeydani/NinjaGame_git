#include "../Header/Engine.h"
#include <algorithm>
#include <iostream>

//Null, because instance will be once initialized if needed
Engine* Engine::m_Instance = 0;

Engine::Engine()
{
}

Engine::~Engine()
{
	delete m_CurrentNinja;
	m_CurrentNinja = nullptr;
}

Engine* Engine::getInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Engine();
	}
	return m_Instance;
}

void Engine::eraseSubStr(std::string& mainStr, const std::string& toErase)
{
	// Search for the substring in string
	size_t pos = mainStr.find(toErase);

	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

bool Engine::loadMapList(string file)
{
	char ch;
	bool l_Result = false;
	string l_Line;
	string l_AbsolutPath;

	//Open file via ifstream for read
	std::ifstream l_FileList(file);

	if (!(l_FileList.fail()))
	{

		while (std::getline(l_FileList, l_Line)) {

			//Replacing '/' with '\\' and so forming the absolute path to m_MapList (Windows)
			std::replace(l_Line.begin(), l_Line.end(), '/', '\\');

			//Erasing filename  - which contains tha map path themselves - from map's filepath
			eraseSubStr(file, "input-all.list");

			m_MapList.push_back(file + l_Line);
		}
		if (m_MapList.size() != 0)
		{
			l_Result = true;
		}
	}
	return l_Result;
}

bool Engine::processMap(string& fileName)
{
	//Coordinate -1 , -1 will make this m_SavedObject 'ignored'
	m_SavedObject.setPosition(position(-1, -1));

	//Clear m_HeatMap
	if (!m_HeatMap.empty())
	{
		m_HeatMap.resize(0);
	}

	//Process map into symbols from current map url
	bool l_Result = m_CurrentMap.stringToMap(fileName);

	//After setting up the Map we need to set up the HeatMap
	//with full of zeros
	int l_Length = m_CurrentMap.getLength();
	int l_Width = m_CurrentMap.getWidth();

	vector<int> l_TempHeatMapRow;

	for (int i = 0; i < l_Length; i++)
	{
		for (int j = 0; j < l_Width; j++)
		{
			l_TempHeatMapRow.push_back(0);
		}
		m_HeatMap.push_back(l_TempHeatMapRow);
		l_TempHeatMapRow.clear();
	}

	//Get Ninja
	m_CurrentNinja = m_CurrentMap.getNinja();

	//Printing out every init map
	m_CurrentMap.render();
	return l_Result;
}

vector<string> Engine::getMaps() const
{
	return m_MapList;
}

Map& Engine::getCurrentMap()
{
	return m_CurrentMap;
}

bool Engine::move()
{

	bool l_PossibleToDestroyHolySymbol = false;

	//Determine if possible to destroy HolySymbol
	l_PossibleToDestroyHolySymbol = b_PossibleToDestroyHolySymbol();

	//Check if Holy Symbol is destroyed
	if (l_PossibleToDestroyHolySymbol)
	{
		cout << "GAME_OVER\n";
		m_CurrentMap.render();
		return true;
	}

	//Check if 'Loop' is detected
	if (b_IsLoopDetected())
	{
		cout << "LOOP\n";
		m_CurrentMap.render();
		return true;
	}

	// If program reaches this point it means we are not (yet)
	// destroyed the Holy Smybol or we are not on a loop map
	moveToNextPosition();

	return l_PossibleToDestroyHolySymbol;
}

bool Engine::b_DestroyHS_or_CollectDestrObj_or_Move(int fixedCoordinate, int startingIdxOffset, bool isXtheFixedCoordinate, bool isDecreasingIdxNeeded, bool isMoveAction)
{
	bool l_Result = false;

	//Depending on the direction left, right, up, down it must be determined
	int l_FixCoordinate = 0;
	//Map width / length
	int l_Length = m_CurrentMap.getLength();
	int l_Width = m_CurrentMap.getWidth();
	//Actual element
	IElement* l_PointingElement = nullptr;

	//Because there are destructibles do not destroy Holy Symbol
	bool l_DestructibleDetected = false;

	//Stepping through until first obstacle appearing in the directions i.e. if
	// isDecreasingIdxNeeded param == TURE, it means the loop approaching Idx 0
	// on the map (Y wise: NORTH and X wise: WEST directions)
	for (int i = ((isXtheFixedCoordinate) ? (m_CurrentNinja->getPosition().getY() + startingIdxOffset) : ((m_CurrentNinja->getPosition().getX() + startingIdxOffset)));
		((isDecreasingIdxNeeded) ? i >= 0 : i < ((isXtheFixedCoordinate) ? l_Length : l_Width));
		((isDecreasingIdxNeeded) ? (i--) : (i++))
		)
	{

		//To avoid vector subscript out of range check if X / Y is within the range
		if (((isXtheFixedCoordinate) ? i : fixedCoordinate) < l_Length && ((isXtheFixedCoordinate) ? i : fixedCoordinate) < l_Width)
		{
			l_PointingElement = m_CurrentMap.getElement(position((isXtheFixedCoordinate) ? i : fixedCoordinate, (isXtheFixedCoordinate) ? fixedCoordinate : i));
		}
		else
		{
			return false;
		}

		const elementType l_CurrentSymbol = l_PointingElement->getType();

		//If the main purpose is to move to a new place
		if (isMoveAction)
		{
			//Storing old Ninja position because maybe stepping on a 'remain seated' 
			//object like Saké, Secret Path or Mirror and it has to be saved
			position l_OldNinjaPos = m_CurrentNinja->getPosition();;
			position l_NewNinjaPos;

			//Deciding "What to do ?" Based on the pointing field on the map
			const movementAction l_ToDo = m_CurrentMap.getNextStep(l_PointingElement);

			//Heatmap coordinates if movement is possible and need an update on it
			const int l_HeatMapX = l_PointingElement->getPosition().getX();
			const int l_HeatMapY = l_PointingElement->getPosition().getY();

			int l_SavedObjectX = m_SavedObject.getPosition().getX();
			int l_SavedObjectY = m_SavedObject.getPosition().getY();
			
			switch (l_ToDo)
			{
			case NO_MOVEMENT:
				return false;
				break;
			case STEP_ON_BLANK_AREA:

				//Setting Ninja's new position
				m_CurrentNinja->setPosition(l_PointingElement->getPosition());
				//Map also needs to be informed that position changed
				m_CurrentMap.setNinjaNewPosition(l_PointingElement, m_CurrentNinja);
				//Notifying m_HeatMap that a new position was stepped onto
				m_HeatMap[l_HeatMapY][l_HeatMapX] += 1;

				//Ok, now ninja knows its place, now we need to modify the Ninja's 'old' place
				if (l_SavedObjectX != -1 && l_SavedObjectY != -1)
				{
					//Meaning we have an object to be constructed
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(m_SavedObject);

					//Reset m_SavedObject. It is enough to reset the coordinates because if there is a need to save an object it will be overwritten
					m_SavedObject.setPosition(position(-1, -1));
				}
				else
				{
					//No object need to be retrieved from the m_SavedObject
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(' ', BlankArea, position(l_OldNinjaPos.getY(), l_OldNinjaPos.getX()));
				}
				cout << "MOVE\n";
				m_CurrentMap.render();
				return true;
				break;
				//TO BE IMPLEMENTED
			case STEP_ON_SHURIKEN:

				//Setting Ninja's new position
				m_CurrentNinja->setPosition(l_PointingElement->getPosition());
				//Map also needs to be informed that position changed
				m_CurrentMap.setNinjaNewPosition(l_PointingElement, m_CurrentNinja);

				//Since Ninja is stepping on a Shuriken it needs to gain a Shuriken in the inventory
				m_CurrentNinja->gainShuriken();

				//Ok, now ninja knows its place, now we need to modify the Ninja's 'old' place
				if (l_SavedObjectX != -1 && l_SavedObjectY != -1)
				{
					//Meaning we have an object to be constructed
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(m_SavedObject);

					//Reset m_SavedObject. It is enough to reset the coordinates because if there is a need to save an object it will be overwritten
					m_SavedObject.setPosition(position(-1, -1));
				}
				else
				{
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(' ', BlankArea, position(l_OldNinjaPos.getY(), l_OldNinjaPos.getX()));
				}
				cout << "MOVE\n";
				m_CurrentMap.render();
				return true;
				break;
			case STEP_ON_MIRROR:
				
				//Setting Ninja's new position
				m_CurrentNinja->setPosition(l_PointingElement->getPosition());
				//Map also needs to be informed that position changed
				m_CurrentMap.setNinjaNewPosition(l_PointingElement, m_CurrentNinja);
				l_NewNinjaPos = l_PointingElement->getPosition();


				//Since Ninja is stepping on a mirror it needs to change the priorities
				//and call flag setting and reverse the current direction priorities
				m_CurrentNinja->setFlippedDirection();
				m_CurrentNinja->reverseCurrentDirectionPriority();

				//Ok, now ninja knows its place, now we need to modify the Ninja's 'old' place
				if( l_SavedObjectX != -1 && l_SavedObjectY != -1)
				{
					//Meaning we have an object to be constructed
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(m_SavedObject);

					//Reset m_SavedObject. It is enough to reset the coordinates because if there is a need to save an object it will be overwritten
					m_SavedObject.setPosition(position(-1, -1));
				}
				else
				{
					//m_SavedObject is missing
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(' ', BlankArea, position(l_OldNinjaPos.getY(), l_OldNinjaPos.getX()));
				}

				//Keep track because this "Mirror" object needs to be saved
				m_SavedObject.setElementType(Mirror);
				m_SavedObject.setPosition(position(l_NewNinjaPos.getY(), l_NewNinjaPos.getX()));
				m_SavedObject.setSymbol('M');

				cout << "MOVE\n";
				m_CurrentMap.render();
				return true;
				break;
			case STEP_ON_SAKE:
				
				//Setting Ninja's new position
				m_CurrentNinja->setPosition(l_PointingElement->getPosition());
				//Map also needs to be informed that position changed
				m_CurrentMap.setNinjaNewPosition(l_PointingElement, m_CurrentNinja);

				l_NewNinjaPos = l_PointingElement->getPosition();
				//Ok, now ninja knows its place, now we need to modify the Ninja's 'old' place
				if (l_SavedObjectX != -1 && l_SavedObjectY != -1)
				{
					//Meaning we have an object to be constructed
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(m_SavedObject);

					//Reset m_SavedObject. It is enough to reset the coordinates because if there is a need to save an object it will be overwritten
					m_SavedObject.setPosition(position(-1, -1));
				}
				else
				{
					//m_SavedObject is missing
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(' ', BlankArea, position(l_OldNinjaPos.getY(), l_OldNinjaPos.getX()));
				}

				//Setting Ninja mode to Breaker/Normal
				(m_CurrentNinja->getMode() == Breaker) ? m_CurrentNinja->setMode(Normal) : m_CurrentNinja->setMode(Breaker);


				//Keep track because this "Sake" object needs to be saved on the map
				m_SavedObject.setElementType(Sake);
				m_SavedObject.setPosition(position(l_NewNinjaPos.getY(), l_NewNinjaPos.getX()));
				m_SavedObject.setSymbol('B');

				cout << "MOVE\n";
				m_CurrentMap.render();
				return true;
				break;
			case PRIO_WEST:
			case PRIO_NORTH:
			case PRIO_SOUTH:
			case PRIO_EAST:

				//Setting Ninja's new position
				m_CurrentNinja->setPosition(l_PointingElement->getPosition());
				//Map also needs to be informed that position changed
				m_CurrentMap.setNinjaNewPosition(l_PointingElement, m_CurrentNinja);

				l_NewNinjaPos = l_PointingElement->getPosition();

				//Ok, now ninja knows its place, now we need to modify the Ninja's 'old' place
				if (l_SavedObjectX != -1 && l_SavedObjectY != -1)
				{
					//Meaning we have an object to be constructed
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(m_SavedObject);

					//Reset m_SavedObject. It is enough to reset the coordinates because if there is a need to save an object it will be overwritten
					m_SavedObject.setPosition(position(-1, -1));
				}
				else
				{
					//m_SavedObject is missing
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(' ', BlankArea, position(l_OldNinjaPos.getY(), l_OldNinjaPos.getX()));
				}
				//Switching new facing direction and saving the Symbol to m_SavedObject
				switch (l_ToDo)
				{
				case PRIO_EAST:

					m_CurrentNinja->setDirection(EAST);
					m_SavedObject.setElementType(D_EAST);
					m_SavedObject.setPosition(position(l_NewNinjaPos.getY(), l_NewNinjaPos.getX()));
					m_SavedObject.setSymbol('E');

					break;
				case PRIO_SOUTH:

					m_CurrentNinja->setDirection(SOUTH);
					m_SavedObject.setElementType(D_SOUTH);
					m_SavedObject.setPosition(position(l_NewNinjaPos.getY(), l_NewNinjaPos.getX()));
					m_SavedObject.setSymbol('S');

					break;
				case PRIO_NORTH:

					m_CurrentNinja->setDirection(NORTH);
					m_SavedObject.setElementType(D_NORTH);
					m_SavedObject.setPosition(position(l_NewNinjaPos.getY(), l_NewNinjaPos.getX()));
					m_SavedObject.setSymbol('N');

					break;
				case PRIO_WEST:

					m_CurrentNinja->setDirection(WEST);
					m_SavedObject.setElementType(D_WEST);
					m_SavedObject.setPosition(position(l_NewNinjaPos.getY(), l_NewNinjaPos.getX()));
					m_SavedObject.setSymbol('W');

					break;
				}

				cout << "MOVE\n";
				m_CurrentMap.render();
				return true;
				break;
			case SP:

				//Getting the position of the pointed Secret Path endpoint pair
				position l_EndPoint = static_cast<SecretPath*>(l_PointingElement)->getEndpoint();

				//Reassigning pointer to the Endpoint
				l_PointingElement = m_CurrentMap.getElement(l_EndPoint);

				//Setting Ninja's new position
				m_CurrentNinja->setPosition(l_EndPoint);
				
				//Map also needs to be informed that position changed
				m_CurrentMap.setNinjaNewPosition(l_PointingElement, m_CurrentNinja);

				l_NewNinjaPos = l_PointingElement->getPosition();

				//Ok, now ninja knows its place, now we need to modify the Ninja's 'old' place
				if (l_SavedObjectX != -1 && l_SavedObjectY != -1)
				{
					//Meaning we have an object to be constructed
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(m_SavedObject);

					//Reset m_SavedObject. It is enough to reset the coordinates because if there is a need to save an object it will be overwritten
					m_SavedObject.setPosition(position(-1, -1));
				}
				else
				{
					//m_SavedObject is missing
					m_CurrentMap.getMap()[l_OldNinjaPos.getY()][l_OldNinjaPos.getX()] = new Symbol(' ', BlankArea, position(l_OldNinjaPos.getY(), l_OldNinjaPos.getX()));
				}

				//Keep track because this EndPoint symbol/position needs to be saved
				m_SavedObject.setElementType(SecretPathPoint);
				m_SavedObject.setPosition(position(l_NewNinjaPos.getY(), l_NewNinjaPos.getX()));
				m_SavedObject.setSymbol(l_PointingElement->getSymbol());

				cout << "SECRET_PATH_MOVE\n";
				m_CurrentMap.render();
				return true;
				break;
			}
		}
		//If Ninja needs to destroy or collect destructibles
		else
		{
			//Exit in case Dojo is the element because Dojo is not possible to be destroyed
			if (l_CurrentSymbol == Dojo)
			{
				break;
			}
			//In case of holy symbol and we have shuriken, let's destroy it, decrease nr. of shurikens
			//and replace it with the HolySymbol
			else if (l_CurrentSymbol == HolySymbol &&
				(m_CurrentNinja->getNoOfShuriken() != 0))
			{
				//If there is no destructible between the Ninja and the Holy Symbol, we can destroy it
				if (!l_DestructibleDetected)
				{
					m_CurrentNinja->throwShuriken();
					cout << "THROW\n";
					l_PointingElement->destroyYourself(true);
					m_CurrentMap.render();
				}
				else
				{
					//Put this into the destructible list
					m_BreakableObstacles.push_back(position((isXtheFixedCoordinate) ? i : fixedCoordinate, (isXtheFixedCoordinate) ? fixedCoordinate : i));
				}
				return true;
			}
			//In case of holy symbol and we are in breaker mode, let's destroy it 
			else if (l_CurrentSymbol == HolySymbol &&
				((m_CurrentNinja->getMode() == Breaker)))
			{
				position l_NinjaPosition = m_CurrentNinja->getPosition();
				position l_CurrentSymbolPosition = l_PointingElement->getPosition();

				//Only if Ninja is 1 step away from Holy Symbol can destroy it.
				if (((l_NinjaPosition.getX() - 1) == l_CurrentSymbolPosition.getX() && l_NinjaPosition.getY() == l_CurrentSymbolPosition.getY()) ||
					((l_NinjaPosition.getX() + 1) == l_CurrentSymbolPosition.getX() && l_NinjaPosition.getY() == l_CurrentSymbolPosition.getY()) ||
					((l_NinjaPosition.getY() - 1) == l_CurrentSymbolPosition.getY() && l_NinjaPosition.getX() == l_CurrentSymbolPosition.getX()) ||
					((l_NinjaPosition.getY() + 1) == l_CurrentSymbolPosition.getY() && l_NinjaPosition.getX() == l_CurrentSymbolPosition.getX())
					)
				{
					cout << "BREAK\n";
					l_PointingElement->destroyYourself(false);
					m_CurrentMap.render();
					return true;
				}

			}
			else if (l_CurrentSymbol == BreakableObstackle)
			{
				m_BreakableObstacles.push_back(position((isXtheFixedCoordinate) ? i : fixedCoordinate,
				                                        (isXtheFixedCoordinate) ? fixedCoordinate : i));
				l_DestructibleDetected = true;
			}
		}

	}
	return  l_Result;
}

void Engine::destroyDestructibles()
{
	//Pointer pointing to the actual element
	IElement* l_PointingElement = nullptr;

	//If HolySymbol is not found, but at least l_BreakableObstaclesis not empty() and
	//we do have shurikens as well, we need to destroy as many as possible
	if (m_BreakableObstacles.empty() != true)
	{
		for (auto t : m_BreakableObstacles)
		{
			if (m_CurrentNinja->getNoOfShuriken() != 0)
			{
				l_PointingElement = m_CurrentMap.getElement(position(t.getY(), t.getX()));
				l_PointingElement->destroyYourself(true);
				m_CurrentNinja->throwShuriken();
				cout << "THROW\n";
				m_CurrentMap.render();
			}
		}
	}

	//We have to clear m_BreableObstacles because next step we need to recalculate them again
	m_BreakableObstacles.clear();
}

bool Engine::possibleToMoveToActualDirection(direction dir)
{
	bool l_Result = false;
	int l_FixCoordinate;
	//Determining if it is possible to move to the actual heading direction

	switch (dir)
	{
	case SOUTH:
		l_FixCoordinate = m_CurrentNinja->getPosition().getX();

		l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, 1, true, false, true);

		if (l_Result)
		{
			return true;
		}
		break;
	case EAST:
		l_FixCoordinate = m_CurrentNinja->getPosition().getY();

		l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, 1, false, false, true);

		if (l_Result)
		{
			return true;
		}
		break;
	case NORTH:
		l_FixCoordinate = m_CurrentNinja->getPosition().getX();

		l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, -1, true, true, true);

		if (l_Result)
		{
			return true;
		}
		break;
	case WEST:
		l_FixCoordinate = m_CurrentNinja->getPosition().getY();

		l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, -1, false, true, true);

		if (l_Result)
		{
			return true;
		}
		break;
	}

	return l_Result;
}

bool Engine::moveToNextPosition()
{
	bool l_Result = false;

	//Depending on the direction left, right, up, down it must be determined
	int l_FixCoordinateY = 0;
	int l_FixCoordinateX = 0;

	//Getting a list of directions ( + current facing direction) starting with the prio
	vector<direction> l_DirectionsBasedOnPrio = m_CurrentNinja->getDirectionPriorities();
	direction l_ActualDirection = m_CurrentNinja->getActualDirection();

	//Checking if direction is flipped
	bool l_IsFlippedDirextion = m_CurrentNinja->isFlippedDirectionPriority();

	//First try to move to the actual direciton, if it is not possible
	//then accoridng to priorities
	if (possibleToMoveToActualDirection(l_ActualDirection))
	{
		return true;
	}

	//Getting fix positions (X or Y coordinate)
	l_FixCoordinateX = m_CurrentNinja->getPosition().getX();
	l_FixCoordinateY = m_CurrentNinja->getPosition().getY();

	//Iterating through l_DirectionBasedOnPrio<directions> with the current priority
	for (int enumDirection = SOUTH; enumDirection <= WEST; enumDirection++)
	{
		if (l_DirectionsBasedOnPrio[enumDirection] == SOUTH)
		{
			// Params:
			// 1: Fix coordinate (X or Y); 2: offset indicating the neighbouring position of the Ninja; 3: is X the Fix coordinate ?
			// 4: The direction is 'decreasing' ? (WEST, NORTH), 5: Is it a move action ?
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinateX, 1, true, false, true);
			m_CurrentNinja->setDirection(SOUTH);

		}
		else if (l_DirectionsBasedOnPrio[enumDirection] == EAST)
		{
			// Params:
			// 1: Fix coordinate (X or Y); 2: offset indicating the neighbouring position of the Ninja; 3: is X the Fix coordinate ?
			// 4: The direction is 'decreasing' ? (WEST, NORTH), 5: Is it a move action ?
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinateY, 1, false, false, true);
			m_CurrentNinja->setDirection(EAST);

		}
		else if (l_DirectionsBasedOnPrio[enumDirection] == NORTH)
		{
			// Params:
			// 1: Fix coordinate (X or Y); 2: offset indicating the neighbouring position of the Ninja; 3: is X the Fix coordinate ?
			// 4: The direction is 'decreasing' ? (WEST, NORTH), 5: Is it a move action ?
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinateX, -1, true, true, true);
			m_CurrentNinja->setDirection(NORTH);
		}
		else if (l_DirectionsBasedOnPrio[enumDirection] == WEST)
		{
			// Params:
			// 1: Fix coordinate (X or Y); 2: offset indicating the neighbouring position of the Ninja; 3: is X the Fix coordinate ?
			// 4: The direction is 'decreasing' ? (WEST, NORTH), 5: Is it a move action ?
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinateY, -1, false, true, true);
			m_CurrentNinja->setDirection(WEST);
		}

		if (l_Result)
		{
			return l_Result;
		}
	}

	//If we reach this point we are sure that movement action was not possible
	return l_Result;
}

bool Engine::b_IsLoopDetected()
{
	//Iterating throug elements and checking if any of it has a value of more then the MAGIC_NUMBER. In such a case we are in a loop.
	//For more info check the description of m_HeatMap
	bool l_Result = false;
	for (auto& t : m_HeatMap)
	{
		for (auto& k : t)
		{
			if (k >= MAGIC_NUMBER)
			{
				return true;
			}
		}
	}

	return l_Result;
}

bool Engine::b_PossibleToDestroyHolySymbol()
{
	bool l_Result = false;

	//Depending on the direction left, right, up, down it must be determined
	int l_FixCoordinate = 0;

	//Getting a list of directions starting with the prio
	vector<direction> l_DirectionsBasedOnPrio = m_CurrentNinja->getDirectionPriorities();
	direction l_ActualDirection = m_CurrentNinja->getActualDirection();

	//Iterating through l_DirectionBasedOnPrio<directions> with the current priority
	for (int enumDirection = l_ActualDirection; enumDirection <= (l_ActualDirection + 3); enumDirection++)
	{
		if (l_DirectionsBasedOnPrio[enumDirection % 4] == SOUTH)
		{
			l_FixCoordinate = m_CurrentNinja->getPosition().getX();

			//Calling function to destroy HS or collect destructible object and not to move (last param is false)!
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, 1, true, false, false);
		}
		else if (l_DirectionsBasedOnPrio[enumDirection % 4] == EAST)
		{
			l_FixCoordinate = m_CurrentNinja->getPosition().getY();

			//Calling function to destroy HS or collect destructible object and not to move (last param is false)!
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, 1, false, false, false);
		}
		else if (l_DirectionsBasedOnPrio[enumDirection % 4] == NORTH)
		{
			l_FixCoordinate = m_CurrentNinja->getPosition().getX();

			//Calling function to destroy HS or collect destructible object and not to move (last param is false)!
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, -1, true, true, false);
		}
		else if (l_DirectionsBasedOnPrio[enumDirection % 4] == WEST)
		{
			l_FixCoordinate = m_CurrentNinja->getPosition().getY();

			//Calling function to destroy HS or collect destructible object and not to move (last param is false)!
			l_Result = b_DestroyHS_or_CollectDestrObj_or_Move(l_FixCoordinate, -1, false, true, false);
		}

		//If any of the function returns true we finished the map
		if (l_Result)
		{
			return l_Result;
		}
	}

	destroyDestructibles();

	//cout << "\n";

	return l_Result;
}
