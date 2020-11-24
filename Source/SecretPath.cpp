#include "../Header/SecretPath.h"

SecretPath::SecretPath():Symbol()
{
}

SecretPath::SecretPath(char symbol, elementType type , position pos, position endpointPos):Symbol(symbol,type,pos),m_EndPointPosition(endpointPos)
{
}

SecretPath::SecretPath(const SecretPath& sp):Symbol(sp.m_Symbol,sp.m_ElementType,sp.m_Position), m_EndPointPosition(sp.m_EndPointPosition)
{
}

void SecretPath::setEndpoint(position pos)
{
	m_EndPointPosition = pos;
}

position SecretPath::getEndpoint()
{
	return m_EndPointPosition;
}
