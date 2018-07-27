// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActions.h"

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
}
QT_BEGIN_NAMESPACE
	class QMenu;
	class QActionGroup;
QT_END_NAMESPACE
namespace qsf
{
	class Map;
	class Layer;
	class Entity;
	class LinkComponent;
}


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
		*    Object QActions category
		*
		*  @note
		*    - Quite similar to "qsf::editor::LayerQActions"
		*/
		class ObjectQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ObjectQActions" unique pluginable QActions ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qActionsManager
			*    The QtActions manager this QtActions instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit ObjectQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ObjectQActions();

			//[-------------------------------------------------------]
			//[ Basic QAction                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the fly to selection Qt action
			*
			*  @return
			*    The fly to selection Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getFlyToSelectionQAction() const;

			/**
			*  @brief
			*    Return the duplicate Qt action
			*
			*  @return
			*    The duplicate Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getDuplicateQAction() const;

			/**
			*  @brief
			*    Return the replace Qt action
			*
			*  @return
			*    The replace Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getReplaceQAction() const;

			/**
			*  @brief
			*    Return the reset to prefab defaults Qt action
			*
			*  @return
			*    The reset to prefab defaults Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getResetToPrefabQAction() const;

			/**
			*  @brief
			*    Return the show in prefab browser Qt action
			*
			*  @return
			*    The show in prefab browser Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getShowInPrefabBrowserQAction() const;

			/**
			*  @brief
			*    Return the select all prefab instances Qt action
			*
			*  @return
			*    The select all prefab instances prefab Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectAllPrefabInstancesQAction() const;

			/**
			*  @brief
			*    Return the update from prefab Qt action
			*
			*  @return
			*    The update from prefab Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getUpdateFromPrefabQAction() const;

			//[-------------------------------------------------------]
			//[ Object Layer QAction                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the select layer Qt action
			*
			*  @return
			*    The select layer Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectLayerQAction() const;

			/**
			*  @brief
			*    Return the move to layer Qt action
			*
			*  @return
			*    The move to layer Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getMoveToLayerQAction() const;

			/**
			*  @brief
			*    Return the copy to layer Qt action
			*
			*  @return
			*    The copy to layer Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCopyToLayerQAction() const;

			//[-------------------------------------------------------]
			//[ Object Group QAction                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the group/ungroup Qt action
			*
			*  @return
			*    The group/ungroup Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getGroupUnGroupQAction() const;

			/**
			*  @brief
			*    Return the save prefab group Qt action
			*
			*  @return
			*    The save prefab group Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSavePrefabGroupQAction() const;

			/**
			*  @brief
			*    Return the save prefab group as Qt action
			*
			*  @return
			*    The save prefab group as Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSavePrefabGroupAsQAction() const;

			//[-------------------------------------------------------]
			//[ Object QAction                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the create object Qt action
			*
			*  @return
			*    The create object Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCreateObjectQAction() const;

			/**
			*  @brief
			*    Return the delete object Qt action
			*
			*  @return
			*    The delete object Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getDeleteObjectQAction() const;

			/**
			*  @brief
			*    Return the copy entity ID Qt action
			*
			*  @return
			*    The copy entity ID Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCopyEntityIdQAction() const;

			/**
			*  @brief
			*    Return the export selection as layer Qt action
			*
			*  @return
			*    The export selection as layer Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getExportSelectionAsLayerQAction() const;

			/**
			*  @brief
			*    Return the create component Qt action
			*
			*  @return
			*    The create component Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCreateComponentQAction() const;

			/**
			*  @brief
			*    Return the delete component Qt action
			*
			*  @return
			*    The delete component Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getDeleteComponentQAction() const;

			/**
			*  @brief
			*    Return the morph component Qt action
			*
			*  @return
			*    The morph component Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getMorphComponentQAction() const;

			/**
			*  @brief
			*    Return the hard link selected objects Qt action
			*
			*  @return
			*    The hard link selected objects Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectHardLinkSelectedQAction() const;

			/**
			*  @brief
			*    Return the soft link selected objects Qt action
			*
			*  @return
			*    The soft link selected objects Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectSoftLinkSelectedQAction() const;

			/**
			*  @brief
			*    Return the link selected objects to slot Qt action
			*
			*  @return
			*    The link selected objects to slot Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectLinkSelectedToSlotQAction() const;

			/**
			*  @brief
			*    Return the select object Qt action
			*
			*  @return
			*    The select object Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectObjectQAction() const;

			/**
			*  @brief
			*    Return the select entity Qt action
			*
			*  @return
			*    The select entity Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectEntityQAction() const;

			/**
			*  @brief
			*    Return the select parent Qt action
			*
			*  @return
			*    The select parent Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectParentQAction() const;

			/**
			*  @brief
			*    Return the unlink object from parent Qt action
			*
			*  @return
			*    The unlink object from parent Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getUnlinkObjectFromParentQAction() const;

			/**
			*  @brief
			*    Return the unlink child object Qt action
			*
			*  @return
			*    The unlink child object Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectUnlinkChildQAction() const;

			/**
			*  @brief
			*    Return the object unlink direct children Qt action
			*
			*  @return
			*    The object unlink direct children Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectUnlinkDirectChildrenQAction() const;

			/**
			*  @brief
			*    Return the object unlink all children Qt action
			*
			*  @return
			*    The object unlink all children Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectUnlinkAllChildrenQAction() const;

			/**
			*  @brief
			*    Return the object unlink all children from slot Qt action
			*
			*  @return
			*    The object unlink all children from slot Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectUnlinkAllChildrenFromSlotQAction() const;

			/**
			*  @brief
			*    Return the toggle object visibility Qt action
			*
			*  @return
			*    The toggle object visibility Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectToggleVisibilityQAction() const;

			/**
			*  @brief
			*    Return the object disband instantiation Qt action
			*
			*  @return
			*    The object disband instantiation Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectDisbandInstantiationQAction() const;

			/**
			*  @brief
			*    Return the object save prefab Qt action
			*
			*  @return
			*    The object save prefab Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectSavePrefabQAction() const;

			/**
			*  @brief
			*    Return the object save prefab as Qt action
			*
			*  @return
			*    The object save prefab as Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectSavePrefabAsQAction() const;

			/**
			*  @brief
			*    Return the object save prefab as with inheritance Qt action
			*
			*  @return
			*    The object save prefab as with inheritance Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getObjectSavePrefabAsWithInheritanceQAction() const;

			//[-------------------------------------------------------]
			//[ Helper                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Add Qt object actions to the given Qt menu
			*
			*  @param[in] qMenu
			*    Qt menu were to add the Qt object actions to
			*  @param[in] linkQActions
			*    Add link related Qt actions? (need a concrete target)
			*/
			void addQActionsToMenu(QMenu& qMenu, bool linkQActions) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup() override;
			virtual void retranslateUi() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// Slots for QAction menus
			void onTriggeredFlyToSelection();
			void onTriggeredSelectLayer();
			void onAboutToShowMoveToLayerMenu();
			void onTriggeredMoveToLayerQActionGroup(QAction* qAction);
			void onAboutToShowCopyToLayerMenu();
			void onTriggeredCopyToLayerQActionGroup(QAction* qAction);
			void onTriggeredDuplicate();
			void onTriggeredReplace();
			void onTriggeredShowInPrefabBrowser();
			void onTriggeredSelectAllPrefabInstances();
			void onTriggeredUpdateFromPrefab();
			void onTriggeredResetToPrefab();
			void onTriggeredGroupUnGroup();
			void onTriggeredSavePrefabGroup();
			void onTriggeredSavePrefabGroupAs();
			void onTriggeredCreateObject();
			void onTriggeredDeleteObject();
			void onTriggeredCopyEntityId();
			void onTriggeredExportSelectionAsLayer();
			void onTriggeredCreateComponentQActionGroup(QAction* qAction);
			void onTriggeredCreateComponent();
			void onTriggeredDeleteMorphComponent();
			void onTriggeredObjectHardLinkSelectedQAction();
			void onTriggeredObjectSoftLinkSelectedQAction();
			void onAboutToShowLinkSlotMenu();
			void onTriggeredLinkSlotQActionGroup(QAction* qAction);
			void onTriggeredSelectObjectQAction();
			void onTriggeredSelectEntityQAction();
			void onTriggeredSelectParentQAction();
			void onTriggeredUnlinkObjectFromParentQAction();
			void onAboutToShowUnlinkChildMenu();
			void onTriggeredUnlinkChildQActionGroup(QAction* qAction);
			void onTriggeredObjectUnlinkDirectChildrenQAction();
			void onTriggeredObjectUnlinkAllChildrenQAction();
			void onTriggeredUnlinkAllChildrenFromSlotQActionGroup(QAction* qAction);
			void onTriggeredObjectDisbandInstantiation();
			void onTriggeredObjectSavePrefab();
			void onTriggeredObjectSavePrefabAs();
			void onTriggeredObjectSavePrefabAsWithInheritance();
			void onTriggeredObjectToggleVisibility();
			void onAboutToShowUnlinkAllChildrenFromSlotMenu();
			// Slots for other signals
			void onQClipboardDataChanged();
			// qsf::editor::EntitySelectionManager
			void onChangeEntity(Entity* entity);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Update the state of the replace Qt action
			*/
			void updateReplaceQAction() const;

			/**
			*  @brief
			*    Fill a layer menu
			*
			*  @param[in] layer
			*    Layer used to fill the Qt menu
			*  @param[in] qMenu
			*    Qt menu to fill
			*  @param[in] qActionGroup
			*    Qt action group to add the layer Qt actions to
			*/
			void fillLayerMenu(const Layer& layer, QMenu& qMenu, QActionGroup& qActionGroup) const;

			/**
			*  @brief
			*    Fill link slot menu
			*
			*  @param[in] linkComponent
			*    Entity used to fill the Qt menu
			*  @param[in] qMenu
			*    Qt menu to fill
			*  @param[in] qActionGroup
			*    Qt action group to add the unlink Qt actions to
			*/
			void fillLinkSlotMenu(const Entity& entity, QMenu& qMenu, QActionGroup& qActionGroup) const;

			/**
			*  @brief
			*    Fill unlink child menu
			*
			*  @param[in] linkComponent
			*    Link component used to fill the Qt menu
			*  @param[in] qMenu
			*    Qt menu to fill
			*  @param[in] qActionGroup
			*    Qt action group to add the unlink Qt actions to
			*/
			void fillUnlinkChildMenu(const LinkComponent& linkComponent, QMenu& qMenu, QActionGroup& qActionGroup) const;

			/**
			*  @brief
			*    Fill unlink all children from slot menu
			*
			*  @param[in] entity
			*    Entity used to fill the Qt menu
			*  @param[in] qMenu
			*    Qt menu to fill
			*  @param[in] qActionGroup
			*    Qt action group to add the unlink Qt actions to
			*/
			void fillUnlinkAllChildrenFromSlotMenu(const Entity& entity, QMenu& qMenu, QActionGroup& qActionGroup) const;

			void objectSavePrefabInternal(bool overwriteExisting, bool saveWithInheritance);
			GlobalAssetId getCurrentEntityGlobalPrefabAssetId() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map&		  mMap;								///< Map instance to operate on, do not destroy the instance
			// Basic
			QAction*	  mFlyToSelectionQAction;			///< Fly to selection, Qt will automatically destroy the instance
			QAction*	  mDuplicateQAction;				///< Duplicate Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mReplaceQAction;					///< Replace Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mShowInPrefabBrowserQAction;		///< Show in prefab browser Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mSelectAllPrefabInstancesQAction;	///< Select all prefab instances Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mUpdateFromPrefabQAction;			///< Update from prefab Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mResetToPrefabQAction;			///< Reset to prefab defaults Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mSelectLayerQAction;				///< Select layer Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mMoveToLayerQAction;				///< Move to layer Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QActionGroup* mMoveToLayerQActionGroup;			///< Move to layer Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuMoveToLayer;				///< Qt menu attached to the move to layer Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QAction*	  mCopyToLayerQAction;				///< Copy to layer Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QActionGroup* mCopyToLayerQActionGroup;			///< Copy to layer Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuCopyToLayer;				///< Qt menu attached to the copy to layer Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			// Object group
			QAction* mGroupUnGroupQAction;		///< Group/ungroup Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSavePrefabGroupQAction;	///< Save prefab group Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSavePrefabGroupAsQAction;	///< Save prefab group as Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			// Object
			QAction*	  mCreateObjectQAction;						///< Create a object and find a unique name for it, Qt will automatically destroy the instance
			QAction*	  mDeleteObjectQAction;						///< Delete object Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mCopyEntityIdQAction;						///< Copy entity ID Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mExportSelectionAsLayerQAction;			///< Export selection as layer Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mCreateComponentQAction;					///< Create component Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuCreateComponent;					///< Qt menu attached to the create component Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QAction*	  mDeleteComponentQAction;					///< Delete component Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mMorphComponentQAction;					///< Morph component Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectHardLinkSelectedQAction;			///< Hard link selected objects Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectSoftLinkSelectedQAction;			///< Soft link selected objects Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectLinkSelectedToSlotQAction;			///< Link selected objects to slot Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QActionGroup* mLinkSlotQActionGroup;					///< Link selected objects to slot Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuLinkSlot;							///< Qt menu attached to the link selected objects to slot Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QAction*	  mSelectObjectQAction;						///< Selects the given object, Qt will automatically destroy the instance
			QAction*	  mSelectEntityQAction;						///< Selects the given entity, Qt will automatically destroy the instance
			QAction*	  mSelectParentQAction;						///< Select parent Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mUnlinkObjectFromParentQAction;			///< Unlink object from parent Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectUnlinkChildQAction;				///< Unlink child object Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QActionGroup* mUnlinkChildQActionGroup;					///< Unlink child object Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuUnlinkChild;						///< Qt menu attached to the unlink child object Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QAction*	  mObjectUnlinkDirectChildrenQAction;		///< Unlink direct children Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectUnlinkAllChildrenQAction;			///< Unlink all children Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectToggleVisibilityQAction;			///< Toggle the hidden/unhidden state of the object selection, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectDisbandInstantiationQAction;		///< Disband instantiation object Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectSavePrefabQAction;					///< Save prefab object Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectSavePrefabAsQAction;				///< Save prefab as object Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mObjectSavePrefabAsWithInheritanceQAction;///< Save prefab as object with inheritance Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuUnlinkAllChildrenFromSlot;			///< Qt menu attached to the unlink all children from slot Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QAction*	  mObjectUnlinkAllChildrenFromSlotQAction;	///< Unlink all children from slot Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QActionGroup* mUnlinkAllChildrenFromSlotQActionGroup;	///< Unlink all children from slot Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance


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
#include "qsf_editor/qaction/edit/ObjectQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ObjectQActions)
