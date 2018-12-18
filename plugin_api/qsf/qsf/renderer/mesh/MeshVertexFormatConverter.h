// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgreDataStream.h>
#include <OGRE/OgreIteratorWrapper.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgreMeshSerializer.h>
#undef ERROR // OGRE includes an OS header resulting in the usual fancy definition-hell, undefine this MS Windows "ERROR"-definition

#include <boost/signals2.hpp>


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
	*   OGRE mesh serializer listener
	*
	*  @remarks
	*    The OGRE default vertex format is fat:
	*    - OGRE vertex format
	*        - FLOAT3 position from source 0
	*        - FLOAT3 normal from source 0
	*        - FLOAT2 texture coordinate from source 0
	*        - FLOAT4 tangent from source 0
	*        - Nasty renderer API dependent "Ogre::VET_COLOUR" from source 0
	*    - OGRE vertex format for skinned meshes
	*        - FLOAT3 position from source 0
	*        - FLOAT3 normal from source 0
	*           - UBYTE4 blend indices from source 2
	*           - FLOAT4 blend weights from source 2
	*        - FLOAT2 texture coordinate from source 1
	*        - FLOAT4 tangent from source 1
	*        - Nasty renderer API dependent "Ogre::VET_COLOUR" from source 0
	*
	*    It's easily possible to slim down the vertex format by using
	*    - Texture coordinate 0: 16 bit precision texture coordinates
	*    - Texture coordinate 1: 16 bit precision QTangent: See http://www.crytek.com/cryengine/presentations/spherical-skinning-with-dual-quaternions-and-qtangents "Spherical Skinning with Dual-Quaternions and QTangents"
	*    - Texture coordinate 2: Vertex color, 8 bit precision per component, four components (not renderer API dependent)
	*    - 16 bit precision blend weights
	*
	*    Sadly, we can't use 16 bit precision texture coordinates, noticed issues in some EMERGENCY 5 assets like
	*    "Global asset ID 1031 - Local asset name "em5/prefab/street/street_crossing_x_01" - Local asset ID 234239163".
	*
	*    This OGRE mesh serializer listener can be considered a hack. The shipped EMERGENCY 5 has meshes with the fat default OGRE vertex format,
	*    but we need something slimmer to reduce the memory requirements. So, we convert meshes after loading them if required.
	*
	*  @todo
	*    - TODO(co) We should really remove this content-compatibility hack as soon as the last EMERGENCY 5 patch is done (this is tool, not runtime functionality)
	*/
	class QSF_API_EXPORT MeshVertexFormatConverter : public Ogre::v1::MeshSerializerListener
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const Ogre::v1::Mesh&)> OgreMeshConverted;	///< This Boost signal is emitted when an OGRE mesh has been converted; parameter: OGRE mesh instance


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::v1::MeshSerializerListener methods ]
	//[-------------------------------------------------------]
	public:
		virtual void processMaterialName(Ogre::v1::Mesh* mesh, Ogre::String* name) override;
		virtual void processSkeletonName(Ogre::v1::Mesh* mesh, Ogre::String* name) override;
		virtual void processMeshCompleted(Ogre::v1::Mesh* mesh) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool vertexFormatMigrationRequired(const Ogre::v1::Mesh& ogreMesh);
		void vertexFormatMigration(Ogre::v1::VertexData& ogreVertexData);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
