// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/type/CampQsfAssetProxy.h"

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Action;
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
	*    Collection of script action; is able to create actions
	*/
	class QSF_API_EXPORT ScriptActionManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		ScriptActionManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ScriptActionManager();

		/**
		*  @brief
		*    Scan the asset system an register all script actions
		*
		*  @note
		*    - Not automatically called in constructor, in the moment the action manager is created, the script assets aren't loaded
		*/
		void registerScriptActions();

		/**
		*  @brief
		*    Reload all registered scripts
		*/
		void reloadScriptActions();

		/**
		*  @brief
		*    Create action with the given actionTypeId
		*
		*  @param[in] actionTypeId
		*    The id of the action
		*
		*  @return
		*    return a action instance of the given type or nullptr in case its not registered
		*/
		Action* createActionById(uint64 actionTypeId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::unordered_map<uint64, AssetProxy>	mScriptActions;		///< Map with name of the scripts and the asset
		bool									mIsInitialized;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ScriptActionManager)
