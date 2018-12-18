// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ScriptSystem::~ScriptSystem()
	{
		// Nothing to do in here
	}

	inline const std::vector<std::string> &ScriptSystem::getScriptLanguages() const
	{
		return mScriptLanguagesList;
	}

	inline std::string ScriptSystem::getScriptLanguageByExtension(const std::string& extension) const
	{
		boost::container::flat_map<std::string, std::string>::const_iterator iterator = mScriptLanguagesByExtension.find(extension);
		return (iterator != mScriptLanguagesByExtension.cend()) ? iterator->second : "";
	}

	inline bool ScriptSystem::isSupported(const std::string& scriptLanguage) const
	{
		boost::container::flat_map<std::string, const camp::Class*>::const_iterator iterator = mScriptLanguagesMap.find(scriptLanguage);
		return (iterator != mScriptLanguagesMap.cend()) ? (nullptr != iterator->second) : false;
	}

	inline const std::vector<ScriptBinding*>& ScriptSystem::getScriptBindings() const
	{
		return mScriptBindings;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* ScriptSystem::getName() const
	{
		return "Script system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
