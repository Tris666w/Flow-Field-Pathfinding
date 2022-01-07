/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Andries Geens
/*=============================================================================*/
// AgarioContactListener.h: Contact Listener wrapper for the agario game 
// Listens for contacts between food-agents and agents-agents
/*=============================================================================*/
#ifndef ELITE_AGARIO_CONTACT_LISTENER
#define ELITE_AGARIO_CONTACT_LISTENER
class AgarioAgent;
class AgarioFood;

class AgarioContactListener : public b2ContactListener
{
public:
    AgarioContactListener();

private:
    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;

private:
    void OnCollisionPlayerPlayer(AgarioAgent* agentA, AgarioAgent* agentB);
    void OnCollisionFoodPlayer(AgarioAgent* agent, AgarioFood* food);

	float m_FoodPerRadius = 0.3f;
};
#endif

