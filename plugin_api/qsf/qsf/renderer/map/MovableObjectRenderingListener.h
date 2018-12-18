// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
}
namespace qsf
{
	class Component;
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
	*    OGRE movable object rendering listener
	*/
	class QSF_API_EXPORT MovableObjectRenderingListener : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MovableObjectRenderingListenerManager;
		friend class detail::OgreMovableObjectListener;		// Needs to call "objectRendering()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    State of the listener
		*/
		enum State
		{
			SLEEPING,	///< Point of interest movable object is currently not rendered and hence this listener is considered to be sleeping
			AWAKE		///< Point of interest movable object is considered to be rendered and hence the listener is awake and might do some work
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline MovableObjectRenderingListener();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MovableObjectRenderingListener();

		/**
		*  @brief
		*    Return whether or not the movable object rendering listener is registered
		*
		*  @return
		*    "true" if the movable object rendering listener is registered, else "false"
		*/
		inline bool isMovableObjectRenderingListenerRegistered() const;

		/**
		*  @brief
		*    Return the state of the listener
		*
		*  @return
		*    The state of the listener
		*/
		inline State getState() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Ease of use management                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the component the listener will spy on
		*
		*  @return
		*    The component the listener will spy on, can be a null pointer, do not destroy the instance
		*
		*  @note
		*    - The default implementation is empty and returns a null pointer
		*    - Ease of use method in case a listener just listens to a single movable object (the common use case)
		*/
		inline virtual Component* getMovableObjectRenderingListenerComponent();

		/**
		*  @brief
		*    Register movable object rendering listener
		*/
		virtual void registerMovableObjectRenderingListener();

		/**
		*  @brief
		*    Unregister movable object rendering listener
		*/
		virtual void unregisterMovableObjectRenderingListener();

		/**
		*  @brief
		*    Movable object has been destroyed
		*
		*  @note
		*    - The default implementation performs a shutdown of the object rendering in case the listener is awake
		*/
		inline virtual void onObjectDestroyed();

		//[-------------------------------------------------------]
		//[ Rendering                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Called when an OGRE movable object is about to be rendered for the first time after it has not been rendered for some time
		*
		*  @note
		*    - The default implementation is empty
		*/
		inline virtual void onObjectRenderingStartup();

		/**
		*  @brief
		*    Called when an OGRE movable object of the camera to be used for rendering
		*
		*  @param[in] component
		*    Currently rendered component
		*  @param[in] ogreCamera
		*    Currently used OGRE camera
		*
		*  @note
		*    - The default implementation is empty
		*/
		inline virtual void onObjectRendering(const Component& component, const Ogre::Camera& ogreCamera);

		/**
		*  @brief
		*    Called when an OGRE movable object is no longer be rendered for some time
		*
		*  @note
		*    - The default implementation is sets the listener state to sleeping
		*    - Automatically called in case the listener is unregistered while not sleeping, derived listeners with additional
		*      logic might e.g. call this method in case the object has not been visible for a certain amount of time
		*/
		inline virtual void onObjectRenderingShutdown();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when an OGRE movable object of the camera to be used for rendering
		*
		*  @param[in] component
		*    Currently rendered component
		*  @param[in] ogreCamera
		*    Currently used OGRE camera
		*/
		void objectRendering(const Component& component, const Ogre::Camera& ogreCamera);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// For debugging only
		bool mAllowRegistration;	///< Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mNumberOfConnections;	///< Number of prey the listener is spying on
		State  mState;					///< Current listener state

		// For debugging only
		uint32 mLastConnectionComponentId;	///< Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define

	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/MovableObjectRenderingListener-inl.h"
