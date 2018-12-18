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
	class ShadowTextureDefinition;
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
	*    Parallel Split Shadow Map (PSSM, aka Cascaded Shadow Map (CSM)) high level shadow setup
	*/
	class QSF_API_EXPORT ShadowSetupPssm : public ShadowSetup
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const float DEFAULT_PSSM_LAMBDA;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		ShadowSetupPssm();
		virtual ~ShadowSetupPssm();
		void setPssmLambda(float pssmLambda);


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
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<Ogre::ShadowTextureDefinition*> OgreShadowTextureDefinitions;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float						 mPssmLambda;
		OgreShadowTextureDefinitions mOgreShadowTextureDefinitions;	///< To be able to update the PSSM lambda later on dynamically during runtime, we need to remember the OGRE shadow texture definitions because the OGRE API doesn't allow a dynamic update of this lambda, don't destroy the instances


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
