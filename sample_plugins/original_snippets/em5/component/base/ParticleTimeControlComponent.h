// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/time/Time.h>


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
	*    EMERGENCY 5 particle time control component class
	*/
	class ParticleTimeControlComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::ParticleTimeControlComponent" unique component ID


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
		inline explicit ParticleTimeControlComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ParticleTimeControlComponent();

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be earliest switched on
		*
		*  @return
		*    The time for the earliest switched on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getEarliestOnTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be earliest switched on
		*
		*  @param[in] time
		*    The time for the earliest switch on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setEarliestOnTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be latest switched on
		*
		*  @return
		*    The time for the earliest switched on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getLatestOnTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be latest switched on
		*
		*  @param[in] time
		*    The time for the earliest switch on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLatestOnTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be earliest switched off
		*
		*  @return
		*    The time for the earliest switched off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getEarliestOffTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be earliest switched off
		*
		*  @param[in] time
		*    The time for the earliest switch off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setEarliestOffTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be latest switched off
		*
		*  @return
		*    The time for the earliest switched off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getLatestOffTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be latest switched off
		*
		*  @param[in] time
		*    The time for the earliest switch off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLatestOffTime(const qsf::Time& time);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateSimulation(bool forceSet);
		void onUpdateSimulation(const qsf::Time& currentTimeOfDay, bool forceSet);
		qsf::Time getTimeFromTimeRange(const qsf::Time& minimumTime, const qsf::Time& maximumTime) const;
		bool isTimeInOnInterval(const qsf::Time& timeToCheck, const qsf::Time& lightsOnTime, const qsf::Time& lightsOffTime);
		void enableParticle(bool enable);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time		mEarliestOnTime;		///< The time when the particle should be switched on (earliest time)
		qsf::Time		mLatestOnTime;			///< The time when the particle should be switched on (latest time)
		qsf::Time		mEarliestOffTime;		///< The time when the particle should be switched off (earliest time)
		qsf::Time		mLatestOffTime;			///< The time when the particle should be switched off (latest time)

		qsf::Time		mOnTime;				///< The time when the particle should be switched on
		qsf::Time		mOffTime;				///< The time when the particle should be switched off

		bool			mOnStatus;				///< Indicates if the particle is on or off

		qsf::JobProxy	mSimulationJobProxy;


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
#include "em5/component/base/ParticleTimeControlComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ParticleTimeControlComponent)
