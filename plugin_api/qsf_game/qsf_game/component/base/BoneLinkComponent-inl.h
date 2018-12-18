// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline BoneLinkComponent::BoneLinkComponent(Prototype* prototype) :
			Component(prototype),
			mOldParentPrototypeId(getUninitialized<uint64>()),
			mObjectRendered(false)
		{
			// Nothing to do in here
		}

		inline BoneLinkComponent::~BoneLinkComponent()
		{
			// Nothing to do in here
		}

		inline const std::string& BoneLinkComponent::getBoneName() const
		{
			return mBoneName;
		}

		inline const glm::vec3& BoneLinkComponent::getLocalPosition() const
		{
			return mLocalPosition;
		}

		inline const glm::quat& BoneLinkComponent::getLocalRotation() const
		{
			return mLocalRotation;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		inline bool BoneLinkComponent::implementsOnComponentPropertyChange() const
		{
			return true;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
