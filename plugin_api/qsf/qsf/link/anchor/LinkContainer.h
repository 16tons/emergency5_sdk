// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/link/LinkDefinitions.h"

#include <boost/optional.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkProxy;
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
	*    Link container class
	*/
	class QSF_API_EXPORT LinkContainer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline LinkContainer();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~LinkContainer();

		/**
		*  @brief
		*    Return the internal link connection map
		*/
		inline const LinkConnectionMap& getLinkConnectionMap() const;

		/**
		*  @brief
		*    Returns "true" if the link container a empty and has no slot
		*/
		inline bool canBeRemoved() const;

		//[-------------------------------------------------------]
		//[ Links                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Find a link by the given remote anchor ID
		*/
		inline Link* getLinkById(const LinkAnchorId& anchorId) const;

		/**
		*  @brief
		*    Insert a link into the link container
		*/
		void insertLink(Link& link, link::Direction linkDirection);

		/**
		*  @brief
		*    Remove a link from the link container
		*/
		bool removeLink(Link& link, link::Direction linkDirection);

		/**
		*  @brief
		*    Build a list of all links in this container
		*/
		void collectLinks(std::vector<Link*>& outLinks, boost::optional<uint32> requiredLinkTypeId = boost::optional<uint32>()) const;

		//[-------------------------------------------------------]
		//[ Link proxy                                            ]
		//[-------------------------------------------------------]
		void registerLinkProxy(LinkProxy& linkProxy);
		void unregisterLinkProxy(LinkProxy& linkProxy);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<LinkProxy*> LinkProxyArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LinkConnectionMap mLinkConnectionMap;	///< Map of links; key is the remote anchor ID, value is a link instance
		LinkProxyArray	  mLinkProxies;			///< Link proxies registered here; no null pointers allowed, do not destroy the instances
		uint32			  mPreventRemoval;		///< If not zero, this link container must not to be removed


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/link/anchor/LinkContainer-inl.h"
