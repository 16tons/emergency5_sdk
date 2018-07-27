// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/NamedIdentifier.h>

#include <boost/noncopyable.hpp>

#include <map>
#include <string>


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
	*    EMERGENCY 5 freeplay density falculator
	*
	*  @todo
	*    - TODO(co) Naming this class "em5::FreeplayDensityCalculator" instead of "em5::DensityCalculator" might be a good idea
	*/
	class DensityCalculator : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string EVENT_DENSITY_DEFINITION_FILENAME;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DensityCalculator();

		/**
		*  @brief
		*    Destructor
		*/
		~DensityCalculator();

		/**
		*  @brief
		*    Load the density factors from JSON file
		*/
		bool loadDataFromFile();

		/**
		*  @brief
		*    Return the current event density
		*/
		float getEventDensity() const;

		/**
		*  @brief
		*    Get the formatted values as string
		*/
		std::string getDebug() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<qsf::NamedIdentifier, float> DensityMap;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Check the version and type of the file
		*/
		bool checkVersion(const boost::property_tree::ptree& pTree, const std::string& version) const;

		/**
		*  @brief
		*    Insert data from json file to map
		*/
		void insertData(const boost::property_tree::ptree& pTree);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DensityMap mDensityMap;		///< Inherits the string IDs and the density values of each counter


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
