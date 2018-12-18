// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/link/LinkDefinitions.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkAnchor;
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
	*    Link anchor adapter base class
	*/
	class QSF_API_EXPORT LinkAnchorAdapter
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		LinkAnchorAdapter(const LinkAnchorId& id);

		/**
		*  @brief
		*    Constructor
		*/
		LinkAnchorAdapter(const LinkAnchor& linkAnchor);

		/**
		*  @brief
		*    Return the anchor unique ID
		*/
		inline const LinkAnchorId& getId() const  { return mId; }


	//[-------------------------------------------------------]
	//[ Public virtual qsf::LinkAnchorAdapter methods         ]
	//[-------------------------------------------------------]
	public:
		virtual LinkAnchor& constructLinkAnchor() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		LinkAnchorId mId;


	};


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	namespace link
	{
		template<typename T>
		struct GetLinkAnchorAdapter
		{
			// This leads to a compiler error for unsupported types
			// -> If you get an error here, you have to include the correct link anchor adapter header for the object
			//    you want to create/get/destroy a link to. To support additional types, define your own adapter class.
			typedef typename T::INVALID_TYPE Adapter;
		};

		template<>
		struct GetLinkAnchorAdapter<LinkAnchor>
		{
			// Use the default link anchor adapter that just wraps the already existing link anchor
			typedef LinkAnchorAdapter Adapter;
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
