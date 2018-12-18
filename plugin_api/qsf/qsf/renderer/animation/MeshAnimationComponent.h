// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener.h"
#include "qsf/base/NamedIdentifier.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
		class OldBone;
		class Skeleton;
		class Animation;
		class AnimationState;
		class OldSkeletonManager;
	}
}
namespace qsf
{
	class MeshAnimationChannel;
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
	*    Mesh animation component class
	*
	*  @note
	*    - Not derived from "qsf::RendererComponent" by intent because it's a logic update and nothing directly visible on the screen
	*
	*  @todo
	*    - TODO(fw): Add more getters like get remaining / absolute play length or access the current animation
	*    - TODO(fw): A method enqueueBlendToAnimation would be nice, to support several animations or blends one after another
	*/
	class QSF_API_EXPORT MeshAnimationComponent : public Component, public JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MeshComponent;	// TODO(co) "qsf::MeshAnimationComponent" holds raw pointers to OGRE entity and animation state instances. Until we find another lightweigth solution this must do the trick.


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::MeshAnimationComponent" unique component ID

		typedef boost::container::flat_map<uint32, MeshAnimationChannel*> ChannelMap;

		enum BlendSynchronization
		{
			SYNC_NONE,		///< Don't sync blended animation with already running animation
			SYNC_AT_START,	///< Start blended animation in sync with the already running animation's next loop start
			SYNC_AT_END,	///< Start blended animation so that blending ends at the already running animation's next loop start
			SYNC_PARALLEL,	///< Blend immediately, but copy the playback offset from the already running animation
		};

		/**
		*  @brief
		*    Bone retargeting
		*/
		enum BoneRetargeting
		{
			ANIMATION_BONE_RETARGETING,			///< Use the original bone translation from the animation skeleton keyframes
			ANIMATION_BONE_SCALE_RETARGETING	///< Scale the bone translation from the animation skeleton keyframes so it matches the target mesh skeleton without distorting it
		};

		enum AnimationType
		{
			ANIMATION_TYPE_NONE,			///< No animation where started
			ANIMATION_TYPE_STOP_ALL,		///< All running animations were explicit stopped
			ANIMATION_TYPE_PLAY_SINGLE,		///< Animation started via playAnimation
			ANIMATION_TYPE_PLAY_MASK,		///< Animation started via playMaskedAnimation
			ANIMATION_TYPE_BLEND_SINGLE,	///< Animation started via blendToAnimation
			ANIMATION_TYPE_BLEND_MASK,		///< Animation started via blendToMaskedAnimation
		};

		// Contains the data (the parameter values) of the last activated animation
		struct LastAnimatonData
		{
			AnimationType				mAnimationType;
			LocalAssetId				mLocalAnimationAssetId1;	// We're intentionally using the local asset ID instead of the global one as it's shorter (relevant for multiplay)
			LocalAssetId				mLocalAnimationAssetId2;
			bool						mLoop;
			bool						mReverse1;
			bool						mReverse2;
			BoneRetargeting				mBoneRetargeting1;
			BoneRetargeting				mBoneRetargeting2;
			std::vector<std::string>	mBoneMask;
			Time						mBlendTime;
			BlendSynchronization		mBlendSync;

			LastAnimatonData() :
				mAnimationType(ANIMATION_TYPE_NONE),
				mLocalAnimationAssetId1(0),
				mLocalAnimationAssetId2(0),
				mLoop(false),
				mReverse1(false),
				mReverse2(false),
				mBoneRetargeting1(ANIMATION_BONE_RETARGETING),
				mBoneRetargeting2(ANIMATION_BONE_RETARGETING),
				mBlendTime(Time::ZERO),
				mBlendSync(SYNC_NONE)
			{
			}
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void preloadSkeletonAnimationResource(GlobalAssetId globalAssetId);


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
		inline explicit MeshAnimationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeshAnimationComponent();

		/**
		*  @brief
		*    Stop all animations currently playing
		*/
		void stopAllAnimations();

		/**
		*  @brief
		*    Play a new animation, replacing the existing one
		*
		*  @param[in] animationAsset
		*    The skeleton asset to use as animation
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting
		*    Bone translation retargeting to use
		*
		*  @return
		*    The newly created mesh animation channel; may be a null pointer, only temporarily use this instance pointer and do not store it (WeakPtr is okay)
		*/
		inline MeshAnimationChannel* playAnimation(const AssetProxy& animationAsset, bool loop, bool reverse = false, BoneRetargeting boneTranslationRetargeting = ANIMATION_BONE_SCALE_RETARGETING);

		/**
		*  @brief
		*    Play 2 new masked animation, replacing the existing one
		*
		*  @param[in] animationAsset1
		*    The skeleton asset to use as animation 1 (mask defines bones we use)
		*  @param[in] animationAsset2
		*    The skeleton asset to use as animation 2 (mask defines bones we don't use)
		*  @param[in] boneMask
		*    List of bone names we want to mask with all child bones (no holes of empty strings, please)
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse1
		*    Set to "true" for a reversed animation 1 playback, "false" otherwise
		*  @param[in] reverse2
		*    Set to "true" for a reversed animation 2 playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting1
		*    Bone translation retargeting to use for animation 1
		*  @param[in] boneTranslationRetargeting2
		*    Bone translation retargeting to use for animation 2
		*
		*  @return
		*    The newly created mesh animation channel of animation 1; may be a null pointer, only temporarily use this instance pointer and do not store it (WeakPtr is okay)
		*/
		inline std::pair<MeshAnimationChannel*, MeshAnimationChannel*> playMaskedAnimation(
			const AssetProxy& animationAsset1,
			const AssetProxy& animationAsset2,
			const std::vector<std::string>& boneMask,
			bool loop,
			bool reverse1 = false,
			bool reverse2 = false,
			BoneRetargeting boneTranslationRetargeting1 = ANIMATION_BONE_SCALE_RETARGETING,
			BoneRetargeting boneTranslationRetargeting2 = ANIMATION_BONE_SCALE_RETARGETING);

		/**
		*  @brief
		*    Play a new animation, blending from the existing one (if there is one)
		*
		*  @param[in] animationAsset
		*    The skeleton asset to use as animation
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] blendLength
		*    The blend time duration
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] blendSync
		*    Blend synchronization mode to use
		*  @param[in] boneTranslationRetargeting
		*    Bone translation retargeting to use
		*
		*  @return
		*    The newly created mesh animation channels; may be a null pointers, only temporarily use this instance pointers and do not store them (WeakPtr is okay)
		*/
		inline MeshAnimationChannel* blendToAnimation(
			const AssetProxy& animationAsset,
			bool loop,
			Time blendLength,
			bool reverse = false,
			BlendSynchronization blendSync = SYNC_NONE,
			BoneRetargeting boneTranslationRetargeting = ANIMATION_BONE_SCALE_RETARGETING);

		/**
		*  @brief
		*    Play a new animation, blending from the existing one (if there is one)
		*
		*  @param[in] animationAsset1
		*    The skeleton asset to use as animation 1 (mask defines bones we use)
		*  @param[in] animationAsset2
		*    The skeleton asset to use as animation 2 (mask defines bones we don't use)
		*  @param[in] boneMask
		*    List of bone names we want to mask with all child bones (no holes of empty strings, please)
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] blendLength
		*    The blend time duration
		*  @param[in] reverse1
		*    Set to "true" for a reversed animation 1 playback, "false" otherwise
		*  @param[in] reverse2
		*    Set to "true" for a reversed animation 2 playback, "false" otherwise
		*  @param[in] blendSync
		*    Blend synchronization mode between the 2 animation
		*  @param[in] boneTranslationRetargeting1
		*    Bone translation retargeting to use
		*  @param[in] boneTranslationRetargeting2
		*    Bone translation retargeting to use
		*
		*  @return
		*    The newly created mesh animation channels; may be a null pointers, only temporarily use this instance pointers and do not store them (WeakPtr is okay)
		*/
		inline std::pair<MeshAnimationChannel*, MeshAnimationChannel*> blendToMaskedAnimation(
			const AssetProxy& animationAsset1,
			const AssetProxy& animationAsset2,
			const std::vector<std::string>& boneMask,
			bool loop,
			Time blendLength,
			bool reverse1 = false,
			bool reverse2 = false,
			BlendSynchronization blendSync = SYNC_NONE,
			BoneRetargeting boneTranslationRetargeting1 = ANIMATION_BONE_SCALE_RETARGETING,
			BoneRetargeting boneTranslationRetargeting2 = ANIMATION_BONE_SCALE_RETARGETING);

		/**
		*  @brief
		*    Return the current playback state
		*
		*  @return
		*    "true" if any animation is playing or enqueued at the moment, "false" otherwise
		*/
		bool isPlaying() const;

		/**
		*  @brief
		*    Check whether the given animation is current playing
		*
		*  @param[in] animationAsset
		*    The animation skeleton asset
		*
		*  @return
		*    "true" if the animation is playing or enqueued at the moment, "false" otherwise
		*/
		bool isAnimationPlaying(const AssetProxy& animationAsset) const;

		inline const ChannelMap& getChannels() const;

		/**
		*  @brief
		*    Next time an animation is played the binding will be updated
		*/
		inline void resetAnimationBindingNextTime();

		/**
		*  @brief
		*    Return whether or not the animation is always updated, regardless of whether or not it's visible
		*/
		inline bool isDoAlwaysUpdateAnimation() const;

		/**
		*  @brief
		*    Return whether or not the animation is always updated, regardless of whether or not it's visible
		*
		*  @note
		*    - Don't set this to true until you're not in need of decent performance
		*/
		void setDoAlwaysUpdateAnimation(bool doAlwaysUpdateAnimation);

		/**
		*  @brief
		*    Set if we want to manually control a bone
		*
		*  @param[in] boneName
		*    Name of the bone we want to have full control
		*  @param[in] manualControl
		*    If we want to manually control the bone or not
		*  @param[in] channelForMaskedBone
		*    The channel we want to set the mask to 1 when we set the bone to not be manually controlled
		*
		*  @return
		*    Pointer to the bone, don't destroy it
		*/
		Ogre::v1::OldBone* manualBoneControl(const std::string& boneName, bool manualControl, MeshAnimationChannel* channelForMaskedBone = nullptr) const;

		/**
		*  @brief
		*    Check if a bone with bone name exists
		*
		*  @param[in] boneName
		*    Name of the bone we want to check
		*
		*  @return
		*    "true" if the skeleton has a bone with our name, else "false"
		*/
		bool isBone(const std::string& boneName) const;

		/**
		*  @brief
		*    Get an animation channel of a given asset if possible
		*
		*  @param[in] animationAsset
		*    The skeleton asset of the animation we want to get
		*
		*  @return
		*    Pointer to the animation channel, can be a null pointer
		*/
		MeshAnimationChannel* getAnimationChannel(const AssetProxy& animationAsset) const;

		/**
		*  @brief
		*    Return the skeleton local asset name of the first channel
		*/
		const std::string& getCurrentAnimation() const;

		/**
		*  @brief
		*    Return the skeleton asset name of the first channel, plus debug information like the current play position
		*/
		std::string getDebugStringFirstChannel() const;

		/**
		*  @brief
		*    Returns the last animation data
		*/
		inline const LastAnimatonData& getLastAnimationData() const;

		/**
		*  @brief
		*    Enforce graphics synchronization
		*
		*  @note
		*    - There are rare situations were one needs the current bone position of an animated skeleton, call this method to ensure the bone is updated
		*    - For performance reasons, do not overuse this method
		*/
		void enforceGraphicsSynchronization();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MeshAnimationComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual const NamedIdentifier& getAnimationJobManagerId() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual void onShutdown() override;
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Return whether or not the animation is considered to be visible
		*/
		bool isVisibleAnimation() const;

		/**
		*  @brief
		*    Play a new animation, replacing the existing one
		*
		*  @param[in] animationAsset
		*    The skeleton asset to use as animation
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting
		*    Bone translation retargeting to use
		*  @param[in] visibleAnimation
		*    Is the animated thing currently visible on screen? (important information for visual glitch avoidance)
		*
		*  @return
		*    The newly created mesh animation channel; may be a null pointer, only temporarily use this instance pointer and do not store it (WeakPtr is okay)
		*/
		MeshAnimationChannel* playAnimationInternal(const AssetProxy& animationAsset, bool loop, bool reverse = false, BoneRetargeting boneTranslationRetargeting = ANIMATION_BONE_SCALE_RETARGETING, bool visibleAnimation = false);

		/**
		*  @brief
		*    Play 2 new masked animation, replacing the existing one
		*
		*  @param[in] animationAsset1
		*    The skeleton asset to use as animation 1 (mask defines bones we use)
		*  @param[in] animationAsset2
		*    The skeleton asset to use as animation 2 (mask defines bones we don't use)
		*  @param[in] boneMask
		*    List of bone names we want to mask with all child bones (no holes of empty strings, please)
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] reverse1
		*    Set to "true" for a reversed animation 1 playback, "false" otherwise
		*  @param[in] reverse2
		*    Set to "true" for a reversed animation 2 playback, "false" otherwise
		*  @param[in] boneTranslationRetargeting1
		*    Bone translation retargeting to use for animation 1
		*  @param[in] boneTranslationRetargeting2
		*    Bone translation retargeting to use for animation 2
		*  @param[in] visibleAnimation
		*    Is the animated thing currently visible on screen? (important information for visual glitch avoidance)
		*
		*  @return
		*    The newly created mesh animation channel of animation 1; may be a null pointer, only temporarily use this instance pointer and do not store it (WeakPtr is okay)
		*/
		std::pair<MeshAnimationChannel*, MeshAnimationChannel*> playMaskedAnimationInternal(
			const AssetProxy& animationAsset1,
			const AssetProxy& animationAsset2,
			const std::vector<std::string>& boneMask,
			bool loop,
			bool reverse1 = false,
			bool reverse2 = false,
			BoneRetargeting boneTranslationRetargeting1 = ANIMATION_BONE_SCALE_RETARGETING,
			BoneRetargeting boneTranslationRetargeting2 = ANIMATION_BONE_SCALE_RETARGETING,
			bool visibleAnimation = false);

		/**
		*  @brief
		*    Play a new animation, blending from the existing one (if there is one)
		*
		*  @param[in] animationAsset
		*    The skeleton asset to use as animation
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] blendLength
		*    The blend time duration
		*  @param[in] reverse
		*    Set to "true" for a reversed animation playback, "false" otherwise
		*  @param[in] blendSync
		*    Blend synchronization mode to use
		*  @param[in] boneTranslationRetargeting
		*    Bone translation retargeting to use
		*  @param[in] visibleAnimation
		*    Is the animated thing currently visible on screen? (important information for visual glitch avoidance)
		*
		*  @return
		*    The newly created mesh animation channels; may be a null pointers, only temporarily use this instance pointers and do not store them (WeakPtr is okay)
		*/
		MeshAnimationChannel* blendToAnimationInternal(
			const AssetProxy& animationAsset,
			bool loop,
			Time blendLength,
			bool reverse = false,
			BlendSynchronization blendSync = SYNC_NONE,
			BoneRetargeting boneTranslationRetargeting = ANIMATION_BONE_SCALE_RETARGETING,
			bool visibleAnimation = false);

		/**
		*  @brief
		*    Play a new animation, blending from the existing one (if there is one)
		*
		*  @param[in] animationAsset1
		*    The skeleton asset to use as animation 1 (mask defines bones we use)
		*  @param[in] animationAsset2
		*    The skeleton asset to use as animation 2 (mask defines bones we don't use)
		*  @param[in] boneMask
		*    List of bone names we want to mask with all child bones (no holes of empty strings, please)
		*  @param[in] loop
		*    Set to "true" for a looped animation, "false" otherwise
		*  @param[in] blendLength
		*    The blend time duration
		*  @param[in] reverse1
		*    Set to "true" for a reversed animation 1 playback, "false" otherwise
		*  @param[in] reverse2
		*    Set to "true" for a reversed animation 2 playback, "false" otherwise
		*  @param[in] blendSync
		*    Blend synchronization mode between the 2 animation
		*  @param[in] boneTranslationRetargeting1
		*    Bone translation retargeting to use
		*  @param[in] boneTranslationRetargeting2
		*    Bone translation retargeting to use
		*  @param[in] visibleAnimation
		*    Is the animated thing currently visible on screen? (important information for visual glitch avoidance)
		*
		*  @return
		*    The newly created mesh animation channels; may be a null pointers, only temporarily use this instance pointers and do not store them (WeakPtr is okay)
		*/
		std::pair<MeshAnimationChannel*, MeshAnimationChannel*> blendToMaskedAnimationInternal(
			const AssetProxy& animationAsset1,
			const AssetProxy& animationAsset2,
			const std::vector<std::string>& boneMask,
			bool loop,
			Time blendLength,
			bool reverse1 = false,
			bool reverse2 = false,
			BlendSynchronization blendSync = SYNC_NONE,
			BoneRetargeting boneTranslationRetargeting1 = ANIMATION_BONE_SCALE_RETARGETING,
			BoneRetargeting boneTranslationRetargeting2 = ANIMATION_BONE_SCALE_RETARGETING,
			bool visibleAnimation = false);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onUpdateEmulation(const JobArguments& jobArguments);
		void onUpdateAnimation(const JobArguments& jobArguments);
		void updateAnimation(float secondsPassed, bool emulated);

		void internalActivate(bool visibleAnimation);
		void internalDeactivate();
		void setJobs(bool visibleAnimation, bool emulation);

		void clearAllChannels();
		MeshAnimationChannel* addChannel(const AssetProxy& animationAsset, BoneRetargeting boneTranslationRetargeting);
		void destroyChannel(MeshAnimationChannel& channel);

		Ogre::v1::Entity* findOgreEntity() const;
		void ogreEntityDestroyed();
		Ogre::v1::AnimationState* getOgreAnimationStateForAnimation(const std::string& animationName, BoneRetargeting boneTranslationRetargeting);

		void synchronizeChannelBlending(MeshAnimationChannel& newChannel, const MeshAnimationChannel& oldChannel, BlendSynchronization blendSync);
		inline void synchronizeOgreAnimationStates();

		void setAnimationState(Ogre::v1::Entity& ogreEntity, const std::string& animationName, BoneRetargeting boneTranslationRetargeting);

		void maskBonesRecursive(Ogre::v1::Skeleton& ogreSkeleton, const Ogre::v1::OldBone& ogreBone, MeshAnimationChannel& meshAnimationChannel, float boneWeight);

		void checkManualBones();

		//[-------------------------------------------------------]
		//[ Animation retargeting                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Take a animation and source skeleton and adds that animation to the target skeleton and returns it
		*
		*  @note
		*    - Based on graphics.stanford.edu/~danielrh/Animation.cpp (public domain, authored by Daniel Horn)
		*/
		Ogre::v1::Animation* retargetAnimation(Ogre::v1::Skeleton& targetOgreSkeleton, const Ogre::v1::Skeleton& sourceOgreSkeleton, const Ogre::v1::Animation& sourceOgreAnimation);

		/**
		*  @brief
		*    Take a string with a skeleton name and an optional string with animation name, and retargets it to the target skeleton
		*
		*  @note
		*    - If name is empty string, it will load the first animation (allowing entire skeleton packages to become "virtual animations"
		*
		*  @see
		*    - "qsf::MeshAnimationComponent::retargetAnimation()"
		*/
		Ogre::v1::Animation* createAnimation(Ogre::v1::Skeleton& targetOgreSkeleton, Ogre::v1::OldSkeletonManager& ogreOldSkeletonManager, const std::string& animationName, const std::string& groupName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ChannelMap			mChannels;						///< Map of channels, local asset ID of the animation as key
		Ogre::v1::Entity*	mOgreEntity;					///< OGRE entity instance, can be a null pointer, just shared, do not destroy it
		uint32				mSkeletonAnimationBlendMode;	///< "Ogre::v1::SkeletonAnimationBlendMode"
		bool				mUseBaseKeyFrame;				///< Ignore the binding pose of animations
		bool				mResetAnimationBinding;			///< Redo animation binding the next time
		bool				mDoAlwaysUpdateAnimation;		///< Do always update the animation, regardless of whether or not it's visible, don't set this to true until you're not in need of decent performance
		JobProxy			mEmulatedAnimationJobProxy;		///< Job proxy for updates while the animation is not visible; gets only a few updates per second
		JobProxy			mAlwaysDoAnimationJobProxy;
		size_t				mMeshChangedStateCount;			///< Holds the count how often the mesh/resource has changed (e.g. due an reload caused by an re-import of the same mesh via the editor)

		LastAnimatonData	mLastAnimationData;				///< Holds the data of the last call to any method which starts/stops an animation


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
#include "qsf/renderer/animation/MeshAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MeshAnimationComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MeshAnimationComponent::BlendSynchronization)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MeshAnimationComponent::BoneRetargeting)
