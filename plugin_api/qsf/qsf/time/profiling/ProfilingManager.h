// Copyright (C) 2012-2017 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Header guard                                          ]
	//[-------------------------------------------------------]
	#pragma once


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "qsf/base/manager/Manager.h"
	#include "qsf/base/NamedIdentifier.h"

	#include <boost/container/flat_map.hpp>


	//[-------------------------------------------------------]
	//[ Forward declarations                                  ]
	//[-------------------------------------------------------]
	namespace qsf
	{
		class ManagedProfilingElement;
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
		*    Collects profiling elements
		*/
		class QSF_API_EXPORT ProfilingManager : public Manager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		friend class ManagedProfilingElement;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_map<uint32, ManagedProfilingElement*> ElementMap;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			ProfilingManager();

			/**
			*  @brief
			*    Destructor
			*/
			~ProfilingManager();

			const ElementMap& getProfilingElementMap() const  { return mElements; }


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void registerProfilingElement(ManagedProfilingElement& element);
			void unregisterProfilingElement(ManagedProfilingElement& element);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ElementMap mElements;


		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf
#endif
