// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"
#include "qsf/map/ground/GroundMapLevel.h"
#include "qsf/debug/DebugDrawProxy.h"

#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class StringHash;
	class BinarySerializer;
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
	*    Ground map class
	*
	*  @note
	*    - A ground map represents the topology of the ground height in a map
	*/
	class QSF_API_EXPORT GroundMap : boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GroundMapManager;		// Must set the mMap member


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Configuration
		{
			uint32							   mFilterMask;			///< Filter bitmask
			bool							   mIsBaseLevel;		///< If "true", this level should be sampled bilinearly and has no gaps
			glm::ivec2						   mResolution;			///< Ground map sampling resolution, i.e. number of samples along x- and z-axis
			float							   mMaximumClimbHeight;	///< Maximum difference of sample point height and level height (for non-base levels) to "climb" up to that level
			uint32							   mWalkablesTag;		///< Tag required in walkable components to be accepted for this ground map
			boost::container::flat_set<uint32> mIncludeComponents;
			boost::container::flat_set<uint32> mHideLayers;
			boost::container::flat_set<uint32> mUnhideLayers;

			explicit Configuration(uint32 id) :
				mFilterMask(id),
				mIsBaseLevel(true),
				mResolution(DEFAULT_RESOLUTION, DEFAULT_RESOLUTION),
				mMaximumClimbHeight(0.0f),
				mWalkablesTag(getUninitialized<uint32>())
			{ }
		};

		static const uint32 DEFAULT_RESOLUTION;
		static const std::string FILE_SIGNATURE;
		static const uint32 FILE_VERSION;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GroundMap(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GroundMap();

		/**
		*  @brief
		*    Get the configuration
		*/
		inline const Configuration& getConfiguration() const;

		/**
		*  @brief
		*    Access the configuration
		*/
		inline Configuration& getConfiguration();

		/**
		*  @brief
		*    Update the content of the ground map
		*
		*  @param[in] allowCaching
		*    Allow caching of the ground map to speed up the initial map loading time
		*
		*  @note
		*    - Warning, calling this method is considered to be expensive. Parts which are considered to be grouped have to be rendered into a texture which then is read back into the main memory.
		*/
		void updateFromMap(bool allowCaching = true);

		/**
		*  @brief
		*    Sample the ground map height at a given position
		*
		*  @param[in] position
		*    World space position where to sample
		*  @param[out] outHeight
		*    The world space height; stays untouched if return value is "false"
		*
		*  @return
		*    "true" if a height value could be retrieved, "false" if not
		*/
		bool sampleHeight(const glm::vec3& position, float& outHeight) const;

		/**
		*  @brief
		*    Sample the ground map height at a given position
		*
		*  @param[in] position
		*    World space position where to sample
		*  @oaram[in] walkableHeight
		*    Walkable level we want to sample (0 to 3)
		*  @param[out] outHeight
		*    The world space height; stays untouched if return value is "false"
		*
		*  @return
		*    "true" if a height value could be retrieved, "false" if not
		*/
		bool sampleHeightAtLevel(const glm::vec3& position, uint32 walkableLevel, float& outHeight) const;

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
		*
		*  @return
		*    "true" if at least one height value could be retrieved, "false" if not
		*
		*  @note
		*    - The output is not automatically cleared
		*/
		bool sampleAllHeightsAtXZ(float positionX, float positionZ, std::vector<float>& outHeights) const;

		/**
		*  @brief
		*    Sample the ground map and give back the walkable level under position
		*
		*  @param[in] position
		*    Our position on where we want to sample
		*  @param[out] outLevel
		*    walkable level from 0 to 3
		*
		*  @return
		*    "true" if at least one level could be retrieved, "false" if not
		*/
		bool sampleWalkableLevel(const glm::vec3& position, uint32& outLevel) const;

		inline uint32 getId() const;
		inline Map* getMap() const;
		inline bool isValid() const;

		inline const GroundMapLevel& getBaseLevel() const;
		inline const std::vector<GroundMapLevel>& getLevels() const;
		inline const glm::vec3& getAabbSize() const;
		inline const glm::vec3& getAabbMin() const;

		bool loadFromFile(const std::string& absoluteFilename);
		void saveToFile(const std::string& absoluteFilename);

		void debugDrawAt(const glm::vec3& position, float radius);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool serialize(BinarySerializer& binarySerializer, bool checkMapVersionString = true);
		void updateFromMapWithOptions(bool writeBitmap);
		void setupPhysicsGroundMap();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		  mId;
		Map*		  mMap;
		bool		  mWasInitialized;			///< "true" if the height map was loaded or built, "false" if not

		// Configuration
		Configuration mConfiguration;

		// Ground map levels
		std::vector<GroundMapLevel> mGroundMapLevels;

		// Bounding box
		glm::vec3	  mAabbMin;					///< World space left top position of the ground map
		glm::vec3	  mAabbSize;				///< World space size of the ground map

		// Debug
		DebugDrawProxy mDebugDrawProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/ground/GroundMap-inl.h"
