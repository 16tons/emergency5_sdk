// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/debug/DebugDrawProxy.h>


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
	*    EMERGENCY 5 boat drop point component
	*/
	class EM5_API_EXPORT BoatDropPointComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::BoatDropPointComponent" unique component ID


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
		explicit BoatDropPointComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BoatDropPointComponent();

		/**
		*  @brief
		*    Show position of the drop point
		*/
		void highlightPosition(bool highlight);

		/**
		*  @brief
		*    Getter and setter
		*/
		inline uint64 getLakeId() const;
		inline void setLakeId(uint64 lakeId);

		inline uint64 getTransporterEntityId() const;
		inline void setTransporterEntityId(uint64 transporterEntityId);

		inline uint64 getBoatEntityId() const;
		inline void setBoatEntityId(uint64 boatEntityId);

		inline size_t getDropMarkerIndex() const;
		inline void setDropMarkerIndex(size_t dropPointIndex);

		qsf::TransformComponent* getBoatDropTransform() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private definition                                    ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64				mTransporterEntityId;	///< The ID of the object, showing the position of the transporter
		uint64				mLakeId;
		uint64				mBoatEntityId;			///< The ID of the object, showing the position where the boat should be dropped/loaded from
		EntityIdArray		mDropMarkerIds;			///< Array of entity IDs marking places to drop injured persons at
		qsf::DebugDrawProxy	mDebugDrawProxy;		///< Only for debugging
		size_t				mDropMarkerIndex;		///< Drop points saving the index of the drop marker makes it easier with collision problems


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, BoatDropPointComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface DropMarkerArray;			///< Array of the particles for the building


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/objects/BoatDropPointComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BoatDropPointComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::BoatDropPointComponent::EntityIdArrayInterface)
