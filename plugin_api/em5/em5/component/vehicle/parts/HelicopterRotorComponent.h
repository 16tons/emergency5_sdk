// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Time;
}


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
	*    EMERGENCY 5 helicopter rotor component class
	*/
	class EM5_API_EXPORT HelicopterRotorComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::HelicopterRotorComponent" unique component ID

		static const float SPEED_TRESHHOLD;				///< Threshold to change from real rotors to blurred rotors
		static const float SPEED_LIMIT;					///< Maximum speed
		static const float SPEED_MAIN_TO_TAIL_FACTOR;	///< Rotation speed factor between main and tail rotor
		static const float SPEED_ACCELERATION_FACTOR;	///< Acceleration factor (used externally)
		static const float SPEED_DECELERATION_FACTOR;	///< Acceleration factor (used externally)

		enum RotorType
		{
			ROTOR_REAL,
			ROTOR_SLOW_BLURRED,
			ROTOR_FAST_BLURRED
		};

		enum RotorPosition
		{
			ROTOR_MAIN,
			ROTOR_TAIL
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
		explicit HelicopterRotorComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HelicopterRotorComponent();

		/**
		*  @brief
		*    Rotor job update
		*/
		void updateRotor(const qsf::Time& timePassed);

		/**
		*  @brief
		*    Returns the rotor type
		*/
		RotorType getRotorType() const;

		/**
		*  @brief
		*   Set the rotor type
		*/
		void setRotorType(RotorType rotorType);

		/**
		*  @brief
		*    Returns the rotor position (main, tail)
		*/
		RotorPosition getRotorPosition() const;

		/**
		*  @brief
		*   Set the rotor position (main, tail)
		*/
		void setRotorPosition(RotorPosition rotorPosition);

		/**
		*  @brief
		*    Get the rotor speed of the real rotorin radian angle change per second
		*/
		float getRotorRealSpeed() const;

		/**
		*  @brief
		*    Set the rotor speed of the real rotorin radian angle change per second; Speed of blurred rotor are calculated
		*/
		void setRotorRealSpeed(float rotorRealSpeed);

		/**
		*  @brief
		*   Check if we have reached the speed limit
		*/
		bool hasMaximalSpeed() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RotorPosition	mRotorPosition;
		RotorType		mRotorType;
		float			mRotorRealSpeed;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HelicopterRotorComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::HelicopterRotorComponent::RotorPosition)
QSF_CAMP_TYPE_NONCOPYABLE(em5::HelicopterRotorComponent::RotorType)
