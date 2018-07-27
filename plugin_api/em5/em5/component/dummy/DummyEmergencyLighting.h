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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 dummy emergency lighting class: Signals to AI whether a vehicle has emergency lighting on (and thus may ignore certain traffic rules)
	*/
	class EM5_API_EXPORT DummyEmergencyLighting : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;					///< "em5::DummyEmergencyLighting" unique component ID
		static const uint32 EMERCENGY_LIGHT_PROPERTY_ID;	///< Unique CAMP ID of the boolean activation property


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
		explicit DummyEmergencyLighting(qsf::Prototype* prototype);

		bool getEmergencyLightingOn() const;
		void setEmergencyLightingOn(bool value);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mEmergencyLightingOn;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI();


	};


	// TODO(co) No inline implementation inside headers, please (we need to be able to keep an overview)
	inline DummyEmergencyLighting::DummyEmergencyLighting(qsf::Prototype* prototype) :
		Component(prototype),
		mEmergencyLightingOn(false)
	{}

	inline bool DummyEmergencyLighting::getEmergencyLightingOn() const
	{
		return mEmergencyLightingOn;
	}

	inline void DummyEmergencyLighting::setEmergencyLightingOn(bool value)
	{
		assignAndPromoteChange(mEmergencyLightingOn, value, EMERCENGY_LIGHT_PROPERTY_ID);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} //em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(em5::DummyEmergencyLighting);
