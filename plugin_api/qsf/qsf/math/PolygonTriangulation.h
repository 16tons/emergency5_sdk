// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/fwd.hpp>

#include <boost/noncopyable.hpp>

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
	*    Static polygon triangulation class
	*/
	class QSF_API_EXPORT PolygonTriangulation : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<glm::vec2> Vec2Array;
		typedef std::vector<glm::vec3> Vec3Array;
		typedef std::vector<uint16> IndexArray;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Triangulate a polygon
		*
		*  @param[in] vertices
		*    Vertices forming the polygon
		*  @param[out] result
		*    Resulting triangles as list of indices
		*/
		static bool triangulate(const Vec2Array& vertices, IndexArray& result);

		/**
		*  @brief
		*    Triangulate a polygon
		*
		*  @param[in] vertices
		*    Vertices forming the polygon
		*  @param[out] result
		*    Resulting triangles as list of indices
		*/
		static bool triangulate(const Vec3Array& vertices, IndexArray& result);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static float getPolygonArea(const Vec2Array& vertices);
		static bool insideTriangle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, const glm::vec2& P);
		static bool snip(const Vec2Array& vertices, int u, int v, int w, const IndexArray& V);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
