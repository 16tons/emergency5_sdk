// Copyright (C) 2012-2018 Promotion Software GmbH


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
namespace Ogre
{
	class StableCsmShadowCameraSetup;
}
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
	*    Cascaded Shadow Map (CSM) high level shadow setup
	*
	*  @todo
	*    - TODO(co) Port to OGRE 2.0
	*/
	class ShadowSetupCsm : public ShadowSetup
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		ShadowSetupCsm();
		virtual ~ShadowSetupCsm();

		Ogre::StableCsmShadowCameraSetup* getOgreCsmShadowCameraSetup() const;


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


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::StableCsmShadowCameraSetup* mOgreCsmShadowCameraSetup;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
