// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/anchor/LinkContainer.h"
#include "qsf/link/LinkDefinitions.h"
#include "qsf/reflection/CampClass.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>


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
	*    Link anchor base class
	*/
	class QSF_API_EXPORT LinkAnchor : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LinkSystem;	// Needs access to private methods
	friend class LinkProxy;		// Needs access to private methods


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline LinkAnchor(const LinkAnchorId& anchorId);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LinkAnchor();

		/**
		*  @brief
		*    Return the anchor's unique ID
		*/
		inline const LinkAnchorId& getId() const;

		/**
		*  @brief
		*    Get a link container by its associated link type ID
		*/
		LinkContainer* getLinkContainerById(const StringHash& linkTypeId, link::Direction linkDirection) const;

		/**
		*  @brief
		*    Get a link container by its type ID and the remote anchor ID
		*/
		Link* getLinkById(const LinkAnchorId& anchorId, const StringHash& linkTypeId, link::Direction linkDirection) const;

		/**
		*  @brief
		*    Return links connected to this anchor, in the given link direction
		*/
		inline const std::vector<Link*>& getLinks(link::Direction linkDirection) const;

		/**
		*  @brief
		*    Collect all links connected to this anchor, in both directions
		*/
		void collectAllLinks(std::vector<Link*>& outLinks);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::LinkAnchor methods             ]
	//[-------------------------------------------------------]
	protected:
		inline virtual void onStructuralChange();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// For friend LinkSystem
		void insertLink(Link& link, link::Direction linkDirection);
		void removeLink(Link& link, link::Direction linkDirection);

		// For friend LinkProxy
		void registerLinkProxy(LinkProxy& linkProxy);
		void unregisterLinkProxy(LinkProxy& linkProxy);

		// Internal
		LinkContainer* internalGetLinkContainer(uint32 linkTypeId, link::Direction linkDirection, bool create);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<Link*>						  LinkArray;
		typedef std::unordered_map<uint32, LinkContainer> LinkContainerMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const LinkAnchorId mId;					///< This anchor's unique ID, const because it may not be modified after constructor
		LinkArray		   mLinks[2];			///< List of connected links, index is a link direction; forward links are owned by this anchor, backward link are owned by the remote anchor
		LinkContainerMap   mLinkContainers[2];	///< Maps of all link containers, index is a link direction; key is the link type ID, value a link container


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/link/anchor/LinkAnchor-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LinkAnchor)
