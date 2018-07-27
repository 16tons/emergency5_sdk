// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>


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
	*    Unique instance interface
	*
	*  @remarks
	*    This class is similar to a singleton, but the instance is created and destroyed from another place.
	*/
	template <typename T>
	class UniqueInstance : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the global unique instance
		*
		*  @return
		*    The global unique instance, null pointer in case there's no such global instance, do no destroy the returned instance
		*/
		static T* getInstance();

		/**
		*  @brief
		*    Return the global unique instance
		*
		*  @return
		*    The global unique instance, throws an exception in case there's no instance, do no destroy the returned instance
		*/
		static T& getInstanceSafe();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		UniqueInstance();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UniqueInstance();


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static T* mInstance;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/UniqueInstance-inl.h"
