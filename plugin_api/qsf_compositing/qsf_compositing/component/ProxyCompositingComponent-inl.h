// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline ProxyCompositingComponent::ProxyCompositingComponent(Prototype* prototype) :
			CompositingComponent(prototype),
			mSourceEntityId(0)
		{
			// Nothing to do in here
		}

		inline ProxyCompositingComponent::~ProxyCompositingComponent()
		{
			// Nothing to do in here
		}

		inline uint64 ProxyCompositingComponent::getSourceEntityId() const
		{
			return mSourceEntityId;
		}

		inline void ProxyCompositingComponent::setSourceEntityId(uint64 sourceEntityId)
		{
			assignAndPromoteChange(mSourceEntityId, sourceEntityId, SOURCE_ENTITY_ID);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf
