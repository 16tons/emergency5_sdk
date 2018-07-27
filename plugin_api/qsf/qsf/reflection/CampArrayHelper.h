// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>

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
namespace camp
{
	class ArrayProperty;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static CAMP array helper class
	*/
	template <typename T>
	class CampArrayHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Serialize a CAMP array property value into a Boost ptree
		*
		*  @param[in] sourceArray
		*    Array to use
		*  @param[in] campArrayProperty
		*    CAMP array property to serialize the value from
		*  @param[out] pTree
		*    Boost ptree to serialize the CAMP property value into
		*
		*  @throw
		*   - Throws an exception in case of an error to be compatible with the external Boost library
		*
		*  @note
		*   - The output ptree will consist of a single child named as the property and holding the property's value
		*/
		static void toBoostPTree(const T& sourceArray, const camp::ArrayProperty& campArrayProperty, boost::property_tree::ptree& pTree);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/reflection/CampArrayHelper-inl.h"
