// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Random.h>
#include <qsf/base/StringParser.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FireEventCandidateSearchHelper::FireEventCandidateSearchHelper() :
		mNumberOfEventTagComponents(0),
		mEventTagManager(nullptr)
	{
		// Nothing here
	}

	FireEventCandidateSearchHelper::~FireEventCandidateSearchHelper()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void FireEventCandidateSearchHelper::setDifficultyTagsByString(const std::string& eventTags)
	{
		// Clear the event list
		mEventTagComponents.clear();
		mEventTagStrings.clear();
		mNumberOfEventTagComponents = 0;

		// Get event tag manager
		qsf::game::EventTagManagerComponent& eventManager = getEventTagManager();

		// Splitt strings
		qsf::StringParser::splitString(eventTags, mEventTagStrings);

		// Gather all event components
		for (const std::string& eventTagString : mEventTagStrings)
		{
			QSF_CHECK(!eventTagString.empty(), "Event tag is not allowed to be empty, there's probably a double semicolon in the event tags of event " << "geteventname...", continue);

			EventTagComponentArray eventTagComponentArray = &eventManager.getEventTagComponentsByTag(qsf::StringHash(eventTagString));
			mEventTagComponents.push_back(eventTagComponentArray);
			mNumberOfEventTagComponents += static_cast<uint32>(eventTagComponentArray->size());
		}
	}

	bool FireEventCandidateSearchHelper::hasDifficultyTag(const qsf::game::EventTagComponent& eventTagComponent) const
	{
		// If no difficulty tags are set
		if (mEventTagStrings.empty())
			return true;

		for (const std::string& eventTagString : mEventTagStrings)
		{
			if (eventTagComponent.hasTag(eventTagString))
				return true;
		}

		return false;
	}

	bool FireEventCandidateSearchHelper::hasDifficultyTag(const qsf::Entity& entity) const
	{
		// Get event tag component
		const qsf::game::EventTagComponent* eventTagComponent = entity.getComponent<qsf::game::EventTagComponent>();
		if (nullptr == eventTagComponent)
			return false;

		return hasDifficultyTag(*eventTagComponent);
	}

	qsf::game::EventTagManagerComponent& FireEventCandidateSearchHelper::getEventTagManager()
	{
		// Instead of using this method, you may prefer the static version of "qsf::game::EventTagManagerComponent::getEventTagComponentsByTag()"

		if (nullptr == mEventTagManager)
		{
			mEventTagManager = QSF_MAINMAP.getCoreEntity().getOrCreateComponent<qsf::game::EventTagManagerComponent>();
			QSF_CHECK(mEventTagManager, "Could not create event tag manager core component", QSF_REACT_THROW);
		}

		return *mEventTagManager;
	}

	qsf::game::EventTagComponent* FireEventCandidateSearchHelper::getRandomEventTagComonent() const
	{
		if (mNumberOfEventTagComponents > 0)
		{
			uint32 eventTagArrayIndex = 0;
			uint32 eventTagComponentIndex = qsf::Random::getRandomUint(0, mNumberOfEventTagComponents - 1);

			for (EventTagComponentArray eventTagComponentArray : mEventTagComponents)
			{
				if (eventTagComponentIndex > eventTagComponentArray->size() - 1)
				{
					eventTagArrayIndex += 1;
					eventTagComponentIndex -= static_cast<uint32>(eventTagComponentArray->size());
				}
				else
				{
					// Get event tag component
					return mEventTagComponents[eventTagArrayIndex]->at(eventTagComponentIndex);
				}
			}
		}

		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
