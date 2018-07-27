// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/placement/PlacementComponent.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class DiverLogic;
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
	*    Pivot on ground placement component class
	*
	*  @remarks
	*    The pivot on ground placement component manipulates the position only, not the rotation. Position is set to the ground level, without any offset.
	*/
	class EM5_API_EXPORT PersonPlacementComponent : public qsf::PlacementComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::PersonPlacementComponent" unique component ID

		enum ActiveGroundMap
		{
			AUTOMATIC,		// Will select water or terrain automatically based on the height difference between them
			FORCE_DEFAULT,	// Only use the map with water (always swim - never be under water)
			FORCE_NO_WATER, // Only use the terrain map - ignore water
			NO_MAP			// Placement does nothing at all
		};

		enum PersonType
		{
			PERSONTYPE_UNINITIALIZED,
			PERSONTYPE_DEFAULT,	///< Default person type
			PERSONTYPE_DIVER	///< Diver requires some special treatment
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool mustBeSwimming(const glm::vec3& position, float swimmingMinimumWaterHeight);
		static bool mustBeSwimming(const glm::vec3& position);


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
		explicit PersonPlacementComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PersonPlacementComponent();

		/**
		*  @brief
		*    The collapsed flag shows if the person is lying injured on the ground. In this case the placement takes the center of the person
		*/
		void setCollapsedFlag(bool active);
		bool getCollapsedFlag() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlacementComponent methods        ]
	//[-------------------------------------------------------]
	public:
		virtual bool applyPlacement(glm::vec3& position, glm::quat& rotation) override;
		virtual bool applyPlacement(glm::vec3& position, const glm::quat& rotation) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Protected static methods                              ]
	//[-------------------------------------------------------]
	protected:
		// NOTE: unlike base-class this returns only false when it fails
		static bool dropToGround(glm::vec3& position, uint32 groundMapFilterMask);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool applyPlacementInternal(glm::vec3& position, glm::quat& rotation, bool needsUpdatedRotation);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PersonType		mPersonType;
		bool			mIsCollapsed;

		qsf::WeakPtr<DiverLogic> mDiverLogic;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonPlacementComponent)
