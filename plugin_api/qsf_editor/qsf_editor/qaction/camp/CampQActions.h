// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActions.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
}
QT_BEGIN_NAMESPACE
	class QMenu;
	class QAction;
	class QActionGroup;
QT_END_NAMESPACE


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract CAMP QActions category
		*/
		class CampQActions : public QActions
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::CampQActions" unique pluginable QActions ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CampQActions();

			/**
			*  @brief
			*    Return the Qt action group
			*
			*  @return
			*    The Qt action group, can be a null pointer, do not destroy the returned instance
			*/
			inline QActionGroup* getQActionGroup() const;

			/**
			*  @brief
			*    Add the view Qt actions to the given Qt menu
			*
			*  @param[in] qMenu
			*    Qt menu to add the view Qt actions to
			*
			*  @return
			*    The number of checked Qt actions
			*/
			int addToMenu(QMenu& qMenu) const;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qActionsManager
			*    The QtActions manager this QtActions instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] classId
			*    ID (result of "camp::StringId(camp::Class::name())", e.g. "qsf::editor::Tool") of the metaclass, no reference by intent
			*/
			CampQActions(QActionsManager* qActionsManager, StringHash classId);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::CampQActions methods   ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Return whether or not a given Qt action is considered to be checked
			*
			*  @param[in] qAction
			*    Qt action to check
			*
			*  @return
			*    "true" if the given Qt action is considered to be checked, else "false"
			*/
			virtual bool isQActionChecked(QAction& qAction) const = 0;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Protected Qt slots (MOC)                              ]
		//[-------------------------------------------------------]
		protected Q_SLOTS:
			virtual void onTriggered(QAction* qAction);


		//[-------------------------------------------------------]
		//[ Protected structures                                  ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Action item
			*/
			struct ActionItem
			{
				const camp::Class* campClass;	///< CAMP class assigned to this item, always valid, do not destroy the instance
				QAction*		   qAction;		///< Qt action assigned to this item, can be a null pointer, do not destroy the instance
				QList<ActionItem>  children;	///< List of child action items
			};


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			QActionGroup* mQActionGroup;	///< Qt action group, can be a null pointer, Qt will destroy the instance automatically
			ActionItem    mRootActionItem;	///< Root action item


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Fill an action item
			*
			*  @param[in] actionItem
			*    Action item to fill
			*/
			void fillActionItem(ActionItem& actionItem);

			/**
			*  @brief
			*    Add the view Qt actions to the given Qt menu, recursive part
			*
			*  @param[in] qMenu
			*    Qt menu to add the view Qt actions to
			*  @param[in] parentActionItem
			*    Action item parent to fill with
			*
			*  @return
			*    The number of checked Qt actions
			*/
			int addToMenu(QMenu& qMenu, const ActionItem& parentActionItem) const;

			/**
			*  @brief
			*    Retranslate UI recursive
			*
			*  @param[in] parentActionItem
			*    Action item parent to update
			*/
			void retranslateUiRecursive(const ActionItem& parentActionItem) const;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/camp/CampQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::CampQActions)
