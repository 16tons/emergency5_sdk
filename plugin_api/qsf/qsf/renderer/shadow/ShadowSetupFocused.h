// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/shadow/ShadowSetup.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroup;
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
	*    Focused shadow mapping high level shadow setup
	*/
	class ShadowSetupFocused : public ShadowSetup
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		ShadowSetupFocused();
		virtual ~ShadowSetupFocused();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ShadowSetup methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void setupOgreCompositorShadowNode(Ogre::CompositorShadowNodeDef& ogreCompositorShadowNodeDef) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::ShadowSetup methods            ]
	//[-------------------------------------------------------]
	protected:
		virtual uint8 getNumberOfShadowMaps() const override;
		virtual void recreate() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
