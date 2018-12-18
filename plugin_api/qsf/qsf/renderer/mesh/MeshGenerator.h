// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

// TODO(sw) review this. I don't like this, but is needed :(
#include <OGRE/OgreHardwareVertexBuffer.h> // Needed for enum VertexElementSemantic because you cannot forward declare an enum (bad MS extension...) gcc error: use of enum ‘VertexElementSemantic’ without previous declaration

#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class VertexDeclaration;
		class HardwareIndexBufferSharedPtr;
	}
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
	*    Mesh generator class
	*
	*  @remarks
	*    The mesh generator supports your custom vertex formats. By default,
	*    a VF with vec4:POSITION, vec3:NORMAL, vec2:TEXCOORD0 is installed and ready
	*    for you to use. But you can easily downgrade to e.g. vec3:POSITION vec4:TEXCOORD easily in
	*    the constructor of your custom MeshGenerator via:
	*
	*		mVertexFormatDeclaration.clear();
	*		mVertexFormatDeclaration.push_back(VertexFormatElement::positionElement());
	*		mVertexFormatDeclaration.push_back(VertexFormatElement::textureCoordinateElement(0, 4));
	*
	*    You can also completely customize the individual VFEs yourself, if required.
	*/
	class QSF_API_EXPORT MeshGenerator : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Helper structure to make vertex copy easy
		*
		*  @remarks
		*    This is by default a very fat vertex which accommodates every possible vertex configuration
		*    and is therefore easy to use, but takes up much more memory than the final mesh!
		*/
		struct RenderVertex
		{
			glm::vec4 position;				///< Object space position of vertex + alpha value
			glm::vec4 normal;				///< Object space normal of vertex
			glm::vec4 color;				///< Vertex diffuse color - TODO(oi) This should ideally be u8vec4 but MeshGenerator only understands float vertex component types atm
			glm::vec2 textureCoordinate;	///< Packed texture coordinate
			glm::vec4 custom[5];			///< Custom per-vertex-data
		};

		/**
		*  @brief
		*    Helper structure to create a vector of SubMesh data
		*/
		struct RenderSubMesh
		{
			std::string			materialName;	// Material name for the sub mesh
			std::vector<uint32>	indices;		// Indices list for the sub mesh
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MeshGenerator();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~MeshGenerator();


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		// Structure to describe a single element in the final vertex format
		struct QSF_API_EXPORT VertexFormatElement
		{
			VertexFormatElement(uint8 numberOfElements, Ogre::VertexElementSemantic elementSemantic, uint8 index = 0)
			{
				elementCount = numberOfElements;
				semantic = elementSemantic;
				semanticIndex = index;
			}

			// Handy short-cuts for creating default VFEs
			static VertexFormatElement positionElement();
			static VertexFormatElement positionWithAlphaElement();
			static VertexFormatElement textureCoordinateElement();
			static VertexFormatElement qtangentElement();
			static VertexFormatElement colorElement();
			static VertexFormatElement customElement(uint8 index, uint8 size);

			VertexFormatElement() :
				elementCount(0),
				semantic(Ogre::VES_POSITION),
				semanticIndex(0)
			{}

			// Number of elements: [0, 4]
			uint8 elementCount;

			// Actual semantic
			Ogre::VertexElementSemantic semantic;

			// Semantic index (e.g. n-th texture coordinate set)
			uint8 semanticIndex;
		};

		// List of single element definitions is a complete sequential vertex declaration
		typedef std::vector<VertexFormatElement> SequentialVertexFormatDeclaration;


	//[-------------------------------------------------------]
	//[ Protected static methods                              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  Appends a VFE to a Ogre::v1::VertexDeclaration and increases the vertex size by the added element
		*/
		static void addVertexElementToVertexDeclaration(const VertexFormatElement& elementFormat, Ogre::v1::VertexDeclaration& ogreVertexDeclaration, size_t& vertexSize);

		/**
		*  Writes a single vertex element record into a stream of interleaved vertex element records. Automatically
		*  takes care of writing the correct number of FLOAT elements into the target data
		*  @note
		*    targetDataPointer is advanced with every element written
		*/
		static void writeSingleVertexElementToData(const VertexFormatElement& vfe, uint8*& targetDataPointer, const float* sourceData);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Create the OGRE mesh by member data
		*
		*  @param[in] ogreMeshName
		*    UTF-8 name of the OGRE mesh
		*/
		void createOgreMesh(const std::string& ogreMeshName);

		/**
		*  @brief
		*    Update the OGRE mesh by member data
		*
		*  @param[in] ogreMeshName
		*    UTF-8 name of the OGRE mesh
		*
		*  @result
		*    false if update did fail and true for success
		*/
		bool updateOgreMesh(const std::string& ogreMeshName);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::vector<RenderVertex>		  mVertices;
		std::vector<RenderSubMesh>		  mRenderSubMesh;
		SequentialVertexFormatDeclaration mVertexFormatDeclaration;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void uploadVertexData(const Ogre::v1::HardwareVertexBufferSharedPtr& ogreVertexBuffer, size_t vertexSize);	// Upload the vertex data to the graphics card
		void uploadIndexData(const RenderSubMesh& renderSubMesh, const Ogre::v1::HardwareIndexBufferSharedPtr& ogreIndexBuffer, size_t numberOfIndices);
		void setOgreBounds(const Ogre::v1::MeshPtr& ogreMeshPtr);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
