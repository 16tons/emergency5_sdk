// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
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
	*    EMERGENCY 5 fire particle component class
	*/
	class EM5_API_EXPORT FireParticleOptionsComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::FireParticleOptionsComponent" unique component ID


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
		inline explicit FireParticleOptionsComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FireParticleOptionsComponent();

		/**
		*  @brief
		*    Return "true" if the particle should be shown when damage level intact is reached
		*/
		inline bool getShowOnIntact() const;

		/**
		*  @brief
		*    Set if the particle should be shown when damage level intact is reached
		*/
		inline void setShowOnIntact(bool show);

		/**
		*  @brief
		*    Return "true" if the particle should be shown when damage level 1 is reached
		*/
		inline bool getShowOnDamageLevel1() const;

		/**
		*  @brief
		*    Set if the particle should be shown when damage level 1 is reached
		*/
		inline void setShowOnDamageLevel1(bool show);

		/**
		*  @brief
		*    Return "true" if the particle should be shown when damage level 2 is reached
		*/
		inline bool getShowOnDamageLevel2() const;

		/**
		*  @brief
		*    Set if the particle should be shown when damage level 2 is reached
		*/
		inline void setShowOnDamageLevel2(bool show);

		/**
		*  @brief
		*    Return "true" if the particle should be shown when damage level 3 is reached
		*/
		inline bool getShowOnDamageLevel3() const;

		/**
		*  @brief
		*    Set if the particle should be shown when damage level 3 is reached
		*/
		inline void setShowOnDamageLevel3(bool show);

		/**
		*  @brief
		*    Return "true" if the particle should be shown for object explosion
		*/
		inline bool getShowOnExplosion() const;

		/**
		*  @brief
		*    Set if the particle should be shown for object explosion
		*/
		inline void setShowOnExplosion(bool show);

		/**
		*  @brief
		*    Return "true" if the particle should be shown when the fire state is large -> false when for the small fire state
		*/
		inline bool getShowOnLargeFireState() const;

		/**
		*  @brief
		*    Set if the particle should be shown when the fire state is large (on false it will shown on fire state small)
		*/
		inline void setShowOnLargeFireState(bool show);

		/**
		*  @brief
		*    Returns the entity ID of the fire light
		*/
		inline uint64 getFireLightEntityId() const;

		/**
		*  @brief
		*    Set the entity ID of the fire light to be used
		*/
		inline void setFireLightEntityId(uint64 fireLightEntity);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum DamageLevelFlags
		{
			DAMAGE_INTACT,
			DAMAGE_LEVEL1,
			DAMAGE_LEVEL2,
			DAMAGE_LEVEL3,
			DAMAGE_EXPLOSION
		};
		typedef qsf::EnumBitmask<uint8, DamageLevelFlags> DamageLevelFlagsSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DamageLevelFlagsSet	mDamageLevelFlags;
		bool				mShowOnLargeFireSate;
		uint64				mFireLightEntityId;		///< The ID of a fire light which gets acitivates on fire particle start and disabled on fire particle stop


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
#include "em5/fire/component/FireParticleOptionsComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireParticleOptionsComponent)
