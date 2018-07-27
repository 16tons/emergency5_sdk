// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/command/mode/CommandMode.h>

#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/reflection/CampClass.h>	// This include is not needed here directly, but most likely in a derived class


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
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
	*    Place person by flying heli command mode
	*
	*  @remarks
	*    This command mode lets the user select a position where to place the injured person from the rescue helicopter (BHC)
	*/
	class EM5_API_EXPORT PlacePersonByFlyingHeliCommandMode : public qsf::game::CommandMode
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::PlacePersonByFlyingHeliCommandMode" unique command pluginable ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlacePersonByFlyingHeliCommandMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlacePersonByFlyingHeliCommandMode();

		/**
		*  @brief
		*    Fill class with all necessary variables
		*/
		void initialize(qsf::Entity& caller);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual void shutdown() override;
		virtual bool executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds) override;
		virtual void updateCommandMode(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool hasCollisionOnTargetPosition(const qsf::Entity* entity, const glm::vec3& position);

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::DebugDrawProxy	mDebugDrawProxy;
		qsf::Entity*		mCaller;
		qsf::Map*			mMap;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PlacePersonByFlyingHeliCommandMode)
