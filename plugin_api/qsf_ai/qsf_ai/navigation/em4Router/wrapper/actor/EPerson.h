// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/wrapper/actor/EGenericObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ General definitions                                   ]
		//[-------------------------------------------------------]
		// Achtung! Unbedingt gleiche Reihenfolge wie PersonRole aus Scripting/Person
		enum EPersonRole
		{
			EPR_UNKNOWN,
			EPR_CIVILIAN,
			EPR_SQUAD,
			EPR_GANGSTER,
			EPR_DROWNING,
			EPR_ANIMAL,
			EPR_PARAMEDIC	// added to use their real collision box
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EPerson wrapper class for a QSF entity
		*/
		class EPerson : public EGenericObject
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			virtual bool IsArrested() const = 0;
			virtual bool IsDiver() const = 0;
			virtual bool IsDrowning() const = 0;
			virtual bool IsJumping() const = 0;
			virtual bool IsCollisionIgnored(EActor& actor) const = 0;
			virtual bool CanRunFromUnitCar() const = 0;
			virtual int GetIsFollowing() const = 0;		// TODO(co) I assume the result is an generic object ID and -1 means not initialized
			virtual int GetLinkedPersonID() const = 0;	// Actor ID, uininitialized value is -1
			virtual glm::vec3 GetRelLinkPosition() const = 0;	// Realtive position of a linked person
			virtual const glm::vec3& getBoundingBoxCenter() = 0;
			virtual const glm::vec3& getBoundingBoxExtents() = 0;
			virtual EPersonRole GetRole() const = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			EPerson() : EGenericObject(EAT_PERSON) {}


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
