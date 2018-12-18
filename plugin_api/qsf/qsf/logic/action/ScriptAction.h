// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/action/Action.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Script;
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
	*    QSF script action class
	*/
	class ScriptAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const NamedIdentifier ACTION_ID;	///< "qsf::ScriptAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		ScriptAction();

		virtual ~ScriptAction();

		/**
		*  @brief
		*    Return the script asset to use
		*
		*  @return
		*    The script asset to use (e.g. "sample/script/lua/squirrel"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::AssetProxy& getScript() const;

		/**
		*  @brief
		*    Set the script asset to use
		*
		*  @param[in] script
		*    The script asset to use (e.g. "sample/script/lua/squirrel")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setScript(const qsf::AssetProxy& script);

		/**
		*  @brief
		*    Reload the script from the script asset
		*/
		void loadScriptInstance();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::Command methods             ]
	//[-------------------------------------------------------]
	public:
		virtual action::Result updateAction(const Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Object methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual Script* getAssociatedScript() const override;


	protected:
		QSF_ADD_EMPTY_DUMMY_ACTION_SERIALIZATION;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void destroyScriptInstance();

		// Helper
		void searchForScriptFunction(const std::string& functionName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::AssetProxy	mScript;			///< The script asset to use (e.g. "sample/script/lua/squirrel")
		qsf::Script*	mScriptInstance;	///< Script instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it

		// Check for existing functions at startup
		bool			mHasMainFunctions;	///< True if the main functions "priority", "checkAvailable", "checkCaller", "checkContext" and "execute" exists


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ScriptAction)
