// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline BoneToBoneLinkComponent::BoneToBoneLinkComponent(Prototype* prototype) :
			BoneLinkComponent(prototype),
			mSetParentTransformToChild1(false),
			mParentEntity2(getUninitialized<uint64>()),
			mSetParentTransformToChild2(false)
		{
			// Nothing to do in here
		}

		inline BoneToBoneLinkComponent::~BoneToBoneLinkComponent()
		{
			// Nothing to do in here
		}

		// Link 1
		inline const std::string& BoneToBoneLinkComponent::getParentBoneName1() const
		{
			return mParentBoneName1;
		}

		inline const std::string& BoneToBoneLinkComponent::getChildBoneName1() const
		{
			return mChildBoneName1;
		}

		inline const glm::vec3& BoneToBoneLinkComponent::getLocalPosition1() const
		{
			return mLocalPosition1;
		}

		inline const glm::quat& BoneToBoneLinkComponent::getLocalRotation1() const
		{
			return mLocalRotation1;
		}

		inline bool BoneToBoneLinkComponent::getSetParentTransformToChild1() const
		{
			return mSetParentTransformToChild1;
		}

		// Link 2
		inline uint64 BoneToBoneLinkComponent::getParentEntity2() const
		{
			return mParentEntity2;
		}

		inline const std::string& BoneToBoneLinkComponent::getParentBoneName2() const
		{
			return mParentBoneName2;
		}

		inline const std::string& BoneToBoneLinkComponent::getChildBoneName2() const
		{
			return mChildBoneName2;
		}

		inline const glm::vec3& BoneToBoneLinkComponent::getLocalPosition2() const
		{
			return mLocalPosition2;
		}

		inline const glm::quat& BoneToBoneLinkComponent::getLocalRotation2() const
		{
			return mLocalRotation2;
		}

		inline bool BoneToBoneLinkComponent::getSetParentTransformToChild2() const
		{
			return mSetParentTransformToChild2;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		inline bool BoneToBoneLinkComponent::implementsOnComponentPropertyChange() const
		{
			// This component implements "onComponentPropertyChange()", so return "true"
			return true;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
