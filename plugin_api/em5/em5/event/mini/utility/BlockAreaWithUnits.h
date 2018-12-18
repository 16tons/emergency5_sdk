// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/WeakPtr.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Objective;
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
	*    Utility class for events when user has to block areas with his units
	*/
	class EM5_API_EXPORT BlockAreaWithUnits
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class ObjectFilter
		{
			ALL_UNITS,
			BARRIER_TAPES
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BlockAreaWithUnits();

		/**
		*  @brief
		*    Destructor
		*/
		~BlockAreaWithUnits();

		uint32 initialize(qsf::Map& map, Objective* objective, uint32 unitsNeededPerArea, const std::string& areasNamingScheme, const std::string& markersNamingScheme, bool switchMarkersWhenBlocked, ObjectFilter objectFilter = ObjectFilter::ALL_UNITS);

		void setObjective(Objective& objective);

		bool updateObjective();
		Objective& getObjectiveSafe();

		void setMarkersActive(bool active);
		std::vector<bool> backupActiveMarkers() const;
		void restoreActiveMarkers(const std::vector<bool>& backup);

		qsf::Entity* getArea(uint32 index) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Map*  mMap;
		Objective* mObjective;
		uint32	   mUnitsNeededPerArea;
		bool	   mSwitchMarkersWhenBlocked;
		ObjectFilter mObjectFilter;

		struct AreaInfo
		{
			qsf::WeakPtr<qsf::Entity> mArea;
			qsf::WeakPtr<qsf::Entity> mMarker;
			bool mMarkerActive;
		};
		std::vector<AreaInfo> mAreas;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
