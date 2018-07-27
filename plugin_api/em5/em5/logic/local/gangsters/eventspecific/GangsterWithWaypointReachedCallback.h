// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"


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
	*    Game logic for gangster type used in ME204 Emergency 2017 campaign event
	*/
	class EM5_API_EXPORT GangsterWithWaypointReachedCallback : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void()> waypointReached;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GangsterWithWaypointReachedCallback();

		/**
		*  @brief
		*    Constructor
		*/
		explicit GangsterWithWaypointReachedCallback(uint32 gameLogicId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GangsterWithWaypointReachedCallback();


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the next waypoint was just reached
		*/
		virtual void onWaypointReached(const Waypoint& waypoint) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterWithWaypointReachedCallback)
