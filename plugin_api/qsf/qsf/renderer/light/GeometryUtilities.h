// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <OGRE/OgreString.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class IndexData;
		class VertexData;
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
	*    Geometry utility class
	*
	*  @note
	*    - Based on the OGRE deferred shading sample
	*/
	class GeometryUtilities : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		// Fill up a fresh copy of vertex data and index data with a sphere's coordinates given the number of rings and the number of segments
		static void createSphere(Ogre::v1::VertexData*& vertexData, Ogre::v1::IndexData*& indexData, uint32 rings, uint32 segments);

		// Fill up a fresh copy of vertex data and index data with a cone's coordinates given the number of vertices in base
		static void createCone(Ogre::v1::VertexData*& vertexData, Ogre::v1::IndexData*& indexData, uint32 verticesInBase);

		// Fill up a fresh copy of vertex data with a normalized quad
		static void createQuad(Ogre::v1::VertexData*& vertexData);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
