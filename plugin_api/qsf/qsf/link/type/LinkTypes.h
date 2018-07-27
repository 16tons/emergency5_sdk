// Copyright (C) 2012-2017 Promotion Software GmbH


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
namespace qsf
{
	class StringHash;
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
	*    Plugin link types collection base class
	*
	*  @remarks
	*    All plugin link types collections should be derived from this class. This makes it easier
	*    to find all classes within e.g. the automatic code documentation.
	*/
	class QSF_API_EXPORT LinkTypes : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		static const uint32 NO_BASE;	///< A default for the "baseTypeId" parameter; it's simply getUninitialized<uint32>()


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Register a link type
		*/
		static void registerLinkType(const StringHash& typeId, const StringHash& baseTypeId, const StringHash& classIdentifier, bool multipleForwardConnections, bool multipleBackwardConnections);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
