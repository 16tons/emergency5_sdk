// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * \brief Class that implements a node of an n-ary tree.
		 * Usage: just inherit with the inheriting class as template paremeter.
		 * (e.g. class SearchNode : public ERouterTreeNode<SearchNode>)
		 *
		 * \note Nodes must be deleted manually!
		 */
		template<class T>
		class ERouterTreeNode
		{
		public:
			ERouterTreeNode()
			{
				static int idGen = 0;
				mId = ++idGen;
			}
			virtual ~ERouterTreeNode() {}


			int GetID() const;
			void ReserveChildren(int num_);
			void AddChild(int child_);
			void RemoveChild(int child_);
			void ReplaceChild(int child_, int newChild_);

			bool HasChild(int child_) const;

			int GetChildNum() const;
			int  GetChild(int i) const;

		protected:
			int mId;
			typedef std::set<int> ChildSet;
			ChildSet mChildren;
		};

		template<class T>
		int ERouterTreeNode<T>::GetID() const
		{
			return mId;
		}

		template<class T>
		void ERouterTreeNode<T>::ReserveChildren(int num_)
		{
		#ifdef WIN32
			// Attention: Non-standard MSVC C++ extension: std::set did not gain a reserve(...)-method in C++11
			mChildren.reserve(num_);
		#endif
		}

		template<class T>
		void ERouterTreeNode<T>::AddChild(int child_)
		{
			mChildren.insert(child_);
		}

		template<class T>
		void ERouterTreeNode<T>::RemoveChild(int child_)
		{
			mChildren.erase(child_);
		}

		template<class T>
		void ERouterTreeNode<T>::ReplaceChild(int child_, int newChild_)
		{
			ChildSet::iterator it = mChildren.find(child_);
			if (it != mChildren.end())
			{
				mChildren.erase(child_);
				mChildren.insert(newChild_);
			}
		}

		template<class T>
		bool ERouterTreeNode<T>::HasChild(int child_) const
		{
			ChildSet::const_iterator it = mChildren.find(child_);
			return (it != mChildren.end());
		}

		template<class T>
		int ERouterTreeNode<T>::GetChildNum() const
		{
			return (int)mChildren.size();
		}

		template<class T>
		int ERouterTreeNode<T>::GetChild(int i) const
		{
			ChildSet::const_iterator it = mChildren.begin();
			for (int j = 0; j < i; ++j)
			{
				QSF_ASSERT(it != mChildren.end(), "ERouterTreeNode<T>::GetChild: Invalid router tree node", QSF_REACT_NONE);
				++it;
			}
			return *it;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
