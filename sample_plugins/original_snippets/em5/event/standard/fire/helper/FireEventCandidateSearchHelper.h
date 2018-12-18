// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/factory/FreeplayEventFactory.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class EventTagComponent;
		class EventTagManagerComponent;
	}
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
	*    EMERGENCY 5 fire event candidate search helper
	*/
	class EM5_API_EXPORT FireEventCandidateSearchHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<std::string>							EventTagStringArray;		// Array of string event tags
		typedef const std::vector<qsf::game::EventTagComponent*>*	EventTagComponentArray;		// Array of string hashed event tags
		typedef std::vector<EventTagComponentArray>					EventTagComponentArrayArray;// Array of arrays of ..


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FireEventCandidateSearchHelper();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FireEventCandidateSearchHelper();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void setDifficultyTagsByString(const std::string& eventTags);

		/**
		*  @brief
		*    Checks if the given EventTagComponent has one of the previously set event tags
		*/
		bool hasDifficultyTag(const qsf::game::EventTagComponent& eventTagComponent) const;
		bool hasDifficultyTag(const qsf::Entity& entity) const;

		/**
		*  @brief
		*    Gives a reference to the event tag manager
		*/
		qsf::game::EventTagManagerComponent& getEventTagManager();

		/**
		*  @brief
		*    Gives a reference to the event tag manager
		*/
		qsf::game::EventTagComponent* getRandomEventTagComonent() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EventTagStringArray					 mEventTagStrings;
		EventTagComponentArrayArray			 mEventTagComponents;
		uint32								 mNumberOfEventTagComponents;
		qsf::game::EventTagManagerComponent* mEventTagManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
