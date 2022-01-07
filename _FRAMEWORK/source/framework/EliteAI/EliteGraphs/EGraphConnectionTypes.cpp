#include "stdafx.h"
#include "EGraphConnectionTypes.h"
#include "EliteGraphUtilities/EGraphVisuals.h"

using namespace Elite;
GraphConnection::GraphConnection(int from /* = invalid_node_index*/ , int to /* = invalid_node_index*/, float cost /* = 1.f*/)
	: m_From(from)
	, m_To(to)
	, m_Cost(cost)
{
}

bool GraphConnection::operator==(const GraphConnection& rhs) const
{
	return rhs.m_From == this->m_From &&
		rhs.m_To == this->m_To &&
		rhs.m_Cost == this->m_Cost;
}

bool GraphConnection::operator!=(const GraphConnection& rhs) const
{
	return !(*this == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------


GraphConnection2D::GraphConnection2D(int from /* = invalid_node_index*/, int to /* = invalid_node_index*/, float cost /* = 1.f*/)
	:GraphConnection(from, to, cost)
	, m_Color(DEFAULT_CONNECTION_COLOR)
{

}

