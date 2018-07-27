// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/component/Component.h>
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
	*    EMERGENCY 2017 Stake Object Component
	*/
	class EM5_API_EXPORT StakeComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::StakeComponent" unique component ID


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
		explicit StakeComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StakeComponent();

		qsf::Transform getActionTransform();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RescueTime, qsf::Time, mRescueTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WitchEntity, uint64, mWitchEntityId);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ActionPosition, uint64, mActionPositionEntityId);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(canBeIgnited, bool, mCanBeIgnited);

		void turnOffIcons();

		void setWitchHidden(bool hidden);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating);
		virtual void onSetActive(bool active);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void internalSetActive(bool active);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time	mRescueTime;
		uint64		mWitchEntityId;		// Id of the witch person
		uint64		mActionPositionEntityId;
		bool		mCanBeIgnited;	// "true" at start, false if a guardsman rescuing is active or finished
		bool		mInternalActive;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::StakeComponent)
