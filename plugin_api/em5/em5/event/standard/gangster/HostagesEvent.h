// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/WeakPtr.h>


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
	*    EMERGENCY 5 freeplay event class for "Hostages"
	*/
	class EM5_API_EXPORT HostagesEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::HostagesEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HostagesEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HostagesEvent();

		/**
		*  @brief
		*    Initialize the event
		*/
		void init(qsf::Entity& entity, bool gangsterGiveUp, const std::string& layerName);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GangsterInsideBuildingAmount, int, mGangsterInsideBuildingAmount);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HostagesAmount, int, mHostagesAmount);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(InjuredHostageAmount, int, mInjuredHostageAmount);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GangsterLogicName, const std::string&, mGangsterLogicName);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HostageSpawnPool, const std::string&, mHostageSpawnPool);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startHintObservers();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<uint64>		  mGangsterIds;
		std::vector<uint64>		  mVictimIds;
		qsf::WeakPtr<qsf::Entity> mTargetBuilding;

		// Configuration
		int			mGangsterInsideBuildingAmount;
		int			mHostagesAmount;
		int			mInjuredHostageAmount;
		bool		mGangsterGiveUp;
		std::string mGangsterLogicName;
		std::string mHostageSpawnPool;
		std::string mLayerName;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HostagesEvent)
