// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <camp/arraymapper.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const NodesComponent::DebugFlagSet& NodesComponent::getDebugFlagSet() const
	{
		return mDebugFlagSet;
	}

	inline void NodesComponent::setDebugFlagSet(const DebugFlagSet& debugFlagSet)
	{
		// Is there a state change?
		if (mDebugFlagSet.getStorage() != debugFlagSet.getStorage())
		{
			// Backup the new flag set
			mDebugFlagSet = debugFlagSet;

			// Call the virtual method
			onSetDebugFlagSet();
		}
	}

	inline uint32 NodesComponent::getNumberOfNodes() const
	{
		return static_cast<uint32>(mNodes.size());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::NodesComponent methods            ]
	//[-------------------------------------------------------]
	inline uint32 NodesComponent::getMiniumNumberOfNodes() const
	{
		return 2;
	}

	inline const std::vector<Node>& NodesComponent::getNodes() const
	{
		return mNodes;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t NodesComponent::NodesInterface::size() const
	{
		return getOwner().getNodes().size();
	}

	inline const Node& NodesComponent::NodesInterface::get(std::size_t index) const
	{
		return getOwner().getNodes().at(index);
	}

	inline void NodesComponent::NodesInterface::set(std::size_t index, const Node& value)
	{
		getOwner().setNode(index, value);
	}

	inline void NodesComponent::NodesInterface::insert(std::size_t before, const Node& value)
	{
		getOwner().insertNode(before, value);
	}

	inline void NodesComponent::NodesInterface::remove(std::size_t index)
	{
		getOwner().removeNode(index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
