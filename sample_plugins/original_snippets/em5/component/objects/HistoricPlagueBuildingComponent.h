// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BuildingComponent;
	class FreeplayEvent;
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
	*  @remarks
	*    EMERGENCY 2016 historic plague building component
	*/
	class EM5_API_EXPORT HistoricPlagueBuildingComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::HistoricPlagueBuildingComponent" unique component ID
		static const std::string DECAL_X;	///< "decal_x" the meta data tag if the "X"


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get the instance of this component or create a new
		*/
		static HistoricPlagueBuildingComponent& getInstance();

		/*
		*  @brief
		*    Set if the person icons should be shown
		*/
		static void showSpawnedEntityIcons(qsf::Entity& entity, bool show);

		static qsf::Entity* getMarkXDecalFromEntity(qsf::Entity& buildingEntity);

		static void showPestIconForBuilding(qsf::Entity& entity);


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
		explicit HistoricPlagueBuildingComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HistoricPlagueBuildingComponent();

		/**
		*  @brief
		*    Spawn Persons at the entity tag, they also added to the freeplay event
		*/
		void activateCleanHousesByEntityTag(FreeplayEvent& freeplayEvent, const std::string& entityTag, qsf::Entity& despawnPoint, qsf::Entity& destinationTrigger, bool spawnPersons);

		bool getMustClean(uint64 entityId) const;
		void setMustClean(qsf::Entity& buildingEntity, bool mustClean = false);

		void setLayerName(const std::string& layername);

		bool isPersonReachedFleeingTrigger() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    spawn Persons at given building
		*/
		void spawnPersonsFromBuildings(std::vector<BuildingComponent*>& spawnBuildings);

		std::vector<BuildingComponent*> collectSpawnBuildingsFromMap();

		void onEntityRemoved(const qsf::MessageParameters& parameters);

		/*
		*  @brief
		*    Set if the building icons should be shown
		*/
		void showBuildingIcons(qsf::Entity& buildingEntity, bool show);

		void setupAndMovePersonToDespawn(qsf::Entity& personEntity, uint64 homeBuildingId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FreeplayEvent*		mParentEvent;
		glm::vec3			mDestinationPosition;
		qsf::Entity*		mDestinationTrigger;
		std::string			mEntityTag;

	// TODO(co) Public data? Is this correct in here?
	public:
		struct MustCleanData
		{
			uint64 entityId;
			bool mustClean;
		};
		std::vector<MustCleanData> mMustCleanBuildings;

	private:
		struct FleeingPersonData
		{
			qsf::WeakPtr<qsf::Entity> personEntity;
			qsf::WeakPtr<qsf::Entity> triggerEntity;
		};

		std::vector<FleeingPersonData> mFleeingPersons;

		qsf::MessageProxy	mEntityReturnedProxy;
		qsf::JobProxy		mUpdateJobProxy;
		uint32				mLayerId;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HistoricPlagueBuildingComponent)
