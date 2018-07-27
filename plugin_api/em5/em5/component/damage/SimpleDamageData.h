// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/reflection/type/CampQsfAssetProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class BasePrototypeManager;
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
	*    EMERGENCY 5 simple damage data class: Contains data for destruction
	*/
	class SimpleDamageData
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit SimpleDamageData();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SimpleDamageData();

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

		void hideParticleChildrenAtMapSave(qsf::BasePrototypeManager& prototypeManager);

		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;	///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Meshes
		qsf::AssetProxy	mIntactMesh;
		qsf::AssetProxy	mDamagedMesh;
		qsf::AssetProxy	mDestroyedMesh;

		// Textures
		qsf::AssetProxy	mSmutTexture;
		qsf::AssetProxy	mDamageTexture;

		// Particles
		EntityIdArray	mDamagedLevel1ParticleIds;
		EntityIdArray	mDamagedLevel2ParticleIds;
		EntityIdArray	mDamagedLevel3ParticleIds;
		EntityIdArray	mDestroyedParticleIds;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, SimpleDamageData, std::vector<uint64>, uint64);
		EntityIdArrayInterface DamagedLevel1Particles;
		EntityIdArrayInterface DamagedLevel2Particles;
		EntityIdArrayInterface DamagedLevel3Particles;
		EntityIdArrayInterface DestroyedParticles;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/damage/SimpleDamageData-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SimpleDamageData)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::SimpleDamageData::EntityIdArrayInterface)
