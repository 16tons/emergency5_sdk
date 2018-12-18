// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/QList>
#include <QtCore/QVariant>


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
		*    Tree class for the base model
		*
		*  @note
		*    - A container for items of data supplied by the base model
		*/
		template<typename T>
		class TreeItem
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				// The QList represents the column, so we can use multi column entries
				TreeItem(const QList<T>& data, TreeItem<T>* parent = nullptr);
				~TreeItem();

				// Add a new child entry
				bool appendChild(TreeItem<T>& child);

				// Add a new child entry at the specified position
				bool insertChild(TreeItem<T>& child, int position);

				// removes the node
				bool deleteChild(int row);

				// Removes only the table entry
				// Attention this could cause memory leaks
				bool deleteChildEntry(int row);

				// Removes all children
				void clearChildrens();

				// Moves a child to a new parent
				bool moveChild(TreeItem<T>& parentItem);

				// Moves a child to a new parent and insert it at the given position
				bool moveChild(TreeItem<T>& parentItem, int position);

				// Returns the parent entry
				TreeItem<T>* parent() const;

				// Returns the child entry from the list
				TreeItem<T>* child(int row) const;

				// Number of children (rows)
				int childCount() const;

				// Number of columns
				int columnCount() const;

				// How big is our tree
				// TODO(ag) Test me please!
				int treeCount(int n = 0) const;

				// Here we get our data to work with
				T* data(int column = 0);

				// The node list row position
				int row() const;

				// For debugging purpose
				void drawTree() const;
				void drawItemWithChildren() const;
				void drawItem() const;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				QList<TreeItem*> mChildItems;
				QList<T>		 mItemData;
				TreeItem<T>*	 mParentItem;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/model/TreeItem-inl.h"
