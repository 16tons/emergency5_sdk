// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InjurePersonAreaComponent::~InjurePersonAreaComponent()
	{
		// Nothing to do in here
	}

	bool InjurePersonAreaComponent::getSimulationState() const
	{
		return mSimulationJobProxy.isValid();
	}

	inline float InjurePersonAreaComponent::getAffectedAreaRadius() const
	{
		return mAffectedAreaRadius;
	}

	inline const std::string& InjurePersonAreaComponent::getInjuriesToUse() const
	{
		return mInjuriesToUse;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
