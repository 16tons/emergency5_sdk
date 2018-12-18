// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/light/LightComponent.h"
#include "qsf/reflection/type/CampGlmVec2.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Light;
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
	*    Sun component class
	*/
	class QSF_API_EXPORT SunComponent : public LightComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::SunComponent" unique component ID
		static const uint32 TIME;				///< "Time" unique class property ID inside the class
		static const uint32 SUNRISE_TIME;		///< "SunriseTime" unique class property ID inside the class
		static const uint32 SUNSET_TIME;		///< "SunsetTime" unique class property ID inside the class
		static const uint32 MOON_PHASE;			///< "MoonPhase" unique class property ID inside the class
		static const uint32 EAST_DIRECTION;		///< "EastDirection" unique class property ID inside the class
		static const uint32 ANGLE_OF_INCIDENCE;	///< "AngleOfIncidence" unique class property ID inside the class


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
		inline explicit SunComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SunComponent();

		/**
		*  @brief
		*    Return the current time
		*
		*  @return
		*    The current time, x = hour in [0, 23]h range, y = minute in [0, 59]m range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec2& getTime() const;

		/**
		*  @brief
		*    Set the current time
		*
		*  @param[in] time
		*    The current time, x = hour in [0, 23]h range, y = minute in [0, 59]m range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setTime(const glm::vec2& time);

		/**
		*  @brief
		*    Return the sunrise time
		*
		*  @return
		*    The sunrise time, x = sunrise hour in [0, 23]h range, y = sunrise minute in [0, 59]m range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec2& getSunriseTime() const;

		/**
		*  @brief
		*    Set the sunrise time
		*
		*  @param[in] sunriseTime
		*    The sunrise time, x = sunrise hour in [0, 23]h range, y = sunrise minute in [0, 59]m range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSunriseTime(const glm::vec2& sunriseTime);

		/**
		*  @brief
		*    Return the sunset time
		*
		*  @return
		*    The sunset time, x = sunset hour in [0, 23]h range, y = sunset minute in [0, 59]m range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec2& getSunsetTime() const;

		/**
		*  @brief
		*    Set the sunset time
		*
		*  @param[in] sunsetTime
		*    The sunset hour, x = sunset hour in [0, 23]h range, y = sunset minute in [0, 59]m range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSunsetTime(const glm::vec2& sunsetTime);

		/**
		*  @brief
		*    Return the moon phase
		*
		*  @return
		*    The moon phase in [-1,1] range, where -1 means fully covered moon, 0 clear moon and 1 fully covered moon
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getMoonPhase() const;

		/**
		*  @brief
		*    Set the moon phase
		*
		*  @param[in] moonPhase
		*    Moon size in [-1,1] range, where -1 means fully covered moon, 0 clear moon and 1 fully covered moon
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMoonPhase(float moonPhase);

		/**
		*  @brief
		*    Return the east direction
		*
		*  @return
		*    The east direction in Euler angles in degree [0,360]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getEastDirection() const;

		/**
		*  @brief
		*    Set east direction
		*
		*  @param[in] eastDirection
		*    East direction in Euler angles in degree [0,360]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setEastDirection(float eastDirection);

		/**
		*  @brief
		*    Return the angle of incidence
		*
		*  @return
		*    The angle of incidence in Euler angles in degree [-90,90]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getAngleOfIncidence() const;

		/**
		*  @brief
		*    Set angle of incidence
		*
		*  @param[in] angleOfIncidence
		*    Angle of incidence in Euler angles in degree [-90,90]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAngleOfIncidence(float angleOfIncidence);

		/**
		*  @brief
		*    Return whether or not to apply sun light shadows
		*
		*  @return
		*    "true" if we need to apply sun light shadows, else "false" (for example, in case the sun color is black, we won't see any shadows produced by the sun)
		*
		*  @note
		*    - Depends on multiple factors like whether or not the sun component is active, shadow casting is active, resulting light color is not too dark etc.
		*/
		bool getApplySunLightShadows() const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the OGRE light instance
		*
		*  @return
		*    The OGRE light instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the OGRE light instance if you don't have to
		*/
		inline Ogre::Light* getOgreLight() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void setOgreSceneNodeVisibility(bool visible) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update the OGRE light
		*/
		void updateLight();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		glm::vec2	mTime;				///< The current time, x = hour in [0, 23]h range, y = minute in [0, 59]m range
		glm::vec2	mSunriseTime;		///< Sunrise time, x = sunrise hour in [0, 23]h range, y = sunrise minute in [0, 59]m range
		glm::vec2	mSunsetTime;		///< Sunset time, x = sunset hour in [0, 23]h range, y = sunset minute in [0, 59]m range
		float		mMoonPhase;			///< Moon phase in [-1,1] range, where -1 means fully covered moon, 0 clear moon and 1 fully covered moon
		float		mEastDirection;		///< East direction in Euler angles in degree [0,360]
		float		mAngleOfIncidence;	///< Angle of incidence in Euler angles in degree [-90,90]
		// Internal only
		Ogre::Light*	 mOgreLight;		///< OGRE light instance, can be a null pointer
		Ogre::SceneNode* mOgreSceneNode;	///< OGRE scene node instance, can be a null pointer
		glm::vec3		 mSunDirection;		///< Sun direction, derived data
		glm::vec3		 mMoonDirection;	///< Moon direction, derived data


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
#include "qsf/renderer/environment/SunComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::SunComponent)
