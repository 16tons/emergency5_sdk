// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ScriptComponent::ScriptComponent(Prototype* prototype) :
		Component(prototype),
		mScriptInstance(nullptr)
	{
		// Nothing to do in here
	}

	inline ScriptComponent::~ScriptComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& ScriptComponent::getScript() const
	{
		return mScript;
	}

	inline const std::string& ScriptComponent::getScriptExecute() const
	{
		return mScriptExecute;
	}

	inline Script* ScriptComponent::getScriptInstance() const
	{
		return mScriptInstance;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
