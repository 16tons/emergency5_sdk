// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/map/MainEventIdentifier.h"

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/base/NamedIdentifier.h>


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
	*    EMERGENCY 5 map identifier class
	*/
	class EM5_API_EXPORT MapIdentifier
	{


	//[-------------------------------------------------------]
	//[ Public static definitions                             ]
	//[-------------------------------------------------------]
	public:
		// Identifiers
		static const std::string MAP_MUNICH;
		static const std::string MAP_HAMBURG;
		static const std::string MAP_BERLIN;
		static const std::string MAP_COLOGNE;
		static const std::string MAP_MEDIEVAL;
		static const std::string MAP_MEDIEVAL_HAMBURG;
		enum MapIndex
		{
			MAP_MUNICH_INDEX = 0,
			MAP_HAMBURG_INDEX,
			MAP_BERLIN_INDEX,
			MAP_COLOGNE_INDEX,
			MAP_MEDIEVAL_INDEX,
			MAP_MEDIEVAL_HAMBURG_INDEX
		};

		// Local map asset names
		static const std::string MAP_MUNICH_LOCAL_ASSET_NAME;
		static const std::string MAP_HAMBURG_LOCAL_ASSET_NAME;
		static const std::string MAP_BERLIN_LOCAL_ASSET_NAME;
		static const std::string MAP_COLOGNE_LOCAL_ASSET_NAME;
		static const std::string MAP_MEDIEVAL_LOCAL_ASSET_NAME;
		static const std::string MAP_MEDIEVAL_HAMBURG_LOCAL_ASSET_NAME;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static MapIdentifier getMunich();
		static MapIdentifier getHamburg();
		static MapIdentifier getBerlin();
		static MapIdentifier getCologne();
		static MapIdentifier getMedivial();
		static MapIdentifier getMedivialHamburg();

		static MapIdentifier fromIdentifier(const std::string& identifier);
		static MapIdentifier fromLocalAssetName(const std::string& localAssetName);
		static MapIdentifier fromGlobalAssetId(const qsf::GlobalAssetId& globalAssetId);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MapIdentifier();

		/**
		*  @brief
		*    Constructor
		*/
		explicit MapIdentifier(int32 index);

		/**
		*  @brief
		*    Destructor
		*/
		~MapIdentifier();

		inline bool isOriginalMap() const;
		inline int32 getIndex() const;
		inline const std::string& getIdentifier() const;
		std::string getLocalizationString() const;
		inline const std::string& getLocalAssetName() const;
		inline const MainEventIdentifier& getSecondLastMainEventIdentifier() const;

		inline const std::string& getWorldEventEventPools() const;
		inline const std::vector<std::string>& getSeasonalEvents() const;

		inline bool operator==(const MapIdentifier& other) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int32					 mIndex;							///< Index of the map: 0..4 for original maps, -1 if map is unknown (modded map, test map, etc.)
		std::string				 mIdentifier;						///< Unique identifier string for the map, empty if map is unknown
		qsf::NamedIdentifier	 mLocalAssetName;					///< Local asset name of the map asset
		MainEventIdentifier		 mSecondLastMainEventIdentifier;	///< Main event identifier of the second last main event of the map
		std::string				 mWorldEventEventPools;				///< Holds the event pool(s) which should be used, when the world event is active
		std::vector<std::string> mSeasonalEvents;					///< Holds the name of the seasonal events, layer and event pool name is created dynamical from the names.


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/map/MapIdentifier-inl.h"
