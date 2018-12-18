// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/renderer/compositor/CompositorWorkspaceConfigurator.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class CompositorManager2;
}
namespace qsf
{
	class Map;
	class RendererSettingsGroup;
	class ParameterGroup;
	namespace compositing
	{
		class CompositingSettingsGroup;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Default compositor workspace configurator pluginable
		*
		*  @note
		*    - Compositing component counterpart is "qsf::compositing::DefaultCompositingComponent"
		*/
		class DefaultCompositorWorkspaceConfigurator : public CompositorWorkspaceConfigurator
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit DefaultCompositorWorkspaceConfigurator(CompositorWorkspaceConfiguratorManager* compositorWorkspaceConfiguratorManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DefaultCompositorWorkspaceConfigurator();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::CompositorWorkspaceConfigurator methods ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getCompositorWorkspaceDefinitionName() const override;
			virtual void configureCompositorWorkspace() override;
			virtual void workspacePreUpdate(Ogre::CompositorWorkspace& ogreCompositorWorkspace) override;
			virtual void passPreExecute(Ogre::CompositorPass& ogreCompositorPass) override;


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
			const CompositingSettingsGroup& mCompositingSettingsGroup;
			Ogre::CompositorManager2*		mOgreCompositorManager2;
			RendererSettingsGroup&			mRendererSettingsGroup;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::DefaultCompositorWorkspaceConfigurator)
