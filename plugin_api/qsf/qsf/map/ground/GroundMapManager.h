// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/base/StringHash.h"
#include "qsf/job/JobProxy.h"
#include "qsf/map/ground/GroundMapManagerConfiguration.h"
#include "qsf/math/Color3.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FastDebugDraw;
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
	*    Ground map manager class
	*
	*  @remarks
	*    Manages a collection of ground maps
	*/
	class QSF_API_EXPORT GroundMapManager : public ElementManager<uint32, GroundMap>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GroundMapManager(Map& map);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GroundMapManager();

		inline Map& getMap() const;
		inline GroundMapManagerConfiguration& getGroundMapManagerConfiguration();
		inline const GroundMapManagerConfiguration& getGroundMapManagerConfiguration() const;

		void removeAllGroundMaps();

		/**
		*  @brief
		*    Return a ground map instance by its ID
		*
		*  @param[in] groundMapId
		*    Unique ID of the ground map
		*
		*  @return
		*    The ground map instance, or a null pointer if it was not found; do not destroy the returned instance
		*/
		inline GroundMap* getGroundMapById(uint32 groundMapId) const;

		/**
		*  @brief
		*    Add a new ground map instance
		*
		*  @param[in] groundMapId
		*    Unique ID of the new ground map
		*
		*  @return
		*    The ground map instance, or a null pointer on error, e.g. ID is in use already; do not destroy the returned instance
		*/
		GroundMap& getOrCreateGroundMapById(uint32 groundMapId);

		/**
		*  @brief
		*    Destroy a ground map instance by its ID
		*
		*  @param[in] groundMapId
		*    Unique ID of the ground map
		*
		*  @return
		*    "true" if the ground map was found and removed, "false" otherwise
		*/
		bool destroyGroundMapById(uint32 groundMapId);

		void updateAllGroundMaps(bool allowCaching = true);

		/**
		*  @brief
		*    Sample the ground map height at a given position
		*
		*  @param[in] position
		*    World space position where to sample
		*  @param[out] outHeight
		*    The world space height; stays untouched if return value is "false"
		*  @param[in] filterBitmask
		*    Only those ground maps are sampled that share at least one bit with the given mask
		*
		*  @return
		*    "true" if a height value could be retrieved, "false" if not
		*/
		bool sampleHeight(const glm::vec3& position, float& outHeight, uint32 filterBitmask = 0xffffffff) const;

		/**
		*  @brief
		*    Sample the ground map height at a given position
		*
		*  @param[in] position
		*    World space position where to sample
		*  @param[out] outHeight
		*    The world space height; stays untouched if return value is "false"
		*  @param[out] walkableLevel
		*    WalkableLevel we want to sample (0 to 3)
		*  @param[in] filterBitmask
		*    Only those ground maps are sampled that share at least one bit with the given mask
		*
		*  @return
		*    "true" if a height value could be retrieved, "false" if not
		*/
		bool sampleHeightAtLevel(const glm::vec3& position, uint32 walkableLevel, float& outHeight, uint32 filterBitmask = 0xffffffff) const;

		/**
		*  @brief
		*    Sample the ground map height at a given position
		*
		*  @param[in] positionX
		*    World space x-position where to sample
		*  @param[in] positionZ
		*    World space z-position where to sample
		*  @param[out] outHeights
		*    The resulting world space heights; can be empty afterwards, or contain one or more sampled heights in arbitrary order
		*  @param[in] filterBitmask
		*    Only those ground maps are sampled that share at least one bit with the given mask
		*
		*  @return
		*    "true" if at least one height value could be retrieved, "false" if not
		*
		*  @note
		*    - The output is not automatically cleared
		*/
		bool sampleAllHeightsAtXZ(float positionX, float positionZ, std::vector<float>& outHeights, uint32 filterBitmask) const;

		/**
		*  @brief
		*    Sample the ground map and give back the walkable level under position
		*
		*  @param[in] position
		*    Our position on where we want to sample
		*  @param[out] outLevel
		*    walkable level from 0 to 3
		*  @param[in] filterBitmask
		*    Only those ground maps are sampled that share at least one bit with the given mask
		*
		*
		*  @return
		*    "true" if at least one level could be retrieved, "false" if not
		*/
		bool sampleWalkableLevel(const glm::vec3& position, uint32& outLevel, uint32 filterBitmask = 0xffffffff) const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the ground map manager
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool startup(bool serverMode);

		/**
		*  @brief
		*    Shutdown the ground map manager
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ Debug output                                          ]
		//[-------------------------------------------------------]
		void debugDrawAt(const glm::vec3& position, float radius = 5.0f);

		bool isDebugDrawEnabled() const;

		/**
		*  @brief
		*    Enables the debug visualization of the ground map data
		*
		*  @param[in] enable
		*    True, debug visualization is enabled, otherwise disabled
		*/
		void enableDebugDraw(bool enable);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void debugJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Map&						  mMap;			///< Map instance this managers belongs to; do not destroy the instance
		bool						  mRunning;		///< "true" if the ground map manager is up-and-running, else "false" (usually when on server)
		GroundMapManagerConfiguration mGroundMapManagerConfiguration;

		// For debug draw of the ground map data
		// TODO(sw) Currently only the first ground map and the base ground map level are supported
		JobProxy mDebugJobProxy;
		FastDebugDraw* mFastDebugDraw;
		struct DebugGroundMapLevelData
		{
			std::vector<glm::vec3> mPositions;
			glm::ivec2			   mResolution;
			glm::vec3			   mAabbMin;
			glm::vec3			   mAabbSize;
			Color3				   mColor;
		};
		std::vector<DebugGroundMapLevelData> mDebugGroundMapLevelData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/ground/GroundMapManager-inl.h"
