// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    Civil persons can spawn from this point; it has a list of valid persons which can spawn from it
	*/
	class EM5_API_EXPORT PersonSpawnPointComponent : public CivilistSpawnPointComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::PersonSpawnPointComponent" unique component ID

		enum PersonSpawnPointType
		{
			PERSONSPAWNPOINTTYPE_ENTRY,
			PERSONSPAWNPOINTTYPE_EXIT,
			PERSONSPAWNPOINTTYPE_GATE,	// In and out
			PERSONSPAWNPOINTTYPE_TRANSIT
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
		inline explicit PersonSpawnPointComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PersonSpawnPointComponent();

		inline PersonSpawnPointType getPersonSpawnPointType() const;
		inline void setPersonSpawnPointType(PersonSpawnPointType personSpawnPointType);


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
		PersonSpawnPointType	mSpawnPointType;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(PersonList, PersonSpawnPointComponent, std::string)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonSpawnPointComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonSpawnPointComponent::PersonSpawnPointType)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::PersonSpawnPointComponent::PersonListInterface)
