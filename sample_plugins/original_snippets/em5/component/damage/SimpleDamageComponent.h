// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/damage/DamageComponent.h"
#include "em5/component/damage/SimpleDamageData.h"

#include <qsf/reflection/type/CampQsfAssetProxy.h>


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
	*    EMERGENCY 5 simple damage component class
	*
	*  @note
	*    - This is only a base class for "em5::BuildingComponent", please don't use it for anything else
	*/
	class EM5_API_EXPORT SimpleDamageComponent : public DamageComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::SimpleDamageComponent" unique component ID


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
		inline explicit SimpleDamageComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SimpleDamageComponent();

		void hideParticleChildrenAtMapSave();

		//[-------------------------------------------------------]
		//[ Damage data getter/setter                             ]
		//[-------------------------------------------------------]
		inline const qsf::AssetProxy& getIntactMesh() const;
		inline void setIntactMesh(const qsf::AssetProxy& mesh);

		inline const qsf::AssetProxy& getDamagedMesh() const;
		inline void setDamagedMesh(const qsf::AssetProxy& mesh);

		inline const qsf::AssetProxy& getDestroyedMesh() const;
		inline void setDestroyedMesh(const qsf::AssetProxy& mesh);

		inline const qsf::AssetProxy& getSmutTexture() const;
		inline void setSmutTexture(const qsf::AssetProxy& smutTexture);

		inline const qsf::AssetProxy& getDamageTexture() const;
		inline void setDamageTexture(const qsf::AssetProxy& damageTexture);

		inline SimpleDamageData::EntityIdArrayInterface& getDamageLevel1Particles();
		inline SimpleDamageData::EntityIdArrayInterface& getDamageLevel2Particles();
		inline SimpleDamageData::EntityIdArrayInterface& getDestroyedParticles();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void applyCurrentMesh();
		const qsf::AssetProxy* getCurrentMesh() const;


	//[-------------------------------------------------------]
	//[ Protected member                                      ]
	//[-------------------------------------------------------]
	protected:
		SimpleDamageData mDamageData;	///< Simple damage data, we are responsible for deleting


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
#include "em5/component/damage/SimpleDamageComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SimpleDamageComponent)
