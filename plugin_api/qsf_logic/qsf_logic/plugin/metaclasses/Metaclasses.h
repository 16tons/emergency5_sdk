// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
}
namespace qsf
{
	namespace logic
	{
		class Plugin;
	}
}


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
		*    CAMP metaclass collection base class
		*
		*  @remarks
		*    All plugin CAMP metaclass collections should be derived from this class. This makes it easier
		*    to find all classes within e.g. the automatic code documentation.
		*
		*    Beside being able to keep an overview, it might also be an issue for the compiler in case a cpp file gets to huge.
		*    In case you receive a
		*      "fatal error C1128: number of sections exceeded object file format limit : compile with /bigobj"
		*    compiler error, it's best to use a separate CAMP metaclass collection class instead of setting special compiler
		*    settings which might cause other follow up issues.
		*/
		class QSF_LOGIC_API_EXPORT Metaclasses : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] plugin
			*    Plugin to register the CAMP metaclasses within, instance must stay valid as long as this metaclass collection instance exists
			*/
			explicit Metaclasses(Plugin& plugin);

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~Metaclasses();

			/**
			*  @brief
			*    Add a CAMP class registered by this metaclass collection
			*
			*  @param[in] campClass
			*    CAMP class registered by this metaclass collection, must stay valid as long as the owner plugin instance exists
			*/
			void addCampClass(const camp::Class& campClass);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Plugin* mPlugin;	///< Plugin to register the CAMP metaclasses within, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
