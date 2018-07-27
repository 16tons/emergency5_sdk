// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/PhysicsComponent.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


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
	*    Physics ragdoll component class
	*
	*  @todo
	*    - TODO(co) Under construction
	*/
	class QSF_API_EXPORT PhysicsRagdollComponent : public PhysicsComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsRagdollComponent" unique component ID
		static const uint32 RAGDOLL_PREFAB;	///< "RagdollPrefab" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit PhysicsRagdollComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsRagdollComponent();

		/**
		*  @brief
		*    Return the ragdoll prefab asset to use
		*
		*  @return
		*    The ragdoll prefab asset to use (e.g. "sample/prefab/ragdoll/human"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getRagdollPrefab() const;

		/**
		*  @brief
		*    Set the ragdoll prefab asset to use
		*
		*  @param[in] ragdollPrefab
		*    The ragdoll prefab asset to use (e.g. "sample/prefab/ragdoll/human")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRagdollPrefab(const AssetProxy& ragdollPrefab);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PhysicsComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onCollisionFilterGroupOrCollisionMaskChanged() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AssetProxy mRagdollPrefab;	///< The ragdoll prefab asset to use (e.g. "sample/prefab/ragdoll/human")


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
#include "qsf/physics/advanced/PhysicsRagdollComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsRagdollComponent)
