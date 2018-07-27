// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/dialog/material/Query.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <QtWidgets/qitemdelegate.h>

#include <set>
#include <mutex>
#include <queue>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QStandardItem;
	class QStandardItemModel;
	class QSortFilterProxyModel;
QT_END_NAMESPACE
namespace Ui
{
	class MaterialBrowserDialog;
}
namespace qsf
{
	namespace editor
	{
		class Action;
		class AssetEditHelper;
		class MaterialPropertyDelegate;
		class MaterialBrowserPropertyFilterModel;
	}
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
		*    QSF editor material dialog
		*/
		class MaterialBrowserDialog : public Dialog
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend MaterialPropertyDelegate;


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;		///< "qsf::editor::MaterialBrowserDialog" unique pluginable dialog ID
			static const int	TYPE_USER_ROLE;		///< Qt user role storing the material property type
			static const int	SOURCE_USER_ROLE;	///< Qt user role storing the material property source
			static const int	VALUE_USER_ROLE;	///< Qt user role storing the material property internal value (shown text might have been changed for the user)


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			void setQStandardItemTextValue(QStandardItem& qStandardItem, const QString& text);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] dialogManager
			*    The dialog manager this dialog instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			MaterialBrowserDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MaterialBrowserDialog();

			/**
			*  @brief
			*    Select material given by a global material asset ID
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the material to select
			*/
			void selectMaterialByGlobalAssetId(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Create a new material
			*
			*  @param[in] copyFromGlobalAssetId
			*    Global asset ID of the asset the new asset is a copy from, can be "qsf::getUninitialized<GlobalAssetId>()" if it's no initially known copy and the user has to decide
			*/
			void createNewMaterialAsset(GlobalAssetId copyFromGlobalAssetId = getUninitialized<GlobalAssetId>());


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			const BaseMaterialProperty* getSelectedMaterialBaseMaterialProperty(const std::string& propertyName) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void closeEvent(QCloseEvent* event) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QObject methods                     ]
		//[-------------------------------------------------------]
		protected:
			// TODO(co) Just a test
			virtual void timerEvent(QTimerEvent* qTimerEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onIdle();

			void onMaterialSelectionChanged(const QModelIndex& current, const QModelIndex& previous);
			void onDoubleClickedMaterialList(const QModelIndex& index);
			void onCustomContextMenuRequestedMaterialList(const QPoint& point);

			void onMaterialPropertyChanged(QStandardItem* item);

			void onMaterialUndoCleanChanged(bool isClean);
			void onMaterialUndoIndexChanged(int index);

			void onClickedMaterialView(const QModelIndex& index);
			void onCustomContextMenuRequestedMaterialView(const QPoint& point);

			void onMaterialPicked(GlobalAssetId globalAssetId);

			void onSearchFieldTextChangedMaterial(const QString& arg1);
			void onSearchFieldTextChangedProperty(const QString& arg1);
			void on_actionSave_triggered();
			void on_actionPickMaterial_triggered();
			void on_actionNewMaterial_triggered();
			void on_actionDeleteMaterial_triggered();
			void on_actionExit_triggered();
			void on_actionCloneMaterial_triggered();
			void on_actionDeleteProperty_triggered();
			void on_actionSaveMaterial_triggered();
			void on_actionForceCompileMaterial_triggered();
			void on_actionNewProperty_triggered();
			// QColorDialog
			void onCurrentColorChanged(const QColor& color);

			void onChangeBaseMaterialTriggered(QAction* triggeredQAction);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void clearMaterialPropertyModel();
			void queueAction(Action* action);
			void requestQuery(Query* query);

			void updateMaterialBrowserTitle();

			void rebuildMaterialList();
			void buildMaterialModel(const MaterialQuery& materialQuery);

			GlobalAssetId getSelectedMaterialGlobalAssetId() const;
			std::string getSelectedMaterialGlobalAssetIdAsString() const;

			void setSelectedMaterialCleanState(bool isClean);

			std::string getPropertyKey(const QModelIndex& index) const;
			std::string getPropertyValue(const QModelIndex& index) const;

			void directlySetSelectedMaterialPropertyValue(const QString& propertyValue);

			void addProperty(QStandardItem* parent, const std::string& propertyName, const EditorMaterialProperty& materialProperty, bool needsTextureCompilation, bool scrollTo = false);

			QString valueToItemText(const std::string& value, BaseMaterialProperty::Type type) const;

			void createMaterialUndoRedoActions(QUndoStack& qUndoStack);
			void clearMaterialUndoRedoActions();

			/**
			*  @brief
			*    Called as soon as submitting the new material asset is done
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the newly created material asset
			*/
			void onNewMaterialAssetSubmitDone(GlobalAssetId globalAssetId);

			bool isCurrentMaterialAssetWritable() const;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct SynchronizationState
			{
				std::mutex mUpdateMutex;
				std::mutex mActionMutex;
				std::mutex mQueryMutex;
			};

			/**
			*  @brief
			*    A snapshot of the material system's state. Lock the mUpdateMutex before accessing.
			*/
			struct MaterialSystemState
			{
				std::vector<std::string> mMaterialList;

				std::vector<std::string> mMaterialFiles;
				std::vector<std::string> mConfigurationFiles;
			};


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::MaterialBrowserDialog*	mUiMaterialBrowserDialog;
			volatile bool				mRequestShowWindow;	// Really should be an std::atomic
			volatile bool				mRequestExit;		// Dito
			SynchronizationState		mSync;
			// Material dialog
			QStandardItemModel*					mMaterialModel;
			QSortFilterProxyModel*				mMaterialProxyModel;
			QStandardItemModel*					mMaterialPropertyModel;
			MaterialBrowserPropertyFilterModel*	mMaterialBrowserPropertyFilterModel;
			/// TODO(co) Is there a better way to ignore manual model changes?
			int					mUpdateQtTimerId;	///< Update Qt timer ID, "0" means no timer
			bool				mMaterialListUpdateRequired;
			bool				mIgnoreMaterialChange;
			bool				mIgnoreMaterialPropertyChange;
			std::queue<Action*>	mActionQueue;
			std::vector<Query*>	mQueries;
			MaterialSystemState	mState;
			GlobalAssetId		mCurrentGlobalAssetId;	///< ID to be used as current global asset ID, "qsf::getUninitialized<GlobalAssetId>()" is there's no current global asset ID
			QStandardItem*		mCurrentQStandardItem;
			QAction*			mMaterialUndoAction;
			QAction*			mMaterialRedoAction;
			QUndoStack*			mCurrentQUndoStack;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


		class MaterialPropertyDelegate : public QItemDelegate
		{
			Q_OBJECT

		public:
			MaterialPropertyDelegate(MaterialBrowserDialog& dialog, QObject* parent = 0);

			QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
			void destroyEditor(QWidget* widget, const QModelIndex& index) const;

			void setEditorData(QWidget* editor, const QModelIndex& index) const;
			void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

			void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

		private Q_SLOTS:
			void onCurrentIndexChanged(int index);
			void onValueChanged(double value);

		private:
			std::string getPropertyKey(const QModelIndex& index) const;

		private:
			MaterialBrowserDialog& mMaterialBrowserDialog;
			mutable QModelIndex	mWorkingIndex; // Gets changed via create editor
			mutable bool mSetDataToEditor;	// Indicates if we change the content of the editor
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::MaterialBrowserDialog)
