// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/MeshAnimationComponent.h"
#include "qsf/debug/DebugDrawProxy.h"
#include "qsf/math/EnumBitmask.h"

#include <glm/glm.hpp>

#include <boost/optional.hpp>


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
	*    Mesh animation test component class
	*
	*  @note
	*    - Extends the mesh animation component with properties to allow for easy animation testing inside the editor
	*/
	class QSF_API_EXPORT MeshAnimationTestComponent : public MeshAnimationComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::MeshAnimationTestComponent" unique component ID
		static const uint32 ACTIVE;				///< "Active" unique class property ID inside the class
		static const uint32 SKELETON_A;			///< "Skeleton_A" unique class property ID inside the class
		static const uint32 SKELETON_B;			///< "Skeleton_B" unique class property ID inside the class
		static const uint32 LOOP;				///< "Loop" unique class property ID inside the class
		static const uint32 REVERSE;			///< "Playback" unique class property ID inside the class
		static const uint32 SPEED;				///< "Speed" unique class property ID inside the class
		static const uint32 USE_RANDOM_OFFSET;	///< "UseRandomOffset" unique class property ID inside the class


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
		inline explicit MeshAnimationTestComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeshAnimationTestComponent();

		/**
		*  @brief
		*    Return the skeleton asset to use as animation source
		*
		*  @return
		*    The skeleton asset to use (e.g. "sample/skeleton/character/female_angry_01") as animation source, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getSkeletonA() const;

		/**
		*  @brief
		*    Set the skeleton asset to use as animation source
		*
		*  @param[in] skeleton
		*    The skeleton asset to use (e.g. "sample/skeleton/character/female_angry_01") as animation source
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSkeletonA(const AssetProxy& skeleton);

		/**
		*  @brief
		*    Return the skeleton asset to use as second animation source
		*
		*  @return
		*    The skeleton asset to use (e.g. "sample/skeleton/character/female_angry_01") as animation source, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getSkeletonB() const;

		/**
		*  @brief
		*    Set the skeleton asset to use as second animation source
		*
		*  @param[in] skeleton
		*    The skeleton asset to use (e.g. "sample/skeleton/character/female_angry_01") as animation source
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSkeletonB(const AssetProxy& skeleton);

		/**
		*  @brief
		*    Return whether or not to loop the animation playback
		*
		*  @return
		*    "true" if the animation playback is looped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getLoop() const;

		/**
		*  @brief
		*    Set whether or not to loop the animation playback
		*
		*  @param[in] loop
		*    "true" if the animation playback should be looped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLoop(bool loop);

		/**
		*  @brief
		*    Return the direction of the animation
		*
		*  @return
		*    false is normal playback; true means reverse playback
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getReverseAnimation();

		/**
		*  @brief
		*    Set animation direction of playback
		*
		*  @param[in] animation
		*   true to reverse the animation; false to normal playback
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setReverseAnimation(bool animation);

		/**
		*  @brief
		*    Return the animation playback speed
		*
		*  @return
		*    Animation playback speed, 1 means normal, <1 slower, 1> faster
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getSpeed() const;

		/**
		*  @brief
		*    Set animation playback speed
		*
		*  @param[in] speed
		*    Animation playback speed, 1 means normal, <1 slower, 1> faster
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSpeed(float speed);

		/**
		*  @brief
		*    Return the bone translation retargeting mode
		*
		*  @return
		*    The bone translation retargeting mode
		*/
		inline BoneRetargeting getBoneTranslationRetargeting() const;

		/**
		*  @brief
		*    Set the bone translation retargeting mode
		*
		*  @param[in] boneRetargeting
		*    The bone translation retargeting mode
		*/
		inline void setBoneTranslationRetargeting(BoneRetargeting boneRetargeting);

		inline float getPrologLength() const;
		inline void setPrologLength(float prologLength);

		inline float getBlendLength() const;
		inline void setBlendLength(float blendLength);

		inline bool getMovementEnabled() const;
		inline void setMovementEnabled(bool enable);

		inline float getMovementA() const;
		inline void setMovementA(float movement);

		inline float getMovementB() const;
		inline void setMovementB(float movement);

		inline bool getRestart() const;
		inline void setRestart(bool);

		inline BlendSynchronization getBlendSyncMode() const;
		inline void setBlendSyncMode(BlendSynchronization blendSyncMode);

		inline bool getMixAnimation() const;
		void setMixAnimation(bool mixAnimation);

		inline const std::string& getMaskBones() const;
		void setMaskBones(const std::string& maskBones);

		inline float getCurrentTime() const;
		inline void setCurrentTime(float currentTime);

		inline bool getUseRandomOffset() const;
		inline void setUseRandomOffset(bool useRandomOffset);

		inline void forceRestart(const qsf::Time& blendTime = qsf::Time::ZERO);

		inline bool isTestComponentUpdatesEnabled() const;
		void setTestComponentUpdatesEnabled(bool testComponentUpdatesEnabled);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			LOOP_ANIMATION,		///< Set if the animation is be looped
			REVERSE_ANIMATION,	///< Set if the animation should reverse playback
			MOVEMENT_ENABLED,	///< Set if there should be movement enabled for this animation
		};
		typedef EnumBitmask<uint8, InternalFlags> InternalFlagSet;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const JobArguments& jobArguments);
		void restartAnimation(const qsf::Time& blendTime = qsf::Time::ZERO);
		void backupPosition();
		void restorePosition();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AssetProxy			 mSkeleton[2];		///< Skeleton assets to use as first and second animation source
		float				 mSpeed;			///< Animation playback speed, 1 means normal, <1 slower, 1> faster
		float				 mPrologLength;		///< Prolog length in seconds; this is the minimum time only the first animation is played before blending
		float				 mBlendLength;		///< Blend length in seconds
		float				 mMovement[2];		///< Movement speeds in units per second
		InternalFlagSet		 mInternalFlags;
		BlendSynchronization mBlendSyncMode;
		BoneRetargeting		 mBoneTranslationRetargeting;
		bool				 mUseRandomOffset;

		// Internal only
		bool					   mTestComponentUpdatesEnabled;
		JobProxy				   mUpdateJobProxy;	///< Job proxy
		DebugDrawProxy			   mDebugDrawProxy;	///< Debug draw proxy for text output
		float					   mCurrentTime;
		bool					   mBlendingStarted;
		bool					   mNeedsRestart;
		boost::optional<glm::vec3> mOriginalPosition;
		bool					   mMixAnimation;
		std::string				   mMaskBones;


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
#include "qsf/renderer/animation/MeshAnimationTestComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MeshAnimationTestComponent)
