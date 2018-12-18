// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/PlaneBoundedVolume.h"


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
	*    Class representing a frustum in 3D space
	*
	*  @remarks
	*    A frustum consists of six clip planes: near, far, left, right, top and bottom plane.
	*    All of these have their normal vector point to the inside of the frustum.
	*/
	class QSF_API_EXPORT Frustum : public PlaneBoundedVolume
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*   Enumeration for clip planes.
		*/
		enum PlaneIndex
		{
			PLANE_LEFT = 0,
			PLANE_RIGHT,
			PLANE_TOP,
			PLANE_BOTTOM,
			PLANE_NEAR,
			PLANE_FAR,
		};

		/**
		* Number of legal values for PlaneIndex enum to allow iterating.
		*/
		static const uint32 _NUM_PLANES = PLANE_FAR + 1;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Frustum();

		/**
		*  @brief
		*    Destructor
		*/
		~Frustum();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
