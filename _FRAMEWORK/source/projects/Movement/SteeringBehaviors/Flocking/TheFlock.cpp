#include "stdafx.h"
#include "TheFlock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"

//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/,
	float worldSize /*= 100.f*/,
	SteeringAgent* pAgentToEvade /*= nullptr*/,
	bool trimWorld /*= false*/)

	: m_FlockSize{ flockSize }
	, m_TrimWorld{ trimWorld }
	, m_WorldSize{ worldSize }
	, m_NrOfNeighbors{ 0 }
	, m_pAgentToEvade{ pAgentToEvade }
{
	m_pEvadeBehavior = new Evade();
	m_pWanderBehavior = new Wander();
	m_pVelMatchBehavior = new VelocityMatch(this);
	m_pCohesionBehavior = new Cohesion(this);
	m_pSeparationBehavior = new Seperation(this);
	m_pSeekBehavior = new Seek();
	m_pBlendedSteering = new BlendedSteering({ { m_pSeparationBehavior,0.48f }, { m_pCohesionBehavior,0.40f }, {m_pVelMatchBehavior,0.53f},{m_pSeekBehavior,0.05f},{m_pWanderBehavior,0.3f} });
	m_pPrioritySteering = new PrioritySteering({ m_pEvadeBehavior,m_pBlendedSteering });

	m_pPartitionedSpace = new CellSpace(worldSize, m_WorldSize, m_NrPartitionRows, m_NrPartitionCols, m_FlockSize);
	m_Neighbors.reserve(m_FlockSize);
	m_Neighbors.resize(m_FlockSize);


	for (int i = 0; i < m_FlockSize; i++)
	{
		m_Agents.push_back(new SteeringAgent());
		m_Agents[i]->SetSteeringBehavior(m_pPrioritySteering);
		m_Agents[i]->SetMaxLinearSpeed(30.f);
		m_Agents[i]->SetAutoOrient(true);
		m_Agents[i]->SetMass(1.f);
		m_Agents[i]->SetPosition({ Elite::randomFloat(0,m_WorldSize),Elite::randomFloat(0,m_WorldSize) });
		m_pPartitionedSpace->AddAgent(m_Agents[i]);
		m_OldPositions.push_back(m_Agents[i]->GetPosition());
	}
	m_Agents.resize(m_FlockSize);

	UpdateEvadeTarget();


}

Flock::~Flock()
{
	SAFE_DELETE(m_pBlendedSteering)
		SAFE_DELETE(m_pPrioritySteering)
		SAFE_DELETE(m_pEvadeBehavior)
		SAFE_DELETE(m_pWanderBehavior)
		SAFE_DELETE(m_pCohesionBehavior)
		SAFE_DELETE(m_pSeparationBehavior)
		SAFE_DELETE(m_pVelMatchBehavior)
		SAFE_DELETE(m_pSeekBehavior)

		SAFE_DELETE(m_pBlendedSteering)
		SAFE_DELETE(m_pPrioritySteering)

		SAFE_DELETE(m_pPartitionedSpace);

	for (auto pAgent : m_Agents)
	{
		SAFE_DELETE(pAgent)
	}
	m_Agents.clear();
}

void Flock::Update(float deltaT)
{

	UpdateEvadeTarget();
	if (m_UsesPartitioning)
	{
		for (size_t i = 0; i < m_Agents.size(); i++)
		{
			m_pPartitionedSpace->RegisterNeighbors(m_Agents[i], m_NeighborhoodRadius);
			m_Agents[i]->Update(deltaT);
			m_Agents[i]->TrimToWorld(Elite::Vector2(0, 0), Elite::Vector2(m_WorldSize, m_WorldSize));

			m_pPartitionedSpace->UpdateAgentCell(m_Agents[i], m_OldPositions[i]);
			m_OldPositions[i] = m_Agents[i]->GetPosition();
		}
	}
	else
	{
		for (SteeringAgent* currentAgent : m_Agents)
		{
			RegisterNeighbors(currentAgent);
			currentAgent->Update(deltaT);
			currentAgent->TrimToWorld(Elite::Vector2(0, 0), Elite::Vector2(m_WorldSize, m_WorldSize));
			m_NrOfNeighbors = 0;
		}
	}
}

void Flock::Render(float deltaT)
{
	for (SteeringAgent* currentAgent : m_Agents)
	{
		currentAgent->SetBodyColor({ 1.f,1.f,0.f });
	}

	if (m_UsesPartitioning)
	{
		if (m_DebugRenderNeighborhood)
		{
			m_pPartitionedSpace->RegisterNeighbors(m_Agents[0], m_NeighborhoodRadius);

			for (int i = 0; i < m_pPartitionedSpace->GetNrOfNeighbors(); i++)
			{
				m_pPartitionedSpace->GetNeighbors()[i]->SetBodyColor({ 0.f,1.f,0.f });
			}
			DEBUGRENDERER2D->DrawCircle(m_Agents[0]->GetPosition(), m_NeighborhoodRadius, { 1.f,1.f,1.f }, 0.5f);
			m_pPartitionedSpace->DebugRenderNeighboringCells();
			CellSpace::RenderBoundingBox(m_Agents[0]->GetPosition(), m_NeighborhoodRadius);
		}
	}
	else
	{
		if (m_DebugRenderNeighborhood)
		{
			RegisterNeighbors(m_Agents[0]);
			for (int i = 0; i < m_NrOfNeighbors; i++)
			{
				m_Neighbors[i]->SetBodyColor({ 0.f,1.f,0.f });
			}
			DEBUGRENDERER2D->DrawCircle(m_Agents[0]->GetPosition(), m_NeighborhoodRadius, { 1.f,1.f,1.f }, 0.5f);
		}
	}

	for (SteeringAgent* currentAgent : m_Agents)
	{
		if (m_DebugRenderSteering)
			currentAgent->SetRenderBehavior(true);
		else
			currentAgent->SetRenderBehavior(false);

	}

	if (m_DebugPartitionGrid)
		m_pPartitionedSpace->RenderCells();


}

const vector<SteeringAgent*>& Flock::GetNeighbors() const
{
	if (m_UsesPartitioning)
		return m_pPartitionedSpace->GetNeighbors();
	else
		return m_Neighbors;
}

void Flock::UpdateAndRenderUI()
{
	//Setup
	int const menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

	std::string text("Amount of agents: " + std::to_string(m_FlockSize));
	ImGui::Text(text.c_str());

	text = std::string("WorldSize: ") + std::to_string(m_WorldSize);
	ImGui::Text(text.c_str());

	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Flocking");
	ImGui::Spacing();

	ImGui::Checkbox("Debug Rendering neighborhood", &m_DebugRenderNeighborhood);
	ImGui::Checkbox("Debug Rendering steering", &m_DebugRenderSteering);
	ImGui::Checkbox("Debug Partitioning grid", &m_DebugPartitionGrid);
	ImGui::Checkbox("Enable Partitioning", &m_UsesPartitioning);

	ImGui::Text("Behavior Weights");
	ImGui::Spacing();

	ImGui::Text("Behavior Weights");
	ImGui::Spacing();

	ImGui::SliderFloat("Separation", &m_pBlendedSteering->GetWeightedBehaviorsRef()[0].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Cohesion", &m_pBlendedSteering->GetWeightedBehaviorsRef()[1].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Velocity match", &m_pBlendedSteering->GetWeightedBehaviorsRef()[2].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Seek", &m_pBlendedSteering->GetWeightedBehaviorsRef()[3].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Wander", &m_pBlendedSteering->GetWeightedBehaviorsRef()[4].weight, 0.f, 1.f, "%.2f");
	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();

}

int Flock::GetNrOfNeighbors() const
{
	if (m_UsesPartitioning)
		return m_pPartitionedSpace->GetNrOfNeighbors();
	else
		return m_NrOfNeighbors;
}

void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	float const squaredNeighborhood = m_NeighborhoodRadius * m_NeighborhoodRadius;

	for (SteeringAgent* pOther : m_Agents)
	{
		if (pOther != pAgent)
		{
			if (DistanceSquared(pAgent->GetPosition(), pOther->GetPosition()) <= squaredNeighborhood)
			{
				if (m_NrOfNeighbors < static_cast<int>(m_Neighbors.size()))
				{
					m_Neighbors[m_NrOfNeighbors] = pOther;
					m_NrOfNeighbors++;
				}
			}
		}
	}
}

Elite::Vector2 Flock::GetAverageNeighborPos() const
{
	Elite::Vector2 averagePos{};

	for (int index = 0; index < GetNrOfNeighbors(); ++index)
	{
		if (m_UsesPartitioning)
			averagePos += m_pPartitionedSpace->GetNeighbors()[index]->GetPosition();
		else
			averagePos += m_Neighbors[index]->GetPosition();
	}

	averagePos = averagePos / static_cast<float>(GetNrOfNeighbors());

	return averagePos;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const
{
	Elite::Vector2 averageVel{};

	for (int index = 0; index < GetNrOfNeighbors(); ++index)
	{
		if (m_UsesPartitioning)
			averageVel += m_pPartitionedSpace->GetNeighbors()[index]->GetLinearVelocity();
		else
			averageVel += m_Neighbors[index]->GetLinearVelocity();
	}

	averageVel = averageVel / static_cast<float>(GetNrOfNeighbors());

	return averageVel;
}

void Flock::SetSeekTarget(const TargetData target) const
{
	m_pSeekBehavior->SetTarget(target);
}

float* Flock::GetWeight(ISteeringBehavior* pBehavior) const
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if (it != weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}

void Flock::UpdateEvadeTarget() const
{
	TargetData evadeTarget;
	evadeTarget.Position = m_pAgentToEvade->GetPosition();
	evadeTarget.Orientation = m_pAgentToEvade->GetOrientation();
	evadeTarget.LinearVelocity = m_pAgentToEvade->GetLinearVelocity();
	evadeTarget.AngularVelocity = m_pAgentToEvade->GetAngularVelocity();
	m_pEvadeBehavior->SetTarget(evadeTarget);

}
