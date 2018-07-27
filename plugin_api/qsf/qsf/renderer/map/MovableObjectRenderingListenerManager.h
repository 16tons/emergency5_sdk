// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class MovableObject;
}
namespace qsf
{
	class Component;
	class MovableObjectRenderingListener;
	namespace detail
	{
		class OgreMovableObjectListener;
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
	*    OGRE movable object rendering listener manager
	*/
	class QSF_API_EXPORT MovableObjectRenderingListenerManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MapSceneManager;					// Creates and manages the instance of this manager
		friend class MovableObjectRenderingListener;	// Calls "addListener()" and "removeListener()"
		friend class Map;								// Calls "removeAllListeners()"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Setup OGRE movable object listener
		*
		*  @param[in] component
		*    Component wrapping up the OGRE movable object
		*  @param[in] ogreMovableObject
		*    Currently rendered OGRE movable object
		*/
		void setupOgreMovableObjectListener(Component& component, Ogre::MovableObject& ogreMovableObject);

		/**
		*  @brief
		*    Called when a component will be rendered by using a given camera
		*
		*  @param[in] component
		*    Currently rendered component
		*  @param[in] ogreCamera
		*    Currently used OGRE camera
		*
		*  @note
		*    - Don't use this method if you don't know what you're doning
		*/
		void objectRendering(Component& component, Ogre::Camera& ogreCamera);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<Component*, detail::OgreMovableObjectListener*> ComponentListenersMap;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		MovableObjectRenderingListenerManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MovableObjectRenderingListenerManager();

		/**
		*  @brief
		*    Add listener
		*
		*  @param[in] component
		*    Component to add a listener for
		*  @param[in] movableObjectRenderingListener
		*    Listener instance which is going to spy on the provided component instance
		*
		*  @return
		*    "true" if all went fine and the listener was added to the component, else "false" (component/listener is already added?)
		*/
		bool addListener(Component& component, MovableObjectRenderingListener& movableObjectRenderingListener);

		/**
		*  @brief
		*    Remove listener
		*
		*  @param[in] component
		*    Component to remove a listener from
		*  @param[in] movableObjectRenderingListener
		*    Listener instance which is going to no longer on the provided component instance
		*
		*  @return
		*    "true" if all went fine and the listener was removed from the component, else "false" (component/listener is not added in the first place?)
		*/
		bool removeListener(Component& component, MovableObjectRenderingListener& movableObjectRenderingListener);

		/**
		*  @brief
		*    Remove all listeners
		*
		*  @param[in] component
		*    Component to remove all listeners from
		*/
		void removeAllListeners(Component& component);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ComponentListenersMap mComponentListenersMap;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
