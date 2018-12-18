// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/wrapper/actor/EActor.h"
#include "qsf_ai/navigation/em4Router/em2012/terrain.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/component/move/MovableComponent.h>

#include <glm/glm.hpp>


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
		// constants for object-flags. Keep congruent with EPrototypeFlag in prototype.h
		enum EObjectFlag
		{
			EOF_NONE					= 0x00000000,
			EOF_PERSON_ENCLOSED			= 0x00000001,	// object has enclosed person
			EOF_LOCKED					= 0x00000002,	// object is locked
			EOF_USABLE					= 0x00000004,	// object can be used (special action)
			EOF_BULLDOZABLE				= 0x00000008,	// can be planated by the bulldozer (wheel loader)
			EOF_TRANSPORTABLE			= 0x00000010,	// object can be loaded up (by vehicle)
			EOF_PULLABLE				= 0x00000020,	// object can be pulled (i.e. a tree trunk by a firefighter)
			EOF_ACCESSIBLE				= 0x00000040,	// object can be entered
			EOF_COOLABLE				= 0x00000080,   // object can be cooled by firefighters
			EOF_SHOOTABLE				= 0x00000100,	// object can be shot
			EOF_CUTABLE					= 0x00000200,	// object can be cut down with chainsaw
			EOF_USABLE_WITH_MEGAPHONE	= 0x00000400,	// megaphone can be used on object
			EOF_RECOVERABLE				= 0x00000800,	// object can be recovered by recovery crane
			EOF_FLOTSAM					= 0x00001000,	// object can be picked up by motor boat
			EOF_HIDDEN					= 0x00002000,	// object is invisible
			EOF_CARRYABLE_BY_BULLDOZER  = 0x00008000,	// can be carried by the bulldozer
			EOF_HAS_FIRE_EXTINGUISHER	= 0x00010000,	// object carries one/many fire extinguishers
			EOF_HAS_SHEARS				= 0x00020000,	// object carries one/many jaws of life
			EOF_HAS_CHAINSAW			= 0x00040000,	// object carries one/many chainsaws
			EOF_HAS_HOSE				= 0x00080000,	// object carries one/many fire hoses
			EOF_HAS_JUMPPAD				= 0x00100000,	// object carries one/many jumppads
			EOF_HAS_ROADBLOCK			= 0x00200000,	// object carries one/many roadblocks
			EOF_HAS_FLASHGRENADE		= 0x00400000,	// object carries one/many flashgrenades
			EOF_HAS_FIREAXE				= 0x00800000,	// object carries one/many fireaxes
			EOF_EMERGENCY				= 0x01000000,	// object is in a emergency situation (can be rescued by Helicopters etc...)

			EOF_ININACCESSIBLEAREA		= 0x40000000,	// no interaction with object possible, only by helicopters
			EOF_BLOCKED					= 0x80000000	// no interaction with object possible
		};

		enum EFloorPlacement
		{
			EFP_NONE,				// frei dreh- und positionierbar
			EFP_AXISALIGNED,		// Objekt wird nicht gedreht, Pivotpunkt mit Offset entscheidet die Höhe
			EFP_BOXALIGNED,			// Objekt frei drehbar, Boundingbox-Auflagepunkte mit Offset entscheiden über Höhe
			EFP_ALIGNED_CORNERS,	// Ecken der Boundingbox entscheiden über Höhe und Ausrichtung, unterste Seite der bbop zählt
			EFP_ALIGNED_SAMPLED,	// genauer als boundingcorners, zeitaufwendiger
			EFP_PHYSICS,			// physik benutzen TODO
			EFP_CUSTOM_PLACEMENT,	// placement wird von abgeleiteter Klasse berechnet (e.g. Person, Vehicle)
			EFP_ALIGNED_CORNERS_FLOOR,	// Ecken der Boundingbox entscheiden über Höhe und Ausrichtung, unterste Seite der bbop zählt; benutzt floor height
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EGenericObject wrapper class for a QSF entity
		*/
		class EGenericObject : public EActor
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			virtual const glm::vec3& GetPosition() = 0;
			virtual const glm::mat3x3& GetRotationMatrix() = 0;
			virtual EObjectTerrainClass GetTerrainClass() = 0;
			virtual EFloorPlacement GetPlacement() = 0;
			virtual void UpdatePlacement(glm::vec3& position_, glm::mat3x3& rotation_) = 0;

			// Bounding
			virtual float GetBoundingRadius() { return mRadius; }
			virtual float GetBoundingRadiusXZ() const { return mRadiusXZ; }
			virtual float GetBoundingRadiusHeight() const { return mRadiusHeight; }
			virtual glm::vec3 GetBBMidPoint() const { return mMidPoint; }

			// Movement
			virtual bool IsMoving() const { return mMovableComponent.valid() ? mMovableComponent->isMoving() : false; }

			// Type related
			inline int GetFlags() const { return mFlags; }
			inline bool IsFlagSet(int flag_) const { return (mFlags&flag_) != 0; }
			inline bool IsHydrant() const { return false; }	// TODO(co) Implement
			// inline bool IsJumppad() const { return false; }	// TODO(ae) not needed for em5
			inline bool hasFireChildren() const { return false; }	// TODO(co) Implement
			virtual int GetCarriedPersonID() const = 0;		// TODO(co) I assume the result is an generic object ID and -1 means not initialized

			void setMovableComponent(MovableComponent* movableComponent) { mMovableComponent = movableComponent; } // No problem setting a nullptr here


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			EGenericObject(EActorType actorType) :
				EActor(actorType),
				mFlags(0)
			{}


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			int	 mFlags;

			WeakPtr<MovableComponent> mMovableComponent; // may be a nullptr


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
