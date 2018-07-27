// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline bool SensorComponent::getForceUpdateFromEditor() const
		{
			return false;
		}

		inline size_t SensorComponent::getSummedNumPerceptions() const
		{
			return mSummedNumPerceptions;
		}

		inline bool SensorComponent::getMayHaveEvents() const
		{
			return mMayHaveEvents;
		}

		inline effort::Indicator SensorComponent::update(const JobArguments* jobArguments)
		{
			return updateForEvent(0, jobArguments);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
