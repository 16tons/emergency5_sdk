// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/ground/GroundMap.h"
#include "qsf/asset/AssetSystemTypes.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GroundMapManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Ground map manager configuration class
	*/
	class QSF_API_EXPORT GroundMapManagerConfiguration
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] groundMapManager
		*    Owner ground map manager
		*/
		explicit GroundMapManagerConfiguration(GroundMapManager& groundMapManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GroundMapManagerConfiguration();

		/**
		*  @brief
		*    Loads the ground map manager configuration from a JSON file
		*
		*  @remarks
		*    Previous configuration is not cleared so you can load multiple configuration files one after another to
		*    union several configuration files (like a big common ground map configuration plus a specialized per-map configuration)
		*/
		void loadConfiguration(const std::string& groundMapConfigurationsFilename);

		/**
		*  @brief
		*    Add all configurations to the owner ground map manager; doesn't destroy already existing ground maps now does it update the ground maps
		*
		*  @param[in] includeEventSpecific
		*    Include event specific ground maps as well?
		*/
		void addConfigurationsToGroundMapManager(bool includeEventSpecific);

		/**
		*  @brief
		*    Add a configuration to the owner ground map manager (including event specific); doesn't destroy already existing ground maps now does it update the ground maps
		*
		*  @param[in] configurationId
		*    ID of the configuration to add
		*
		*  @return
		*    "true" if all went fine, else "false" (e.g. map mismatch)
		*/
		bool addAndUpdateGroundMapByConfigurationId(uint32 configurationId);

		/**
		*  @brief
		*    Destroy all ground maps
		*/
		void destroyAllGroundMaps();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Stores per-ground map configuration plus additional metadata needed for the compiler
		*/
		struct ExtendedGroundMapConfiguration
		{
			uint32									  configurationId;
			uint32									  groundMapId;
			GroundMap::Configuration				  groundMapConfiguration;
			boost::container::flat_set<GlobalAssetId> forMaps;
			bool									  eventSpecific;
			explicit ExtendedGroundMapConfiguration(uint32 _configurationId) :
				configurationId(_configurationId),
				groundMapId(getUninitialized<uint32>()),
				groundMapConfiguration(getUninitialized<uint32>()),
				eventSpecific(false)
			{ }
		};

		/**
		*  @brief
		*    List of individual ground map configurations
		*/
		typedef std::vector<ExtendedGroundMapConfiguration> ExtendedGroundMapConfigurations;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GroundMapManager&				mGroundMapManager;	///< Owner ground map manager
		ExtendedGroundMapConfigurations mConfigurations;	///< List of every configuration that should be created by the compiler


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
