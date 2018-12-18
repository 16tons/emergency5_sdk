// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/component/Component.h>


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
		*    Inventory component class
		*/
		class QSF_GAME_API_EXPORT InventoryComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::InventoryComponent" unique component ID


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
			inline explicit InventoryComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~InventoryComponent();

			/**
			*  @brief
			*    Set the active equipment ID
			*
			*  @param[in] id
			*    ID of the equipment entity that is used at the moment
			*
			*  @note
			*    - Connected to camp
			*/
			inline void setActiveEquipmentId(uint64 id);

			/**
			*  @brief
			*    Return the equipment entity ID
			*
			*  @return
			*     The equipment entity ID
			*
			*  @note
			*    - Connected to camp
			*/
			inline uint64 getActiveEquipmentId() const;

			/**
			*  @brief
			*    Check if there is an active equipment
			*/
			inline bool hasActiveEquipment() const;

			/**
			*  @brief
			*    Show/hide the active equipment
			*/
			void showActiveEquipment(bool show) const;

			const std::vector<uint64>& getEquipments() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetSimulating(bool simulating) override;


		//[-------------------------------------------------------]
		//[ Private definition                                    ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<uint64> PrefabIdArray;		///< Array of prefab global asset IDs


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setAllInventoryMeshes(bool show);

			/**
			*  @brief
			*    Set the active equipment entity ID
			*
			*  @param[in] id
			*    Equipment entity ID to be used
			*/
			void setActiveEquipment(uint64 id);


		//[-------------------------------------------------------]
		//[ Private static methods                                ]
		//[-------------------------------------------------------]
		private:
			static const std::string& getEquipmentNameFromId(uint64 globalAssetId);
			static uint64 getEquipmentIdFromName(const std::string& localAssetName);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to CAMP
			PrefabIdArray mInventoryIds;
			uint64		  mActiveEquipmentId;	///< Equipment ID which is active at the moment


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

		public:
			QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(PrefabIdArrayInterface, InventoryComponent, PrefabIdArray, std::string);
			PrefabIdArrayInterface InventoryArray;		///< Array of prefab global asset IDs in the inventory


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/equipment/InventoryComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::InventoryComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::game::InventoryComponent::PrefabIdArrayInterface)
