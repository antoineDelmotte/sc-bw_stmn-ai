#pragma once
#include <../BehaviorTree/bt.hpp>

class CheckBoolCondition : public bt::Node
{
public : 

	CheckBoolCondition(bt::Blackboard blackboard, std::string boolName)
	{
		m_blackboard = blackboard;
		m_boolName = boolName;
	}

    Status Tick()
	{
		if (m_blackboard.GetBool(m_boolName))
		{
			return Status::Success;
		}
		else
			return Status::Failure;
	}

private :
	std::string m_boolName;
	bt::Blackboard m_blackboard;
};

