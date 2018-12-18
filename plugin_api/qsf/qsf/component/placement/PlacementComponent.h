// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Placement component base class
	*
	*  @remarks
	*    A placement component corrects the positioning and orientation of moving entities.
	*/
	class QSF_API_EXPORT PlacementComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PlacementComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Drop the given position to the ground
		*
		*  @param[in,out] map
		*    The map
		*  @param[in,out] position
		*    Position to move to the ground
		*  @param[in] groundMapFilterMask
		*    Ground map filter bitmask
		*
		*  @return
		*    "true" if a ground was found at the xz-position, "false" if there is no ground there
		*/
		static bool dropToGround(Map& map, glm::vec3& position, uint32 groundMapFilterMask = 0xffffffff);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PlacementComponent();

		/**
		*  @brief
		*    Drop the given position to the ground
		*
		*  @param[in,out] position
		*    Position to move to the ground
		*  @param[in] groundMapFilterMask
		*    Ground map filter bitmask
		*
		*  @return
		*    "true" if a ground was found at the xz-position, "false" if there is no ground there
		*/
		bool dropToGround(glm::vec3& position, uint32 groundMapFilterMask = 0xffffffff) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlacementComponent methods        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Apply the placement to the given transformation
		*
		*  @param[in,out] position
		*    The position to be taken as input and manipulated if necessary
		*  @param[in,out] rotation
		*    The rotation to be taken as input and manipulated if necessary, in case you don't need an updated rotation use a constant reference for performance reasons
		*
		*  @return
		*    "true" in case anything was changed by the placement application, "false" if both position and rotation did not have to be manipulated
		*
		*  @note
		*    - The default implementation redirects to the not rotation constant variant
		*/
		inline virtual bool applyPlacement(glm::vec3& position, glm::quat& rotation);
		virtual bool applyPlacement(glm::vec3& position, const glm::quat& rotation);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit PlacementComponent(Prototype* prototype);


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/placement/PlacementComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PlacementComponent)
