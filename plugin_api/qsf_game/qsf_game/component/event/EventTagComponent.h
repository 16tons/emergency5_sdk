// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/component/Component.h>

#include <vector>


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
		*    QSF game event tag component class
		*/
		class QSF_GAME_API_EXPORT EventTagComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class EventTagManagerComponent;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::EventTagComponent" unique component ID

			typedef NamedIdentifier EventTag;						// String hashed version of the event tag
			typedef std::vector<EventTag> EventTagArray;			// Array of stringhashed event tags
			typedef std::vector<std::string> EventTagStringArray;	// Array of string event tags


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			inline explicit EventTagComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline ~EventTagComponent();

			/**
			*  @brief
			*    Check whether the given tag is present
			*/
			bool hasTag(const StringHash& tag, bool checkLinked = false) const;
			bool hasTag(const std::string& tag, bool checkLinked = false) const;

			/**
			*  @brief
			*    Return array of event tags
			*
			*  @remarks
			*    Use this only when game simulation is running, otherwise the array will be empty
			*/
			inline const EventTagArray& getEventTags() const;

			/**
			*  @brief
			*    Check whether the given tag is a starting substring in the events.
			*    Return the matching string or "" if the eventTag is not part of the eventTags. Eg. "Hostage_01" = getTagStartsWithTag("Hostage")
			*/
			const std::string& getTagStartsWithTag(const std::string& eventTag) const;

			/**
			*  @brief
			*    Add a single event tag
			*/
			bool addEventTag(const std::string& tag);

			/**
			*  @brief
			*    Remove a single event tag
			*/
			bool removeEventTag(const StringHash& tag);

			/**
			*  @brief
			*    Set event tags as string (separated by semicolons)
			*/
			void setEventTagsByString(const std::string& eventTagString);

			/**
			*  @brief
			*    Get event tags as string (separated by semicolons)
			*/
			const std::string& getEventTagsAsString() const;

			/**
			*  @brief
			*    Add multiple event tags
			*/
			void addEventTagsByString(const std::string& eventTagString);

			/**
			*  @brief
			*    Remove multiple event tags
			*/
			void removeEventTagsByString(const std::string& eventTagString);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(BinarySerializer& serializer) override;
			virtual bool onStartup() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Helper method to set the event tag array
			*/
			void updateEventTagsFromString();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EventTagArray		mEventTagArray;
			mutable std::string	mEventTagString;
			mutable bool		mDirtyEventTagString;


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
#include "qsf_game/component/event/EventTagComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::EventTagComponent)
