// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/action/Action.h"
#include "qsf/reflection/type/CampQsfTime.h"
#include "qsf/renderer/animation/MeshAnimationChannel.h"
#include "qsf/renderer/animation/MeshAnimationComponent.h"


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
	*    Play animation action
	*/
	class QSF_API_EXPORT PlayAnimationAction : public Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const NamedIdentifier ACTION_ID;	///< "qsf::PlayAnimationAction" unique action identifier


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
		*
		*  @param[in] startAnimationNowInInit
		*    "true" if the animation should start in the init function, "false" when it should start in the usual onStartup.
		*/
		void init(const std::string& animationPath, bool waitForFinish = false, bool loopAnimation = false, bool reverse = false, Time blendTime = Time::ZERO, const boost::optional<Time> maxPlayTime = boost::optional<Time>(), bool startAnimationNowInInit = false);

		void initWithAnimationRetargeting(const std::string& animationPath, MeshAnimationComponent::BoneRetargeting boneTranslationRetargeting = MeshAnimationComponent::ANIMATION_BONE_SCALE_RETARGETING, bool waitForFinish = false, bool loopAnimation = false, bool reverse = false);

		/**
		*  @brief
		*    Additional initialization: Set range of animation offsets
		*/
		void setAnimationOffsetRange(float startOffset, float endOffset);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual action::Result updateAction(const Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool startAnimation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		WeakPtr<MeshAnimationChannel> mMeshAnimationChannel;
		std::string mAnimationPath;
		bool		mWaitForFinish;
		bool		mLoopAnimation;
		bool		mReverse;
		Time		mBlendTime;			///< Blend time, may be zero if no blending should be used
		boost::optional<Time>	mMaxPlayTime;
		boost::optional<float>	mStartOffset;
		boost::optional<float>	mEndOffset;
		bool		mInitSucceeded;
		MeshAnimationComponent::BoneRetargeting mBoneTranslationRetargeting;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PlayAnimationAction)
