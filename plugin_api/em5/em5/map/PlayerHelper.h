// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/StringHash.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Player;
}
namespace qsf
{
	class Entity;
	class Map;
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
	*    EMERGENCY 5 player helper class
	*/
	class EM5_API_EXPORT PlayerHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit inline PlayerHelper(Player& player);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~PlayerHelper();

		// Available checks
		bool isUnitAvailable(const qsf::Map& map, const qsf::StringHash& identifier) const;
		uint32 getUnitAvailableCount(const qsf::Map& map, const qsf::StringHash& identifier) const;

		// Number of units
		/**
		*  @brief
		*    Returns the number of units which are available for the given unit type string (available = unit not blocked (dead/injured in HQ))
		*/
		uint32 getNumberOfUnit(const qsf::StringHash& identifier) const;
		uint32 getNumberAmbulanceMedics() const;
		uint32 getNumberAmbulanceParamedics() const;
		uint32 getNumberFireFightersFirefighter() const;
		uint32 getNumberPolicePolice() const;
		uint32 getNumberRescueDogLeader() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Player& mPlayer;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/map/PlayerHelper-inl.h"
