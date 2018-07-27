// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/SpawnPointComponent.h"


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
	*    EMERGENCY 5 spawn point component
	*
	*  @remarks
	*    Units can spawn from this point; it has a list of valid units which can spawn from it
	*/
	class EM5_API_EXPORT UnitSpawnPointComponent : public SpawnPointComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::UnitSpawnPointComponent" unique component ID

		enum UnitSpawnPointType
		{
			UNITSPAWNPOINTTYPE_ENTRY,
			UNITSPAWNPOINTTYPE_EXIT
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static UnitSpawnPointComponent* findUnitSpawnPoint(const qsf::StringHash& unitTypeId, UnitSpawnPointType unitSpawnPointType, const qsf::Map& map, const glm::vec3& targetPosition);


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
		inline explicit UnitSpawnPointComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~UnitSpawnPointComponent();

		inline UnitSpawnPointType getUnitSpawnPointType() const;
		inline void setUnitSpawnPointType(UnitSpawnPointType unitSpawnPointType);

		inline const std::vector<std::string>& getUnitList() const;
		inline void setUnitList(const std::vector<std::string>& unitNames);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Public virtual em5::SpawnPointComponent methods       ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool isEntry() const override;
		inline virtual bool isExit() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum MatchType
		{
			MATCH_NONE = 0,		///< Unit ID does not match at all
			MATCH_WILDCARD_1,	///< Unit ID is matched by the general wildcard matching everything
			MATCH_WILDCARD_2,	///< Unit ID is matched by a wildcard with lower specialization (department OR vehicle type)
			MATCH_WILDCARD_3,	///< Unit ID is matched by a wildcard with high specialization (department AND vehicle type)
			MATCH_EXACT,		///< Unit ID is explicitly listed in unit spawn point component
		};
		typedef boost::container::flat_map<uint32, MatchType> UnitIdMatchMap;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		MatchType matchesUnitTypeId(uint32 unitTypeId) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		UnitSpawnPointType  mSpawnPointType;
		UnitIdMatchMap		mMatchingUnitIds;		///< Set of unit name hashes, built from the unit list with wildcards resolved


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(UnitList, UnitSpawnPointComponent, std::string)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/UnitSpawnPointComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::UnitSpawnPointComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::UnitSpawnPointComponent::UnitSpawnPointType)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::UnitSpawnPointComponent::UnitListInterface)
