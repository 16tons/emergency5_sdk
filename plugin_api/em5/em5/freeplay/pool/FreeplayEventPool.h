// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/StringHash.h>

#include <boost/property_tree/ptree.hpp>	// TODO(co) Can we get rid of this heavy weighted include, please? (this does not mean just moving it to the cpp file while this header still has a real dependency)

#include <unordered_map>
#include <string>
#include <vector>


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
	*    EMERGENCY 5 freeplay event pool class
	*/
	class EM5_API_EXPORT FreeplayEventPool
	{


	//[-------------------------------------------------------]
	//[ Fiends                                                ]
	//[-------------------------------------------------------]
		friend class FreeplayEventPoolManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<boost::property_tree::ptree> EventPTreeArray;

		struct EventDefinition
		{
			enum ProcessingState
			{
				NOT_PROCESSED,
				PROCESSED,
				ERROR
			};

			std::string		 mName;				///< Name as denoted in the event definition JSON file
			EventPTreeArray	 mEventPTrees;		///< Property trees holding the actual event definition; there may be more than one if using multiple variants
			EventDefinition* mCoreDefinition;	///< Pointer to the core event definition; may be a null pointer
			ProcessingState	 mProcessingState;	///< Only for internal usage during load
		};
		typedef std::vector<EventDefinition*> EventDefinitionArray;
		typedef std::unordered_map<uint32, EventDefinition*> EventDefinitionMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		FreeplayEventPool();
		~FreeplayEventPool();

		/**
		*  @brief
		*    Get the name of the pool
		*/
		const std::string& getName() const;

		/**
		*  @brief
		*    Return a reference to the array of event definitions
		*/
		const EventDefinitionArray& getEventDefinitions() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*   Clear all "mTreeMap" and all property trees of it
		*/
		void clear();

		/**
		*  @brief
		*    Insert a pair of the key and property tree to the map
		*/
		bool insertEventPTrees(const std::string& key, const EventPTreeArray& factoryPTree);

		/**
		*  @brief
		*    Get an event definition by its name
		*/
		EventDefinition* getEventDefinition(const qsf::StringHash& key);

		/**
		*  @brief
		*    Return a reference to the map of event definitions by their name
		*/
		const EventDefinitionMap& getEventDefinitionMap() const;

		/**
		*  @brief
		*    Set the name of the pool
		*/
		void setName(const std::string& name);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EventDefinitionArray mEventDefinitionArray;	///< Array of event definitions
		EventDefinitionMap	 mEventDefinitionMap;	///< Map of event definitions; key is the name of the factory
		std::string			 mName;					///< Name of the event pool


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
