// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/gamelogic/GameLogic.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
}


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
	*    EMERGENCY 5 observer base class
	*/
	class EM5_API_EXPORT Observer : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplayEvent;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Observer();

		/**
		*  @brief
		*    Get the observer's name
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set the observer's name; the name has no special meaning internally, use it as you like, e.g. to identify a single observer
		*/
		inline void setName(const std::string& name);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Start up the game logic
		*/
		virtual bool onStartup() override;

		/**
		*  @brief
		*    Shut down the game logic
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		explicit Observer(uint32 typeId);

		/**
		*  @brief
		*     Get parent event
		*/
		FreeplayEvent* getParentEvent() const;

		/**
		*  @brief
		*    Create observer
		*/
		template<typename T>
		T& createObserver(uint64 entityId);

		/**
		*  @brief
		*    Get observer
		*/
		template<typename T>
		T* getObserver(uint64 entityId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FreeplayEvent* mParentEvent;	///< Pointer to the owning freeplay event; do not destroy the instance
		std::string    mName;			///< An observer's name is free for arbitrary use, it has no meaning internally


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/freeplay/observer/Observer-inl.h"
