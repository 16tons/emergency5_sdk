// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Plane.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Transform;
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
	*    Class representing a 3D space volume bounded by a set of planes.
	*
	*  @remarks
	*    All planes have to be oriented in such a way that their normal vectors point to the inside of the volume.
	*/
	class QSF_API_EXPORT PlaneBoundedVolume
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<Plane> PlaneArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlaneBoundedVolume();

		/**
		*  @brief
		*    Remove all planes
		*/
		void clear();

		/**
		*  @brief
		*    Return number of planes
		*/
		uint32 getNumberOfPlanes() const;

		/**
		*  @brief
		*    Get a plane by its plane index
		*/
		const Plane& getPlaneByIndex(uint32 index) const;

		/**
		*  @brief
		*    Set a plane by its plane index
		*/
		void setPlaneByIndex(uint32 index, const Plane& plane);

		/**
		*  @brief
		*    Add a plane
		*/
		void addPlane(const Plane& plane);

		/**
		*  @brief
		*    Check whether or not the volume contains the given point
		*/
		bool contains(const glm::vec3& point) const;

		//[-------------------------------------------------------]
		//[ Transformation                                        ]
		//[-------------------------------------------------------]
		PlaneBoundedVolume& operator *=(const Transform& worldTransform);


	//[-------------------------------------------------------]
	//[ Protected members                                     ]
	//[-------------------------------------------------------]
	protected:
		PlaneArray mPlanes;	///< Array of planes


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
