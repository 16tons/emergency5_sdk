// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/message/MessageProxy.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class EntityInterpolation;
}
namespace em5
{
	class MiniMapIconRegistrationLogic;
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
	*    EMERGENCY 5 game animation class
	*/
	class EM5_API_EXPORT GameAnimation : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GameAnimation();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameAnimation();

		/**
		*  @brief
		*    Startup the game animation instance
		*/
		void startup();

		/**
		*  @brief
		*    Shutdown the game animation instance
		*/
		void shutdown();

		/**
		*  @brief
		*    Update the game animation clock
		*
		*  @param[in] baseClock
		*    Clock to take time progress from
		*/
		void updateClock(const qsf::Clock& baseClock);

		qsf::EntityInterpolation& getEntityInterpolation() const;

		MiniMapIconRegistrationLogic& getMiniMapIconRegistrationLogic() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update the game animation
		*/
		void updateAnimation(const qsf::Clock& clock);

		/**
		*  @brief
		*    React to game speed change
		*/
		void onGameSpeedChange(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Clock*						mAnimationClock;				///< Game animation clock; always valid, we have to destroy the instance in case we no longer need it
		qsf::EntityInterpolation*		mEntityInterpolation;			///< Entity interpolation controller; always valid, we have to destroy the instance in case we no longer need it
		qsf::MessageProxy				mGameSpeedChangeListener;		///< Listen to game speed changes
		MiniMapIconRegistrationLogic*	mMiniMapIconRegistrationLogic;	///< Registers and unregisters icons for the minimap


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
