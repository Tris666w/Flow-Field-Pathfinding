#include "stdafx.h"
#include "FlowFieldFlock.h"

#include "FlowFieldSteering.h"
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/Flocking/FlockingSteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/SpacePartitioning/SpacePartitioning.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

FlowFieldFlock::FlowFieldFlock(Graph2D flowField, int nrCols, int nrRows, float cellSize, int flockSize, float agentSpeed)
	:m_FlockSize(flockSize),
	m_AgentSpeed(agentSpeed),
	m_pSeparationBehavior(new Seperation(this)),
	m_pCohesionBehavior(new Cohesion(this)),
	m_pVelMatchBehavior(new VelocityMatch(this)),
	m_pWanderBehavior(new Wander()),
	m_pFlowFieldGraph(flowField)
{


	//----------------------------------
	//INITIALIZE STEERING
	//----------------------------------
	m_pFlowFieldBehavior = new FlowFieldSteering(m_pFlowFieldGraph);

	m_pBlendedSteering = new BlendedSteering({ {m_pFlowFieldBehavior,0.5f}, { m_pSeparationBehavior,0.145f }, { m_pCohesionBehavior,0.12f }, {m_pVelMatchBehavior,0.16f},{m_pWanderBehavior,0.75f} });

	m_WorldDimensions.x = static_cast<float>(nrCols) * cellSize;
	m_WorldDimensions.y = static_cast<float>(nrRows) * cellSize;

	m_pPartitionedSpace = new CellSpace(m_WorldDimensions.x,
		m_WorldDimensions.y, nrRows, nrCols, m_FlockSize);

	for (int i = 0; i < m_FlockSize; i++)
	{
		m_Agents.push_back(new SteeringAgent());
		m_Agents[i]->SetSteeringBehavior(m_pBlendedSteering);
		m_Agents[i]->SetMaxLinearSpeed(30.f);
		m_Agents[i]->SetMaxLinearSpeed(agentSpeed);
		m_Agents[i]->SetAutoOrient(true);
		m_Agents[i]->SetMass(1.f);
		m_Agents[i]->SetPosition({ Elite::randomFloat(0,m_WorldDimensions.x),Elite::randomFloat(0,m_WorldDimensions.y) });
		m_pPartitionedSpace->AddAgent(m_Agents[i]);
		m_OldPositions.push_back(m_Agents[i]->GetPosition());
	}
	m_Agents.resize(m_FlockSize);
}

FlowFieldFlock::~FlowFieldFlock()
{
	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pWanderBehavior);
	SAFE_DELETE(m_pCohesionBehavior);
	SAFE_DELETE(m_pSeparationBehavior);
	SAFE_DELETE(m_pVelMatchBehavior);
	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPartitionedSpace);
	SAFE_DELETE(m_pFlowFieldBehavior);

	for (auto pAgent : m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();
}

void FlowFieldFlock::Update(float deltaT)
{
	for (size_t i = 0; i < m_Agents.size(); i++)
	{
		m_pPartitionedSpace->RegisterNeighbors(m_Agents[i], m_NeighborhoodRadius);
		m_Agents[i]->Update(deltaT);
		m_Agents[i]->TrimToWorld(Elite::Vector2(0, 0), Elite::Vector2(m_WorldDimensions.x, m_WorldDimensions.y));

		m_pPartitionedSpace->UpdateAgentCell(m_Agents[i], m_OldPositions[i]);
		m_OldPositions[i] = m_Agents[i]->GetPosition();
	}
}

const vector<SteeringAgent*>& FlowFieldFlock::GetNeighbors() const
{
	return m_pPartitionedSpace->GetNeighbors();
}

int FlowFieldFlock::GetNrOfNeighbors() const
{
	return m_pPartitionedSpace->GetNrOfNeighbors();
}

Elite::Vector2 FlowFieldFlock::GetAverageNeighborPos() const
{
	Elite::Vector2 averagePos{};

	for (int index = 0; index < GetNrOfNeighbors(); ++index)
		averagePos += m_pPartitionedSpace->GetNeighbors()[index]->GetPosition();

	averagePos = averagePos / static_cast<float>(GetNrOfNeighbors());

	return averagePos;
}

Elite::Vector2 FlowFieldFlock::GetAverageNeighborVelocity() const
{
	Elite::Vector2 averageVel{};

	for (int index = 0; index < GetNrOfNeighbors(); ++index)
		averageVel += m_pPartitionedSpace->GetNeighbors()[index]->GetLinearVelocity();

	averageVel = averageVel / static_cast<float>(GetNrOfNeighbors());

	return averageVel;
}

void FlowFieldFlock::ChangeAmountOfAgents(int newAmount)
{
	int vectorSize = static_cast<int>(m_Agents.size());

	if (newAmount == vectorSize || (vectorSize <= 0 && newAmount <= 0))
		return;


	if (newAmount > vectorSize)
	{
		while (newAmount > vectorSize)
		{
			auto agent = new SteeringAgent();
			m_Agents.push_back(agent);
			agent->SetSteeringBehavior(m_pBlendedSteering);
			agent->SetMaxLinearSpeed(m_AgentSpeed);
			agent->SetAutoOrient(true);
			agent->SetMass(0.1f);
			agent->SetPosition({ Elite::randomFloat(0,m_WorldDimensions.x),Elite::randomFloat(0,m_WorldDimensions.y) });

			++vectorSize;
			if (newAmount > m_pPartitionedSpace->GetNeighborVectorSize())
				m_pPartitionedSpace->AddPossibleNeighbor();
			m_OldPositions.push_back({});

		}
		return;
	}


	if (newAmount < vectorSize)
	{
		int const agentsDiff = vectorSize - newAmount;

		for (int i = 0; i < agentsDiff; i++)
		{
			auto agent = m_Agents.front();
			std::_Erase_remove(m_Agents, agent);
			m_pPartitionedSpace->RemoveAgent(agent);
			m_OldPositions.pop_back();
			SAFE_DELETE(agent);
		}
	}

}
