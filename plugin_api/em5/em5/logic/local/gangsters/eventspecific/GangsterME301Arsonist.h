// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"

#include <qsf/message/MessageProxy.h>


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
	*    Game logic for gangster type used in "Arsonist" Emergency 20 years event
	*/
	class EM5_API_EXPORT GangsterME301Arsonist : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;		// "em5::GangsterME301Arsonist" game logic type ID

		static const uint32 MESSAGE_FIRE_STARTED;	// Message emitted when gangster started a fire again


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterME301Arsonist();

		void setTimeToStartFire(qsf::Time time);
		void setFirefighterBuildingEvacuateTime(qsf::Time time);
		void setMaxBurningObjectsToStartFire(uint32 value);
		void initializeWithTargets(const std::vector<qsf::Entity*>& targetBuildings);

		void startFireAtRandomTarget();
		void placeAtNextTarget();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSimulationUpdate(const qsf::JobArguments& jobArguments) override;
		virtual void onIdle() override;
		virtual void onWaypointReached(const Waypoint& waypoint) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct BuildingInfo
		{
			qsf::WeakPtr<qsf::Entity> mEntity;
			glm::vec3 mPosition;
			bool mVisited = false;
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onStartFireMessage(const qsf::MessageParameters& parameters);

		bool chooseNextTarget();
		void startFireInBuilding(BuildingInfo& building, bool fullFire);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time mTimeToStartFire;
		qsf::Time mFirefighterBuildingEvacuateTime;
		uint32 mMaxBurningObjectsToStartFire;

		std::vector<BuildingInfo> mBuildingsGroup01;
		std::vector<BuildingInfo> mBuildingsGroup02;

		glm::vec3 mFinalPosition;
		size_t mNextTargetIndex;
		bool mAllDone;

		qsf::MessageProxy mStartFireMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterME301Arsonist)
