// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/asset/AssetSystemTypes.h"

#include <boost/noncopyable.hpp>


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
	*    Global asset ID provider base class
	*/
	class QSF_API_EXPORT GlobalAssetIdProvider : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GlobalAssetIdProvider();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GlobalAssetIdProvider methods     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return an unused global asset ID
		*
		*  @return
		*    Unused global asset ID, "qsf::getUninitialized<GlobalAssetId>()" if invalid
		*
		*  @note
		*    The default implementation always returns "qsf::getUninitialized<GlobalAssetId>()"
		*/
		virtual GlobalAssetId acquireGlobalAssetId();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		GlobalAssetIdProvider();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
