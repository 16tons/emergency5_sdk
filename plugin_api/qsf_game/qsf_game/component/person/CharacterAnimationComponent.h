// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"
#include "qsf_game/characteranimation/CharacterAnimationSet.h"

#include <qsf/component/Component.h>
#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>
#include <qsf/job/JobProxy.h>

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace qsf
{
	class MovableComponent;
	class MeshAnimationComponent;
	class PlayCharacterAnimationComponent;
}


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
		*    Character controller component class
		*
		*  @remarks
		*    This component assigns animations to characters depending on their current movement speed (see "qsf::MovableComponent").
		*    Feed it with animation definitions that setup animations for standing and different move speeds.
		*    Each animation definition holds a minimum speed required for this animation to be considered at all.
		*    From all those, the one with the highest minimum speed gets chosen.
		*
		*  @todo
		*    - TODO(fw): There is currently no support for masked animations
		*/
		class QSF_GAME_API_EXPORT CharacterAnimationComponent : public Component, public JobProxyMovableObjectRenderingListener
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::CharacterAnimationComponent" unique component ID


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
			explicit CharacterAnimationComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CharacterAnimationComponent();

			/**
			*  @brief
			*    Forces an update to the animation. No blending is used
			*/
			void forceUpdate();
			void forceUpdateWithBlending();

			/**
			*  @brief
			*    Return the name of the character animation set asset to use
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			const std::string& getAnimationSetName() const;

			/**
			*  @brief
			*    Set the name of the character animation set asset to use
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			void setAnimationSetNameByName(const std::string& animationSetName);

			/**
			*  @brief
			*    Return the current configuration, i.e. the animations defined
			*
			*  @return
			*    The list of animation definitions
			*/
			const std::vector<CharacterAnimationSet::AnimationData>& getAnimations() const;

			/**
			*  @brief
			*    Clear the list of animation definitions
			*/
			void clearAnimations();

			/**
			*  @brief
			*    Reloads the list of animations from the animation set
			*/
			void reloadAnimationSet();

			/**
			*  @brief
			*    Add a new animation definition
			*
			*  @param[in] skeleton
			*    The skeleton asset to use as animation
			*  @param[in] minimumSpeed
			*    Minimum character move speed for this animation to be used
			*  @param[in] defaultSpeed
			*    Character move speed at which the animation should use normal playback speed
			*  @param[in] blendTime
			*    Time to blend into this animation
			*
			*  @return
			*    The newly created animation definition
			*/
			CharacterAnimationSet::AnimationData& addAnimation(const AssetProxy& skeleton, float minimumSpeed, float defaultSpeed = 0.0f, Time blendTime = Time::fromSeconds(0.4f));

			/**
			*  @brief
			*    Access an animation definition for reading or writing
			*
			*  @return
			*    The animation definition
			*
			*  @note
			*    - Throws an exception in case the index is not valid
			*/
			CharacterAnimationSet::AnimationData& accessAnimation(uint32 index);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::game::CharacterAnimationComponent methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onUpdate(const JobArguments& jobArguments);
			virtual float getCurrentSpeed();
			virtual void onCurrentAnimationDataChanged(const CharacterAnimationSet::AnimationData& newAnimation);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup() override;
			virtual void onSetActive(bool active) override;
			virtual void onSetSimulating(bool simulating) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onObjectRenderingStartup() override;
			virtual void onObjectRenderingShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void loadAnimationSet();
			void updateEnabledState(bool active, bool simulating);
			void updateJob(const JobArguments& jobArguments);

			uint32 calcAnimationIndexToUse();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string											mAnimationSetName;	///< The name of the character animation set asset to use
			std::vector<CharacterAnimationSet::AnimationData>	mAnimations;		///< List of animations

			float  mCharacterMoveSpeed;				///< Current move speed of character
			float  mLastCharacterMoveSpeed;			///< Move speed of character in last update
			uint32 mCurrentAnimationIndex;
			bool	mUseBlending;
			AssetProxy	mCurrentMaskAnimationAsset;
			float		mCurrentAnimationWeight;
			float		mTargetAnimationWeight;

			WeakPtr<PlayCharacterAnimationComponent> mAnimationComponent;
			WeakPtr<MovableComponent>		mMovableComponent;
			JobProxy mJobProxy;
			JobProxy mAnimateAnimationWeightJobProxy;

			WeakPtr<MeshAnimationComponent> mMeshAnimationComponent; // Needed to check if the "old" mesh animation component exists and to destroy it on first update


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
#include "qsf_game/component/person/CharacterAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::CharacterAnimationComponent)
