// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/civilist/CivilistSpawnPointComponent.h"


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
	*    EMERGENCY 5 vehicle spawn point component
	*
	*  @remarks
	*    Civil vehicles can spawn from this point; it has a list of valid vehicles which can spawn from it
	*/
	class EM5_API_EXPORT VehicleSpawnPointComponent : public CivilistSpawnPointComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::VehicleSpawnPointComponent" unique component ID

		enum VehicleSpawnPointType
		{
			VEHICLESPAWNPOINTTYPE_ENTRY,
			VEHICLESPAWNPOINTTYPE_EXIT,
			VEHICLESPAWNPOINTTYPE_TRANSIT
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
		inline explicit VehicleSpawnPointComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VehicleSpawnPointComponent();

		inline VehicleSpawnPointType getVehicleSpawnPointType() const;
		inline void setVehicleSpawnPointType(VehicleSpawnPointType vehicleSpawnPointType);


	//[-------------------------------------------------------]
	//[ Public virtual em5::SpawnPointComponent methods       ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool isEntry() const override;
		inline virtual bool isExit() const override;
		inline virtual bool isTransit() const override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		VehicleSpawnPointType mSpawnPointType;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(VehicleList, VehicleSpawnPointComponent, std::string)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/civilist/VehicleSpawnPointComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleSpawnPointComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleSpawnPointComponent::VehicleSpawnPointType)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::VehicleSpawnPointComponent::VehicleListInterface)
