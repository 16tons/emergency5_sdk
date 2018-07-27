// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/MeshAnimationComponent.h"
#include "qsf/math/EnumBitmask.h"
#include "qsf/time/Time.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"
#include "qsf/base/manager/FastPodAllocator.h"
#include "qsf/base/WeakPtr.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
		class AnimationState;
	}
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
	*    Mesh animation channel
	*/
	class QSF_API_EXPORT MeshAnimationChannel : public WeakPtrTarget, public boost::noncopyable, public FastPodAllocator<MeshAnimationChannel>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MeshAnimationComponent;	// Needs access to private members


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline MeshAnimationChannel();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeshAnimationChannel();

		/**
		*  @brief
		*    Initialize with the given OGRE animation state
		*/
		void initializeWithOgreAnimationState(Ogre::v1::AnimationState& ogreAnimationState);

		/**
		*  @brief
		*    Get animation length; this is always positive, never zero
		*/
		inline Time getLength() const;

		/**
		*  @brief
		*    Set animation loop flag
		*/
		inline bool getLoop() const;

		/**
		*  @brief
		*    Set animation loop flag
		*/
		void setLoop(bool loop);

		/**
		*  @brief
		*    Get animation reverse flag
		*/
		inline bool getReverse() const;

		/**
		*  @brief
		*    Set animation reverse flag
		*/
		inline void setReverse(bool reverse);

		/**
		*  @brief
		*    Get playback offset between 0.0 and 1.0
		*/
		inline float getOffset() const;

		/**
		*  @brief
		*    Set playback offset
		*/
		void setOffset(float offset);

		/**
		*  @brief
		*    Get playback time position
		*/
		inline Time getTimePosition() const;

		/**
		*  @brief
		*    Set playback time position
		*/
		void setTimePosition(Time position);

		/**
		*  @brief
		*    Get remaining wait time
		*/
		inline Time getWaitTime() const;

		/**
		*  @brief
		*    Set wait time before the animation starts or continues (i.e. you can use this to temporarily pause the animation)
		*/
		void setWaitTime(Time waitTime);

		/**
		*  @brief
		*    Get animation playback speed; default is 1.0f
		*/
		inline float getSpeed() const;

		/**
		*  @brief
		*    Set animation playback speed; default is 1.0f, should not be < 0.0f
		*/
		void setSpeed(float speed);

		/**
		*  @brief
		*    Get animation weight
		*/
		inline float getWeight() const;

		/**
		*  @brief
		*    Set animation weight (for blending with another animation) and weight change rate per second
		*/
		void setWeight(float weight);

		/**
		*  @brief
		*    Get animation weight after delta time
		*/
		inline float getWeightValue() const;

		/**
		*  @brief
		*    Set animation weight (for blending with another animation) after delta time
		*/
		inline void setWeightValue(float weightValue);

		/**
		*  @brief
		*    Get animation weight delta
		*/
		inline float getWeightDelta() const;

		/**
		*  @brief
		*    Set animation weight delta
		*/
		inline void setWeightDelta(float weightDelta);

		/**
		*  @brief
		*    Check whether or not the animation is finished
		*
		*  @remarks
		*    A looped animation is never finished
		*/
		inline bool isFinished() const;

		/**
		*  @brief
		*    Set animation weight (for blending with another animation) and weight change rate per second
		*
		*  @param[in] ogreEntity
		*    OGRE entity where to remove the animation channel from
		*/
		void removeFromOgreEntity(Ogre::v1::Entity& ogreEntity);

		/**
		*  @brief
		*    Update the channel, including application of weight delta
		*/
		bool updateChannel(float secondsPassed);

		/**
		*  @brief
		*    Return a debug string with information about the channel's current state
		*/
		std::string getDebugString() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Return if we have a bone mask or not
		*/
		inline bool hasBoneMask() const;

		/**
		*  @brief
		*    Return the mask of this bone
		*
		*  @param[in] boneHandle
		*    OGRE handle of the bone we want to mask with all its child bones
		*/
		float getBoneMask(size_t boneHandle) const;

		/**
		*  @brief
		*    Mask this bone and every other bone away from this animation
		*
		*  @param[in] boneHandle
		*    OGRE handle of the bone we want to mask
		*  @param[in] boneWeight
		*    Weight between 0.0 and 1.0 for bone mask
		*/
		void setBoneMask(size_t boneHandle, float boneWeight = 1.0f);

		/**
		*  @brief
		*    Create a mask for all bone from this animation
		*
		*  @param[in] numberOfBones
		*    Number of bones the skeleton has
		*  @param[in] boneWeight
		*    Weight between 0.0 and 1.0 for bone mask
		*/
		void resetBoneMask(size_t numberOfBones, float boneWeight = 0.0f);

		/**
		*  @brief
		*    Destroy bone mask
		*/
		void destroyBoneMask();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			LOOP_ANIMATION,		///< Set if the animation is be looped
			REVERSE_ANIMATION,	///< Set if the animation should reverse playback
			ANIMATION_FINISHED	///< Set if the animation is finished playing
		};
		typedef EnumBitmask<uint8, InternalFlags> InternalFlagSet;
		typedef std::vector<float> BoneBlendMask;	///< Typedef for an array of float values used as a bone blend mask


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetProxy								mSkeleton;						///< Skeleton asset to use as animation source
		MeshAnimationComponent::BoneRetargeting	mBoneTranslationRetargeting;	///< Used bone translation retargeting setting
		BoneBlendMask*							mBoneBlendMask;					///< The blend mask (containing per bone weights)
		Time									mLength;						///< Animation length
		Time									mWaitTime;						///< Wait time before playback starts
		float									mOffset;						///< Current animation playback offset (normalized position) between 0.0 and 1.0
		float									mSpeed;							///< Animation playback speed, 1.0 is normal speed
		float									mWeight;						///< Animation weight (influence) between 0.0 and 1.0
		float									mWeightValue;					///< Animation weightValue (influence) between 0.0 and 1.0 after WeightDelta time
		float									mWeightDelta;					///< Weight change per second
		InternalFlagSet							mInternalFlags;					///< Set of internal flags, see enum "InternalFlags"
		// Temporary (can be thrown way and reconstructed just using the data from above)
		Ogre::v1::AnimationState* mOgreAnimationState;	///< OGRE animation state instance, can be a null pointer
		bool					  mOgreAnimationStateCreationFailed;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/MeshAnimationChannel-inl.h"
