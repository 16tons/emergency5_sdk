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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract manager interface
	*
	*  @remarks
	*    A "manager" can be part of a "system" and orchestrates a certain aspect of a "system". Examples:
	*    - Map test manager (part of the map)
	*    - View manager (part of the editor)
	*
	*    All managers should be derived from this class. This makes it easier
	*    to find all managers within e.g. the automatic code documentation.
	*/
	class QSF_API_EXPORT Manager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Manager();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Manager();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager-inl.h"
