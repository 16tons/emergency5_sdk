// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/pluginable/Pluginable.h"
#include "qsf/renderer/compositor/CompositorWorkspaceConfiguratorManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class CompositorPass;
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
	*    Abstract compositor workspace configurator pluginable
	*/
	class QSF_API_EXPORT CompositorWorkspaceConfigurator : public Pluginable<CompositorWorkspaceConfiguratorManager>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorWorkspaceConfiguratorManager;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CompositorWorkspaceConfigurator();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CompositorWorkspaceConfigurator methods ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the name of the compositor workspace definition to use
		*
		*  @return
		*    The name of the compositor workspace definition to use
		*/
		virtual std::string getCompositorWorkspaceDefinitionName() const = 0;

		// TODO(co) Document
		virtual void configureCompositorWorkspace() = 0;
		virtual void workspacePreUpdate(Ogre::CompositorWorkspace& ogreCompositorWorkspace) = 0;
		virtual void passPreExecute(Ogre::CompositorPass& ogreCompositorPass) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		explicit CompositorWorkspaceConfigurator(CompositorWorkspaceConfiguratorManager* compositorWorkspaceConfiguratorManager);


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CompositorWorkspaceConfigurator)
