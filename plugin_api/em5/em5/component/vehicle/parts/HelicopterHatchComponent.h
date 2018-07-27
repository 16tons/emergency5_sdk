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
#include <qsf/job/JobProxy.h>


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
	*    EMERGENCY 5 helicopter hatch component
	*/
	class EM5_API_EXPORT HelicopterHatchComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::HelicopterHatchComponent" unique component ID

		enum HelicopterHatchState
		{
			CLOSED,
			START_OPENING,
			OPENING_HATCH,
			LOWER_STRETCHER,
			PICKUP_PERSON,
			HIGHER_STRETCHER,
			TRANSPORT_INJURED,
			CLOSEING_HATCH
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
		explicit HelicopterHatchComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HelicopterHatchComponent();

		/**
		*  @brief
		*    Start the opening animation, creates the stretcher and lower the stretcher
		*
		*  @note
		*    - This activate an job. Automatically deactivate the job after the process is finished.
		*/
		void openHatch(qsf::Entity& targetEntity, bool targetIsInjured);

		/**
		*  @brief
		*    Start lowering the stretcher
		*
		*  @note
		*    - This activate an job. Automatically deactivate the job after the process is finished.
		*/
		void startLowerStretcherWithInjured(const glm::vec3& targetPosition);

		/**
		*  @brief
		*    Start raising the stretcher
		*
		*  @param[in] isTargetInjured
		*   "true" the stretcher will remain visible on the highest position. Otherwise the it is moved inside the helicopter, disappears and the hatch is closed.
		*
		*  @note
		*    - This activate an job. Automatically deactivate the job after the process is finished.
		*/
		void raiseStretcher(bool isTargetInjured);

		bool isLowerStretcher() const;
		bool isRaiseStretcher() const;

		/**
		*  @brief
		*    "true" if the helicopter is transported an injured person
		*/
		bool isTransportingInjuredPerson() const;

		qsf::Entity* getStretcherEntity() const;
		qsf::Entity* getTargetFromStretcher();

		float getLowerStretcherSpeed() const;
		void setLowerStretcherSpeed(float meterPerSecond);

		float getRaiseStretcherSpeed() const;
		void setRaiseStretcherSpeed(float meterPerSecond);

		float getInjuredPersonFlyHigh() const;
		void setInjuredPersonFlyHigh(float injuredPersonFlyHigh);

		// Link target and play animation
		void placeTargetOnStretcher();

		// Unlink target and place it on the ground
		void placeTargetOnGround(qsf::Entity& targetEntity);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void activateJob();
		void deactivateJob();
		void updateJob(const qsf::JobArguments& jobArguments);

		void createStretcher();
		bool updateLowerStretcher(float secondsPassed);
		bool updateHigherStretcher(float secondsPassed);
		void closeHatch();
		void destroyStretcher();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HelicopterHatchState		mHelicopterHatchState;
		qsf::WeakPtr<qsf::Entity>	mStretcherEntity;
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		bool		mTargetIsInjured;
		glm::vec3	mTargetPosition;

		float		mLowerHatchSpeed;		///< Speed in meter per second
		float		mRaiseHatchSpeed;		///< Speed in meter per second
		float		mInjuredPersonFlyHigh;	///< In meters

		// Internal
		qsf::JobProxy		mUpdateJobProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HelicopterHatchComponent)
