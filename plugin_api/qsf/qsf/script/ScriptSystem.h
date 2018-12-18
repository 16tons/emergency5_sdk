// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"

#include <boost/container/flat_map.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
}
namespace qsf
{
	class Script;
	class ClassSystem;
	class ScriptBinding;
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
	*    Script system class
	*
	*  @note
	*    - The script system and related classes are based on codes from the open-source engine PixelLight (MIT license, www.pixellight.org), for Python, JavaScript and AngleScript backends see PixelLight
	*/
	class QSF_API_EXPORT ScriptSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ScriptSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ScriptSystem();

		/**
		*  @brief
		*    Return a list of the names of the supported script languages
		*
		*  @return
		*    List of the names of the supported script languages
		*/
		inline const std::vector<std::string>& getScriptLanguages() const;

		/**
		*  @brief
		*    Return the name of a script language by its filename extension
		*
		*  @param[in] extension
		*    Script language filename extension to check (e.g. "lua" or "js")
		*
		*  @return
		*    Script language, empty string on error (possibly not supported filename extension)
		*/
		inline std::string getScriptLanguageByExtension(const std::string& extension) const;

		/**
		*  @brief
		*    Check whether or not a given script language is supported
		*
		*  @param[in] scriptLanguage
		*    Script language to check (e.g. "Lua" or "JavaScript")
		*
		*  @return
		*    "true" if the given script language is supported, else "false"
		*/
		inline bool isSupported(const std::string& scriptLanguage) const;

		/**
		*  @brief
		*    Return the filename extension of a given script language
		*
		*  @param[in] scriptLanguage
		*    Script language (e.g. "Lua" or "JavaScript") to return the filename extension (e.g. "lua" or "js") from
		*
		*  @return
		*    The filename extension of the given script language, empty string on error (possibly not supported script language)
		*
		*  @note
		*    - If the script language has more than one filename extension, the first filename extension will be returned
		*/
		std::string getScriptLanguageExtension(const std::string& scriptLanguage) const;

		/**
		*  @brief
		*    Return a list of all script binding instances
		*
		*  @return
		*    List of all script binding instances
		*/
		inline const std::vector<ScriptBinding*>& getScriptBindings() const;

		/**
		*  @brief
		*    Create a script instance
		*
		*  @param[in] scriptLanguage
		*    Script language to use
		*  @param[in] addBindings
		*    If "true", add all available script bindings automatically (see "qsf::Script::addBindings()")
		*
		*  @return
		*    The created script instance, null pointer on error, destroy the instance if you no longer need it
		*/
		Script* create(const std::string& scriptLanguage, bool addBindings = true);

		/**
		*  @brief
		*    Create a script instance by using a given filename
		*
		*  @param[in] filename
		*    Script filename
		*  @param[in] addBindings
		*    If "true", add all available script bindings automatically (see "qsf::Script::addBindings()")
		*  @param[in] stringFormat
		*    String encoding format to use when dealing with string functions (not supported by all file implementations)
		*
		*  @return
		*    The created script instance, null pointer on error (Unknown filename extension? File not found? Error within the script?), destroy the instance if you no longer need it
		*
		*  @remarks
		*    While the "create()"-method only creates an empty script instance, the "createByFilename()"-method will
		*    - Create an empty script instance by using the filename extension as indication of the script language to use
		*    - Load in the script source code by using the given script filename
		*    - Assign the loaded script source code to the created, previously empty, script
		*    The result is a ready to be used script by just using this one method. The drawback is, that you can't use
		*    custom global functions when using this method because the custom global functions have to be defined before
		*    the script source code is assigned to the script (resulting in script compilation).
		*
		*  @note
		*    - Convenience method
		*    - It's not recommended to use Unicode by because internally wchar_t is used and this data type has not
		*      the same size on every platform (use ASCII or UTF8 instead)
		*/
		// TODO(co) Port my PixelLight script integration to QSF: "stringFormat" , std::string::EFormat nStringFormat = String::ASCII
		Script* createByFilename(const std::string& filename, bool addBindings = true);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Register all known and supported CAMP script classes
		*/
		void registerScriptClasses();

		/**
		*  @brief
		*    Unregister all known and supported CAMP script classes
		*/
		void unregisterScriptClasses();

		/**
		*  @brief
		*    Called after a new CAMP class has been registered
		*
		*  @param[in] added
		*    New new CAMP class
		*/
		void classAdded(const camp::Class& added);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ClassSystem&												mClassSystem;
		bool														mRunning;						///< "true" if the script system is up-and-running, else "false"
		// Script languages
		std::vector<std::string>									mScriptLanguagesList;			///< List of script languages
		boost::container::flat_map<std::string, const camp::Class*>	mScriptLanguagesMap;			///< Map of script languages (key = script language name, e.g. "Lua" or "JavaScript")
		boost::container::flat_map<std::string, std::string>		mScriptLanguagesByExtension;	///< Map of script languages by extension (key = extension, e.g. "lua" or "js")
		// Script bindings
		std::vector<ScriptBinding*>									mScriptBindings;				///< List of script bindings, we're responsible for destroying the instances in case we no longer need them


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/script/ScriptSystem-inl.h"
