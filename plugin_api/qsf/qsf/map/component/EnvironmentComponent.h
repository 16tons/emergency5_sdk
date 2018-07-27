// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/reflection/type/CampQsfColor3.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneManager;
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
	*    Environment component interface
	*
	*  @note
	*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
	*/
	class QSF_API_EXPORT EnvironmentComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::EnvironmentComponent" unique component ID
		// Light
		static const uint32 AMBIENT_COLOR;				///< "AmbientColor" unique class property ID inside the class
		static const uint32 AMBIENT_TEMPERATURE;		///< "AmbientTemperature" unique class property ID inside the class
		// Fog
		static const uint32 FOG_DIFFUSE_COLOR;			///< "FogDiffuseColor" unique class property ID inside the class
		static const uint32 ATMOSPHERE_FOG_START;		///< "AtmosphereFogStart" unique class property ID inside the class
		static const uint32 ATMOSPHERE_FOG_DENSITY;		///< "AtmosphereFogDensity" unique class property ID inside the class
		static const uint32 GROUND_FOG_START;			///< "GroundFogStart" unique class property ID inside the class
		static const uint32 GROUND_FOG_DENSITY;			///< "GroundFogDensity" unique class property ID inside the class
		static const uint32 GROUND_FOG_HEIGHT_OFFSET;	///< "GroundFogHeightOffset" unique class property ID inside the class
		static const uint32 GROUND_FOG_HEIGHT_DENSITY;	///< "GroundFogHeightDensity" unique class property ID inside the class
		// Other
		static const uint32 WIND_DIRECTION;				///< "WindDirection" unique class property ID inside the class


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
		inline explicit EnvironmentComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~EnvironmentComponent();

		//[-------------------------------------------------------]
		//[ Light                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the ambient color
		*
		*  @return
		*    The ambient color, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const Color3& getAmbientColor() const;

		/**
		*  @brief
		*    Set the ambient color
		*
		*  @param[in] ambientColor
		*    The ambient color of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAmbientColor(const Color3& ambientColor);

		/**
		*  @brief
		*    Return the ambient temperature
		*
		*  @return
		*    The ambient temperature (in Kelvin)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getAmbientTemperature() const;

		/**
		*  @brief
		*    Set the ambient temperature
		*
		*  @param[in] ambientTemperature
		*    The ambient temperature (in Kelvin) of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAmbientTemperature(float ambientTemperature);

		//[-------------------------------------------------------]
		//[ Fog                                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the fog diffuse color
		*
		*  @return
		*    The fog diffuse color, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const Color3& getFogDiffuseColor() const;

		/**
		*  @brief
		*    Set the fog diffuse color
		*
		*  @param[in] fogDiffuseColor
		*    The fog diffuse color
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setFogDiffuseColor(const Color3& fogDiffuseColor);

		/**
		*  @brief
		*    Return the distance in world units at which atmosphere fog starts to encroach
		*
		*  @return
		*    The distance in world units at which atmosphere fog starts to encroach
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getAtmosphereFogStart() const;

		/**
		*  @brief
		*    Set the distance in world units at which atmosphere fog starts to encroach
		*
		*  @param[in] atmosphereFogStart
		*    The distance in world units at which atmosphere fog starts to encroach
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setAtmosphereFogStart(float atmosphereFogStart);

		/**
		*  @brief
		*    Return the atmosphere fog density
		*
		*  @return
		*    The atmosphere fog density [0..1]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getAtmosphereFogDensity() const;

		/**
		*  @brief
		*    Set the atmosphere density
		*
		*  @param[in] atmosphereFogDensity
		*    The atmosphere fog density [0..1]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setAtmosphereFogDensity(float atmosphereFogDensity);

		/**
		*  @brief
		*    Return the distance in world units at which ground fog starts to encroach
		*
		*  @return
		*    The distance in world units at which ground fog starts to encroach
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getGroundFogStart() const;

		/**
		*  @brief
		*    Set the distance in world units at which ground fog starts to encroach
		*
		*  @param[in] groundFogStart
		*    The distance in world units at which ground fog starts to encroach
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setGroundFogStart(float groundFogStart);

		/**
		*  @brief
		*    Return the ground fog density
		*
		*  @return
		*    The ground fog density [0..1]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getGroundFogDensity() const;

		/**
		*  @brief
		*    Set the ground fog density
		*
		*  @param[in] groundFogDensity
		*    The ground fog density [0..1]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setGroundFogDensity(float groundFogDensity);

		/**
		*  @brief
		*    Return the ground fog height offset
		*
		*  @return
		*    The ground fog height offset
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getGroundFogHeightOffset() const;

		/**
		*  @brief
		*    Set the ground fog height offset
		*
		*  @param[in] groundFogHeightOffset
		*    The ground fog height offset
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setGroundFogHeightOffset(float groundFogHeightOffset);

		/**
		*  @brief
		*    Return the ground fog height density
		*
		*  @return
		*    The ground fog height density [0..1]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getGroundFogHeightDensity() const;

		/**
		*  @brief
		*    Set the ground fog height density
		*
		*  @param[in] groundFogHeightDensity
		*    The ground fog height density [0..1]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setGroundFogHeightDensity(float groundFogHeightDensity);

		//[-------------------------------------------------------]
		//[ Other                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the wind direction
		*
		*  @return
		*    The wind direction in Euler angles in degree [0,360]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getWindDirection() const;

		/**
		*  @brief
		*    Set wind direction
		*
		*  @param[in] windDirection
		*    Wind direction in Euler angles in degree [0,360]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setWindDirection(float windDirection);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the OGRE scene manager instance
		*
		*  @return
		*    The OGRE scene manager instance, can be a null pointer, do no destroy the returned instance
		*/
		Ogre::SceneManager* getOgreSceneManager() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		// Light
		Color3	mAmbientColor;				///< The ambient color
		float	mAmbientTemperature;		///< The ambient temperature (in Kelvin)
		// Fog
		Color3	mFogDiffuseColor;			///< The fog diffuse color
		float	mAtmosphereFogStart;		///< The distance in world units at which atmosphere fog starts to encroach
		float	mAtmosphereFogDensity;		///< The atmosphere fog density [0..1]
		float	mGroundFogStart;			///< The distance in world units at which ground fog starts to encroach
		float	mGroundFogDensity;			///< The ground fog density [0..1]
		float	mGroundFogHeightOffset;		///< The ground fog height offset
		float	mGroundFogHeightDensity;	///< The ground fog height density [0..1]
		// Other
		float	mWindDirection;				///< Wind direction in Euler angles in degree [0,360]


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
#include "qsf/map/component/EnvironmentComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::EnvironmentComponent)
