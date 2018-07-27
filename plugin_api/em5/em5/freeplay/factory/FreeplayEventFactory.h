// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/reflection/object/Object.h>

#include <boost/property_tree/ptree.hpp>	// TODO(co) Can we get rid of this heavy weighted include, please? (this does not mean just moving it to the cpp file while this header still has a real dependency)

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
	class FreeplayEventTriggerInfo;
}
namespace qsf
{
	class Entity;
	class Layer;
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
	*    EMERGENCY 5 freeplay event factory base class
	*
	*  @remarks
	*    A freeplay event factory implementation is responsible for generating freeplay event instances of its specific type,
	*    and also for checking whether the preconditions for that type of event are currently met.
	*/
	class EM5_API_EXPORT FreeplayEventFactory : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FreeplayEventPoolManager;		// May use the initialize method.


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<boost::property_tree::ptree> VariantArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		FreeplayEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FreeplayEventFactory();

		/**
		*  @brief
		*    Return event factory's unique ID
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return event's name
		*/
		inline const std::string& getEventName() const;

		/**
		*  @brief
		*    Return whether or not the factory is currently enabled for the game
		*/
		inline bool isEnabled() const;

		/**
		*  @brief
		*    Return trigger chance for this event factory
		*/
		inline float getTriggerChance() const;

		/**
		*  @brief
		*    Set trigger chance for this event factory
		*/
		inline void setTriggerChance(float triggerChance);

		/**
		*  @brief
		*    Get event density
		*/
		inline float getEventDensity() const;

		/**
		*  @brief
		*    Set event desity
		*/
		inline void setEventDensity(float eventDensity);

		/**
		*  @brief
		*    Get start event
		*/
		inline bool getStartEvent() const;

		/**
		*  @bief
		*    Set start event
		*/
		inline void setStartEvent(bool startEvent);

		/**
		*  @brief
		*    Get limit of appearances
		*/
		inline uint32 getMaxAppearances() const;

		/**
		*  @bief
		*    Set limit of appearances
		*/
		inline void setMaxAppearances(uint32 maxAppearances);

		/**
		*   @brief
		*    Get the event tag (used to search the target entity)
		*/
		inline const std::string& getEventTag() const;

		/**
		*   @brief
		*    Set the event tag (used to search the target entity)
		*/
		inline void setEventTag(const std::string& eventTag);

		/**
		*  @brief
		*    Get the variants of the freeplay event factory
		*/
		inline const VariantArray& getVariants() const;

		/**
		*  @brief
		*    Set the variants of the freeplay event factory
		*/
		inline void setVariants(const VariantArray& variants);

		/**
		*  @brief
		*    Get a random variant of the variant array
		*/
		const boost::property_tree::ptree& getRandomVariant() const;

		/**
		*  @brief
		*    Get a main variant of the variant array
		*/
		const boost::property_tree::ptree& getMainVariant() const;

		/**
		*  @brief
		*    Get a variant of the variant array by index
		*/
		const boost::property_tree::ptree& getVariantByIndex(uint32 index) const;

		/**
		*  @brief
		*    Compare this factory's unique ID to an other factory's unique ID
		*/
		inline bool operator ==(const FreeplayEventFactory& other) const;

		/**
		*  @brief
		*    Compare this factory's name to an std::string
		*/
		inline bool operator ==(const std::string& other) const;

		/**
		*  @brief
		*    Compare this factory's unique ID to a uint32
		*/
		inline bool operator ==(uint32 other) const;

		/**
		*  @brief
		*    Check if the event preconditions are met and if so, generate an event instance
		*
		*  @return
		*    Generated freeplay event if triggering succeeded, or a null pointer otherwise
		*/
		FreeplayEvent* tryTriggerEvent(const FreeplayEventTriggerInfo& triggerInfo);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		template<typename T> T& createAndInitializeEvent();
		FreeplayEvent& createAndInitializeEventByClassName(const std::string& className);

		void deserializeRandomFreeplayVariant(FreeplayEvent& freeplayEvent);
		void deserializeFreeplayVariantByIndex(FreeplayEvent& freeplayEvent, uint32 index);

		bool isEventAlreadyRunning() const;

		// Only to be used inside "tryTriggerEvent", will throw an exception otherwise
		const FreeplayEventTriggerInfo& getTriggerInfo() const;
		void setTriggerFailReason(const std::string& reason) const;

		/**
		*  @brief
		*    Try to get a randomly chosen layer to use for a event
		*
		*  @param[in] eventLayerName
		*    Optional name of a layer; if not empty, this specific layer will be returned
		*  @param[in] baseLayerName
		*    Name of a base layer containing sublayers that each represent an event location; one of the children will get chosen randomly
		*  @param[in] checkCandidateCallback
		*    Callback that has to check a candidate layer to return; if "false" is returned, the candidate won't be used a result
		*
		*  @return
		*    The layer or a null pointer if none could be found
		*/
		qsf::Layer* getCandidateEventLayer(const std::string& eventLayerName, const std::string& baseLayerName, const boost::function<bool(const qsf::Layer&)>& checkCandidateCallback) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Set event name and factory ID
		*/
		void initialize(const std::string& eventName);

		void initializeEventInstance(FreeplayEvent& freeplayEvent);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::string			 mEventTag;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::NamedIdentifier mName;				///< Name and ID of the event, will be default name for generated event instances
		bool				 mIsEnabled;		///< If "false", the factory is currently disabled and can't generate events right now

		bool				 mStartEvent;		///< Is the event allowed to be triggered at the start of a game. //TODO(jm): Make use of this!
		float				 mTriggerChance;	///< Probability factor for this event type to be triggered
		float				 mEventDensity;		///< Factor the event would add to the event would add to the event density
		uint32				 mMaxAppearances;	///< Limit for the number of total appearances of that event factory; 0 means no limit

		VariantArray		 mVariants;			///< Array of variants that the factory can create

		FreeplayEventTriggerInfo const* mTriggerInfo;	///< Currently used event trigger info; valid only inside of "tryTriggerEvent" calls and below, do not destroy the instance


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/freeplay/factory/FreeplayEventFactory-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FreeplayEventFactory)
