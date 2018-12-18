// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/base/idgenerator/ConsecutiveIdGenerator.h"

#include <boost/container/flat_map.hpp>
#include <boost/signals2.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class CameraComponent;
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
	*    Map system class
	*
	*  @note
	*    - A map system is a collection of maps
	*/
	class QSF_API_EXPORT MapSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<Map*>						 MapList;
		typedef boost::container::flat_map<uint32, Map*> MapMap;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const CameraComponent&)> PreCompositorWorkspaceUpdate;	///< This Boost signal is emitted before a compositor workspace gets updated; parameter: used camera component inside one of the maps managed by the map system


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MapSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MapSystem();

		//[-------------------------------------------------------]
		//[ Main map                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not a main map is currently set
		*
		*  @return
		*    "true" if a main map is currently set, else "false"
		*/
		inline bool hasMainMap() const;

		/**
		*  @brief
		*    Return the main map instance
		*
		*  @return
		*    The main map instance, do not destroy the instance
		*/
		inline Map& getMainMap() const;

		/**
		*  @brief
		*    Set the main map instance by using its unique ID
		*
		*  @param[in] id
		*    Unique map ID
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool setMainMap(uint32 id);

		//[-------------------------------------------------------]
		//[ Map management                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a new map instance
		*
		*  @param[in] startup
		*    "true" to startup the map and initialize related map aspects, else "false" to have just the map data
		*  @param[in] numberOfWorkerThreads
		*    Number of worker threads to use, 0 for automatic detection, 1 for single-threaded processing, n for multi-threaded processing
		*
		*  @return
		*    The created map instance, null pointer on error, do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique map ID instead)
		*/
		Map* createMap(bool startup, uint8 numberOfWorkerThreads = 0);

		/**
		*  @brief
		*    Create a new map instance with an enforced given unique map identifier
		*
		*  @param[in] id
		*    The unique map identifier
		*  @param[in] startup
		*    "true" to startup the map and initialize related map aspects, else "false" to have just the map data
		*  @param[in] numberOfWorkerThreads
		*    Number of worker threads to use, 0 for automatic detection, 1 for single-threaded processing, n for multi-threaded processing
		*
		*  @return
		*    The created map instance, null pointer on error (e.g. ID already used), do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique map ID instead)
		*
		*  @note
		*    - Use "createMap()" instead of "createMapWithId()" whenever possible, do not enforce unique map identifier if you can avoid it
		*/
		Map* createMapWithId(uint32 id, bool startup, uint8 numberOfWorkerThreads = 0);

		/**
		*  @brief
		*    Destroy a map instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique map identifier
		*
		*  @return
		*    "true" if all went fine, else "false" (unknown map identifier?)
		*/
		bool destroyMapById(uint32 id);

		/**
		*  @brief
		*    Destroy all maps within this map system
		*/
		void destroyAllMaps();

		/**
		*  @brief
		*    Return a map instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique map identifier
		*
		*  @return
		*    The requested map instance, null pointer on error (maybe the ID is unknown?), do not destroy the instance
		*/
		inline Map* getMapById(uint32 id) const;

		/**
		*  @brief
		*    Return the map list
		*
		*  @return
		*    Reference to the internal map list, do not manipulate the list or destroy the maps
		*/
		inline const MapList& getMaps() const;

		//[-------------------------------------------------------]
		//[ Entity ID generation                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Generate a new entity ID
		*
		*  @return
		*    The new entity ID; you have to make sure yourself that it is no duplicate of an existing ID inside the concrete map
		*/
		uint64 generateEntityId();

		/**
		*  @brief
		*    Set entity ID generation mode
		*
		*  @param[in] useRandomIds
		*    If "true", generate random entity IDs, else consecutively enumerated entity IDs starting from 1
		*/
		void setUseRandomIdGeneration(bool useRandomIds);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the map system
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Create the main map instance
		*/
		virtual bool onStartup(bool serverMode) override;

		/**
		*  @brief
		*    Shutdown the map system
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Destroy the main map instance
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Validate that there's a main map
		*
		*  @note
		*    - While the map system is up-and-running, there must always be a valid main map
		*/
		void validateMainMap();

		/**
		*  @brief
		*    Destroy the given map instance
		*
		*  @param[in] map
		*    Map instance to destroy, the instance is no longer valid after this method call
		*/
		void destroyMapInstance(Map& map);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						   mRunning;		///< "true" if the map system is up-and-running, else "false"
		ConsecutiveIdGenerator<uint32> mIdGenerator;	///< Map ID generator
		Map*						   mMainMap;		///< Main map instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MapList						   mMapList;		///< List of all maps within the map system
		MapMap						   mMapMap;			///< Map map with unique map identifier as key and pointer to the map as value
		bool						   mUseRandomIdGeneration;	///< If "true", generate random entity IDs, otherwise consecutive IDs -- TODO(fw): This has to be extended, see "MapSystem::generateEntityId()" implementation


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/MapSystem-inl.h"
