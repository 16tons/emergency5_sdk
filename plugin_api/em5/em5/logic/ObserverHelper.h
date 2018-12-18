// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Observer helper can create observers
	*/
	class EM5_API_EXPORT ObserverHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit ObserverHelper(FreeplayEvent& freeplayEvent);

		/**
		*  @brief
		*    Destructor
		*/
		~ObserverHelper();


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create and insert a new observer associated with a given entity ID
		*/
		template<typename T>
		T& createObserver(uint64 entityId) const;

		/**
		*  @brief
		*    Create and insert a new observer associated to the core entity
		*/
		template<typename T>
		T& createGeneralObserver() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FreeplayEvent& mEvent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/logic/ObserverHelper-inl.h"
