// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/damage/DamageComponent.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/math/EnumBitmask.h>

#include <glm/gtc/quaternion.hpp> // For glm::quat


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class TreeFallDownAnimation;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 tree component class
	*/
	class EM5_API_EXPORT TreeComponent : public DamageComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TreeFallDownAnimation;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::TreeComponent" unique component ID


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
		inline explicit TreeComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TreeComponent();

		/**
		*  @brief
		*    Call this method to cut down the tree
		*/
		void cutTree(qsf::Entity& firefighterEntity);

		/**
		*  @brief
		*    "true" if the tree is not cut, else false
		*/
		bool isIntact() const;

		/**
		*  @brief
		*    Returns the tree top entity
		*/
		qsf::Entity* getTreeTop();

		/**
		*  @brief
		*    Returns if the tree was cut
		*/
		bool getWasCut() const;

		/**
		*  @brief
		*    Mark the tree as cut or not
		*
		*  @note
		*    This method is mainly used by the multiplayer code on the client side
		*/
		void setWasCut(bool wasCut);


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Check if the tree component is ready for cleanup
		*
		*  @return
		*    Returns "true" if the component can be cleaned up, "false" otherwise
		*/
		inline virtual bool isReadyForCleanup() const override;

		/**
		*  @brief
		*    Clean up the tree, i.e. resets it to its initial non-chopped state
		*/
		virtual void cleanup() override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetIntact() override;
		virtual void onSetDamagedLevel1() override;
		virtual void onSetDamagedLevel2() override;
		virtual void onSetDestroyed() override;
		virtual void onSetFireLifePercentage(float fireLifePercentage) override;
		virtual void onFireStarted(FreeplayEvent* freeplayEvent) override;

		virtual void setCustomParametersForFireGlowAndSmutOverlay(const qsf::MeshComponent& meshComponent) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateMaterialAndLeafParticles(bool treeParticlesActive) const;
		void setDamageOverlaysActive(bool smutActive, bool emissiveActive) const;
		void destroyTreeFallDownAnimation();


	//[-------------------------------------------------------]
	//[ Private definition                                    ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			TREE_IS_CUT		///< Is set when the tree is already cut
		};
		typedef qsf::EnumBitmask<uint8, InternalFlags> InternalFlagSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		InternalFlagSet	mInternalFlags;				///< Set of internal flags, see enum "InternalFlags"
		qsf::JobProxy	mFireFadeAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame
		bool			mTreeParticlesEnabled;
		glm::quat		mInitialChildRotation;		///< Holds the initial rotation of the child entity which defines the tree top
		TreeFallDownAnimation* mTreeFallDownAnimation;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/vegetation/TreeComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::TreeComponent)
