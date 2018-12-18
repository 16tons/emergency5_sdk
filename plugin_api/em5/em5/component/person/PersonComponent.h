// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/component/person/PersonComponent.h>

#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>
#include <qsf/math/EnumBitmask.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class PersonAnimationController;
	class GangsterBaseLogic;
}


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
	*    EMERGENCY 5 person component class
	*
	*  @note
	*    - Reference to a container (vehicle or building) is saved in the movable component, not here
	*/
	class EM5_API_EXPORT PersonComponent : public qsf::game::PersonComponent, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PersonInWindowObserver;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::PersonComponent" unique component ID

		enum Age
		{
			AGE_UNDEFINED = 0,
			AGE_CHILD,		///< Person is a child
			AGE_YOUTH,		///< Person is an adolescent
			AGE_ADULT,		///< Person is an adult
			AGE_SENIOR		///< Person is a retiree or old person
		};

		enum PersonFlags
		{
			FLAG_DRUNK,					///< Person is drunk
			FLAG_MARKED_AS_GANGSTER,	///< Marked with the gangster icon
			FLAG_STUNNED,				///< Person got stunned with pepper spray
			FLAG_HANDCUFFED,			///< Person is already handcuffed
			FLAG_INVOLVED_IN_FIGHT,		///< Person is currently involved in a fight
			FLAG_LIE,					///< Person does currently lie on the ground
			FLAG_NEW_SKELETON,			///< Person has the new skeleton
			FLAG_WAVES_OUT_OF_HOUSE,	///< Person waves out of burning house, i.e. is standing on roof or balcony
			FLAG_CAN_USE_UMBRELLA,		///< Person can use umbrella
			FLAG_IGNORE_MEGAPHONE		///< Person cannot be influenced by megaphone
		};
		typedef qsf::EnumBitmask<uint16, PersonFlags> PersonFlagSet;

		enum BoundingBoxType
		{
			BOX_UNKNOWN,	// Bounding box has not be changed yet by this action, but can have any size from previous actions
			BOX_LYING,		// Bounding box for injured lying on ground
			BOX_KNEEING,	// Bounding box for injured kneeing
			BOX_DROWNING,	// Bounding box for drowning in water
			BOX_STANDING	// Bounding box for normal and injuries while standing (contamination)
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
		explicit PersonComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PersonComponent();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Age, Age, mAge);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HomeBuildingId, uint64, mHomeBuildingId);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BodyMass, float, mBodyMass);

		QSF_DEFINE_FLAG_GETTER_AND_SETTER(WavesOutOfBurningHouse, mPersonFlags, FLAG_WAVES_OUT_OF_HOUSE);
		QSF_DEFINE_FLAG_GETTER_AND_SETTER(CanUseUmbrella, mPersonFlags, FLAG_CAN_USE_UMBRELLA);
		QSF_DEFINE_FLAG_GETTER_AND_SETTER(IgnoreMegaphone, mPersonFlags, FLAG_IGNORE_MEGAPHONE);

		/**
		*  @brief
		*    Set a persons state to stunned; pushes an "BeStunnedAction" and init with the parameter time stunned
		*    and the ID of the guy who is responsible for the action (e.g. the SEK unit who threw the stun grenade)
		*/
		void setStunned(uint64 initiatorId, const qsf::Time& timeStunned, bool stunnedFromPepper, bool stunnedFromDogAttack = false);

		/**
		*  @brief
		*    Quick getter for the person flags
		*/
		inline bool getPersonFlags(PersonComponent::PersonFlags flag) const;

		/**
		*  @brief
		*    Quick setter for the person flags
		*/
		inline void setPersonFlags(PersonComponent::PersonFlags flag, bool value);

		/**
		*  @brief
		*    Quick getter in order to expose the gangster flag to CAMP
		*/
		inline bool getGangsterFlag() const;

		/**
		*  @brief
		*    Quick setter in order to expose the gangster flag to CAMP
		*/
		void setGangsterFlag(bool value);

		/**
		*  @brief
		*    Quick getter for the handcuffed flag
		*/
		inline bool getHandcuffedFlag() const;

		/**
		*  @brief
		*    Prevent the person animation controller from running
		*
		*  @remarks
		*    This is internally a counter. Animations run only when _no one_ prevents animations.
		*    So be sure to give this free again when blocking is no longer necessary.
		*/
		void preventUpdateAnimations(bool prevent);

		/**
		*  @brief
		*    Reset the person animation controller, so it can choose again what kind of animation should be played; use this to interrupt animations
		*/
		void clearAnimation();

		/**
		*  @brief
		*    Helper method to replace the current gangster type
		*
		*  @return
		*    The new gangster logic instance, or a null pointer in case of an error
		*/
		GangsterBaseLogic* changeGangsterType(const qsf::StringHash& newGangsterTypeId);

		/**
		*  @brief
		*    Helper method to replace the current gangster game logic
		*
		*  @return
		*    The new gangster logic instance, or a null pointer in case of an error
		*
		*  @note
		*    - This method is deprecated, better use "changeGangsterType" instead
		*    - The gangster logic must have the word "Gangster" in the class name
		*/
		GangsterBaseLogic* changeGangsterLogic(const qsf::StringHash& newLogicId);

		/**
		*  @brief
		*    Return the person type as string "male", "female", "child", "senior"
		*/
		const std::string& getPersonTypeAsString(bool uppercase = false) const;

		void giveRandomName();
		void giveRandomNameHistoric();

		/**
		*  @brief
		*    An ugly work around for an missing deboard animation from the paramedic
		*
		*  @note TODO(mk) Try to remove this function and fix the problem with the missing deboard animation for the paramedic
		*/
		void forceIdleAnimationUpdate();

		void forceAnimationUpdate();

		/**
		*  @brief
		*    Change the ogre boundingbox. Defaults are usually fine and no change is necessary.
		*    Exception is when a drowning persons is moved to land, then it has to change it's
		*    bounding box.
		*/
		BoundingBoxType getBoundingBoxType() const;
		void setBoundingBoxType(BoundingBoxType type);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateAnimation(const qsf::JobArguments& jobArguments);

		void updateShaderHeatValue();

		GangsterBaseLogic* changeGangsterLogicInternal(uint32 newGangsterLogicId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		Age				mAge;				///< Person's age, see "Age" enum
		PersonFlagSet	mPersonFlags;		///< Flags of this person
		uint64			mHomeBuildingId;	///< ID of the person's home building; only for map residents, otherwise this is "getUninitialized<uint64>()"
		float			mBodyMass;			///< Person's body mass in kilograms (needed for physics, especially explosions)

		// Internal
		qsf::JobProxy				mAnimatingJobProxy;
		PersonAnimationController*	mPersonAnimationController;	///< Person animation controller, valid while the component is running, destroy the instance if you no longer need it
		int							mPreventUpdateAnimations;	///< No automatic animations when > 0

		BoundingBoxType				mBoundingBoxType;
		qsf::WeakPtr<qsf::Entity>	mCollisionEntity;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/person/PersonComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonComponent::Age)
