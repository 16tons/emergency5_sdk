// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/math/GlmHelper.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Transform;
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
	*    Camera control component class
	*/
	class QSF_API_EXPORT CameraControlComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::CameraControlComponent" unique component ID
		static const uint32 LOCKED;			///< "Locked" unique class property ID inside the class


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
		inline explicit CameraControlComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CameraControlComponent();

		/**
		*  @brief
		*    Return currently set distance of camera to its target position
		*/
		inline float getTargetDistance() const;

		/**
		*  @brief
		*    Set camera's distance to target position without moving the camera itself
		*/
		inline void setTargetDistanceWithoutMoving(float distance);

		/**
		*  @brief
		*    Set camera's distance to target position by moving the camera towards or away from it
		*/
		void setTargetDistance(float distance);

		/**
		*  @brief
		*    Change camera's distance to target position by moving the camera towards or away from it
		*/
		void changeTargetDistanceExponential(float exponent);

		/**
		*  @brief
		*    Returns the target position from the camera
		*/
		glm::vec3 getTargetPosition() const;

		/**
		*  @brief
		*    Return whether or not the camera control is locked
		*
		*  @return
		*    "true" if the camera control is locked, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool isLocked() const;

		/**
		*  @brief
		*    Set whether or not the camera control is locked
		*
		*  @param[in] locked
		*    "true" if the camera control is locked, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setLocked(bool locked);

		float getPitch() const;
		void setPitch(float pitchDegrees);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		bool mLocked;
		// Internal
		float mTargetDistance;	///< Distance to target point to rotate around; can be 0.0f for rotation around itself
		float mPitch;


	public:
		// TODO(fw) This is a HACK for the GC version
		boost::function<void(qsf::Transform&, float&)> mCorrectTransformationCallback;


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
#include "qsf/renderer/utility/CameraControlComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CameraControlComponent)
