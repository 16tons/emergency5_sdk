/*=============================================================================
Description: the Dynamic Huffman Encoding Tree class

Revision history:
* Created by Lin Luo, December 5, 2005
=============================================================================*/

#pragma once

#include <unordered_map>

class HuffmanEncodingTree
{
public:
	HuffmanEncodingTree() : mRootNode(nullptr), mZeroNode(nullptr)
	{
	}

	~HuffmanEncodingTree()
	{
		Reset();
	}

	IBitStream* Encode(const BucketByteArray& buffer_, int numBytesToEncode_)
	{
		Reset();

		mRootNode = mZeroNode = MEMORYPOOL_ALLOCATE(s_huffmanEncodingTreeNodePool);

		IBitStream* bitstream = IBitStream::CreateInstance();

		for ( int i = 0; i < numBytesToEncode_; ++i )
		{
			Value2TreeNodeMap::const_iterator itor = mValue2TreeNodeMap.find(buffer_[i]);
			if ( itor == mValue2TreeNodeMap.end() )
			{
				// the value does not exist in the tree yet, write the path from the root node to
				// the zero node, if the tree is not degenerated (empty), then write the value itself
				if (mRootNode != mZeroNode)
					WritePath(mZeroNode, bitstream);
				bitstream->Write( (uint8_t)buffer_[i] );

				// now spawn a new value node and a new NYT node and update the tree
				SpawnAndUpdate(buffer_[i]);
			}
			else
			{
				assert(buffer_[i] == itor->second->value);

				// the value exists in the tree, write out its Huffman code (path from root to node)
				WritePath(itor->second, bitstream);

				// update the tree from this value node up (increment weights and swap nodes, if necessary)
				UpdateTree(itor->second);
			}
		}

		return bitstream;
	}

	bool Decode(const IBitStream* bitstream_, size_t numBitsToDecode_, BucketByteArray& buffer_)
	{
		assert(numBitsToDecode_ <= bitstream_->GetBitLength());

		Reset();

		mRootNode = mZeroNode = MEMORYPOOL_ALLOCATE(s_huffmanEncodingTreeNodePool);

		buffer_.clear();

		size_t bitLength = numBitsToDecode_;
		while (bitLength > 0)
		{
			HuffmanEncodingTreeNode* treeNode = mRootNode;
			while (treeNode != nullptr)
			{
				if (treeNode->left == nullptr && treeNode->right == nullptr) // a leaf node
				{
					if (treeNode->weight == 0) // the 0-node
					{
						if (bitLength < 8)
						{
							// not enough bits to read
							assert(false);
							return false;
						}
						// we have hit the 0-node, read 8 bits for the original character
						uint8_t value = 0;
						bitstream_->Read(value);
						buffer_.push_back( (byte)value );
						bitLength -= 8;

						SpawnAndUpdate( (byte)value );
					}
					else // a leaf node with value
					{
						buffer_.push_back( (byte)treeNode->value );

						UpdateTree(treeNode);
					}

					// restart from the root node
					break;
				}
				else // an internal node
				{
					assert(treeNode->left != nullptr && treeNode->right != nullptr);

					if (bitLength < 1)
					{
						// should have at least 1 bit to read
						assert(false);
						return false;
					}
					bool indicator = false;
					bitstream_->Read(indicator);
					if (indicator == false)
						treeNode = treeNode->left;
					else
						treeNode = treeNode->right;
					--bitLength;

					// continue searching until we reach a leaf node
					continue;
				}
			}
		}

		return true;
	}

private:
	struct HuffmanEncodingTreeNode
	{
		HuffmanEncodingTreeNode() : value(0), weight(0), left(nullptr), right(nullptr), parent(nullptr)
		{
		}

		byte value;
		unsigned weight;

		HuffmanEncodingTreeNode* left;
		HuffmanEncodingTreeNode* right;
		HuffmanEncodingTreeNode* parent;
	};

	static CMemoryPool<HuffmanEncodingTreeNode> s_huffmanEncodingTreeNodePool;

	void Reset()
	{
		if (true)
		{
			Weight2OrderedTreeNodeBlockMap::iterator itor, next = mWeight2OrderedTreeNodeBlockMap.begin();
			while ( next != mWeight2OrderedTreeNodeBlockMap.end() )
			{
				itor = next++;

				while ( !itor->second.empty() )
				{
					s_huffmanEncodingTreeNodePool.Free( itor->second.front() );
					itor->second.pop_front();
				}

				mWeight2OrderedTreeNodeBlockMap.erase(itor);
			}
		}

		if (true)
		{
			Value2TreeNodeMap::iterator itor, next = mValue2TreeNodeMap.begin();
			while ( next != mValue2TreeNodeMap.end() )
			{
				itor = next++;

				mValue2TreeNodeMap.erase(itor);
			}
		}

		s_huffmanEncodingTreeNodePool.Free(mZeroNode);
	}

	void UpdateTree(HuffmanEncodingTreeNode* treeNode_)
	{
		if (treeNode_ == nullptr)
			return;

		bool areParentsUpdated = false;
		HuffmanEncodingTreeNode* treeNode = mWeight2OrderedTreeNodeBlockMap[treeNode_->weight].back();
		if (treeNode_ != treeNode)
		{
			// the node is not the maximum ordered element in its weight block, swap the two nodes if they are not child and parent
			if (treeNode_->parent != treeNode)
			{
				// first swap children of the two nodes
				if (treeNode_->left) treeNode_->left->parent = treeNode;
				if (treeNode_->right) treeNode_->right->parent = treeNode;

				if (treeNode->left) treeNode->left->parent = treeNode_;
				if (treeNode->right) treeNode->right->parent = treeNode_;

				HuffmanEncodingTreeNode* tempNode = nullptr;

				tempNode = treeNode_->left;
				treeNode_->left = treeNode->left;
				treeNode->left = tempNode;

				tempNode = treeNode_->right;
				treeNode_->right = treeNode->right;
				treeNode->right = tempNode;

				// swap values and weights of the two nodes, without touching their orders
				byte value = treeNode_->value;
				treeNode_->value = treeNode->value;
				treeNode->value = value;

				unsigned weight = treeNode_->weight;
				treeNode_->weight = treeNode->weight;
				treeNode->weight = weight;

				// remap value to node if the node is a leaf
				if (treeNode_->left == nullptr && treeNode_->right == nullptr)
					mValue2TreeNodeMap[treeNode_->value] = treeNode_;
				if (treeNode->left == nullptr && treeNode->right == nullptr)
					mValue2TreeNodeMap[treeNode->value] = treeNode;

				// treeNode_ should now point to the swapped node
				treeNode_ = treeNode;

				// pointers in the ordered tree node block remain unchanged
			}
			else
			{
				// if the two nodes are child and parent, we should first manipulate its parents
				UpdateTree(treeNode_->parent);
				areParentsUpdated = true;
			}
		}

		// remove it from the current weight block
		mWeight2OrderedTreeNodeBlockMap[treeNode_->weight].pop_back();

		// increment the weight of the node and add it to the front of the new weight block
		mWeight2OrderedTreeNodeBlockMap[++treeNode_->weight].push_front(treeNode_);

		// update the tree from the node's new parent if not yet updated
		if (!areParentsUpdated)
			UpdateTree(treeNode_->parent);
	}

	void SpawnAndUpdate(byte value_)
	{
		// the old 0-node should now be pushed into its corresponding weight block
		mZeroNode->weight = 1; // the old 0-node now has weight 1
		mWeight2OrderedTreeNodeBlockMap[mZeroNode->weight].push_front(mZeroNode);

		// now spawn a new value node (right) containing the new value
		mZeroNode->right = MEMORYPOOL_ALLOCATE(s_huffmanEncodingTreeNodePool);
		mZeroNode->right->value = value_;
		mZeroNode->right->weight = 1;
		mZeroNode->right->parent = mZeroNode;
		mValue2TreeNodeMap[value_] = mZeroNode->right;
		// lower order tree nodes with the same weight go to the front of the block
		mWeight2OrderedTreeNodeBlockMap[mZeroNode->right->weight].push_front(mZeroNode->right);

		// now spawn a new 0-node (left)
		mZeroNode->left = MEMORYPOOL_ALLOCATE(s_huffmanEncodingTreeNodePool);
		mZeroNode->left->parent = mZeroNode;

		// update the tree from the old 0-node's parent up (increment weights and swap nodes, if necessary)
		UpdateTree(mZeroNode->parent);

		// update the new 0-node
		mZeroNode = mZeroNode->left;
	}

	void WritePath(const HuffmanEncodingTreeNode* treeNode_, IBitStream* bitstream_)
	{
		std::deque<bool> path;

		while (treeNode_->parent != nullptr)
		{
			if (treeNode_ == treeNode_->parent->left)
				path.push_front(false); // false(0) for left child
			else if (treeNode_ == treeNode_->parent->right)
				path.push_front(true); // true(1) for right child
			else
				assert( !_T("treeNode is not a child of its parent!") );

			treeNode_ = treeNode_->parent;
		}
		assert(treeNode_ == mRootNode);

		// write the path code
		for ( size_t i = 0; i < path.size(); ++i )
			bitstream_->Write(path[i]);
	}

	HuffmanEncodingTreeNode* mRootNode;
	HuffmanEncodingTreeNode* mZeroNode;

	typedef std::unordered_map<byte, HuffmanEncodingTreeNode*> Value2TreeNodeMap;
	Value2TreeNodeMap mValue2TreeNodeMap;

	typedef std::list<HuffmanEncodingTreeNode*> OrderedTreeNodeBlock;
	typedef std::unordered_map<unsigned, OrderedTreeNodeBlock> Weight2OrderedTreeNodeBlockMap;
	Weight2OrderedTreeNodeBlockMap mWeight2OrderedTreeNodeBlockMap;
};

