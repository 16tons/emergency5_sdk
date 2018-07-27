// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/detail/utf8_codecvt_facet.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global functions                                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Set Boost UTF-8 locale
	*
	*  @remarks
	*    Under MS Windows, Boost doesn't interpret given strings as UTF-8 strings
	*    -> Instead of writing "boost::filesystem::exists(myUtf8String)" one has to write
	*       "boost::filesystem::exists(boost::nowide::widen(myUtf8String))" which is awful error prone
	*    -> After the following line, one can write directly "boost::filesystem::exists(myUtf8String)" and be done with it
	*    -> Sadly, since we linking Boost statically, this change has to be applied in each and every module
	*       (aka shared library aka dll). Best place is inside the constructor of our plugin class.
	*/
	void SetBoostUtf8Locale()
	{
		#ifdef WIN32
			boost::filesystem::path::imbue(std::locale(std::locale(), new boost::filesystem::detail::utf8_codecvt_facet));
		#endif
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
