/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EBlackboard.h: Blackboard implementation
/*=============================================================================*/
#ifndef ELITE_BLACKBOARD
#define ELITE_BLACKBOARD

//Includes
#include <unordered_map>

namespace Elite
{
	//-----------------------------------------------------------------
	// BLACKBOARD TYPES (BASE)
	//-----------------------------------------------------------------
	class IBlackBoardField
	{
	public:
		IBlackBoardField() = default;
		virtual ~IBlackBoardField() = default;
	};

	//BlackboardField does not take ownership of pointers whatsoever!
	template<typename T>
	class BlackboardField : public IBlackBoardField
	{
	public:
		explicit BlackboardField(T data) : m_Data(data)
		{}
		T GetData() { return m_Data; };
		void SetData(T data) { m_Data = data; }

	private:
		T m_Data;
	};

	//-----------------------------------------------------------------
	// BLACKBOARD (BASE)
	//-----------------------------------------------------------------
	class Blackboard final
	{
	public:
		Blackboard() = default;
		~Blackboard()
		{
			for (auto el : m_BlackboardData)
				SAFE_DELETE(el.second);
			m_BlackboardData.clear();
		}

		Blackboard(const Blackboard& other) = delete;
		Blackboard& operator=(const Blackboard& other) = delete;
		Blackboard(Blackboard&& other) = delete;
		Blackboard& operator=(Blackboard&& other) = delete;

		//Add data to the blackboard
		template<typename T> bool AddData(const std::string& name, T data)
		{
			auto it = m_BlackboardData.find(name);
			if (it == m_BlackboardData.end())
			{
				m_BlackboardData[name] = new BlackboardField<T>(data);
				return true;
			}
			printf("WARNING: Data '%s' of type '%s' already in Blackboard \n", name.c_str(), typeid(T).name());
			return false;
		}

		//Change the data of the blackboard
		template<typename T> bool ChangeData(const std::string& name, T data)
		{
			auto it = m_BlackboardData.find(name);
			if (it != m_BlackboardData.end())
			{
				BlackboardField<T>* p = dynamic_cast<BlackboardField<T>*>(m_BlackboardData[name]);
				if (p)
				{
					p->SetData(data);
					return true;
				}
			}
			printf("WARNING: Data '%s' of type '%s' not found in Blackboard \n", name.c_str(), typeid(T).name());
			return false;
		}

		//Get the data from the blackboard
		template<typename T> bool GetData(const std::string& name, T& data)
		{
			BlackboardField<T>* p = dynamic_cast<BlackboardField<T>*>(m_BlackboardData[name]);
			if (p != nullptr)
			{
				data = p->GetData();
				return true;
			}
			printf("WARNING: Data '%s' of type '%s' not found in Blackboard \n", name.c_str(), typeid(T).name());
			return false;
		}

	private:
		std::unordered_map<std::string, IBlackBoardField*> m_BlackboardData;
	};
}
#endif