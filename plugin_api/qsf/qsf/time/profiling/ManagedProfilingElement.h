// Copyright (C) 2012-2018 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Header guard                                          ]
	//[-------------------------------------------------------]
	#pragma once


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "qsf/time/profiling/ProfilingElement.h"
	#include "qsf/base/NamedIdentifier.h"


	//[-------------------------------------------------------]
	//[ Forward declarations                                  ]
	//[-------------------------------------------------------]
	namespace qsf
	{
		class ProfilingManager;
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
		*    Managed profiling element class
		*/
		class QSF_API_EXPORT ManagedProfilingElement : public ProfilingElement
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		friend class ProfilingManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			explicit ManagedProfilingElement(const std::string& name);

			/**
			*  @brief
			*    Constructor
			*/
			ManagedProfilingElement(const std::string& name, const std::string& parentName);

			/**
			*  @brief
			*    Destructor
			*/
			~ManagedProfilingElement();

			/**
			*  @brief
			*    Return unique ID, which is the hashed name
			*/
			inline uint32 getId() const;

			/**
			*  @brief
			*    Return unique name
			*/
			inline const std::string& getName() const;

			/**
			*  @brief
			*    Return parent's unique ID, which is the hashed name
			*/
			inline uint32 getParentId() const;

			/**
			*  @brief
			*    Return parent's name
			*/
			inline const std::string& getParentName() const;

			// Support the "registerUpdate(const qsf::HighResolutionStopwatch&)" variant as well
			using ProfilingElement::registerUpdate;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::ProfilingElement methods          ]
		//[-------------------------------------------------------]
		public:
			virtual void registerUpdate(const Time& timeElapsed, int64 memoryConsumed) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void tryRegisteringAtManager();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ProfilingManager*	mManager;		///< Profiling manager this element is registered at; do not destroy the instance
			NamedIdentifier		mName;			///< Unique name of this profiling element
			NamedIdentifier		mParentName;	///< Parent element name; may be uninitialized


		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf


	//[-------------------------------------------------------]
	//[ Implementation                                        ]
	//[-------------------------------------------------------]
	#include "qsf/time/profiling/ManagedProfilingElement-inl.h"
#endif
