// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/NamedIdentifier.h"
#include "qsf/component/Component.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class SpatialPartitionManagerComponent;
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
	*    QSF spatial partition member component
	*
	*  @note
	*    About usage: To add a entity to a spatial partition, just add this
	*    component to the entity and request partition membership with "setPartitionMember"
	*
	*    Another use case would be to add all entities with a component of a specific type to a well-known partition.
	*    This can be accomplished by adding code like this to the onStartup/onShutdown method of the
	*    components which should enforce partition membership:
	*
	*	 bool MyComponent::onStartup()
	*	 {
	*        // getOrCreateComponent() ensures that the member component will be created on-demand
	*		 SpatialPartitionMemberComponent* partitionMember = getEntity().getOrCreateComponent<SpatialPartitionMemberComponent>();
	*
	*		 if (partitionMember)
	*		 {
	*            // Register the entity as a member of the partition with the Id 0
	*			 partitionMember->setPartitionMember(0, true);
	*		 }
	*
	*		 return true;
	*	}
	*
	*	 void MyComponent::onShutdown()
	*	 {
	*	     // Note the use of "getComponent" instead of "getOrCreateComponent" which should NOT be used inside shutdown methods!
	*		 SpatialPartitionMemberComponent* partitionMember = getEntity().getComponent<SpatialPartitionMemberComponent>();
	*
	*		 if (partitionMember)
	*		 {
	*            // Unegister the entity as a member of the partition with the Id 0
	*			 partitionMember->setPartitionMember(0, false);
	*		 }
	*	}
	*
	*/
	class QSF_API_EXPORT SpatialPartitionMemberComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SpatialPartitionManagerComponent;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::SpatialPartitionMemberComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		// TODO(fw): Move this stuff elsewhere maybe?
		/**
		*  @brief
		*    Register a component instance in the partition associated with its component ID
		*/
		static void registerComponentInPartition(Component& component);

		/**
		*  @brief
		*    Unregister a component instance from the partition associated with its component ID
		*/
		static void unregisterComponentInPartition(Component& component);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit SpatialPartitionMemberComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~SpatialPartitionMemberComponent();

		void setPartitionMember(uint32 partitionIndex, bool isMember, Component* quickDirectReference);

		inline const std::pair<glm::vec3, glm::vec3>& getBounds() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;

		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		SpatialPartitionManagerComponent& getManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec3 mPosition;
		std::pair<glm::vec3, glm::vec3> mBounds;


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
#include "qsf/component/spatial/SpatialPartitionMemberComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::SpatialPartitionMemberComponent)
