#include "../Header/Map.h"
#include <iostream>


Map::Map()
{
	m_Length = 0;
	m_Width = 0;
}

Map::~Map()
{
	for (auto& i : m_Map)
	{
		for (auto& j : i)
		{
			delete j;
			j = nullptr;
		}
		i.clear();
	}
	m_Map.clear();
	m_SecretPaths.clear();
}

bool Map::reInitToEmpty()
{
	//Delete, reset each and every element in the class
	bool l_Result = false;

	m_Width =
		m_Length = 0;
	
	for (auto& i : m_Map)
	{
		for (auto& j : i)
		{
			delete j;
			j = nullptr;
		}
		i.clear();
	}
	m_Map.clear();
	m_SecretPaths.clear();

	(m_Map.empty()) ? l_Result = true : l_Result = false;

	return l_Result;
	
}

void Map::render()
{
	for (auto& i : m_Map)
	{
		for (auto& j : i)
		{
			cout << (j->getSymbol());
		}
		cout << "\n";
	}
}

void Map::addParticipant(IElement* element)
{
		m_Map[m_Length][m_Width] = element;
}

bool Map::stringToMap(string filename)
{
	char ch;
	bool l_Result = false;

	//Try opening file
	std::ifstream file(filename);

	if (!(file.fail()))
	{
		while (file >> noskipws >> ch) {

			determinElement(ch);
		}
		matchSecretPaths();

		//Supposingly if width and length are not 0 meaning we 
		//could open the file and read something useful from it
		if (m_Length != 0 || m_Width != 0)
		{
			l_Result = true;
		}
	}

	return l_Result;
}

void Map::matchSecretPaths()
{
	//Iterate through SecretPaths if any exists
	for(auto& sp:m_SecretPaths)
	{
		char l_StartSymbol = sp.second.getSymbol();
		int l_StartX = sp.second.getX();
		int l_StartY = sp.second.getY();

		for(auto& ep:m_SecretPaths)
		{
			char l_EndSymbol = ep.second.getSymbol();
			int l_EndX = ep.second.getX();
			int l_EndY = ep.second.getY();

			//If X or Y position is not the same BUT the symbol YES there is a match
			if(((sp.second.getX() != ep.second.getX()) ||
				(sp.second.getY() != ep.second.getY()) )&&
				(l_EndSymbol == l_StartSymbol))
			{
				//We have a 'match'
				SecretPath* l_StartPoint = dynamic_cast<SecretPath*>(m_Map[l_StartY][l_StartX]);
				SecretPath* l_EndPoint = dynamic_cast<SecretPath*>(m_Map[l_EndY][l_EndX]);

				//Only set endpoints if objects are valid
				if (l_StartPoint && l_EndPoint) 
				{
					l_StartPoint->setEndpoint(position(l_EndY, l_EndX));
					l_EndPoint->setEndpoint(position(l_StartY, l_StartX));
				}
				
			}
		}
	}
}

void Map::determinElement(char ch)
{
	IElement* t = NULL;
	static int IdxPath = 0;
	
	if(m_Length == 0 && m_Width == 0)
	{
		//Adding new 'line / row' to map
		this->m_Map.emplace_back();
	}
	switch(ch)
	{
	case '@':
		t = new Ninja("KeszeyDaniel",ch,NinjaFighter,position(m_Length,m_Width));
		break;
	case '#':
		t = new Symbol(ch,Dojo, position(m_Length,m_Width));
		break;
	case ' ':
		t = new Symbol(ch, BlankArea, position(m_Length, m_Width));
		break;
	case '*':
		t = new Symbol(ch, Shuriken, position(m_Length, m_Width));
		break;
	case 'X':
		t = new Symbol(ch, BreakableObstackle, position(m_Length, m_Width));
		break;
	case '$':
		t = new Symbol(ch, HolySymbol, position(m_Length, m_Width));
		break;
	case 'M':
		t = new Symbol(ch, Mirror, position(m_Length, m_Width));
		break;
	case 'B':
		t = new Symbol(ch, Sake, position(m_Length, m_Width));
		break;
	case 'S':
		t = new Symbol(ch, D_SOUTH, position(m_Length, m_Width));
		break;
	case 'E':
		t = new Symbol(ch, D_EAST, position(m_Length, m_Width));
		break;
	case 'N':
		t = new Symbol(ch, D_NORTH, position(m_Length, m_Width));
		break;
	case 'W':
		t = new Symbol(ch, D_WEST, position(m_Length, m_Width));
		break;
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
		t = new SecretPath(ch,SecretPathPoint,position(m_Length,m_Width),position());
		//Inserting secret paths to container having them saved and accessed quicker runtime-wise

		if (t) 
		{
			m_SecretPaths[IdxPath++] = secretPathHelper(ch, m_Length, m_Width);
		}
		
		break;
	case '\0':
	case '\n':
		m_Length++;
		m_Width = 0;
		m_Map.emplace_back();
		break;
	default:
		break;
	}

	//Only put item into the map if created object is valid
	if (t)
	{
		m_Map[m_Length].push_back(t);
		m_Width++;
	}
}

Ninja* Map::getNinja()
{

	for (auto& i : m_Map)
	{
		for (auto& j : i)
		{
			if (j->getType() == NinjaFighter)
			{
				return (Ninja*)j;
			}
		}
	}
	return nullptr;
}

int Map::getLength()
{
	return m_Length;
}

int Map::getWidth()
{
	return m_Map[0].size();
}

IElement* Map::getElement(position pos)
{
	return m_Map[pos.getY()][pos.getX()];
}

movementAction Map::getNextStep(IElement* field)
{
	char ch = field->getSymbol();
	switch (ch)
	{
	case '@':
		return NO_MOVEMENT;
		break;
	case '#':
		return NO_MOVEMENT;
		break;
	case ' ':
		return STEP_ON_BLANK_AREA;
		break;
	case '*':
		return STEP_ON_SHURIKEN;
		break;
	case 'X':
		return NO_MOVEMENT;
		break;
	case '$':
		return NO_MOVEMENT;
		break;
	case 'M':
		return STEP_ON_MIRROR;
		break;
	case 'B':
		return STEP_ON_SAKE;
		break;
	case 'S':
		return PRIO_SOUTH;
		break;
	case 'E':
		return PRIO_EAST;
		break;
	case 'N':
		return PRIO_NORTH;
		break;
	case 'W':
		return PRIO_WEST;
		break;
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
		return SP;
		break;
	default:
		return NO_MOVEMENT;
		break;
	}
}

void Map::setNinjaNewPosition(IElement* headingElement, IElement* Ninja)
{
	int l_Y = headingElement->getPosition().getY();
	int l_X = headingElement->getPosition().getX();
	m_Map[l_Y][l_X] = Ninja;
}

vector<vector<IElement*>>& Map::getMap()
{
	return m_Map;
}
