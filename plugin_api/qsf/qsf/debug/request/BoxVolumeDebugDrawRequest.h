// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawRequest.h"
#include "qsf/math/Transform.h"
#include "qsf/math/Color4.h"

#include <glm/glm.hpp>

#include <vector>


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
	*    Box volume debug draw request implementation
	*
	*  @todo
	*    TODO(fw): Parameter "filledOpacity" is not supported yet
	*/
	class QSF_API_EXPORT BoxVolumeDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<BoxVolumeDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] center
		*    Center of the box
		*  @param[in] rotation
		*    Orientation of the box
		*  @param[in] extents
		*    Extents vector, i.e. full size of the box
		*  @param[in] borderWidth
		*    Relative width of the box's border, between 0.0f and 1.0f
		*  @param[in] color
		*    Draw color, filled area is drawn in the same color made transparent
		*  @param[in] filledOpacity
		*    Opacity between 0.0f and 1.0f of the inner area; if 0.0f (or below), the inner area will not be drawn at all
		*/
		inline BoxVolumeDebugDrawRequest(const glm::vec3& center, const glm::quat& rotation, const glm::vec3& extents, float borderWidth = 0.1f, const Color4& color = Color4::WHITE, float filledOpacity = 0.0f);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BoxVolumeDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline BoxVolumeDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Transform mTransform;
		float	  mBorderWidth;
		Color4	  mColor;			///< Draw color
		float	  mFilledOpacity;	///< Opacity of filled area


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/BoxVolumeDebugDrawRequest-inl.h"
