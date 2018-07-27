// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"
#include "qsf/component/Component.h"
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
	*    Script component class
	*
	*  @remarks
	*    There are the following known build-in global script functions which are used as entry points:
	*    - "onStartup":  Name of the optional script function called by C++ when the script component should startup itself
	*    - "onUpdate":   Name of the optional script function called by C++ when the script component should update itself	TODO(co) Currently it's unclear whether or not the update remains in here, maybe it's moved to the game logic system
	*    - "onShutdown": Name of the optional script function called by C++ when the script component should shutdown itself
	*
	*  @todo
	*    - TODO(co) At the moment this script component is primarily for rabid prototyping and proof-of-concept of the script system
	*/
	class QSF_API_EXPORT ScriptComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::ScriptComponent" unique component ID
		static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
		static const uint32 SCRIPT;			///< "Script" unique class property ID inside the class
		static const uint32 SCRIPT_EXECUTE;	///< "ScriptExecute" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit ScriptComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ScriptComponent();

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
		inline const AssetProxy& getScript() const;

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
		void setScript(const AssetProxy& script);

		/**
		*  @brief
		*    Return the script source code to execute
		*
		*  @return
		*    The script source code to execute
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::setScriptExecute()"
		*/
		inline const std::string& getScriptExecute() const;

		/**
		*  @brief
		*    Set the script source code to execute
		*
		*  @param[in] scriptExecute
		*    The script source code to execute
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Directly called after calling the global "onStartup" script function, also called in case another script to execute is called later on
		*/
		void setScriptExecute(const std::string& scriptExecute);

		/**
		*  @brief
		*    Return the script instance
		*
		*  @return
		*    The script instance, can be a null pointer, do not destroy the instance
		*/
		inline Script* getScriptInstance() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Object methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual Script* getAssociatedScript() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Set the script instance to use
		*
		*  @note
		*   - Do only call this method if the script component is currently running
		*/
		void setScriptInstance();

		/**
		*  @brief
		*    Destroy the used script instance
		*/
		void destroyScriptInstance();

		/**
		*  @brief
		*    Enable/disable the automatic script update
		*
		*  @param[in] enabled
		*    "true" to enable the automatic script update, else "false"
		*/
		void setAutomaticScriptUpdate(bool enabled);

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobProxy	mUpdateJobProxy;	///< Job proxy
		// Connected to the CAMP reflection system
		AssetProxy	mScript;			///< The script asset to use (e.g. "sample/script/lua/squirrel")
		Script*		mScriptInstance;	///< Script instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		std::string mScriptExecute;		///< Script source code to execute, can be used to set global variables (Lua example: ScriptExecute="public.Speed=0.07 public.Radius=0.01")


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/script/ScriptComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ScriptComponent)
