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
	class MaskedAdditiveBlendingNode;
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
	*    Play skeletal animation component class
	*
	*  @note
	*    - This component plays a skeletal keyframe files and can blend to a new one
	*/
	class QSF_API_EXPORT PlaySkeletalAnimationComponent : public AdvancedAnimationComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::PlaySkeletalAnimationComponent" unique component ID
		static const uint32 MAIN_SKELETON_ANIMATION;	///< "MainSkeletonAnimation" unique class property ID inside the class
		static const uint32 BLEND_LENGTH;				///< "BlendLength" unique class property ID inside the class
		static const uint32 LOOP;						///< "Loop" unique class property ID inside the class
		static const uint32 REVERSE;					///< "Reverse" unique class property ID inside the class
		static const uint32 SPEED;						///< "Speed" unique class property ID inside the class
		static const uint32 BLEND_SYNCHRONIZATION;		///< "BlendSynchronization" unique class property ID inside the class

		enum BlendSynchronization
		{
			SYNC_NONE,		///< Don't sync blended animation with already running animation
			SYNC_PROGRESS,	///< Blend immediately, but copy the audio progress (in percent of total length) from the already running animation
			SYNC_TIME,		///< Blend immediately, but copy the audio progress time from the already running animation
			SYNC_AT_START,	///< Start blended animation in sync with the already running animation's next loop start
			SYNC_AT_END,	///< Start blended animation so that its end is in sync with the already running animation's end
		};


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
		inline explicit PlaySkeletalAnimationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PlaySkeletalAnimationComponent();

		/**
		*  @brief
		*    Return the main key frame skeleton animation asset
		*
		*  @return
		*    The skeleton animation asset to use (e.g. "sample/skeleton/character/female_angry_01") as animation source, do no destroy the returned instance
		*/
		inline const AssetProxy& getMainSkeletonAnimation() const;

		/**
		*  @brief
		*    Set the main key frame skeleton asset to use as animation source
		*
		*  @param[in] animationProxy
		*    The skeleton asset to use (e.g. "sample/skeleton/character/female_angry_01") as animation source
		*
		*  @note
		*    - This will start playAnimation(animationProxy) for new animation proxies)
		*    - This function is used for simple animation in our editor
		*/
		void setMainSkeletonAnimation(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Get/Set properties for Camp and our Component
		*/
		inline float getBlendLength() const;
		inline void setBlendLength(float blendLength);

		inline bool getLoop() const;
		void setLoop(bool loop);

		inline bool getReverse() const;
		void setReverse(bool reverse);

		inline float getSpeed() const;
		void setSpeed(float speed);

		inline BlendSynchronization getBlendSynchronization() const;
		void setBlendSynchronization(BlendSynchronization blendSynchronization);

		/**
		*  @brief
		*    Play a new or old animation, replacing the existing one
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset to use as animation
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting
		*    Using bone translation retargeting or not
		*/
		void playAnimation(const AssetProxy& animationProxy, bool loop = true, bool reverse = false, bool boneRetargeting = true);

		/**
		*  @brief
		*    Pause an animation
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset
		*/
		void pauseAnimation(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Resumes an animation
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset
		*/
		void resumeAnimation(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Stop and remove all animations, go back to binding pose
		*/
		void stopAllAnimations();

		/**
		*  @brief
		*    Linear blend to a new animation, replacing the existing one at the end
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset to use as animation
		*  @param[in] blendLength
		*    Time we use for our linear blending
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting
		*    Using bone translation retargeting or not
		*  @param[in] blendSynchronization
		*    Set a special blend synchronization to time your animation better
		*/
		void blendToAnimation(const AssetProxy& animationProxy, Time blendLength, bool loop = true, bool reverse = false, bool boneRetargeting = true, BlendSynchronization blendSynchronization = SYNC_NONE);

		/**
		*  @brief
		*    Add a masked animation or reset an existing masked animation
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset to use as animation
		*  @param[in] boneMask
		*    A handle of a bone mask, no animation will be added if boneMask is not valid
		*  @param[in] blendLength
		*    Time we use for our linear blending
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting
		*    Using bone translation retargeting or not
		*  @param[in] blendSynchronization
		*    Set a special blend synchronization to time your animation better
		*/
		void addMaskAnimation(const AssetProxy& animationProxy, const BoneMaskHandle& boneMask, Time blendLength, bool loop = true, bool reverse = false, bool boneRetargeting = true, BlendSynchronization blendSynchronization = SYNC_NONE);

		/**
		*  @brief
		*    Add a new animation (Use with care, it is an advanced feature)
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset to use as animation
		*  @param[in] boneMask
		*    A handle of a bone mask, add a unmasked animation handle is invalid
		*  @param[in] weight
		*    Permanent weight for this animation
		*  @param[in] persistent
		*    Persistent animations are not removed when their weight drops to 0, use removePersistentAnimation instead
		*  @param[in] blendLength
		*    Time we use for our linear blending
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting
		*    Using bone translation retargeting or not
		*  @param[in] blendSynchronization
		*    Set a special blend synchronization to time your animation better
		*/
		void addAnimation(const AssetProxy& animationProxy, const BoneMaskHandle& boneMask, float weight, bool persistent, Time blendLength, bool loop = true, bool reverse = false, bool boneRetargeting = true, BlendSynchronization blendSynchronization = SYNC_NONE);

		/**
		*  @brief
		*    Remove a masked animation
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset of the masked animation
		*  @param[in] blendLength
		*    The blend length to blend the mask out
		*/
		void removeMaskAnimation(const AssetProxy& animationProxy, Time blendLength);

		/**
		*  @brief
		*    Remove any persistent animation (masked or weighted)
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset of the masked animation
		*  @param[in] blendLength
		*    The blend length to blend the mask out
		*/
		void removePersistentAnimation(const AssetProxy& animationProxy, Time blendLength);

		/**
		*  @brief
		*    Check if this animation proxy is existing in this component
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset
		*
		*  @note
		*    Animation could be paused or blendend out, use this only to check if the animation still exists
		*/
		inline bool hasAnimation(const AssetProxy& animationProxy) const;

		/**
		*  @brief
		*    Check if this animation is played | paused
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset
		*
		*  @note
		*    - Don't confuse with hasAnimation, if an animation doesn't exists it is not playing or pausing
		*/
		bool isAnimationPlaying(const AssetProxy& animationProxy);
		bool isAnimationPausing(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Check if this animation is remaining and not blending out
		*
		*  @param[in] animationProxy
		*    The skeleton animation asset
		*
		*  @note
		*    - Use together with addMaskAnimation and removeMaskAnimation for better performance
		*/
		bool isAnimationRemaining(const AssetProxy& animationProxy);

		/**
		*  For the following functions:
		*
		*  @note
		*    - Don't use this functions for frequent updates, Only animation components should do that internaly
		*    - Check with hasAnimation if an animation exists or properties can be just default values
		*/

		/**
		*  @brief
		*    Get/Set if an animation is looped or not, will unpause an animation
		*/
		inline bool isAnimationLooped(const AssetProxy& animationProxy) const;
		inline void setAnimationLoop(const AssetProxy& animationProxy, bool loop);

		/**
		*  @brief
		*    Get/Set if an animation is reversed or not
		*/
		inline bool isAnimationReversed(const AssetProxy& animationProxy) const;
		inline void setAnimationReverse(const AssetProxy& animationProxy, bool reverse);

		/**
		*  @brief
		*    Get/Set the animation speed (as factor to normal speed: 1x, double speed 2x ec.)
		*
		*  @note
		*    - Negative speed will set the reverse flag and a reversed animation will get back a negative speed
		*    - 0 speed will halt the animation but not pause it, it is still considered playing
		*/
		inline float getAnimationSpeed(const AssetProxy& animationProxy) const;
		inline void setAnimationSpeed(const AssetProxy& animationProxy, float speed);
		inline void setAllAnimationsSpeed(float speed);

		/**
		*  @brief
		*    Get the bone mask of an animation, you can change the bone mask
		*
		*  @return
		*    Returns a pointer to the bone mask, or a nullptr if no bone mask is set, do not delete it
		*/
		inline BoneMaskHandle getAnimationBoneMask(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Get the time length of an animation
		*/
		Time getAnimationTimeLength(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Get the time progress of an animation
		*/
		Time getAnimationTimeProgress(const AssetProxy& animationProxy);

		/**
		*  @brief
		*    Get/Set the animation weight (Use with care, it is an advanced feature)
		*
		*  @note
		*    - Check that the sum of all weights is 1.0, but ignore masked animations
		*/
		inline float getAnimationWeight(const AssetProxy& animationProxy) const;
		inline void setAnimationWeight(const AssetProxy& animationProxy, float weight);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;	// Overwritten functions from component to startup and shutdown the component


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AdvancedAnimationComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onPreCalculation(const JobArguments& jobArguments) override;
		virtual void onPostCalculation(const JobArguments& jobArguments) override;
		virtual void onNotRendered(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Simple parameter how to play an animation file
		*/
		struct PlayChannel
		{
			KeyFrameAnimationNode* node;
			BoneMaskHandle boneMask;
			float weight;
			bool persistent;

			bool loop;
			bool pause;
			float speed;

			float blendLength;
			float blendWeight;
			bool blendOut;
			float blendDelay;

			PlayChannel() : node(nullptr), weight(1.0f), persistent(false), loop(true), pause(false), speed(1.0f), blendLength(0.0f), blendWeight(1.0f), blendOut(false), blendDelay(0.0f){};
		};


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Get a pointer to the channel with this animation proxy
		*/
		PlayChannel* getChannel(const AssetProxy& animationProxy);
		const PlayChannel* getChannel(const AssetProxy& animationProxy) const;

		/**
		*  @brief
		*    Get the main play channel key frame animation node and create it if necessary
		*/
		KeyFrameAnimationNode& getOrCreateNode(PlayChannel& channel);

		/**
		*  @brief
		*    Remove a key frame animation node from a channel
		*/
		void removeNode(PlayChannel& channel);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update the blending node structure
		*/
		void updateBlendingNode();

		/**
		*  @brief
		*    Update the blending node structure
		*/
		void updateBlendingWeight();

		/**
		*  @brief
		*    Update inverse bone mask
		*/
		void updateInverse();

		/**
		*  @brief
		*    Update the progress
		*/
		void updateProgress(float secondsPassed);

		/**
		*  @brief
		*    Update the non rendered time to progress if we change nodes while we are not rendered
		*/
		void updateNonRenderedTimeToProgress();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Play channel with nodes
		std::deque<PlayChannel>	mChannel;

		// Nodes
		MaskedAdditiveBlendingNode*	mBlendingNode;

		// Internal
		float					mTimeNotRendered;
		BoneMaskHandle			mInverseBoneMask;

		// TODO(tl): We could move the properties to a new class to save memory if we use this component in a game
		// Camp Properties
		AssetProxy				mAnimationProxy;
		float					mBlendLength;
		bool					mLoop;
		bool					mReverse;
		float					mSpeed;
		BlendSynchronization	mBlendSynchronization;


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
#include "qsf/renderer/animation/advanced/component/PlaySkeletalAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PlaySkeletalAnimationComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PlaySkeletalAnimationComponent::BlendSynchronization)
