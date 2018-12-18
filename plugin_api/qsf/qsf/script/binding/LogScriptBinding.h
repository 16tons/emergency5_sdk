// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/script/ScriptBinding.h"


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
	*    Log script binding class
	*/
	class LogScriptBinding : public ScriptBinding
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		LogScriptBinding();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LogScriptBinding();

		//[-------------------------------------------------------]
		//[ Connected to the CAMP reflection system               ]
		//[-------------------------------------------------------]
		void outputTrace(const std::string& text) const;
		void outputDebug(const std::string& text) const;
		void outputInfo(const std::string& text) const;
		void outputWarning(const std::string& text) const;
		void outputError(const std::string& text) const;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LogScriptBinding)
