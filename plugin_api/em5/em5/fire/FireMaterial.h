// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/reflection/type/CampQsfTime.h>


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
	*    Fire material class
	*
	*  @remarks
	*    A fire material contains detailed information about the behaviour of a specific fire. Its represented by an asset.
	*
	*    An entity can have multiple child entities with fire components. Every fire component has a fire material.
	*    All visual effects are part of the fire component of an entity.
	*/
	class EM5_API_EXPORT FireMaterial : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public static constants                               ]
	//[-------------------------------------------------------]
	public:
		static const std::string DEFAULT_FIREMATERIAL_NAME;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit FireMaterial(uint32 key);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FireMaterial();

		/**
		*  @brief
		*    Get unique fire material ID
		*/
		inline uint32 getFireMaterialId() const;

		/**
		*  @brief
		*    Set fire material ID
		*/
		inline void setFireMaterialId(uint32 id);

		/**
		*  @brief
		*    Get name of the fire material
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set name of the fire material
		*/
		inline void setName(const std::string& name);

		/**
		*  @brief
		*    Get the combustion value, if this value is reached, the fire starts burning
		*/
		inline float getCombustionValue() const;

		/**
		*  @brief
		*    Set the combustion value, if this value is reached, the fire starts burning
		*/
		inline void setCombustionValue(float combustionValue);

		/**
		*  @brief
		*    Get the burn time, the time the entity burns after start fire and no other touching
		*/
		inline qsf::Time getBurnTime() const;

		/**
		*  @brief
		*    Set the burn time, the time the entity burns after start fire and no other touching
		*/
		inline void setBurnTime(qsf::Time burnTime);

		/**
		*  @brief
		*    Get the life value, the amount of energy the entity can take till it is completely destroyed; its greater or equal than the charge time
		*
		*  @note
		*    - The life value is no member variable, it uses other values for calculating the value
		*/
		float getLifeValue() const;

		/**
		*  @brief
		*    Set the life value, the amount of energy the entity can take till it is completly destroyed; must be greater than the charge time
		*
		*  @note
		*    - The life value is no member variable, it changes the life time of the entity
		*/
		void setLifeValue(float lifeValue);

		/**
		*  @brief
		*    Get the start energy value, the amount of energy the fire starts with
		*/
		inline float getStartEnergyValue() const;

		/**
		*  @brief
		*    Set the start energy value, the amount of energy the fire starts with
		*/
		inline void setStartEnergyValue(float startEnergyValue);

		/**
		*  @brief
		*    Get the maximum energy a fire can have
		*/
		inline float getMaxEnergyValue() const;

		/**
		*  @brief
		*    Set the maximum energy a fire can have
		*/
		inline void setMaxEnergyValue(float maxEnergyValue);

		/**
		*  @brief
		*    Get the time the fire needs to grow to his maximum energy value
		*/
		inline qsf::Time getChargeTime() const;

		/**
		*  @brief
		*    Set the time the fire needs to grow to his maxium energy value
		*/
		inline void setChargeTime(qsf::Time chargeTime);

		/**
		*  @brief
		*    Get the value the fire grows per second (eg. "5")
		*
		*  @note
		*    - The value is not stored inside a member variable, it's calculated from "(mMaxEnergyValue - mStartEnergyValue) / mChargeTime"
		*/
		inline float getChargeValue() const;

		/**
		*  @brief
		*    Set the value the fire grows per second (eg. "5")
		*
		*  @note
		*    - The value is not stored inside a member variable, it's set via the charge time
		*/
		inline void setChargeValue(float chargeValue);

		/**
		*  @brief
		*    Get the self cooling value, amount of energy the fire shrinks in case of an extinguished fire
		*/
		inline float getSelfCoolingValue() const;

		/**
		*  @brief
		*    Set the self cooling value, amount of energy the fire shrinks in case of an extinguished fire
		*/
		inline void setSelfCoolingValue(float selfCoolingValue);

		/**
		*  @brief
		*    Get the fire resistance value, amount of energy is substracted of receiving energy
		*/
		inline float getFireResistance() const;

		/**
		*  @brief
		*    Set the fire resistance value, amount of energy is substracted of receiving energy
		*/
		inline void setFireResistance(float fireResistance);

		/**
		*  @brief
		*    Get the blast force value, amount of energy is given to entities in the blast radius in case of explosion
		*/
		inline float getBlastForce() const;

		/**
		*  @brief
		*    Set the blast force value, amount of energy is given to entities in the blast radius in case of explosion
		*/
		inline void setBlastForce(float blastForce);

		/**
		*  @brief
		*    Get the blast radius, entities in the radius receive the blast force value in case of a combustion
		*/
		inline float getBlastRadius() const;

		/**
		*  @brief
		*    Set the blast radius, entities in the radius receive the blast force value in case of a combustion
		*/
		inline void setBlastRadius(float blastRadius);

		/**
		*  @brief
		*    Check whether this material can explode at all
		*/
		inline bool canExplode() const;

		/**
		*  @brief
		*    Get the full amount of energy, the fire would damage the entity (see setLifeValue() for details) in his "heating-phase"
		*
		*  @note
		*    - This value is smaller or equal to the LifeValue of the entities (because it's only a part of the full burning process)
		*/
		float getAmountChargeValue() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		 mFireMaterialId;	///< ID of the fire material
		std::string	 mName;				///< Name of the fire material
		float		 mCombustionsValue;	///< Combustion value is the amount of fire energy the fire needs to ignite fire
		qsf::Time	 mBurnTime;			///< Time the entity can burn, before it is completly destroyed
		float		 mStartEnergyValue;	///< In case the entity starts burning, it has this amount of starting energy
		float		 mMaxEnergyValue;	///< This amount of energy is the highest possible
		qsf::Time	 mChargeTime;		///< Time fire need to grow to maximum energy value
		float		 mSelfCoolingValue;	///< The fire loses this amount of energy every second
		float		 mFireResistance;	///< Ignore this amount of energy every second
		float		 mBlastForce;		///< One time damage in case of explosion
		float		 mBlastRadius;		///< Radius of explosions


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
#include "em5/fire/FireMaterial-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireMaterial)
