// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
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
	*    Encapsulates the common owner date for debug draw requests
	*/
	class QSF_API_EXPORT DebugDrawOwnerData : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/** Default construct that defines no owner at all */
		inline explicit DebugDrawOwnerData();

		/** Construct with an owner component */
		inline explicit DebugDrawOwnerData(Component* component);

		/** Return owner component, or nullptr if none is set */
		inline Component* getComponent() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Component* mComponent;	///< Pointer to a component, or nullptr if none is set


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawOwnerData-inl.h"
