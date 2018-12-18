// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/base/idgenerator/ConsecutiveIdGenerator.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Objective;
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
	*    EMERGENCY 5 objective manager
	*
	*  @todo
	*    - TODO(fw): Think about making a template of this, similar to qsf::ElementManager, but without taking over the ownership of the elements registered
	*/
	class EM5_API_EXPORT ObjectiveManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ObjectiveList;			// May insert serialized objectives
		friend class FreeplaySystem;		// For serialization


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ObjectiveManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ObjectiveManager();

		/**
		*  @brief
		*    Find an objective by its ID
		*
		*  @return
		*    The objective or a null pointer if it was not found; do not destroy the instance
		*/
		Objective* getObjectiveById(uint32 id) const;

		void registerObjective(Objective& objective);
		void unregisterObjective(Objective& objective);

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint32, Objective*> ObjectiveMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::ConsecutiveIdGenerator<uint32> mIdGenerator;	///< Objective ID generator
		ObjectiveMap						mObjectives;	///< Objective, mapped by their IDs


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
