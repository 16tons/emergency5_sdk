// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AiLevelOfDetailComponent::AiLevelOfDetailComponent(qsf::Prototype* prototype) :
		Component(prototype),
		mUseFullDetail(true) // TODO(vs) or is it safe to calculate an initial value here?
	{
		// Nothing to do in here
	}

	inline bool AiLevelOfDetailComponent::useFullDetail() const
	{
		return mUseFullDetail;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
