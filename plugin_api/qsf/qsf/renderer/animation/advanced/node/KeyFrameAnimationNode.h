// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AnimationNode.h"
#include "qsf/asset/AssetProxy.h"


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
	*    Key frame animation node, it will read a key frame skeleton file and give back the interpolated result at a point
	*/
	class QSF_API_EXPORT KeyFrameAnimationNode : public AnimationNode
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] advancedAnimationComponent
		*    A reference to an advanced animation component where this animation node lies
		*/
		explicit KeyFrameAnimationNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~KeyFrameAnimationNode();

		/**
		*  @brief
		*    Return the key frame skeleton animation asset proxy
		*
		*  @return
		*    The skeleton animation asset proxy to use
		*/
		inline const AssetProxy& getAnimationProxy() const;

		/**
		*  @brief
		*    Return the key frame skeleton animation name
		*
		*  @return
		*    The skeleton animation name
		*/
		inline const std::string& getAnimationName() const;

		/**
		*  @brief
		*    Set the key frame skeleton animation asset proxy to use as animation source
		*
		*  @param[in] animationProxy
		*    Key frame skeleton animation asset proxy
		*/
		void setAnimationProxy(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Return animation progress
		*
		*  @return
		*    Value between 0.0 and 1.0
		*/
		inline float getProgress() const;

		/**
		*  @brief
		*    Return animation progress as time passed
		*/
		inline float getProgressAsTime() const;

		/**
		*  @brief
		*    Set animation progress (will be wrapped into [0.0, 1.0[)
		*/
		void setProgress(float progress);

		/**
		*  @brief
		*    Add animation progress by giving a timestep (in sec.) forward, or backward
		*/
		void addProgressByTimeStep(float timeStep);

		/**
		*  @brief
		*    Return true if we retarget our skeletons from key frame files to our skeleton
		*/
		inline bool getRetarget() const;

		/**
		*  @brief
		*    Set if we want to retarget skeletons from key frame files to our skeleton
		*
		*  @param[in] retarget
		*    "true" to enable retargeting, "false" to disable it
		*/
		void setRetarget(bool retarget);

		/**
		*  @brief
		*    Return time length of the key frame skeleton file
		*
		*  @return
		*    Time in seconds
		*/
		inline float getTimeLength() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AnimationNode methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void calculate() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input parameter
		qsf::AssetProxy	mAnimationProxy;
		float			mProgress;
		bool			mRetarget;

		// Output parameter
		float			mTimeLength;
		std::string		mAnimationName;	///< Key frame skeleton filename


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/KeyFrameAnimationNode-inl.h"
