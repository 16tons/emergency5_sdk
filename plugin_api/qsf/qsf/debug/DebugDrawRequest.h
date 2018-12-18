// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class DebugDrawObject;
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
	*    Abstract debug draw request class
	*
	*  @remarks
	*    Contains the common parts that make up a debug draw request - but the base class is abstract.
	*    You need to have the data from the specific instances to actually draw something.
	*
	*  @note
	*    - Lightweight class whose instances may be copied via the default mechanisms
	*    - Spatial information is in local space by default
	*/
	class QSF_API_EXPORT DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Apply this request to a debug draw object instance
		*
		*  @param[in] debugDrawObject
		*    DebugDrawObject instance to apply the request to
		*/
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		inline DebugDrawRequest();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DebugDrawRequest();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawRequest-inl.h"
