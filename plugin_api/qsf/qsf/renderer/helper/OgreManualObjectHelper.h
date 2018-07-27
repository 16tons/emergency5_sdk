// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreColourValue.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class ManualObject;
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
	*    Helper class for building OGRE manual object geometry
	*
	*  @todo
	*    - TODO(fw): Should we rewrite method interface to use GLM and QSF classes instead of OGRE? (This is already done for addPolygon, which was added last.)
	*/
	class QSF_API_EXPORT OgreManualObjectHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] ogreManualObject
		*    Target OGRE manual object to be edited/expanded, must stay valid as long as this OGRE manual object helper instance exists
		*/
		explicit OgreManualObjectHelper(Ogre::ManualObject& ogreManualObject);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreManualObjectHelper();

		/**
		*  @brief
		*    Set material name to be used for geometry created by following calls
		*/
		void setMaterial(const Ogre::String& materialName);

		/**
		*  @brief
		*    Set vertex color to be used for geometry created by following calls
		*/
		void setColor(const Ogre::ColourValue& color);

		/**
		*  @brief
		*    Add box geometry to the manual object instance
		*
		*  @param[in] center
		*    Center point of the box. Transformation matrix not automatically applied to this.
		*  @param[in] transformation
		*    Transformation matrix that is applied to all generated vertices
		*  @param[in] extents
		*    Extents of the box describing HALF side lengths (x, y, z)
		*/
		void addBox(const Ogre::Vector3& center, const Ogre::Matrix3& transformation, const Ogre::Vector3& extents);

		/**
		*  @brief
		*    Add torus geometry around the x-axis to the manual object instance
		*
		*  @param[in] center
		*    Center point of the torus in local space. Transformation matrix not automatically applied to this.
		*  @param[in] transformation
		*    Transformation matrix that is applied to all generated vertices
		*  @param[in] outerRadius
		*    Outer radius of the torus, i.e. radius of the circle the torus is built around
		*  @param[in] innerRadius
		*    Inner radius of the torus, i.e. distance of every point of the torus surface from the circle with outerRadius. Should be less than outerRadius.
		*  @param[in] segments
		*    Number of segments, i.e. detail level for the circle with outerRadius that the torus is built around. Recommended values range from 8 to 64.
		*  @param[in] slices
		*    Number of slices, i.e. detail level for torus curved surface around the circle the torus is built around. Recommended values range from 4 to 32.
		*/
		void addTorus(const Ogre::Vector3& center, const Ogre::Matrix3& transformation, float outerRadius, float innerRadius, int segments, int slices);

		/**
		*  @brief
		*    Add cylinder mantle geometry along x-axis to the manual object instance
		*
		*  @param[in] startPoint
		*    Center point of lower ring in local space. Transformation matrix not automatically applied to this.
		*  @param[in] transformation
		*    Transformation matrix that is applied to all generated vertices
		*  @param[in] radius
		*    Radius of the cylinder (results in expansion along y- and z-axis)
		*  @param[in] height
		*    Height of the cylinder (along x-axis)
		*  @param[in] slices
		*    Number of slices, i.e. detail level for cylinder's curved surface. Recommended values range from 8 to 64.
		*/
		void addCylinderMantle(const Ogre::Vector3& startPoint, const Ogre::Matrix3& transformation, float radius, float height, int slices);
		void addCylinderMantle(const Ogre::Vector3& startPoint, const Ogre::Matrix3& transformation, float radius, float radiusAtTop, float height, int slices);

		/**
		*  @brief
		*    Add cone mantle geometry to the manual object instance
		*
		*  @param[in] startPoint
		*    Center point of lower ring in local space. Transformation matrix not automatically applied to this.
		*  @param[in] transformation
		*    Transformation matrix that is applied to all generated vertices
		*  @param[in] radius
		*    Radius of the cone's base surface (results in expansion along y- and z-axis)
		*  @param[in] height
		*    Height of the cone (along x-axis)
		*  @param[in] slices
		*    Number of slices, i.e. detail level for cones's curved surface. Recommended values range from 8 to 64.
		*/
		void addConeMantle(const Ogre::Vector3& startPoint, const Ogre::Matrix3& transformation, float radius, float height, int slices);

		/**
		*  @brief
		*    Add a polygon to the manual object instance
		*
		*  @param[in] vertices
		*    Array of vertices forming the polygon
		*  @param[in] filledOpacity
		*    Opacity between 0.0f and 1.0f of the inner area; if 0.0f (or below), the inner area will not be drawn at all
		*  @param[in] closed
		*    If true, a closed polygon is drawn (i.e. first and last vertices are connected by a segment)
		*/
		void addPolygon(const std::vector<glm::vec3>& vertices, float filledOpacity, bool closed = true);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::ManualObject* mOgreObject;	///< Pointer to OGRE manual object we are editing, always valid, do not destroy the instance
		Ogre::String		mMaterialName;	///< OGRE Material name used for all geometry created
		Ogre::ColourValue	mColor;			///< Vertex color to be used


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
