// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/command/Command.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class CommandManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract EMERGENCY 5 command base class
	*/
	class EM5_API_EXPORT Command : public qsf::game::Command
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct IconSettings
		{
			bool		mShowAsGuiButton;		// "true" if commando has an representation as button
			bool		mStartCommandMode;		// "true" if activating starts a special mode
			bool		mEnabledInMultiplay;	// "true" if it is usable in multiplay
			std::string	mButtonIconPath;		// Path of the button icon
			std::string	mMouseIconName;			// Name of the mouse icon (this name is used to set the new cursor via librocket)
			std::string mTooltip;				// Tooltip string

			IconSettings() :
				mShowAsGuiButton(false),
				mStartCommandMode(false),
				mEnabledInMultiplay(true)
			{}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		virtual const IconSettings& getIconSettings() const;

		/**
		*  @brief
		*    Indicates if target highlighting for this command is enabled/disabled
		*/
		virtual bool highlightTarget() const;

		/**
		*  @brief
		*    Returns the help page name
		*/
		const std::string& getHelpPageName() const;

		/**
		*  @brief
		*    Returns the priority of the command buttons  (Bigger priority comes first)
		*/
		int32 getButtonPriority() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @note
		*    - Protected to hide an instancing of this class
		*/
		Command(qsf::game::CommandManager* commandManager, uint32 commandId);

		/**
		*  @brief
		*    Modification of the "checkCallerActionPriority" but return "false" if callers action is with blocking priority
		*/
		bool checkCallerActionPriorityNoBlocking(qsf::Entity& caller, uint32 maximumPriority) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		IconSettings	mIconSettings;		///< The command's icon settings structure
		std::string		mHelpPageName;		///< Holds the corresponding help page name
		int32			mButtonPriority;	///< Priority of command buttons on screen (Bigger priority comes first)


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::Command)
