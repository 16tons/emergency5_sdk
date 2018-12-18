// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/message/MessageProxy.h>


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
	*    EMERGENCY 5 freeplay event "complex fire"
	*/
	class EM5_API_EXPORT ComplexFireEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ComplexFireEventFactory;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::ComplexFireEventFactory" unique freeplay event ID

		typedef std::vector<qsf::Entity*> EntityArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ComplexFireEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComplexFireEvent();

		/**
		*  @brief
		*    Get number persons that have to die so that the event fails
		*/
		uint32 getDeadPersonsToFail() const;

		/**
		*  @brief
		*    Set number persons that have to die so that the event fails
		*/
		void setDeadPersonsToFail(uint32 personsToFail);

		/**
		*  @brief
		*    Get or set if persons get damaged
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonDamage, bool, mPersonDamage);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonMissing, bool, mPersonMissing);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Injuries, const std::string&, mInjuries);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives(const qsf::Entity& targetEntity);
		void startHintObservers(const qsf::Entity& targetEntity);
		bool inflameEntity(qsf::Entity& entity);
		qsf::Entity* spawnEntity(qsf::Entity& spawnpoint);
		void onAllPersonsFound();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64		mMetaEntityId;			///< ID of the meta entity or of the first inflamed entity
		EntityArray mInflameEntities;		///< Array of entities to be inflamed
		EntityArray mSpawnPointEntities;	///< Array of entities to be inflamed
		uint32		mDeadPersonsToFail;		///< Number of persons that have to die until the player has failed this event
		bool		mPersonDamage;			///< Should persons be damaged by this event?
		bool		mPersonMissing;			///< Should there be missing persons?
		std::string	mInjuries;

		// Intern
		bool				mAllPersonsFound;
		qsf::MessageProxy	mMissingPersonMessageProxy;
		qsf::Entity*		mIconEntity;
		std::vector<uint64> mMissingPersonEntityIds;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ComplexFireEvent)
