// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/manager/Manager.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}
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
	*    EMERGENCY 5 freeplay event factory manager
	*/
	class EM5_API_EXPORT FreeplayEventFactoryManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplaySystem;			// May insert and remove freeplay events
	friend class FreeplayEventPoolManager;	// May create events


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<FreeplayEventFactory*> FactoryArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FreeplayEventFactoryManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FreeplayEventFactoryManager();

		/**
		*  @brief
		*    Return a randomly chosen event factory
		*
		*  @param[in] onlyMarkedAsStart
		*    If true then only events marked as start events are selected
		*
		*  @return
		*    The chosen event factory, or a null pointer in case there is none available or the random selection failed
		*
		*  @remarks
		*    Takes into account "enabled"-state and trigger chance factor of the factories.
		*/
		FreeplayEventFactory* getRandomEventFactory(bool onlyMarkedAsStart) const;

		/**
		*  @brief
		*    Return an event factory with the given ID
		*/
		FreeplayEventFactory* getEventFactoryById(uint32 id) const;

		FreeplayEventFactory* getEventFactoryByIndex(uint32 index) const;

		inline const FactoryArray& getEventFactories() const  { return mFactories; }


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Create an event factory by name
		*
		*  @param[in] key
		*    Name of the factory (example: "em5::MedicalEmergencyEventFactory")
		*/
		static FreeplayEventFactory* createFreeplayFactoryByName(const std::string& key);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*   Clear all factories of this factory manager
		*/
		void clearFactoryArray();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FactoryArray mFactories;		///< Array of event factory instances; the factory manager is owner of each instance and responsible for destroying them


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
