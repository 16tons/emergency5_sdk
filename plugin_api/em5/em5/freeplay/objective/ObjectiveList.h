// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/objective/Objective.h"

#include <boost/container/flat_map.hpp>


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
	*    EMERGENCY 5 objective list class
	*/
	class EM5_API_EXPORT ObjectiveList : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<uint32, Objective*> ObjectiveInstanceMap;
		typedef std::pair<uint32, Objective*> ObjectiveInstancePair;

		typedef boost::container::flat_map<uint32, uint32> ObjectiveOrderPriorityMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ObjectiveList();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ObjectiveList();

		const ObjectiveInstanceMap& getObjectives() const;
		void getObjectivesSorted(std::vector<Objective*>& outObjectives) const;

		void clearObjectives();
		void insertObjective(Objective& objective);

		Objective* getObjectiveByTypeId(uint32 typeId);
		const Objective* getObjectiveByTypeId(uint32 typeId) const;

		Objective& getOrCreateObjective(uint32 typeId, Objective::ObjectiveType type, uint32 eventId, uint32 orderPriority = 100);

		void removeObjective(uint32 typeId);

		void defineObjectiveOrderPriority(uint32 typeId, uint32 orderPriority);
		void defineObjectiveOrderPriorities(const std::vector<uint32>& typeIds, uint32 orderPriorityStart = 10, uint32 orderPriorityStep = 10);

		void swapWith(ObjectiveList& otherList);

		uint32 countObjectivesOfType(Objective::ObjectiveType type) const;

		bool checkSuccess() const;
		bool checkFailure() const;

		void debugLogFailure() const;

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void countObjectivesOfType(Objective::ObjectiveType type, uint32& number, uint32& accomplished) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ObjectiveInstanceMap mObjectives;
		ObjectiveOrderPriorityMap mOrderPriorityMap;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
