// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/plugin/metaclasses/Metaclasses.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static CAMP general plugin metaclass collection class
		*/
		class GeneralMetaclasses : public Metaclasses
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		// Empty fake Q_OBJECT macro to silence the lupdate Qt tool ("Class '<x>' lacks Q_OBJECT macro" warnings)
		#ifndef Q_OBJECT
			#define Q_OBJECT
			#define QSF_FAKE_Q_OBJECT
		#endif
		Q_OBJECT
		#ifdef QSF_FAKE_Q_OBJECT
			#undef Q_OBJECT
			#undef QSF_FAKE_Q_OBJECT
		#endif


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class Plugin;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] plugin
			*    Plugin to register the CAMP metaclasses within, instance must stay valid as long as this metaclass collection instance exists
			*/
			explicit GeneralMetaclasses(Plugin& plugin);

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~GeneralMetaclasses();

			/**
			*  @brief
			*    Register the CAMP metaclasses
			*/
			void registerMetaclasses();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
