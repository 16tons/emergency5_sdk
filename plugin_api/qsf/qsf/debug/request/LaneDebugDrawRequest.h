// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawRequest.h"
#include "qsf/math/Color4.h"

#include <glm/glm.hpp>

#include <boost/container/flat_map.hpp>

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
	*    Lane debug draw request implementation
	*/
	class QSF_API_EXPORT LaneDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend std::allocator<LaneDebugDrawRequest>;	// Necessary for allowing std containers to use the illegal default constructor


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Set the material for the debugging view
		*/
		static inline void setLaneDebugDrawMaterial(uint32 laneType, const Color4& color, const std::string& texture);

		/**
		*  @brief
		*    Set the color for the debugging view
		*/
		static inline void setLaneDebugDrawColor(uint32 laneType, const Color4& color);

		/**
		*  @brief
		*    Get the color to apply when creating debug drawings for a certain lane type.
		*    Returns a Color4::GREEN silently as default if no type is registered.
		*/
		static Color4 getLaneDebugColor(uint32 laneType);

		/**
		*  @brief
		*    Set the texture for the debugging view
		*/
		static inline void setLaneDebugDrawTexture(uint32 laneType, const std::string& texture);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vertices
		*    Positions of polygon vertices
		*  @param[in] widthStart
		*    Width of the stripe at the start point
		*  @param[in] widthEnd
		*    Width of the stripe at the end point
		*  @param[in] arrowLength
		*    Length of each arrow in the stripe, negative values are also possible and reverse the arrow direction; a value of 0.0f will lead to no arrows being drawn at all
		*  @param[in] laneType
		*    Lane type
		*  @param[in] isHighlighted
		*    Lane highlighed?
		*/
		inline LaneDebugDrawRequest(const std::vector<glm::vec3>& vertices, float widthStart, float widthEnd, float arrowLength, uint32 laneType, bool isHighlighted);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LaneDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Material structure for the debugging view with color and texture for every lane type
		*/
		struct LaneDebugDrawMaterial
		{
			boost::container::flat_map<uint32, Color4> color;
			boost::container::flat_map<uint32, std::string> texture;
		};

		static LaneDebugDrawMaterial sLaneDebugDrawMaterial;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<glm::vec3> mVertices;		///< List of vertices
		float				   mWidthStart;		///< Width of the lane at the start point
		float				   mWidthEnd;		///< Width of the lane at the end point
		float				   mArrowLength;	///< Distance between two arrows
		uint32				   mLaneType;		///< Our lane type
		bool				   mIsHighlighted;	///< Flag if selection is highlighted


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/LaneDebugDrawRequest-inl.h"
