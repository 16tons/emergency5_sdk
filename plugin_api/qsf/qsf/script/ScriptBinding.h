// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"


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
	*    Abstract script binding class
	*
	*  @remarks
	*    This is actually a blank CAMP reflection system class. CAMP classes which are only used for "script binding" should be
	*    derived from this class. This doesn't mean that it will be impossible to bind any other CAMP class
	*    instance to a script - but usually it's useful to have such an abstract base class.
	*
	*    Usually the CAMP script binding class instance methods will be available to the script as simple
	*    global functions. So, this has nothing to do with using OOP within scripts. It's just adding global
	*    functions to scripts.
	*
	*    Each script binding metaclass should have the following tags:
	*    - "ScriptNamespace": Namespace to use inside scripts (for example: "qsf.log")
	*/
	class QSF_API_EXPORT ScriptBinding : public Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ScriptBinding();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		ScriptBinding();


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ScriptBinding)
