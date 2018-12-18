// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/link/LinkDefinitions.h"

#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkAnchor;
	class LinkTypeManager;
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
	*    Link system implementation
	*/
	class QSF_API_EXPORT LinkSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const LinkConnectionMap EMPTY_LINK_CONNECTION_MAP;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		LinkSystem();

		/**
		*  @brief
		*    Destructor
		*/
		~LinkSystem();

		/**
		*  @brief
		*    Return the link type manager instance
		*/
		inline LinkTypeManager& getLinkTypeManager();

		//[-------------------------------------------------------]
		//[ Link anchors                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the link anchor for a given object
		*/
		template<typename T>
		LinkAnchor* getLinkAnchor(const T& object) const;

		/**
		*  @brief
		*    Get a link anchor by its unique ID
		*/
		LinkAnchor* getLinkAnchorById(const LinkAnchorId& anchorId) const;

		/**
		*  @brief
		*    Get or create the link anchor for a given object
		*/
		template<typename T>
		LinkAnchor& getOrCreateLinkAnchor(const T& object) const;

		/**
		*  @brief
		*    Get or create a link anchor
		*/
		LinkAnchor& getOrCreateLinkAnchorByAdapter(const LinkAnchorAdapter& adapter);

		/**
		*  @brief
		*    Destroy the link anchor for a given object
		*/
		template<typename T>
		bool destroyLinkAnchor(const T& object);

		/**
		*  @brief
		*    Destroy a link anchor by its unique ID
		*/
		bool destroyLinkAnchorById(const LinkAnchorId& anchorId);

		//[-------------------------------------------------------]
		//[ Links                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get a link by its type and source and target link anchor adapter
		*/
		Link* getLinkByAdapters(const LinkAnchorAdapter& source, const LinkAnchorAdapter& target, const StringHash& linkTypeId) const;

		/**
		*  @brief
		*    Get a link by its type and source and target object
		*/
		template<typename A, typename B>
		Link* getLink(const A& source, const B& target, const StringHash& linkTypeId) const;

		/**
		*  @brief
		*    Get a link by its type and source and target link anchor adapter; create it if it does not exist yet
		*/
		Link* getOrCreateLinkByAdapters(const LinkAnchorAdapter& source, const LinkAnchorAdapter& target, const StringHash& linkTypeId);

		/**
		*  @brief
		*    Get a link by its type and source and target object; create it if it does not exist yet
		*/
		template<typename A, typename B>
		Link* getOrCreateLink(const A& source, const B& target, const StringHash& linkTypeId);

		/**
		*  @brief
		*    Destroy a link by its type and source and target link anchor adapter
		*/
		bool destroyLinkByAdapters(const LinkAnchorAdapter& source, const LinkAnchorAdapter& target, const StringHash& linkTypeId);

		/**
		*  @brief
		*    Destroy a link by its type and source and target object
		*/
		template<typename A, typename B>
		bool destroyLink(const A& source, const B& target, const StringHash& linkTypeId);

		/**
		*  @brief
		*    Destroy a link
		*/
		void destroyLink(Link& link);

		//[-------------------------------------------------------]
		//[ Link maps                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get a link connection map by a link anchor adapter, the link type and direction
		*/
		const LinkConnectionMap& getLinkConnectionMapByAdapter(const LinkAnchorAdapter& adapter, const StringHash& linkTypeId, link::Direction linkDirection) const;

		/**
		*  @brief
		*    Get a link connection map by a linked object, the link type and direction
		*/
		template<typename T>
		const LinkConnectionMap& getLinkConnectionMap(const T& object, const StringHash& linkTypeId, link::Direction linkDirection) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get the name of the system
		*
		*  @return
		*    Null terminated ASCII name of the system, never a null pointer, do not manipulate or destroy the returned name
		*/
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the system
		*
		*  @param[in] serverMode
		*    "true" if the system should start in server mode, else "false"
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Server mode means that e.g. there's no need for audio & visual stuff because there's no display or sound hardware in the first place
		*/
		virtual bool onStartup(bool serverMode) override;

		/**
		*  @brief
		*    Shutdown the system
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Get a link by its type and source and target anchor
		*/
		Link* internalGetLink(LinkAnchor& source, LinkAnchor& target, const StringHash& linkTypeId) const;

		/**
		*  @brief
		*    Get a link by its type and source and target anchor; create it if it does not exist yet
		*/
		Link* internalGetOrCreateLink(LinkAnchor& source, LinkAnchor& target, const StringHash& linkTypeId);

		/**
		*  @brief
		*    Destroy a link by its type and source and target anchor
		*/
		bool internalDestroyLink(LinkAnchor& source, LinkAnchor& target, const StringHash& linkTypeId);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::map<LinkAnchorId, LinkAnchor*> LinkAnchorMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LinkAnchorMap	 mLinkAnchorMap;
		LinkTypeManager& mLinkTypeManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/link/LinkSystem-inl.h"
