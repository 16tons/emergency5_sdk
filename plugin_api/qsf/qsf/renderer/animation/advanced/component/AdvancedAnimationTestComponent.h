// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AdvancedAnimationComponent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class KeyFrameAnimationNode;
	class AdditiveBlendingNode;
	class LocomotionNode;
	class BoneLookAtNode;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Advanced animation test component class
	*
	*  @note
	*    - This component includes random tests for many nodes types
	*    - It should be used for experiments and can be changed at will
	*    - Do not use this class in final implementations
	*/
	class QSF_API_EXPORT AdvancedAnimationTestComponent : public AdvancedAnimationComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::AdvancedAnimationTestComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit AdvancedAnimationTestComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AdvancedAnimationTestComponent();

		/**
		*  @brief
		*    Return the key frame skeleton animation asset proxy
		*
		*  @return
		*    The keyframe skeleton animation asset proxy to use (e.g. "sample/skeleton/character/female_angry_01") as animation source, do no destroy the returned instance
		*/
		inline const AssetProxy& getSkeletonAnimation1() const;
		inline const AssetProxy& getSkeletonAnimation2() const;

		/**
		*  @brief
		*    Set the key frame skeleton animation asset proxy to use as animation source
		*
		*  @param[in] animationProxy
		*    The keyframe skeleton animation asset proxy to use (e.g. "sample/skeleton/character/female_angry_01") as animation source
		*/
		void setSkeletonAnimation1(const AssetProxy& animationProxy);
		void setSkeletonAnimation2(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Return the name of the bones
		*/
		inline const std::string& getLeftFootBone() const;
		inline const std::string& getRightFootBone() const;
		inline const std::string& getNeckBone() const;

		/**
		*  @brief
		*    Set the name of the bones
		*/
		void setLeftFootBone(const std::string& leftFoot);
		void setRightFootBone(const std::string& rightFoot);
		void setNeckBone(const std::string& neck);

		/**
		*  @brief
		*    Set the walking cycle of the animation we did give this component
		*/
		inline const glm::vec3& getWalkingCycle() const;
		void setWalkingCycle(const glm::vec3& walkingCycle);

		/**
		*  @brief
		*    Set the entity we want to look at
		*/
		inline uint64 getHeadLookAtEntityId() const;
		void setHeadLookAtEntityId(uint64 lookAtEntityId);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;	// Overwritten functions from component to startup and shutdown the component


	//[-------------------------------------------------------]
	//[ Protected virtual AdvancedAnimationComponent methods  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Before we calculate the animation nodes we need to set all parameters
		*/
		virtual bool onPreCalculation(const JobArguments& jobArguments);

		/**
		*  @brief
		*    After we calculate the animation nodes we need to scheck for errors and results
		*/
		virtual void onPostCalculation(const JobArguments& jobArguments);

		/**
		*  @brief
		*    If we are not visible and not rendered we can still update the time if we want
		*/
		virtual void onNotRendered(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Nodes
		KeyFrameAnimationNode*	mKeyFrameAnimationNode1;
		KeyFrameAnimationNode*	mKeyFrameAnimationNode2;
		AdditiveBlendingNode*	mAdditiveBlendingNode;
		LocomotionNode*			mLocomotionNode;
		BoneLookAtNode*			mBoneLookAtNode;

		// Parameter
		AssetProxy				mAnimationProxy1;
		AssetProxy				mAnimationProxy2;
		std::string				mLeftFootBone;
		std::string				mRightFootBone;
		std::string				mNeckBone;
		glm::vec3				mWalkingCycle;
		WeakPtr<Entity>			mHeadLookAtEntity;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/component/AdvancedAnimationTestComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AdvancedAnimationTestComponent)
