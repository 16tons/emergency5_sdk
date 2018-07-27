// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Proxy.h"
#include "qsf/link/LinkDefinitions.h"

#include <boost/function.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkAnchor;
	class LinkContainer;
	class LinkAnchorAdapter;
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
	*    Link proxy class
	*/
	class QSF_API_EXPORT LinkProxy : public Proxy
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LinkAnchor;
	friend class LinkContainer;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/** Function binding signature for callbacks */
		typedef void CallbackSignature(const Link&);

		/** Boost function binding definition for callbacks */
		typedef boost::function<CallbackSignature> CallbackBinding;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline LinkProxy();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~LinkProxy();

		/**
		*  @brief
		*    Return this proxy's link direction
		*/
		inline link::Direction getLinkDirection() const;

		//[-------------------------------------------------------]
		//[ Proxy registration                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Register at the given link anchor
		*/
		template<typename T>
		void registerAt(const T& anchorObject, const StringHash& linkTypeId, link::Direction linkDirection);

		/**
		*  @brief
		*    Register at the given link anchor
		*/
		void registerAtAdapter(const LinkAnchorAdapter& adapter, const StringHash& linkTypeId, link::Direction linkDirection);

		/**
		*  @brief
		*    Unregister this link proxy
		*/
		void unregister();

		/**
		*  @brief
		*    Remove the callback issued after a link was added
		*/
		inline void clearCallbackForLinkAdded();

		/**
		*  @brief
		*    Remove the callback issued after a link got removed
		*/
		inline void clearCallbackForLinkRemoved();

		/**
		*  @brief
		*    Add a callback issued after a link was added
		*/
		inline void setCallbackForLinkAdded(const CallbackBinding& functionBinding);

		/**
		*  @brief
		*    Add a callback issued after a link got removed
		*/
		inline void setCallbackForLinkRemoved(const CallbackBinding& functionBinding);


		//[-------------------------------------------------------]
		//[ Connections (read-access)                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the map of link connections from this proxy
		*/
		const LinkConnectionMap& getLinkConnectionMap() const;

		/**
		*  @brief
		*    Return the first link to any remote anchor
		*/
		Link* getLink() const;

		/**
		*  @brief
		*    Return the link to a given remote anchor
		*/
		template<typename T>
		Link* getLink(const T& anchorObject) const;

		/**
		*  @brief
		*    Return the remote anchor of the first link connection, or a null pointer if there is none
		*/
		LinkAnchor* getRemoteAnchor() const;

		/**
		*  @brief
		*    Return the remote anchor of the first link connection, or a null pointer if there is none
		*/
		template<typename T>
		T* getRemoteAnchor() const;

		/**
		*  @brief
		*    Return the remote anchor unique ID of the first link connection, or qsf::getUninitialized<LinkAnchorId>() if there is none
		*/
		const LinkAnchorId& getRemoteAnchorId() const;

		//[-------------------------------------------------------]
		//[ Connections (write-access)                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Connect to the given object; depending on the link type, an existing connecting gets replaced or another one is added
		*/
		template<typename T>
		Link* connectTo(const T& anchorObject);

		/**
		*  @brief
		*    Disconnect from the given object
		*
		*  @return
		*    "true" in case an existing connection of the associated link type was removed, "false" otherwise
		*/
		template<typename T>
		bool disconnectFrom(const T& anchorObject);

		/**
		*  @brief
		*    Remove all connections from/to this proxy
		*/
		void disconnectAll();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline void setConnectedContainer(LinkContainer* linkContainer);
		inline void invalidate();

		inline void onLinkAdded(const Link& link);
		inline void onLinkRemoved(const Link& link);

		Link* getLinkByAnchor(const LinkAnchorAdapter& adapter) const;
		Link* connectToAnchor(const LinkAnchorAdapter& adapter);
		bool disconnectFromAnchor(const LinkAnchorAdapter& adapter);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			mLinkTypeId;
		link::Direction	mLinkDirection;
		LinkAnchor*		mLinkAnchor;
		LinkContainer*	mConnectedContainer;

		CallbackBinding mLinkAddedCallback;
		CallbackBinding mLinkRemovedCallback;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/link/LinkProxy-inl.h"
