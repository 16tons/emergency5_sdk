// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Proxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class DebugDrawManager;
	class DebugDrawObject;
	class DebugDrawRequest;
	class DebugDrawLifetimeData;
	class DebugDrawOwnerData;
	class Component;
	class Transform;
	class TransformComponent;
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
	*    Debug draw proxy class
	*/
	class QSF_API_EXPORT DebugDrawProxy : public Proxy
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class DebugDrawManager;	///< Debug draw manager may change private data
	friend class DebugDrawObject;	///< Debug draw object may invalidate the proxy


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline DebugDrawProxy();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~DebugDrawProxy();

		/**
		*  @brief
		*    Return the proxy's ID or "qsf::getUninitialized<uint32>()" if proxy is invalid
		*/
		uint32 getId() const;

		/**
		*  @brief
		*    Returns false if proxy is not registered or has become invalid
		*/
		inline bool isValid() const;

		/**
		*  @brief
		*    Register at a given debug draw manager
		*/
		bool registerAt(DebugDrawManager& debugDrawManager);

		/**
		*  @brief
		*    Register at the debug draw manager associated with the given component
		*/
		bool registerForComponent(const Component& component);

		/**
		*  @brief
		*    Unregister at the debug draw manager and remove debug draw requests
		*
		*  @return
		*    "true" if unregistration was successful, "false" if proxy was not registered at all
		*/
		inline bool unregister();

		/**
		*  @brief
		*    Clear all requests, without registration and especially without touching lifetime or owner data
		*/
		void clearRequests();

		/**
		*  @brief
		*    Add a debug draw request
		*
		*  @param[in] requests
		*    The debug draw request to add
		*/
		void addRequest(const DebugDrawRequest& request);

		/**
		*  @brief
		*    Detach the debug draw requests associated with this proxy
		*
		*  @remarks
		*    The requests will stay alive and are shown until their lifetime is over.
		*    You can use this method to create debug draws that are independent of any debug draw proxy.
		*    If you want them to use a limited lifetime, call "setLifetime" before detaching, otherwise they will be shown infinitely.
		*/
		void detachRequests();

		/**
		*  @brief
		*    Set lifetime data for the requests associated with this proxy
		*
		*  @param[in] lifetimeData
		*    Debug draw lifetime data
		*/
		void setLifetime(const DebugDrawLifetimeData& lifetimeData);

		/**
		*  @brief
		*    Set the lifetime in seconds for the requests associated with this proxy
		*
		*  @param[in] seconds
		*    Debug draw lifetime in seconds
		*
		*  @note
		*    - This is just an ease-of-use method
		*/
		void setLifetimeInSeconds(float seconds);

		/**
		*  @brief
		*    Set owner data for the requests associated with this proxy
		*
		*  @param[in] debugDrawOwnerData
		*    Gives information about the draw request's owner, used for allowing selection of an entity by the draw request's geometry
		*/
		void setOwner(const DebugDrawOwnerData& ownerData);

		/**
		*  @brief
		*    Set the transform of a debug draw request
		*
		*  @param[in] transform
		*    New transform of the debug draw request
		*
		*  @return
		*    "true" if all went fine and the debug draw request was updated, else "false"
		*/
		inline bool setTransform(const Transform& transform) const;

		/**
		*  @brief
		*    Set the transform of a debug draw request
		*
		*  @param[in] transformComponent
		*    Pointer to a transform component to copy the transform from; may be a null pointer
		*
		*  @return
		*    "true" if all went fine and the debug draw request was updated, else "false" (e.g. on null pointer parameter)
		*/
		bool trySetTransform(const TransformComponent* transformComponent) const;

		/**
		*  @brief
		*    Set rendering space of the draw request
		*
		*  @param[in] drawInClipSpace
		*    If true, render in clip space (with coordinates in [-1, 1]), otherwise in world space
		*
		*  @return
		*    "true" if all went fine and the debug draw request was updated, else "false"
		*/
		bool setDrawInClipSpace(bool drawInClipSpace) const;

		/**
		*  @brief
		*    Set the object in billboard state, orientates to the camera
		*
		*  @return
		*    "true" if all went fine and the debug draw request was updated, else "false"
		*/
		bool setDrawAsBillboard(bool drawAsBillboard);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline void invalidate();
		bool unregisterInternal();

		void setTransformInternal(const qsf::Transform& transform) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DebugDrawObject* mDebugDrawObject;	///< The debug draw object instance this proxy is assigned to, can be a null pointer, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawProxy-inl.h"
