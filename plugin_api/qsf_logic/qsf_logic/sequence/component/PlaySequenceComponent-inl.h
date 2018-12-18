// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline PlaySequenceComponent::PlaySequenceComponent(Prototype* prototype) :
			Component(prototype),
			mLoop(true),
			mSequencePlayer(nullptr)
		{
			// Nothing to do in here
		}

		inline PlaySequenceComponent::~PlaySequenceComponent()
		{
			// Nothing to do in here
		}

		inline const AssetProxy& PlaySequenceComponent::getSequence() const
		{
			return mSequence;
		}

		inline bool PlaySequenceComponent::getLoop() const
		{
			return mLoop;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
