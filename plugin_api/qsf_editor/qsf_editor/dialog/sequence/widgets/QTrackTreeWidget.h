// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>

#include <QtWidgets/qtreewidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMenu;
QT_END_NAMESPACE
namespace qsf
{
	namespace logic
	{
		class SequenceTrack;
		class SequenceTrackEntityComponentProperty;
	}
	namespace editor
	{
		class SequenceDialog;
		class SequenceEditorWrapper;
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
		*    TODO(co) Comment me
		*/
		class QTrackTreeWidget : public QTreeWidget
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			QTrackTreeWidget(SequenceDialog* dialog, QWidget* parent);
			void rebuildTrackTree();

			/**
			*  @brief
			*    Return tree item name of a given sequence track instance
			*/
			QString getItemName(const logic::SequenceTrack& sequenceTrack) const;

			/**
			*  @brief
			*    Looks up the tree item for the given sequence track
			*/
			QTreeWidgetItem* findParentItemForSequenceTrack(const logic::SequenceTrack& sequenceTrack) const;

			void setActiveSequence(SequenceEditorWrapper* sequence);


		//[-------------------------------------------------------]
		//[ Public Qt slots (MOC)                                 ]
		//[-------------------------------------------------------]
		public Q_SLOTS:
			void enabledEntityAdded(uint64 entityId);
			void notifySelectionChanged();
			void sequenceTrackAdded(uint32 newTrackId);
			void handleTrackAdded(logic::SequenceTrack& sequenceTrack);
			void sequenceTrackRemoved(uint32 trackId);


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void showEvent(QShowEvent*) override;
			virtual void dragEnterEvent(QDragEnterEvent* qDragEnterEvent) override;
			virtual void dragMoveEvent(QDragMoveEvent* event) override;
			virtual void dropEvent(QDropEvent* qDropEvent) override;
			virtual void contextMenuEvent(QContextMenuEvent* qContextMenuEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QAbstractItemView methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			enum eTreeItems
			{
				TREE_ITEM_DIRECTOR,
				TREE_ITEM_ENTITY,
				TREE_ITEM_ENTITY_COMPONENT_PROPERTY_TRACK,
				TREE_ITEM_ANY_TRACK
			};


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onAddECPTrackToEntity(QAction* action);
			void onAddDirectorTrack(QAction* action);
			void onRemoveTrack(QAction* action);
			void onRemoveAllTracksFromEntity();
			void onItemDoubleClicked(QTreeWidgetItem* item, int column);
			void onTriggeredExpandAllSequenceTracks();
			void onTriggeredCollapseAllSequenceTracks();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void fillContextMenuWithDefaultActions(QMenu& contextMenu) const;

			/**
			*  @brief
			*    Looks up the tree item for the given sequence track
			*/
			QTreeWidgetItem* findParentItemForSequenceTrackRecursive(const logic::SequenceTrack& sequenceTrack, QTreeWidgetItem& parentItem) const;

			/**
			*  @brief
			*    Create a track sub-item and adds it to the tree
			*/
			void createEntityComponentPropertyTrackItemIntoTree(logic::SequenceTrackEntityComponentProperty& ecpTrack, QTreeWidgetItem* entityItem, uint64 entityId);

			void createTrackItemIntoTree(const QString& trackName, QTreeWidgetItem* parentItem, logic::SequenceTrack& sequenceTrack, const QList<QVariant>& trackData);

			/**
			*  @brief
			*    Deep-adds a track to the tree (i.e. it also creates parent items in the tree view etc. if they are not already existing)
			*/
			void addTrackToTree(logic::SequenceTrackEntityComponentProperty& track);	// TODO(co) Unused method?

			/**
			*  @brief
			*    Looks up the tree item for the given entity Id, so track items can be queried/added thereunder
			*/
			QTreeWidgetItem* createOrFindParentItemForEntity(uint64 trackEntityId);

			/**
			*  @brief
			*    Look up or created item for the director tracks
			*/
			QTreeWidgetItem* createOrFindDirectorItem();

			/**
			*  @brief
			*    Internal notification for entity
			*/
			void entitySelectionChangedInternally(uint64 entityId);

			//[-------------------------------------------------------]
			//[ Qt user data list data extraction                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the sequence track instance assigned with the tree item
			*/
			logic::SequenceTrack* getSequenceTrack(const QList<QVariant>& itemData) const;

			/**
			*  @brief
			*    Return the entity component property sequence track instance assigned with the tree item
			*/
			logic::SequenceTrackEntityComponentProperty* getSequenceTrackEntityComponentProperty(const QList<QVariant>& itemData) const;

			/**
			*  @brief
			*    Return the tree item type
			*/
			eTreeItems getItemType(const QList<QVariant>& itemData) const;

			/**
			*  @brief
			*    Return the ID of the entity assigned with the tree item
			*/
			uint64 getItemEntityId(const QList<QVariant>& itemData) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceDialog*			mSequenceDialog;
			SequenceEditorWrapper*	mSequenceEditorWrapper;
			float					mBeginTime;
			float					mEndTime;
			bool					mIsUpdatingSelectionFromSignal;	///< Flag needed to avoid creating selection operations when we actually update the selection from an operation


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
