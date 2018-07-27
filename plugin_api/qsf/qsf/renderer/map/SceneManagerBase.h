// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgreSceneManager.h>
#undef TRANSPARENT // OGRE includes an OS header resulting in the usual fancy definition-hell, undefine this MS Windows "ERROR"-definition
#undef ERROR // OGRE includes an OS header resulting in the usual fancy definition-hell, undefine this MS Windows "ERROR"-definition


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace detail
	{
		class OgreCompositorWorkspaceListener;
	}
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
	*    Specialized QSF OGRE base scene manager
	*/
	class QSF_API_EXPORT SceneManagerBase : public Ogre::SceneManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class detail::OgreCompositorWorkspaceListener;	///< Needs to be able to set "qsf::SceneManagerBase::mCurrentOgreCompositorPass"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		SceneManagerBase(const Ogre::String& instanceName, size_t numWorkerThreads, Ogre::InstancingThreadedCullingMethod threadedCullingMethod);
		virtual ~SceneManagerBase();
		Ogre::CompositorPass* getCurrentOgreCompositorPass() const;


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::SceneManager methods             ]
	//[-------------------------------------------------------]
	public:
		virtual const Ogre::String& getTypeName() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::SceneManager methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void _renderSingleObject(Ogre::Renderable* pRend, const Ogre::MovableObject *pMovableObject, bool casterPass, bool dualParaboloid);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::CompositorPass* mCurrentOgreCompositorPass;	///< Set by "qsf::detail::OgreCompositorWorkspaceListener"


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
