// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

// Forward declaration for boost::property_tree::ptree

#include <string>

namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;

		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
		typedef basic_ptree<std::wstring, std::wstring, std::less<std::wstring> > wptree;
	}
}
