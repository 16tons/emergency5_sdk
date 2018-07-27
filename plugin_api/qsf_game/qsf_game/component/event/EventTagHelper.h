// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/map/Entity.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkComponent;
	class Layer;
	namespace game
	{
		class EventTagManagerComponent;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF game event tag helper
		*/
		class QSF_GAME_API_EXPORT EventTagHelper
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] contextName
			*    Context name to display in logged output and message boxes; has no further meaning
			*/
			explicit EventTagHelper(const std::string& contextName);

			/**
			*  @brief
			*    Destructor
			*/
			~EventTagHelper();

			/**
			*  @brief
			*    Check if there were any constraint violations so far
			*/
			bool everythingAsExpected() const;

			/**
			*  @brief
			*    Reset tracking of constraint violations
			*/
			void reset();

			/**
			*  @brief
			*    Shortcut method to check if a certain entity has the given event tag
			*/
			bool checkEntityHasEventTag(const qsf::Entity& entity, const std::string& tag) const;

			//[-------------------------------------------------------]
			//[ Single entity                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get a single event-tagged entity needed
			*
			*  @param[in] outEntity
			*    The entity reference is stored in this pointer
			*  @param[in] tag
			*    The event tag to search for
			*/
			void acquireTaggedEntity(qsf::Entity*& outEntity, const std::string& tag);
			void acquireTaggedEntity(qsf::WeakPtr<Entity>& outEntity, const std::string& tag);

			/**
			*  @brief
			*    Get a single event-tagged entity needed
			*
			*  @param[in] outEntity
			*    The entity reference is stored in this pointer
			*  @param[in] tag
			*    The event tag to search for
			*
			*  @return
			*    Will return "true" if the entity was found, or "false" otherwise (in this case a log message is also provided)
			*
			*  @remarks
			*    Will not throw any errors. Might display message boxes in development build.
			*/
			bool tryAcquireTaggedEntity(qsf::Entity*& outEntity, const std::string& tag);
			bool tryAcquireTaggedEntity(qsf::WeakPtr<qsf::Entity>& outEntity, const std::string& tag);

			/**
			*  @brief
			*    Get a random event-tagged entity
			*
			*  @param[in] tag
			*    The event tag to search for
			*
			*  @return
			*    The entity pointer, or a null pointer if none was found
			*
			*  @remarks
			*    Will not throw any errors or display messages
			*/
			qsf::Entity* getRandomTaggedEntity(const std::string& tag);

			/**
			*  @brief
			*    Get a single event-tagged entity needed, that is a child of the given entity
			*
			*  @param[out] outEntity
			*    The entity reference is stored in this pointer
			*  @param[in] parentEntity
			*    The entity parent entity whose child is wanted
			*  @param[in] tag
			*    The event tag to search for
			*/
			bool getFirstChildEntityWithEventTag(qsf::WeakPtr<qsf::Entity>& outEntity, const qsf::WeakPtr<qsf::Entity>& parentEntity, const std::string& tag) const;
			bool getFirstChildEntityWithEventTag(qsf::WeakPtr<qsf::Entity>& outEntity, const qsf::Entity& parentEntity, const std::string& tag) const;
			bool getFirstChildEntityWithEventTag(qsf::Entity*& outEntity, const qsf::Entity& parentEntity, const std::string& tag) const;
			bool getFirstChildEntityWithEventTag(qsf::LinkComponent*& linkComponent, const qsf::Entity& parentEntity, const std::string& tag) const;
			bool getFirstChildEntityWithEventTag(qsf::LinkComponent*& linkComponent, const qsf::LinkComponent& parentLinkComponent, const std::string& tag) const;

			//[-------------------------------------------------------]
			//[ Multiple entities                                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get multiple event-tagged entities, throws exception on constraint violations
			*
			*  @param[out] outEntities
			*    The entities found
			*  @param[in] tag
			*    The event tag to search for. There can be a boost::format placeholder in the tag. In this case, the
			*    utility will search for a sequence of entities, e.g.: "TAG_%01i" => TAG_01, TAG_02, TAG_03, ... until
			*    an entity is no longer found. Please note the entity number always starts at 1!
			*  @param[in] minimumNumberNeeded
			*    The minimum number of entities needed; if there are less found, the method will throw an exception
			*/
			void acquireTaggedEntities(std::vector<qsf::Entity*>& outEntities, const std::string& tag, uint32 minimumNumberNeeded = 0);
			void acquireTaggedEntities(std::vector<qsf::WeakPtr<qsf::Entity>>& outEntities, const std::string& tag, uint32 minimumNumberNeeded = 0);

			/**
			*  @brief
			*    Tries to retrieve multiple event-tagged entities without throwing exceptions
			*
			*  @param[out] outEntities
			*    The entities found
			*  @param[in] tag
			*    The event tag to search for. There can be a boost::format placeholder in the tag. In this case, the
			*    utility will search for a sequence of entities, e.g.: "TAG_%01i" => TAG_01, TAG_02, TAG_03, ... until
			*    an entity is no longer found. Please note the entity number always starts at 1!
			*  @param[in] minimumNumberNeeded
			*    The minimum number of entities needed
			*
			*  @remarks
			*    Will not throw any errors. Might display message boxes in development build.
			*
			*  @return
			*    Will return true if there have been found enough entities or false if there were some missing (a log message is provided then)
			*/
			bool tryAcquireTaggedEntities(std::vector<qsf::Entity*>& outEntities, const std::string& tag, uint32 minimumNumberNeeded = 0);
			bool tryAcquireTaggedEntities(std::vector<qsf::WeakPtr<qsf::Entity>>& outEntities, const std::string& tag, uint32 minimumNumberNeeded = 0);

			//[-------------------------------------------------------]
			//[ Entities starting with tag                            ]
			//[-------------------------------------------------------]
			void acquireEntitiesWithStartingTag(std::vector<qsf::WeakPtr<qsf::Entity>>& outEntities, const std::string& eventTagPrefix);

			bool startsWithEventTag(const qsf::Entity& entity, const std::string& eventTagPrefix) const;

			/**
			*  @brief
			*    Returns the index at the end of an numbered event tag. Using for numbered eventTags like "gangster_3".
			*/
			int getIndexFromEventTag(const qsf::Entity& entity, const std::string& eventTag) const;

			//[-------------------------------------------------------]
			//[ Tagged entities inside layer                          ]
			//[-------------------------------------------------------]
			qsf::Entity& acquireTaggedEntityInLayer(const std::string& eventTag, const qsf::Layer& layer);
			void acquireTaggedEntityInLayer(qsf::Entity*& outEntity, const std::string& eventTag, const qsf::Layer& layer);
			void acquireTaggedEntitiesInLayer(std::vector<qsf::Entity*>& outEntities, const std::string& eventTag, const qsf::Layer& layer);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			bool internalTryAcquireTaggedEntity(qsf::Entity*& outEntity, const std::string& tag, bool raiseErrors);
			std::string getMessagePrefix() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EventTagManagerComponent& mEventTagManager;
			std::string				  mContextName;
			bool					  mAnythingNotAsExpected;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
