// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>
#include <qsf/reflection/type/CampQsfTime.h>
#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 play animation action
	*/
	class EM5_API_EXPORT PlayAnimationAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PlayAnimationAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlayAnimationAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlayAnimationAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const std::string& animationPath, bool waitForFinish = false, bool loopAnimation = false, bool reverse = false, qsf::Time blendTime = qsf::Time::ZERO, const boost::optional<qsf::Time> maxPlayTime = boost::optional<qsf::Time>());

		// Just clear the person animation (which enforces that it will check for a new animation state)
		void initClearPersonAnimation();

		void initWithAnimationRetargeting(const std::string& animationPath, qsf::MeshAnimationComponent::BoneRetargeting boneTranslationRetargeting = qsf::MeshAnimationComponent::ANIMATION_BONE_SCALE_RETARGETING, bool waitForFinish = false, bool loopAnimation = false, bool reverse = false);

		/**
		*  @brief
		*    Additional initialization: Set range of animation offsets
		*/
		void setAnimationOffsetRange(float startOffset, float endOffset);

		void setMaskedAnimation(const std::string& maskAnimationPath, const std::string& boneMask);

		void setEnforceGraphicsSynchronization(bool value);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool startAnimation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::MeshAnimationChannel>		 mMeshAnimationChannel;
		qsf::AssetProxy								 mAnimationAssetProxy;
		bool										 mWaitForFinish;
		bool										 mLoopAnimation;
		bool										 mReverse;
		qsf::Time									 mBlendTime;			///< Blend time, may be zero if no blending should be used
		boost::optional<qsf::Time>					 mMaxPlayTime;
		boost::optional<float>						 mStartOffset;
		boost::optional<float>						 mEndOffset;
		qsf::MeshAnimationComponent::BoneRetargeting mBoneTranslationRetargeting;
		bool										 mClearPersonAnimation;
		bool										 mPreventUpdateAnimations;

		// For masked animations
		qsf::AssetProxy mMaskAnimationAssetProxy;
		std::string		mBoneMask;

		// Fix for animation outside of the screen
		bool			mEnforceGraphicsSynchronization;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::PlayAnimationAction)
