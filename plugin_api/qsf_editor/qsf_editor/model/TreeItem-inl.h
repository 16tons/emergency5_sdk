// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/log/LogSystem.h>

#include <QtCore/QStringList>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		template <typename T>
		TreeItem<T>::TreeItem(const QList<T>& data, TreeItem<T>* parent) :
			mItemData(data),
			mParentItem(parent)
		{
			// Nothing to do in here
		}

		template <typename T>
		TreeItem<T>::~TreeItem()
		{
			clearChildrens();
		}

		template <typename T>
		bool TreeItem<T>::appendChild(TreeItem<T>& item)
		{
			mChildItems.append(&item);

			return true;
		}

		template <typename T>
		bool TreeItem<T>::insertChild(TreeItem<T>& child, int position)
		{
			if (position < 0 || position > mChildItems.size())
				return false;

			mChildItems.insert(position, &child);

			return true;
		}

		template <typename T>
		bool TreeItem<T>::deleteChild(int row)
		{
			if (row >= 0 && mChildItems.size() > row)
			{
				TreeItem* treeItem = mChildItems.takeAt(row); // Removes the item also from row position
				if (nullptr != treeItem)
				{
					QSF_CHECK(!treeItem->childCount(), "Deleted tree item has " << treeItem->childCount() << " children", QSF_REACT_NONE);
					delete treeItem;
					treeItem = nullptr;
				}

				return true;
			}
			else
			{
				QSF_ERROR("Deleted tree item is out of range", QSF_REACT_NONE);
			}

			return false;
		}

		template <typename T>
		bool TreeItem<T>::deleteChildEntry(int row)
		{
			if (row >= 0 && mChildItems.size() > row)
			{
				mChildItems.removeAt(row);
				return true;
			}
			else
			{
				QSF_ERROR("Deleted tree item is out of range", QSF_REACT_NONE);
			}

			return false;
		}

		template <typename T>
		void TreeItem<T>::clearChildrens()
		{
			qDeleteAll(mChildItems);
			mChildItems.clear();
		}

		template<typename T>
		bool TreeItem<T>::moveChild(TreeItem<T>& parentItem)
		{
			// Remove the entry from the list
			if (!parent()->deleteChildEntry(row()))
				return false;

			// Add the entry to the new layer
			if (!parentItem.appendChild(*this))
				return false;

			// Set the parent
			mParentItem = &parentItem;

			return true;
		}

		template<typename T>
		bool TreeItem<T>::moveChild(TreeItem<T>& parentItem, int position)
		{
			// Remove the entry from the list
			if (!parent()->deleteChildEntry(row()))
				return false;

			// Add the entry to the new layer
			if (position >= parentItem.childCount() || position < 0)
			{
				if (!parentItem.appendChild(*this))
					return false;
			}
			else
			{
				if (!parentItem.insertChild(*this, position))
					return false;
			}

			// Set the parent
			mParentItem = &parentItem;

			return true;
		}

		template <typename T>
		TreeItem<T>* TreeItem<T>::parent() const
		{
			return mParentItem;
		}

		template <typename T>
		TreeItem<T>* TreeItem<T>::child(int row) const
		{
			return (row >= 0 && mChildItems.size() > row) ? mChildItems[row] : nullptr;
		}

		template <typename T>
		int TreeItem<T>::childCount() const
		{
			return mChildItems.count();
		}

		template <typename T>
		int TreeItem<T>::columnCount() const
		{
			return mItemData.count();
		}

		template <typename T>
		int TreeItem<T>::treeCount(int n) const
		{
			++n;

			for (int i = 0; mChildItems.size() > i; ++i)
			{
				n = mChildItems[i]->treeCount(n);
			}

			return n;
		}

		template <typename T>
		T* TreeItem<T>::data(int column)
		{
			// Don't use ".value()" here, else you can't write to the reference
			return &mItemData[column];
		}

		template <typename T>
		int TreeItem<T>::row() const
		{
			return (nullptr != mParentItem) ? mParentItem->mChildItems.indexOf(const_cast<TreeItem<T>*>(this)) : 0;
		}

		template <typename T>
		void TreeItem<T>::drawTree() const
		{
			// Find root
			TreeItem* rootItem = this;
			while (nullptr != rootItem->parent())
			{
				rootItem = rootItem->parent();
			}

			// Draw full tree
			rootItem->drawItemWithChildren();
		}

		template <typename T>
		void TreeItem<T>::drawItemWithChildren() const
		{
			drawItem();

			// Recursive draw all children
			for (int i = 0; i < childCount(); ++i)
			{
				child(i)->drawItemWithChildren();
			}
		}

		template <typename T>
		void TreeItem<T>::drawItem() const
		{
			std::stringstream builder;
			builder << "item: " << row() + 1 << "/" << ((nullptr != parent()) ? parent()->childCount() : 1) << "\n";
			for (int i = 0; i < columnCount(); ++i)
			{
				builder << "column " << i + 1 << ": " << mItemData.value(i);
			}
			QSF_LOG_PRINT(INFO, builder.str().c_str());
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
