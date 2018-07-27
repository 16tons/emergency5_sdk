// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/forward/BoostPTree.h>
#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class ActivityStep;
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
	*    Activity step factory
	*/
	class EM5_API_EXPORT ActivityStepFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ActivityStepFactory();

		/**
		*  @brief
		*    Destructor
		*/
		~ActivityStepFactory();

		/**
		*  @brief
		*    Create an activity step instance by the given key
		*
		*  @param[in] key
		*    Activity step key to identify the class to construct an instance of
		*  @param[in] pTree
		*    Boost property tree to initialize the new activity step instance
		*
		*  @return
		*    The activity step instance; ownership is transferred, i.e. the caller is resposible for destroying the instance again
		*/
		ActivityStep* createActivityStepFromBoostPTree(const qsf::NamedIdentifier& key, const boost::property_tree::ptree& pTree);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void classAdded(const camp::Class& added);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
#ifdef RETAIL
		typedef boost::container::flat_map<qsf::NamedIdentifier, const camp::Class*> ClassMap;
#else
		typedef std::map<qsf::NamedIdentifier, const camp::Class*> ClassMap;		// std::map for easier debugging
#endif


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ClassMap mClassMap;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
