// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/math/EnumBitmask.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Play animation component class
		*
		*  @remarks
		*    Use this component to edit animations for entities to be played at startup.
		*    Playback can be single or looped, and can be stopped by any game logic setting another animation.
		*    This component will then not automatically switch back to the edited animation.
		*/
		class QSF_GAME_API_EXPORT PlayAnimationComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::PlayAnimationComponent" unique component ID
			static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
			static const uint32 SKELETON;		///< "Skeleton" unique class property ID inside the class
			static const uint32 LOOP;			///< "Loop" unique class property ID inside the class
			static const uint32 REVERSE;		///< "Playback" unique class property ID inside the class
			static const uint32 SPEED;			///< "Speed" unique class property ID inside the class


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
			inline explicit PlayAnimationComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~PlayAnimationComponent();

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
			inline const AssetProxy& getSkeleton() const;

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
			void setSkeleton(const AssetProxy& skeleton);

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
			inline MeshAnimationComponent::BoneRetargeting getBoneTranslationRetargeting() const;

			/**
			*  @brief
			*    Set the bone translation retargeting mode
			*
			*  @param[in] boneRetargeting
			*    The bone translation retargeting mode
			*/
			inline void setBoneTranslationRetargeting(MeshAnimationComponent::BoneRetargeting boneRetargeting);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSetActive(bool active) override;

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup() override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			enum InternalFlags
			{
				LOOP_ANIMATION,		///< Set if the animation is be looped
				REVERSE_ANIMATION	///< Set if the animation should reverse playback
			};
			typedef EnumBitmask<uint8, InternalFlags> InternalFlagSet;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateJob(const JobArguments& jobArguments);
			void restartAnimation();
			void stopAllAnimations();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			AssetProxy		mSkeleton;		///< Skeleton assets to use as animation source
			float			mSpeed;			///< Animation playback speed, 1 means normal, <1 slower, 1> faster
			InternalFlagSet	mInternalFlags;
			MeshAnimationComponent::BoneRetargeting mBoneTranslationRetargeting;

			// Internal
			JobProxy		mUpdateJobProxy;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/base/PlayAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::PlayAnimationComponent)
