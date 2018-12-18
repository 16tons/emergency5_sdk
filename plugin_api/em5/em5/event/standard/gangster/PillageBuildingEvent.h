// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Transform;
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
	*    EMERGENCY 5 freeplay event "pillage building event"
	*/
	class EM5_API_EXPORT PillageBuildingEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::PillageBuildingEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PillageBuildingEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PillageBuildingEvent();

		/**
		*  @brief
		*    Initialization used by factory
		*
		*  @param[in] targetBuilding
		*    Entity for the building were the gangster are hidding
		*/
		void init(qsf::Entity& targetBuilding);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GangsterInsideBuilding, int, mGangsterInsideBuilding)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GangsterOutsideBuilding, int, mGangsterOutsideBuilding)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FailAttackBuildingTime, qsf::Time, mFailAttackBuildingTimer);

		void setAmbientLayerName(const std::string& layerName);

		void setGangsterLogicName(const std::string& gangsterLogicName);
		const std::string& getGangsterLogicName() const;


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual bool checkEventConfiguration() override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onAttackBuilding(const qsf::MessageParameters& parameters);
		void spawnGangsterOutsideBuilding(const qsf::Transform& spawnPosition, int gangsterIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity> mTargetBuilding;			///< Building to pillage
		qsf::MessageProxy	mAttackBuildingFinishedMessageProxy;

		int mGangsterInsideBuilding;
		int mGangsterOutsideBuilding;
		std::string mGangsterLogicName;
		std::string mAmbientLayerName;				///< The name of the ambient layer, if set the layer is enabled for this event

		// Event fail
		bool mGangsterInBuilding;
		qsf::Time mFailAttackBuildingTimer;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PillageBuildingEvent)
