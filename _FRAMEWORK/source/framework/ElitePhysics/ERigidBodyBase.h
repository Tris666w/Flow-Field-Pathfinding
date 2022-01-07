/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERigidBodyBase.h: 2D RigidBody base for Engine.
/*=============================================================================*/
#ifndef ELITE_PHYSICS_RIGIDBODY_BASE
#define	ELITE_PHYSICS_RIGIDBODY_BASE
namespace Elite 
{
	//--- Includes ---
	#include "EPhysicsTypes.h"
	#include "EPhysicsShapes.h"

	template<typename translationType, typename orientationType>
	class RigidBodyBase final
	{
	private:
		//=== Internal Type Defines ===
		typedef ETransform<translationType, orientationType> internalTransformType;

	public:
		//=== Constructors & Destructors ===
		RigidBodyBase(const RigidBodyDefine& define, const internalTransformType& initialTransform,
			PhysicsFlags userFlags = PhysicsFlags::Default);
		~RigidBodyBase();

		//=== RigidBody Functions ===
		void AddShape(Elite::EPhysicsShape* pShape);
		void RemoveAllShapes();

		internalTransformType GetTransform();
		void SetTransform(const internalTransformType& transform);
		translationType GetPosition();
		void SetPosition(const translationType& pos);
		orientationType GetRotation();
		void SetRotation(const orientationType& rot);

		void SetMass(float m);
		float GetMass();

		translationType GetLinearVelocity();
		void SetLinearVelocity(const translationType& linearVelocity);
		translationType GetAngularVelocity();
		void SetAngularVelocity(const translationType& angularVelocity);

		void SetLinearDamping(float damping);
		float GetLinearDamping();

		void AddForce(const translationType& force, EForceMode mode, bool autoWake = true);
		void AddTorque(const translationType& torque, EForceMode mode, bool autoWake = true);

		ERigidBodyType GetType() { return m_RigidBodyInformation.type; }
		int GetUserDefinedFlags();
		void SetUserDefinedFlags(PhysicsFlags flags);
		void AddUserDefinedFlags(PhysicsFlags flags);
		void RemoveUserDefinedFlags(PhysicsFlags flags);
		void SetUserData(RigidBodyUserData pUserData) { m_pUserData = pUserData; }
		RigidBodyUserData GetUserData() const { return m_pUserData; }

	private:
		//=== Datamembers ===
		std::vector<void*> m_vFixtures = {};
		internalTransformType m_Transform = {};
		RigidBodyDefine m_RigidBodyInformation = {};
		void* m_pBody = nullptr;
		RigidBodyUserData m_pUserData = {};
		PhysicsFlags m_UserDefinedFlags = PhysicsFlags::Default;

		//=== Internal Functions ===
		void Initialize();
	};
}
#endif