// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/spatial/SpatialPartitionMemberComponent.h"
#include "qsf/component/Component.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class SpatialComponentPartition2D;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF spatial partition manager component class
	*    To look up entities which are members of spatial partitions, you're right here.
	*    To add entities into a spatial partition, please look at SpatialPartitionMemberComponent
	*
	*    There is a maximum number of MAX_PARTITIONS (default: 32) partitions available. These
	*    are assigned by availability. So you can always have a maximum of 32 different partitions in use.
	*
	*    Externally, partitions are identified by 32bit IDs which makes is easy to e.g. re-use Component-type IDs
	*    as partition IDs when a grouping of partitions by specific components is favored
	*
	*  @note
	*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
	*/
	class QSF_API_EXPORT SpatialPartitionManagerComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SpatialPartitionMemberComponent;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::SpatialPartitionManagerComponent" unique component ID
		static const uint32 MAX_PARTITIONS = 32;

		struct Member
		{
			uint32 partitionMembershipBitset;
		};

		// Shortcuts
		typedef boost::container::flat_map<SpatialPartitionMemberComponent*, Member> MemberMap;		// Map to store member -> reference relations


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit SpatialPartitionManagerComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~SpatialPartitionManagerComponent();

		/**
		*  @brief
		*    Looks up all components which are known to be inside the given AABB. Only looks into
		*    the given partition.
		*
		*  @note
		*    The result set of components are references to the components which have been registered as
		*    the "quickDirectReference" of the insertMember() call.
		*/
		bool findComponentsInBounds(uint32 partitionId, const glm::vec2& min, const glm::vec2& max, std::vector<Component*>& outComponentsFound) const;

		/**
		*  @brief
		*    Looks up all components which are known to be inside the given circle. Only looks into
		*    the given partition.
		*
		*  @note
		*    The result set of components are references to the components which have been registered as
		*    the "quickDirectReference" of the insertMember() call.
		*/
		bool findComponentsInCircle(uint32 partitionId, const glm::vec2& center, float radius, std::vector<Component*>& outComponentsFound) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void onShutdown() override;
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Insert event tag component to the manager
		*
		*  @param[in] member
		*    Reference to the actual member component which is doing this registration
		*  @param[in] partitionId
		*    32bit id of the partition the component should be added to
		*  @param[in] quickDirectReference
		*    Component reference to store in the partition, which is also returned in lookups/searches
		*/
		void addMember(SpatialPartitionMemberComponent& member, uint32 partitionId, Component* quickDirectReference);

		/**
		*  @brief
		*    Remove from a specific partition
		*/
		void removeMember(SpatialPartitionMemberComponent& member, uint32 partitionId);

		/**
		*  @brief
		*    Remove from all partitions
		*/
		void removeMember(SpatialPartitionMemberComponent& member);

		/**
		*  @brief
		*    Invoked by members when they notice that they moved
		*/
		void memberMoved(SpatialPartitionMemberComponent& member);

		SpatialComponentPartition2D* findPartition(uint32 partitionId);
		const SpatialComponentPartition2D* findPartition(uint32 partitionId) const;
		bool findPartitionIndex(uint32 partitionId, uint32& outIndex) const;
		SpatialComponentPartition2D* addPartition(uint32 partitionId);
		void removePartition(uint32 partitionId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MemberMap					  mMembers; ///< Set of all members
		SpatialComponentPartition2D** mPartitions;
		void*						  mPartitionsMemory;

		uint32 mPartitionIds[MAX_PARTITIONS];
		bool   mPartitionUsed[MAX_PARTITIONS];


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/spatial/SpatialPartitionManagerComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::SpatialPartitionManagerComponent)
