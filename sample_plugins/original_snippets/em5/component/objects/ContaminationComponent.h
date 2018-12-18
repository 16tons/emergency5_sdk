// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/injury/Injury.h"

#include <qsf/component/Component.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class ContaminationCloudComponent;
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
	*    EMERGENCY 5 contamination component class
	*/
	class EM5_API_EXPORT ContaminationComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::ContaminationComponent" unique component ID

		// Shortcuts
		typedef Injury::ContaminationType ContaminationType;


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
		inline explicit ContaminationComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ContaminationComponent();

		/**
		*  @brief
		*    Start the contamination spreading simulation
		*/
		void startSimulation();

		/**
		*  @brief
		*    Stop the contamination spreading simulation
		*/
		void stopSimulation();

		/**
		*  @brief
		*    Get contamination type
		*/
		inline ContaminationType getContaminationType() const;

		/**
		*  @brief
		*    Set contamination type
		*/
		inline void setContaminationType(ContaminationType contaminationType);

		/**
		*  @brief
		*    Get contamination limit type
		*/
		inline uint32 getContaminationLimit() const;

		/**
		*  @brief
		*    Set contamination limit type
		*/
		inline void setContaminationLimit(uint32 contaminationLimit);

		/**
		*  @brief
		*    Increase the persons that are contaminated by this industrial plant
		*/
		void increaseContaminatedPersons();

		/**
		*  @brief
		*    Get the number of persons which got contaminated by the plant.
		*    Counter only increases (no decrease by decontamination)
		*/
		inline uint32 getContaminatedPersons() const;

		/**
		*  @brief
		*    Get all contamination cloud components that this contamination component manages
		*/
		void getContaminationCloudComponents(std::vector<ContaminationCloudComponent*>& components, bool showError = true) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual void onSetDebug(bool debug) override;


	//[-------------------------------------------------------]
	//[ Private definition                                    ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> CloudIdArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	public:
		// Connected to CAMP
		CloudIdArray			  mContaminationCloudIds;	///< Array of contamination cloud entity IDs
		Injury::ContaminationType mContaminationType;		///< What kind of contamination are the clouds spreading
		uint32					  mContaminationLimit;		///< Limit of persons that are contaminated by the industrial plant
		uint32					  mContaminatedPersons;		///< Persons that were contaminated


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, ContaminationComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface CloudArray;			///< Array of the particles for the building


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/objects/ContaminationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ContaminationComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::ContaminationComponent::EntityIdArrayInterface)
