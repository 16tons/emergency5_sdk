// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Equipment component class
		*/
		class QSF_GAME_API_EXPORT EquipmentComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::EquipmentComponent" unique component ID

			enum EquipmentType
			{
				EQUIPMENTTYPE_NONE = 0,		///< The default value for equipment
				EQUIPMENTTYPE_RIGHTHANDED,
				EQUIPMENTTYPE_LEFTHANDED,
				EQUIPMENTTYPE_TWOHANDED,
				EQUIPMENTTYPE_RIGHTHANDED_TO_EQUIP,	///< To support bone to bone link for right hand
				EQUIPMENTTYPE_LEFTHANDED_TO_EQUIP,	///< To support bone to bone link for left hand
				EQUIPMENTTYPE_RIGHTARM,		///< To support bandaged arms
				EQUIPMENTTYPE_LEFTARM,		///< To support bandaged arms
				EQUIPMENTTYPE_RIGHTUPPERARM,
				EQUIPMENTTYPE_LEFTUPPERARM,
				EQUIPMENTTYPE_RIGHTLEG,		///< To support bandaged legs
				EQUIPMENTTYPE_LEFTLEG,		///< To support bandaged legs
				EQUIPMENTTYPE_BODY,
				EQUIPMENTTYPE_BACK,
				EQUIPMENTTYPE_HEAD
			};


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
			inline explicit EquipmentComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~EquipmentComponent();

			inline const std::string& getEquipmentName() const;
			inline void setEquipmentName(const std::string& name);

			inline const glm::vec3& getPositionOffset() const;
			inline void setPositionOffset(const glm::vec3& offset);

			inline bool getEquipFromVehicle() const;
			inline void setEquipFromVehicle(bool equipFromVehicle);

			inline const AssetProxy& getIdleAnimation() const;
			inline void setIdleAnimation(const AssetProxy& idleAnimation);

			inline const AssetProxy& getRunAnimation() const;
			inline void setRunAnimation(const AssetProxy& runAnimation);

			inline EquipmentType getEquipmentType() const;
			inline void setEquipmentType(EquipmentType equipmentType);

			inline bool getUseSharedAnimation() const;
			inline void setUseSharedAnimation(bool useSharedAnimation);

			inline const std::string& getLocalizedEquipmentId() const;
			inline void setLocalizedEquipmentId(const std::string& localizedEquipmentId);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Private definition                                    ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<uint64> EntityIdArray;		///< Array of entity IDs


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string   mEquipmentName;			///< Name of the Equipment
			bool		  mEquipFromVehicle;		///< "true" if entity can equip the equipment instantly, false if it has to get it from somewhere else (eg an vehicle)
			glm::vec3	  mPositionOffset;			///< Offset of the position
			EquipmentType mEquipmentType;			///< The used type of the equipment, important for animation handling
			bool		  mUseSharedAnimation;		///< "true" if the movement/idle animation is shared, else if the full animation is played
			std::string	  mLocalizedEqiupmentId;	///< The localized string id for the equipment name

			EntityIdArray mParticlesIds;	///< Array of particles entity IDs

			///< Because an equipment has an impact on the optical look of an entity, its optional to give other animations
			AssetProxy mIdleAnimation;
			AssetProxy mRunAnimation;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

		public:
			QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, EquipmentComponent, std::vector<uint64>, uint64);
			EntityIdArrayInterface ParticlesArray;			///< Array of the particles for the building


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/equipment/EquipmentComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::EquipmentComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::EquipmentComponent::EquipmentType)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::game::EquipmentComponent::EntityIdArrayInterface)
