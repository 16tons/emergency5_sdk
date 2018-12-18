// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/command/Command.h"

#include <qsf/reflection/type/CampQsfAssetProxy.h>


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
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF script command class
		*/
		class ScriptCommand : public qsf::game::Command
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::game::ScriptCommand" unique command pluginable ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit ScriptCommand(qsf::game::CommandManager* commandManager);

			virtual ~ScriptCommand();

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
			virtual bool checkAvailable() override;
			virtual bool checkCaller(qsf::Entity& caller) override;
			virtual bool checkContext(const qsf::game::CommandContext& context) override;
			virtual void execute(const qsf::game::CommandContext& context) override;
			virtual std::string getName() const override;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Object methods                    ]
		//[-------------------------------------------------------]
		public:
			virtual Script* getAssociatedScript() const override;


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
	} // game
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::ScriptCommand)
