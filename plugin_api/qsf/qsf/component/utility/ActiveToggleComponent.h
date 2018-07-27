// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/job/JobProxy.h"


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
	*    Active toggle component class
	*
	*  @todo
	*    - TODO(mh) multiplier currently not used, implement this
	*    - TODO(mh) deactivate only some components; allow for eg a blinking rotating light; on this state currently not possible
	*/
	class QSF_API_EXPORT ActiveToggleComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::ActiveToggleComponent" unique component ID
		static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
		static const uint32 MULTIPLIER;		///< "Multiplier" unique class property ID inside the class
		static const uint32 TIME_ON;		///< "TimeOn" unique class property ID inside the class
		static const uint32 TIME_OFF;		///< "TimeOff" unique class property ID inside the class


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
		inline ActiveToggleComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ActiveToggleComponent();

		/**
		*  @brief
		*    Get the toggle multiplier
		*
		*  @return
		*    The multiplier
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getMultiplier() const;

		/**
		*  @brief
		*    Set the multiplier for each step
		*
		*  @param[in] multiplier
		*    Multiplier for each step
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMultiplier(float multiplier);

		/**
		*  @brief
		*    Get the time on
		*
		*  @return
		*    The time on
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getTimeOn() const;

		/**
		*  @brief
		*    Set the time on
		*
		*  @param[in] timeOn
		*    Time on for the toggling
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTimeOn(float timeOn);

		/**
		*  @brief
		*    Get the time off
		*
		*  @return
		*    The time off
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getTimeOff() const;

		/**
		*  @brief
		*    Set the time off
		*
		*  @param[in] timeOff
		*    Time off for the toggling
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTimeOff(float timeOff);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const qsf::JobArguments& jobArguments);

		/**
		*  @brief
		*    Activate/deactivate all components in this entity
		*
		*  @param[in] active
		*    Active state to set
		*  @param[in] force
		*    If "true", force active state application, ignoring cached internal state
		*/
		void setActiveAllComponents(bool active, bool force = false);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float			mMultiplier;		///< Multiplier of the rotation
		float			mTimeOn;			///< Time on
		float			mTimeOff;			///< Time off
		// Internal
		qsf::JobProxy	mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame
		float			mTime;				///< Timer for the toggle
		bool			mActiveState;		///< Current active state


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
#include "qsf/component/utility/ActiveToggleComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ActiveToggleComponent)
