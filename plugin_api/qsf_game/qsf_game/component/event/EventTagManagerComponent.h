// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/component/event/EventTagComponent.h"

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
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
		*    QSF game event tag manager component class
		*
		*  @note
		*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
		*/
		class QSF_GAME_API_EXPORT EventTagManagerComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class EventTagComponent;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "em5::EventTagManagerComponent" unique component ID

			// Shortcuts
			typedef EventTagComponent::EventTag EventTag;
			typedef EventTagComponent::EventTagArray EventTagArray;

			typedef std::vector<EventTagComponent*> EventTagComponentArray;	///< Vector of event tag components

			struct EventTagData
			{
				EventTagComponentArray mEventTagComponents;
				std::string	mEventTag;
			};

			typedef std::unordered_map<uint32, EventTagData> EventTagMap;	///< Map with hashed event tag string as key


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static const EventTagComponentArray& getEventTagComponentsByTag(const qsf::StringHash& eventTag, Map& map);
			static EventTagComponentArray getEventTagComponentsStartsWithTag(const std::string& eventTag, Map& map);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			inline explicit EventTagManagerComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline ~EventTagManagerComponent();

			/**
			*  @brief
			*    Get array of event tag components by event tag
			*
			*  @param[in] eventTag
			*    The event tag
			*
			*  @return
			*    The array of all event tag components including the given event tag; this is empty if the tag is used no where
			*/
			const EventTagComponentArray& getEventTagComponentsByTag(const qsf::StringHash& eventTag) const;

			/**
			*  @brief
			*    Get array of event tag components by event tag.
			*
			*  @param[in] eventTag
			*    The event tag
			*
			*  @return
			*    The array of all event tag components including the given event tag. Includes the following numbers like "_01"; this is empty if the tag is used no where
			*/
			EventTagComponentArray getEventTagComponentsStartsWithTag(const std::string& eventTag) const;

			qsf::Entity* getEntityByTag(const qsf::StringHash& eventTag) const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Insert event tag component to the manager
			*/
			void insertEventTagComponent(EventTagComponent& eventTagComponentToBeInsert);

			/**
			*  @brief
			*    Remove event tag component of the manager
			*/
			void removeEventTagComponent(EventTagComponent& eventTagComponentToRemove);

			/**
			*  @brief
			*    Add event tag to event tag component
			*/
			void addEventTagToEventTagComponent(EventTagComponent& eventTagComponent, const qsf::NamedIdentifier& eventTag);

			/**
			*  @brief
			*    Remove event tag of event tag component
			*/
			void removeEventTagOfEventTagComponent(EventTagComponent& eventTagComponent, const qsf::StringHash& eventTag);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EventTagMap mEventTagMap;	///< Map of event tag components


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/event/EventTagManagerComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::EventTagManagerComponent)
