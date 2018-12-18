// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/base/PropertyHelper.h>

#include <glm/gtc/quaternion.hpp>


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
	*    EMERGENCY 5 vehicle wheel component class
	*/
	class EM5_API_EXPORT WheelComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::WheelComponent" unique component ID

		enum WheelType
		{
			WHEELTYPE_UNDEFINED = 0,	///< Undefined wheel type
			WHEELTYPE_FRONT_LEFT,		///< Left front wheel type
			WHEELTYPE_FRONT_RIGHT,		///< Right front wheel type
			WHEELTYPE_BACK_LEFT,		///< Left Back wheel type
			WHEELTYPE_BACK_RIGHT,		///< Right Back wheel type
			WHEELTYPE_CHAIN_RIGHT,		///< Right chain wheel type
			WHEELTYPE_CHAIN_LEFT		///< Left chain wheel type
		};


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
		inline explicit WheelComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~WheelComponent();

		void updateWheel(float movedDistance, float turningAngle);

		/**
		*  @brief
		*    Check if we have a chain-wheel
		*/
		bool isChain() const;

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WheelType, WheelType, mWheelType)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(OriginalLocalRotation, const glm::quat&, mOriginalLocalRotation)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Radius, float, mRadius)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DistanceFromTurnPoint, float, mDistanceFromTurnPoint)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AngularPosition, float, mAngleInRadians)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ChainSpeedV, float, mChainSpeedV)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ChainSpeedU, float, mChainSpeedU)


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::quat	mOriginalLocalRotation;
		float		mRadius;
		float		mAngleInRadians;	///< Radians the wheel is rotated
		WheelType	mWheelType;
		float		mChainSpeedV;
		float		mChainSpeedU;
		float		mDistanceFromTurnPoint;
		float		mChainOffsetU;
		float		mChainOffsetV;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/parts/WheelComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::WheelComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::WheelComponent::WheelType)
