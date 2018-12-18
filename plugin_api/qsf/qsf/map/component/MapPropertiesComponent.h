// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/component/MapPropertiesBaseComponent.h"
#include "qsf/reflection/type/CampGlmVec3.h"
#include "qsf/debug/DebugDrawProxy.h"
#include "qsf/math/Color4.h"


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
	*    Concrete map properties component class
	*
	*  @remarks
	*    There are three map dimension related values:
	*      - Event boundary (gray debug visualization): Gameplay related boundary in which events can spawn, usually smaller as the map boundary
	*      - Map boundary (white debug visualization): Gameplay related boundary which considers everything outside not relevant to the game, usually larger as the event boundary. Defining where entities and the camera may freely move. The mini map will cover this defined area.
	*      - Bounding box (yellow debug visualization): Technical related boundary, used e.g. inside the renderer, usually larger as the map boundary. The ground map will cover this defined area.
	*
	*  @todo
	*    - TODO(co) EMERGENCY 5 "PersonSpawnCount" and "VehicleSpawnCount" should never ever been added inside this technical base class.
	*    - TODO(vs) Perhaps consider a more descriptive and flexible system that returns bounding corners for different usecases like entity navigation, camera movement, fire spreadage and so forth.
	*               Each map can then define internally whether an area is reused for different usecases or several boxes are stored.
	*/
	class QSF_API_EXPORT MapPropertiesComponent : public MapPropertiesBaseComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;					///< "qsf::MapPropertiesComponent" unique component ID
		static const uint32 EVENT_BOUNDARY_TOP_LEFT;		///< "EventBoundaryTopLeft" unique property ID
		static const uint32 EVENT_BOUNDARY_BOTTOM_RIGHT;	///< "EventBoundaryBottomRight" unique property ID
		static const uint32 MAP_BOUNDARY_TOP_LEFT;			///< "MapBoundaryTopLeft" unique property ID
		static const uint32 MAP_BOUNDARY_BOTTOM_RIGHT;		///< "MapBoundaryBottomRight" unique property ID
		static const uint32 BOUNDING_BOX_TOP_LEFT;			///< "BoundingBoxTopLeft" unique property ID
		static const uint32 BOUNDING_BOX_BOTTOM_RIGHT;		///< "BoundingBoxBottomRight" unique property ID
		static const uint32 CAMERA_MARGIN_WIDTH;			///< "CameraMarginWidth" unique property ID
		static const uint32 START_CAMERA_ENTITY_ID;			///< "StartCameraEntityId" unique property ID
		static const uint32 MAP_TIME_HOUR;					///< "MapTimeHour" unique property ID
		static const uint32 MAP_TIME_MINUTES;				///< "MapTimeMinutes" unique property ID
		static const uint32 MAP_TIME_SECONDS;				///< "MapTimeSeconds" unique property ID
		static const uint32 START_WEATHER_SET_ID;			///< "StartWeatherSetId" unique property ID
		static const uint32 PERSON_SPAWN_COUNT_ID;			///< "PersonSpawnCount" unique property ID
		static const uint32 VEHICLE_SPAWN_COUNT_ID;			///< "VehicleSpawnCount" unique property ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit MapPropertiesComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MapPropertiesComponent();

		//[-------------------------------------------------------]
		//[ Boundaries                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the current event boundary top left corner
		*
		*  @return
		*    The current event boundary top left corner world space position (gray debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getEventBoundaryTopLeft() const;

		/**
		*  @brief
		*    Set the current event boundary top left corner
		*
		*  @param[in] eventBoundaryTopLeft
		*    The new current event boundary top left corner world space position (gray debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setEventBoundaryTopLeft(const glm::vec3& eventBoundaryTopLeft);

		/**
		*  @brief
		*    Return the current event boundary bottom right corner
		*
		*  @return
		*    The current event boundary bottom right corner world space position (gray debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getEventBoundaryBottomRight() const;

		/**
		*  @brief
		*    Set the current event boundary bottom right corner
		*
		*  @param[in] eventBoundaryBottomRight
		*    The new current event boundary bottom right corner world space position (gray debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setEventBoundaryBottomRight(const glm::vec3& eventBoundaryBottomRight);

		/**
		*  @brief
		*    Return the current map boundary top left corner
		*
		*  @return
		*    The current map boundary top left corner world space position (white debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getMapBoundaryTopLeft() const;

		/**
		*  @brief
		*    Set the current map boundary top left corner
		*
		*  @param[in] mapBoundaryTopLeft
		*    The new current map boundary top left corner world space position (white debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMapBoundaryTopLeft(const glm::vec3& mapBoundaryTopLeft);

		/**
		*  @brief
		*    Return the current map boundary bottom right corner
		*
		*  @return
		*    The current map boundary bottom right corner world space position (white debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getMapBoundaryBottomRight() const;

		/**
		*  @brief
		*    Set the current map boundary bottom right corner
		*
		*  @param[in] mapBoundaryBottomRight
		*    The new current map boundary bottom right corner world space position (white debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMapBoundaryBottomRight(const glm::vec3& mapBoundaryBottomRight);

		/**
		*  @brief
		*    Return the current bounding box top left corner
		*
		*  @return
		*    The current bounding box top left corner world space position (yellow debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getBoundingBoxTopLeft() const;

		/**
		*  @brief
		*    Set the current bounding box top left corner
		*
		*  @param[in] boundingBoxTopLeft
		*    The new current bounding box top left corner world space position (yellow debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBoundingBoxTopLeft(const glm::vec3& boundingBoxTopLeft);

		/**
		*  @brief
		*    Return the current bounding box bottom right corner
		*
		*  @return
		*    The current bounding box bottom right corner world space position (yellow debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getBoundingBoxBottomRight() const;

		/**
		*  @brief
		*    Set the current bounding box bottom right corner
		*
		*  @param[in] boundingBoxBottomRight
		*    The new current bounding box bottom right corner world space position (yellow debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBoundingBoxBottomRight(const glm::vec3& boundingBoxBottomRight);

		/**
		*  @brief
		*    Return the camera margin width
		*
		*  @return
		*    The current camera margin width in world units (cyan debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getCameraMarginWidth() const;

		/**
		*  @brief
		*    Set the camera margin width
		*
		*  @param[in] width
		*    The new camera margin width in world units (cyan debug visualization)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setCameraMarginWidth(float width);

		//[-------------------------------------------------------]
		//[ Miscellaneous                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the current start camera entity ID
		*
		*  @return
		*    The current start camera entity ID
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint64 getStartCameraEntityId() const;

		/**
		*  @brief
		*    Set the start camera entity ID
		*
		*  @param[in] startCameraEntityId
		*    The new start camera entity ID
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setStartCameraEntityId(uint64 startCameraEntityId);

		/**
		*  @brief
		*    Get the starting map hour
		*
		*  @return
		*    The starting map time hour
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline int getMapTimeHour() const;

		/**
		*  @brief
		*    Set the map time hour
		*
		*  @param[in] mapTimeHour
		*    The new MapTimeHour
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMapTimeHour(int mapTimeHour);

		/**
		*  @brief
		*    Get the starting map time minutes
		*
		*  @return
		*    The starting map time minutes
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline int getMapTimeMinutes() const;

		/**
		*  @brief
		*    Set the map time minutes
		*
		*  @param[in] mapTimeMinutes
		*    The new map time minutes
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMapTimeMinutes(int mapTimeMinutes);

		/**
		*  @brief
		*    Get the starting map time seconds
		*
		*  @return
		*    The starting map time seconds
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline int getMapTimeSeconds() const;

		/**
		*  @brief
		*    Set the map time seconds
		*
		*  @param[in] mapTimeSeconds
		*    The new map time seconds
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMapTimeSeconds(int mapTimeSeconds);

		/**
		*  @brief
		*    Get the person spawn count maximum
		*
		*  @return
		*    The maximum count
		*/
		inline uint32 getPersonSpawnCount() const;

		/**
		*  @brief
		*    Set the person spawn count maximum
		*
		*  @param[in] personSpawnCount
		*    Set the maximum count
		*/
		inline void setPersonSpawnCount(uint32 personSpawnCount);

		/**
		*  @brief
		*    Get the vehicle spawn count maximum
		*
		*  @return
		*    The maximum count
		*/
		inline uint32 getVehicleSpawnCount() const;

		/**
		*  @brief
		*    Set vehicle spawn count maximum
		*
		*  @param[in] vehicleSpawnCount
		*    Set the maximum count
		*/
		inline void setVehicleSpawnCount(uint32 vehicleSpawnCount);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetDebug(bool debug) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addBoundingBoxDebugVisualization(const glm::vec3& topLeft, const glm::vec3& bottomRight, const Color4& color);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		glm::vec3		mEventBoundaryTopLeft;		///< Event boundary corner world space position (gray debug visualization)
		glm::vec3		mEventBoundaryBottomRight;	///< Event boundary corner world space position (gray debug visualization)
		glm::vec3		mMapBoundaryTopLeft;		///< Map boundary corner world space position (white debug visualization)
		glm::vec3		mMapBoundaryBottomRight;	///< Map boundary corner world space position (white debug visualization)
		glm::vec3		mBoundingBoxTopLeft;		///< Bounding box corner world space position (yellow debug visualization)
		glm::vec3		mBoundingBoxBottomRight;	///< Bounding box corner world space position (yellow debug visualization)
		float			mCameraMarginWidth;			///< Width of additional margin inside the map boundaries where the game camera gets stopped
		uint64			mStartCameraEntityId;		///< The start camera entity's ID, by default uninitialized
		int				mMapTimeHour;				///< Map time hours, using 24h system, values between 0-23
		int				mMapTimeMinutes;			///< Map time minutes, values between 0-59
		int				mMapTimeSeconds;			///< Map time seconds, values between 0-59
		uint32			mPersonSpawnCount;			///< The person spawn count, values between 0-10000
		uint32			mVehicleSpawnCount;			///< The vehicle spawn count, values between 0-10000
		// Internal only
		DebugDrawProxy mDebugDrawProxy;	///< Debug draw proxy for map boundary visualization


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/component/MapPropertiesComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MapPropertiesComponent)
