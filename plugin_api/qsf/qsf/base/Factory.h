// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/pointee.hpp>


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
	*    Factory function object class similar to boost::factory
	*
	*  @remarks
	*    This class is a function object which can be used to create instances of a type (on heap)
	*/
	template<typename Pointer>
	struct Factory
	{
	public:
		typedef typename boost::remove_cv<Pointer>::type result_type;  // Needed when using boost::bind with this object
		typedef typename boost::pointee<result_type>::type value_type; // Needed when using boost::bind with this object

		template<class... argTs>
		result_type operator()(argTs&&... args) const
		{
			return new value_type(std::forward<argTs>(args)...);
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
