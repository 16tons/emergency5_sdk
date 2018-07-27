// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Mesh;
	}
}
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
	*    Mesh analysing manager class
	*
	*  @todo
	*    TODO(np) This class is under construction
	*    It will need a manager class that holds descriptions of base meshes, so that meshes
	*    only analysed once
	*/
	class QSF_API_EXPORT AnalysedMesh
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// The next structs and classes are used to hold the analysed mesh data
		// First the structures are filled and than converted, so that the final
		// data only hold the shape data of the base mesh.

		// Vertex of base mesh
		struct BaseMeshVertex
		{
			glm::vec3 position;				///< Position of vertex
			glm::vec3 positionWorld;		///< Used for temporarily transformed positions
			glm::vec3 normal;				///< Normal of vertex
			glm::vec3 normalWorld;			///< Used for temporarily transformed normal
			glm::vec2 textureCoordinate;	///< 2D texture coordinate of vertex
		};

		// Triangle of base mesh
		struct BaseMeshTriangle
		{
			uint32 index[3];	///< 3 indices to a vertex, that construct a triangle
			glm::vec3 minBounding;	///< Minimum axis aligned bounding box in world space
			glm::vec3 maxBounding;	///< Maximum axis aligned bounding box in world space
		};

		// Line of base mesh
		struct BaseMeshLine
		{
			uint32 index[2]; ///< 2 indices to a vertex, that construct a line
		};

		// Submesh of basemesh
		class BaseMeshSub
		{
		public:
			BaseMeshTriangle* baseMeshTriangle;		///< Dynamic array of triangles
			uint32			  numberOfTriangles;	///< Count of dynamic array of triangles
			BaseMeshLine*	  baseMeshLine;			///< Dynamic array of lines
			uint32			  numberOfLines;		///< Count of dynamic array of lines
			std::string		  materialName;			///< Material name of submesh

			/**
			*  @brief
			*    Default constructor
			*/
			BaseMeshSub() :
				baseMeshTriangle(nullptr),
				numberOfTriangles(0),
				baseMeshLine(nullptr),
				numberOfLines(0)
			{
			}

			/**
			*  @brief
			*    Destructor
			*/
			~BaseMeshSub()
			{
				clear();
			}

			/**
			*  @brief
			*    Remove all triangle and line data
			*/
			void clear()
			{
				// Are there triangles?
				if (nullptr != baseMeshTriangle)
				{
					// Then free triangles, uninitialize the pointers and set count to zero
					delete [] baseMeshTriangle;
					baseMeshTriangle = nullptr;
					numberOfTriangles = 0;
				}

				// Are there lines?
				if (nullptr != baseMeshLine)
				{
					// Then free lines, uninitialize the pointers and set count to zero
					delete [] baseMeshLine;
					baseMeshLine = nullptr;
					numberOfLines = 0;
				}
			}
		};

		// Mesh data of basemesh
		class BaseMeshData
		{
		public:
			BaseMeshVertex* baseMeshVertex;		///< Dynamic array of vertex data
			uint32			numberOfVertices;	///< Count of dynamic array of vertex data
			BaseMeshSub*	baseMeshSub;		///< Dynamic array of submeshes
			uint32			numberOfSubMeshes;	///< Count of dynamic array of submeshes

			/**
			*  @brief
			*    Default constructor
			*/
			BaseMeshData() :
				baseMeshVertex(nullptr),
				numberOfVertices(0),
				baseMeshSub(nullptr),
				numberOfSubMeshes(0)
			{
			}

			/**
			*  @brief
			*    Destructor
			*/
			~BaseMeshData()
			{
				clear();
			}

			/**
			*  @brief
			*    Remove all vertices and submeshes
			*/
			void clear()
			{
				// Are there vertices?
				if (nullptr != baseMeshVertex)
				{
					// Remove vertices, uninitialize the pointers and set count to zero
					delete [] baseMeshVertex;
					baseMeshVertex = nullptr;
					numberOfVertices = 0;
				}

				// Are there submehses?
				if (nullptr != baseMeshSub)
				{
					// Remove submeshes, uninitialize the pointers and set count to zero
					delete [] baseMeshSub;
					baseMeshSub = nullptr;
					numberOfSubMeshes = 0;
				}
			}

			/**
			*  @brief
			*    Deep copy BaseMeshData from another
			*
			*  @param[in] source
			*    Source data
			*/
			void copyFrom(const BaseMeshData& source)
			{
				// Copy vertices
				if (source.numberOfVertices > 0)
				{
					numberOfVertices = source.numberOfVertices;
					baseMeshVertex = new BaseMeshVertex[numberOfVertices];
					memcpy(baseMeshVertex, source.baseMeshVertex, sizeof(BaseMeshVertex) * numberOfVertices);
				}

				// Copy submeshes
				if (source.numberOfSubMeshes > 0)
				{
					numberOfSubMeshes = source.numberOfSubMeshes;
					baseMeshSub = new BaseMeshSub[numberOfSubMeshes];
					for (size_t i = 0; i < numberOfSubMeshes; ++i)
					{
						BaseMeshSub& destinationBaseMeshSub = baseMeshSub[i];
						const BaseMeshSub& sourceBaseMeshSub = source.baseMeshSub[i];

						destinationBaseMeshSub.materialName = sourceBaseMeshSub.materialName;

						// Copy triangles of submesh
						if (sourceBaseMeshSub.numberOfTriangles > 0)
						{
							destinationBaseMeshSub.numberOfTriangles = sourceBaseMeshSub.numberOfTriangles;
							destinationBaseMeshSub.baseMeshTriangle = new BaseMeshTriangle[destinationBaseMeshSub.numberOfTriangles];
							memcpy(destinationBaseMeshSub.baseMeshTriangle, sourceBaseMeshSub.baseMeshTriangle, sizeof(BaseMeshTriangle) * destinationBaseMeshSub.numberOfTriangles);
						}

						// Copy lines of submesh
						if (sourceBaseMeshSub.numberOfLines > 0)
						{
							destinationBaseMeshSub.numberOfLines = sourceBaseMeshSub.numberOfLines;
							destinationBaseMeshSub.baseMeshLine = new BaseMeshLine[destinationBaseMeshSub.numberOfLines];
							memcpy(destinationBaseMeshSub.baseMeshLine, sourceBaseMeshSub.baseMeshLine, sizeof(BaseMeshLine) * destinationBaseMeshSub.numberOfLines);
						}
					}
				}
			}


		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AnalysedMesh();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~AnalysedMesh();

		/**
		*  @brief
		*    Analyse the base mesh and fill the shape data
		*/
		const BaseMeshData* analyseBaseMesh(const Ogre::v1::Mesh& ogreMesh);

		/**
		*  @brief
		*    Get the base mesh data
		*/
		const BaseMeshData* getBaseMeshData();

		/**
		*  @brief
		*    Transforms the vertex data by a specified transformation (e.g. for intersection tests)
		*
		*  @param[in] transform
		*    Transformation
		*/
		void transformVertices(const Transform& transform);

		/**
		*  @brief
		*    Get the axis aligned bounding box of transformed vertices
		*
		*  @param[out] minBound
		*    Receives the minimum bound of the AABB
		*  @param[out] maxBound
		*    Receives the maximum bound of the AABB
		*/
		void getBoxAabb(glm::vec3& minBound, glm::vec3& maxBound);

		/**
		*  @brief
		*    Get intersection point of ray and triangles
		*
		*  @param[in]  rayOrigin
		*    Origin of ray
		*  @param[in]  rayDirection
		*    Direction of ray
		*  @param[out] intersectionPoint
		*    Receives the local space triangle hit position
		*
		*  @return
		*    "true" if a triangle has been hit, else "false"
		*/
		bool intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& intersectionPoint);

		/**
		*  @brief
		*    Get intersection point of a up-ray and triangles
		*
		*  @param[in]  rayOrigin
		*    Origin of ray
		*  @param[out] intersectionPoint
		*    Receives the local space triangle hit position
		*
		*  @return
		*    "true" if a triangle has been hit, else "false"
		*
		*  @note
		*    - This function has much more performance then intersect method
		*/
		bool intersectUp(const glm::vec3& rayOrigin, glm::vec3& intersectionPoint);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BaseMeshData mBaseMeshData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
