// Copyright (C) 2012-2018 Promotion Software GmbH


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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
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
	*    Mesh vertex normals debug draw request implementation
	*
	*  @todo
	*    - TODO(vs) Uses a probably non portable friend declaration to hide an ugly default constructor creating an illegal state temporarily when used inside a std::vector.
	*    - TODO(co) Ugly hacked together for the first playable of EMERGENCY 5 (we required a way to visualize the vertex normals of vegetation meshes which are using custom normals)
	*/
	class QSF_API_EXPORT MeshVertexNormalsDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<MeshVertexNormalsDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] ogreEntity
		*    OGRE entity the mesh vertex normals should be shown from
		*  @param[in] lineLength
		*    Line length
		*  @param[in] color
		*    Draw color
		*/
		inline MeshVertexNormalsDebugDrawRequest(Ogre::v1::Entity& ogreEntity, float lineLength, const Color4& color = Color4::WHITE);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MeshVertexNormalsDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Ogre::v1::Entity* mOgreEntity;	// TODO(co) Stupid solution
		float			  mLineLength;
		Color4			  mColor;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline MeshVertexNormalsDebugDrawRequest();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/MeshVertexNormalsDebugDrawRequest-inl.h"
