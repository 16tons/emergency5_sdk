// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>
#include <qsf/math/AxisAlignedBoundingBox.h>
#include <qsf/base/error/BoostAssertHandler.h>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace ai
	{
		class RouterScene;
		class EGenericObject;
		class ETerrain;
	}
}


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
		enum EActorType
		{
			EAT_UNKNOWN				= 0x00000001,
			EAT_FLOOR				= EAT_UNKNOWN,
			EAT_OBJECT				= 0x00000002,
			EAT_HOUSE				= 0x00000004,
			EAT_VEHICLE				= 0x00000008,
			EAT_PERSON				= 0x00000010,
			EAT_LIQUID				= 0x00000020,
			EAT_VIRTUAL				= 0x00000040,
		//	EAT_FIREOBJECT			= 0x00000080,	// TODO(co) Probably not needed in QSF
		//	EAT_PATH				= 0x00000100,	// TODO(co) Probably not needed in QSF
		//	EAT_FORCEFIELD			= 0x00000200,	// TODO(co) Probably not needed in QSF
		//	EAT_FORCEVOLUME			= 0x00000400,	// TODO(co) Probably not needed in QSF
		//	EAT_TRIGGER				= 0x00000800,	// TODO(co) Probably not needed in QSF
		//	EAT_STREET				= 0x00001000,	// TODO(co) Probably not needed in QSF
		//	EAT_OPEN_HOUSE			= 0x00002000,	// TODO(co) Probably not needed in QSF
		//	EAT_SPAWNPOINT			= 0x00004000,	// TODO(co) Probably not needed in QSF
		//	EAT_DETAILPOLYGON		= 0x00008000,	// TODO(co) Probably not needed in QSF
		//	EAT_WAITINGPOINT		= 0x00010000,	// TODO(co) Probably not needed in QSF
		//	EAT_AMBIENTPOLYGON		= 0x00020000,	// TODO(co) Probably not needed in QSF
		//	EAT_BRIDGEINSTALLPOINT	= 0x00040000,	// TODO(co) Probably not needed in QSF
		//	EAT_STOPPINGPOINT		= 0x00080000,	// TODO(co) Probably not needed in QSF
		//	EAT_DECAL				= 0x00100000,	// TODO(co) Probably not needed in QSF
		//	EAT_STARTENTRYPOINT		= 0x00200000,	// TODO(co) Probably not needed in QSF
			EAT_ALL					= 0xFFFFFFFF,

			EATF_GENERICOBJECT		= EAT_OBJECT | EAT_HOUSE | EAT_VEHICLE | EAT_PERSON		// Originally included EAT_OPEN_HOUSE as well
		};

		enum ETargetPoint
		{
			ETP_ANY,					// nicht in Verbindung mit Move oder FindPath verwenden, GetTargetPoint liefert Position vor Objekt, abhängig von der aktuellen Position des Initiators
			ETP_EQUIPMENTDOOR,			// GetTargetPoint liefert Position vor Tür EDAT_EQUIPMENT von Fahrzeugen
			ETP_SHEARSDOOR,				// GetTargetPoint liefert Position vor Tür EDAT_SPECIAL von Fahrzeugen
			ETP_SHEARSDOOR_OFFSET,		// GetTargetPoint liefert Position vor Tür EDAT_SPECIAL von Fahrzeugen mit vergrößertem Abstand
			ETP_PASSENGERDOOR,			// GetTargetPoint liefert Position vor Tür EDAT_PERSON von Fahrzeugen
			ETP_REARDOOR,				// GetTargetPoint liefert Position vor Tür EDAT_SPECIAL von Fahrzeugen
			ETP_FIREHOSE_HOOKUP,		// = ETP_ANY
			ETP_ENGINE,					// GetTargetPoint liefert Position innerhalb eines Fahrzeugs (im vorderen Drittel)
			ETP_EXTINGUISH,				// GetTargetPoint liefert Position in Distanz <TYP>JETLENGTH, Handling übernimmt dennoch RF: Distanzziel mit Distanz <TYP>JETLENGTH
		/* TODO(ae) not needed for em5
			ETP_EXTINGUISH_DLK_M07,		// GetTargetPoint liefert Position in Distanz <TYP>JETLENGTH, Handling übernimmt dennoch RF: Distanzziel mit Distanz <TYP>JETLENGTH
		*/
			ETP_EXTINGUISHHOSE,			// wie ETP_EXTINGUISH nur dass RF EFPF_NOCURVES-Flag verwendet
			ETP_ENGINE_EXTINGUISH,		// GetTargetPoint liefert Position vor einem Fahrzeug (vorderes Drittel des Fahrzeugs), Handling übernimmt RF bei Fahrzeugen: Distanzziel mit Distanz <TYP>JETLENGTH, Personen laugen direkt Position an
			ETP_AXE,					// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz AXELENGTH
			ETP_CHAINSAW,				// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz CHAINSAWLENGTH
			ETP_ENTRANCEDOOR,			// GetTargetPoint liefert Position vor der Eingangstür eines OpenHouse
			ETP_MEGAPHONE_DISTANCE,		// GetTargetPoint liefert Position in Distanz MEGAPHONEDISTANCE, Handling übernimmt dennoch RF: Distanzziel mit Distanz MEGAPHONEDISTANCE
			ETP_DLK_BASKET,				// GetTargetPoint liefert Position des Korbes eines DLK
			ETP_LOADUP,					// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz ASF_LOADDISTANCE
			ETP_ENTRY_WINDOW_PARKING,	// GetTargetPoint liefert Installationsposition für DLK an OpenHouse
			ETP_DLK_BASKET_BASE,		// GetTargetPoint liefert Ein-Aussteigposition vor dem Korb eines DLK
			ETP_TREATMENT,				// GetTargetPoint liefert (Behandlungs-)Position neben Verletztem
			ETP_UNLOAD,					// GetTargetPoint liefert Abladeposition
			ETP_ENTRY_WINDOW,			// GetTargetPoint liefert Position vor dem Fenster eines OpenHouse
			ETP_INSTALL_FGRB,			// GetTargetPoint liefert Position zum Aufbauen der Pontonbrücke
			ETP_SHOOT,					// GetTargetPoint liefert kein definiertes Ergebnis, nur für Aufruf von FindPath verwenden, dann Distanzziel mit zu übergebender Distanz
			ETP_FOLLOW,					// nicht mit GetTargetPoint verwenden, nur bei FindPath für verfolgen
			ETP_DLK_EXTINGUISH,			// nur für Häuser definiert, GetTargetPoint liefert die Position des, dem Initiator am nächsten gelegene, Brandobjekt des Hauses
			ETP_UNLOAD_TFMB,			// nicht mit GetTargetPoint verwenden, nur für FindPath in Verbindung mit LandingStage verwenden
			ETP_FLAME_EFFECT,			// GetTargetPoint liefert Position des am nächsten gelegenen FireObjects
			ETP_FLAME_EFFECT_INACTIVE,	// GetTargetPoint liefert Position des am nächsten gelegenen FireObjects (aktive als auch inaktive)
			ETP_FLAME_EFFECT_INHOUSEFIRE,			// GetTargetPoint liefert Position des am nächsten gelegenen InHouseFire FireObjects
			ETP_CROSS_BRIDGE,			// GetTargetPoint liefert Position vor Pontonbrücke
			ETP_OPPOSITE_BRIDGE,		// nur intern, bitte immer ETP_CROSS_BRIDGE benutzen
			ETP_EXTINGUISH_PERSON,		// GetTargetPoint liefert Position vor dem am nächsten gelegenen FireObjects im Abstand <Typ>JETLENGTH
		/* TODO(ae) not needed for em5
			ETP_EXTINGUISH_PERSON_INHOUSEFIRE, // GetTargetPoint liefert Position vor dem am nächsten gelegenen InHouseFire FireObjects im Abstand <Typ>JETLENGTH
			ETP_PONTON_BRIDGE,			// nur intern, bitte immer ETP_CROSS_BRIDGE benutzen
		*/
			ETP_TOUCHPERSON,			// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz TOUCHPERSONDISTANCE
			ETP_USE,					// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz USEDISTANCE
			ETP_OBJECTSURFACE,			// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz OBJECTSURFACEDISTANCE
			ETP_FREE_CONNECTOR,			// nur für Fahrzeuge definiert, GetTargetPoint liefert Position vor dem Schlauchanschluss
			ETP_RANDOM,					// nur für virtuelles Objekt definiert, GetTargetPoint liefert zufällige Position innerhalb des virtuellen Objekts
			ETP_HOUSE_SAFE_DISTANCE,	// GetTargetPoint liefert Position in Distanz HOUSESAFEDISTANCE, Handling übernimmt dennoch RF: Distanzziel mit Distanz HOUSESAFEDISTANCE
		/* TODO(ae) not needed for em5
			ETP_PLANATE,				// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz PLANATEDISTANCE
		*/
			ETP_PUTFOLLOWERINCAR,		// GetTargetPoint liefert Mittelpunkt, Handling übernimmt RF: Distanzziel mit Distanz PUTFOLLOWERINCARDISTANCE

			ETP_MAX,
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EActor wrapper class for a QSF entity
		*/
		class EActor
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class RouterScene;	// Needs to set the actor ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			virtual Entity& getQsfEntity() = 0;
			virtual bool hasTag(const StringHash& tag) const = 0;

			inline EActorType GetType() const  { return mActorType; }
			inline int GetID() const { return mID; }
			inline int GetParentID() const { return -1; }				// TODO(co) Implement (-1 = not initialized in old code base)
			inline std::string GetName() const { return "unnamed"; }	// TODO(co) Implement

			// World space axis aligned bounding box
			inline const AxisAlignedBoundingBox& GetBoundingBox() const { return mAxisAlignedBoundingBox; }
			virtual float GetBoundingRadius() = 0;
			virtual float GetBoundingRadiusXZ() const = 0;
			virtual float GetBoundingRadiusHeight() const = 0;
			virtual glm::vec3 GetBBMidPoint() const = 0;

			// Collision
			virtual void SetBoundingData(const glm::vec3& midPoint_, float radius_, float radiusXZ_, float height_) { mMidPoint = midPoint_; mRadius = radius_; mRadiusXZ = radiusXZ_; mRadiusHeight = height_; };
			void SetBoundingDataIsSetByRouter(bool value_) { mBoundingDataIsSetByRouter = value_; }
			void SetHasRouterCollisionObject(bool has_) { mHasRouterCollisionObject = has_; }

			// Terrain
			virtual ETerrain* GetTerrain() = 0;
			virtual ETerrain* GetTerrain(int index) = 0;

			virtual bool IsWalkable() = 0;

			// owner: -1: world, 0-x: player
			virtual void initOwnerPlayer() = 0;
			int GetOwnerPlayer() const { return mOwnerPlayer; }
			void SetOwnerPlayerInternal(int owner_) { mOwnerPlayer = owner_; }


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			EActor(EActorType actorType) :
				mActorType(actorType),
				mID(getUninitialized<int>()),
				mHasRouterCollisionObject(false),
				mBoundingDataIsSetByRouter(false),
				mRadius(0.0f),
				mRadiusXZ(0.0f),
				mRadiusHeight(0.0f),
				mOwnerPlayer(-1)
			{}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setId(int id)  { mID = id; }


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			EActorType				mActorType;
			int						mID;
			AxisAlignedBoundingBox	mAxisAlignedBoundingBox;

			bool					mHasRouterCollisionObject;
			bool					mBoundingDataIsSetByRouter;
			glm::vec3				mMidPoint;
			float					mRadius;
			float					mRadiusXZ;
			float					mRadiusHeight;

			int						mOwnerPlayer;

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
