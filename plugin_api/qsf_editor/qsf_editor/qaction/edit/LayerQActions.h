// Copyright (C) 2012-2017 Promotion Software GmbH


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
QT_BEGIN_NAMESPACE
	class QMenu;
	class QActionGroup;
QT_END_NAMESPACE
namespace qsf
{
	class Map;
	class Layer;
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
		*    Layer QActions category
		*
		*  @note
		*    - Quite similar to "qsf::editor::ObjectQActions"
		*/
		class LayerQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::LayerQActions" unique pluginable QActions ID


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
			explicit LayerQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LayerQActions();

			//[-------------------------------------------------------]
			//[ Basic QAction                                         ]
			//[-------------------------------------------------------]
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

			/**
			*  @brief
			*    Return the reparent layer Qt action
			*
			*  @return
			*    The reparent layer Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getReparentLayerQAction() const;

			/**
			*  @brief
			*    Return the load layer in-game Qt action
			*
			*  @return
			*    The load layer in-game Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getLoadLayerInGameQAction() const;

			//[-------------------------------------------------------]
			//[ Visibility QAction                                    ]
			//[-------------------------------------------------------]
			inline QAction* getHideAllLayersQAction() const;
			inline QAction* getHideAllLayersExceptSelectedQAction() const;
			inline QAction* getHideNotInGameLoadedLayersQAction() const;
			inline QAction* getUnhideAllLayersQAction() const;


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
			void onAboutToShowMoveToLayerMenu();
			void onTriggeredMoveToLayerQActionGroup(QAction* qAction);
			void onAboutToShowCopyToLayerMenu();
			void onTriggeredCopyToLayerQActionGroup(QAction* qAction);
			void onTriggeredLoadLayerInGameQAction(bool checked);
			void onTriggeredHideAllLayersQAction(bool checked);
			void onTriggeredHideAllLayersExceptSelectedQAction(bool checked);
			void onTriggeredNotInGameLoadedLayersQAction(bool checked);
			void onTriggeredUnhideAllLayersQAction(bool checked);
			// Slots for other signals
			void onQClipboardDataChanged();
			void onChangeLayer(Layer* layer);

			void onAboutToShowReparentLayerMenu();
			void onTriggeredReparentLayerQActionGroup(QAction* qAction);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
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
			*  @param[in] ignoreSelectedLayer
			*    Indicates if the currently selected layer and its childs should be ignored (true) or not (false)
			*/
			void fillLayerMenu(const Layer& layer, QMenu& qMenu, QActionGroup& qActionGroup, bool ignoreSelectedLayer = false) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map&		  mMap;									///< Map instance to operate on, do not destroy the instance
			QAction*	  mMoveToLayerQAction;					///< Move to layer Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QActionGroup* mMoveToLayerQActionGroup;				///< Move to layer Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuMoveToLayer;					///< Qt menu attached to the move to layer Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QAction*	  mCopyToLayerQAction;					///< Copy to layer Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QActionGroup* mCopyToLayerQActionGroup;				///< Copy to layer Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuCopyToLayer;					///< Qt menu attached to the copy to layer Qt action, always valid, we're responsible for destroying the instance in case we no longer need it
			QAction*	  mLoadLayerInGameQAction;				///< Load layer in-game Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mHideAllLayersQAction;				///< Hide all layers Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mHideAllLayersExceptSelectedQAction;	///< Hide all layers except selected Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mHideNotInGameLoadedLayersQAction;	///< Hide not in-game loaded layers Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mUnhideAllLayersQAction;				///< Unhide all layers Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction*	  mReparentLayerQAction;				///< Reparent layer Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QActionGroup* mReparentLayerQActionGroup;			///< Reparent ayer Qt action group instance which is dynamically filled with concrete Qt actions, can be a null pointer, Qt will automatically destroy the instance
			QMenu*		  mQMenuReparentLayer;					///< Qt menu attached to the reparent  layer Qt action, always valid, we're responsible for destroying the instance in case we no longer need it


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
#include "qsf_editor/qaction/edit/LayerQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::LayerQActions)
