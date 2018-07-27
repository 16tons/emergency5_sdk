// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/pool/FreeplayEventPool.h"

#include <qsf/base/manager/Manager.h>

#include <stack>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEventFactory;
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
	*    EMERGENCY 5 freeplay event pool manager
	*/
	class EM5_API_EXPORT FreeplayEventPoolManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FreeplaySystem;


	//[-------------------------------------------------------]
	//[ Public declarations                                   ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<FreeplayEventFactory*> FactoryList;
		typedef std::unordered_map<std::string, FreeplayEventPool> EventPoolMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FreeplayEventPoolManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FreeplayEventPoolManager();

		/**
		*  @brief
		*    Return a reference to the map of all available event pools
		*/
		const EventPoolMap& getEventPoolMap() const;

		/**
		*  @brief
		*    Create all factories of the pool name to the factory array
		*
		*  @param[in] eventPoolName
		*    The name of the event pool we want to create
		*  @param[out] factoryList
		*    Contains all created factories
		*
		*  @return
		*    "True" when the event pool exists otherwise false
		*/
		bool createEventFactoriesFromPool(const std::string& eventPoolName, FactoryList& factoryList);

		bool removeEventFactoriesFromPool(const std::string& eventPoolName, FactoryList& factoryList);

		/**
		*  @brief
		*    Create an event factory by its events pool name and event name
		*/
		FreeplayEventFactory* createEventFactoryByName(const std::string& eventPoolName, const std::string& eventName);

		/**
		*  @brief
		*    Get the reference of the last used event factory
		*/
		FreeplayEventFactory* getLastEventFactory();

		/**
		*  @brief
		*    Load the assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Startup of the manager
		*/
		void startup();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct EventAddress
		{
			std::string mPoolKey;
			std::string mEventKey;
		};


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Get an event address from the given "Core" entry
		*/
		static void getAddressFromCoreString(EventAddress& outEventAddress, const std::string& coreString, const std::string& currentPoolKey);

		/**
		*  @brief
		*    Merge a derived tree with additional properties from its base tree
		*
		*  @param[in] basePTree
		*    The base tree to derive from
		*  @param[in/out] derivedPTree
		*    The derived tree that is changed
		*/
		static void mergePropertyTree(const boost::property_tree::ptree& basePTree, boost::property_tree::ptree& derivedPTree);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Insert tree into the event pool
		*/
		void loadEventPoolDefinition(const std::string& poolName, const boost::property_tree::ptree& poolPTree);

		/**
		*  @brief
		*    Insert event tree to a specific event pool
		*/
		void insertEventFactoryTree(const std::string& poolKey, const std::string& eventKey, const std::vector<boost::property_tree::ptree>& eventPTrees);

		/**
		*  @brief
		*    Derive all factories in the event pools
		*/
		void processEventDerivation();

		bool processEventDerivationRecursive(FreeplayEventPool::EventDefinition& eventDefinition, FreeplayEventPool& eventPool, uint32 cycleCounter);

		/**
		*  @brief
		*    Deserialize a freeplay event factory and inserts factory to manager
		*
		*  @return
		*    Pointer to the event factory if everything went fine, or a null pointer if none could be created
		*/
		FreeplayEventFactory* deserializeFreeplayFactory(const std::string& eventName, const boost::property_tree::ptree& factoryPTree);

		FreeplayEventFactory* deserializeFreeplayFactoryWithVariants(const std::string& eventName, const FreeplayEventPool::EventPTreeArray& eventVariants);

		/**
		*  @brief
		*    Clear the event pool
		*/
		void clearEventPool();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EventPoolMap		  mEventPools;			///< Map of event pools (key: pool name)
		FreeplayEventFactory* mLastEventFactory;	///< The factory from the latest created event. This reference is used to restart main events.


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
