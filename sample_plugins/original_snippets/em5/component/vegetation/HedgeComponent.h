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
	*    EMERGENCY 5 hedge component class
	*
	*  @todo
	*    - TODO(fw): A shared base class for trees and hedges would be nice ("em5::VegetationComponent"?)
	*/
	class EM5_API_EXPORT HedgeComponent : public DamageComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::HedgeComponent" unique component ID


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
		inline explicit HedgeComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~HedgeComponent();

		/**
		*  @brief
		*    Call this method to cut down the hedge
		*/
		void cutHedge();

		//[-------------------------------------------------------]
		//[ Damage data getter/setter                             ]
		//[-------------------------------------------------------]
		inline const qsf::AssetProxy& getDestroyedGroundDecalMesh() const;
		inline void setDestroyedGroundDecalMesh(const qsf::AssetProxy& destroyedGroundDecalMesh);


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
		*    Clean up the hedge, i.e. resets it to its initial non-chopped state
		*/
		virtual void cleanup() override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


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
		void setDamageOverlaysActive(bool smutActive, bool emissiveActive);
		void sendGetsDamagedMessage();
		void setCollisionEnabled(bool collisionEnabled);


	//[-------------------------------------------------------]
	//[ Private definition                                    ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			HEDGE_IS_CUT		///< Is set when the hedge is already cut
		};
		typedef qsf::EnumBitmask<uint8, InternalFlags> InternalFlagSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		InternalFlagSet	mInternalFlags;	///< Set of internal flags, see enum "InternalFlags"
		glm::vec3		mScaleTemp;		///< Temp scale for the transform component
		qsf::AssetProxy	mDestroyedGroundDecalMesh;
		uint64			mGroundDecalEntityId;


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
#include "em5/component/vegetation/HedgeComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HedgeComponent)
