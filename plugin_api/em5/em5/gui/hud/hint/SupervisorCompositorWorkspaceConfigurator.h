// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/job/JobProxy.h>
#include <qsf/renderer/compositor/CompositorWorkspaceConfigurator.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class CompositorManager2;
}
namespace qsf
{
	class Map;
	class JobArguments;
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
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 supervisor compositor workspace configurator pluginable
	*
	*  @note
	*    - Compositing component counterpart is "em5::SupervisorCompositingComponent"
	*
	*  @todo
	*    - TODO(co) Cleanup
	*/
	class SupervisorCompositorWorkspaceConfigurator : public qsf::CompositorWorkspaceConfigurator
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit SupervisorCompositorWorkspaceConfigurator(qsf::CompositorWorkspaceConfiguratorManager* compositorWorkspaceConfiguratorManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SupervisorCompositorWorkspaceConfigurator();


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
		void onSettingsGroupPropertyChanged(const qsf::ParameterGroup& parameterGroup, uint32 propertyId);

		void updateAnimationJob(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const qsf::compositing::CompositingSettingsGroup& mCompositingSettingsGroup;
		Ogre::CompositorManager2*						  mOgreCompositorManager2;
		qsf::RendererSettingsGroup&						  mRendererSettingsGroup;

		// Heat compositor
		qsf::JobProxy	mAnimationUpdateJobProxy;	///< Update job proxy, called once a frame with time passed measured in game time
		float			mAnimationTimer;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SupervisorCompositorWorkspaceConfigurator)
