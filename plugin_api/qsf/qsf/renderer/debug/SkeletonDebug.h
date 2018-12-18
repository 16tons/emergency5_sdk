// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>

#include <OGRE/OgreQuaternion.h>
#include <OGRE/OgreVector3.h>

#include <vector>


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
namespace qsf
{
	class Map;
	class CameraComponent;
	class GridSceneManager;
	class SkeletonBoneDebugText;
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
	*    Skeleton debug class
	*
	*  @remarks
	*    The OGRE implementation "Ogre::v1::Entity::_updateRenderQueue()" says
	*      "HACK to display bones
	*      This won't work if the entity is not centered at the origin
	*      TODO work out a way to allow bones to be rendered when Entity not centered"
	*    Meaning "Ogre::v1::Entity::setDisplaySkeleton()" is quite useless.
	*
	*  @note
	*    - This skeleton debug class is based on http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Skeleton+Debugger
	*/
	class SkeletonDebug : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		SkeletonDebug(Map& map, Ogre::v1::Entity& ogreEntity, GridSceneManager& gridSceneManager);
		~SkeletonDebug();

		bool getShowAxis() const;
		void setShowAxis(bool show);

		bool getShowBones() const;
		void setShowBones(bool show);

		bool getShowNames() const;
		void setShowNames(bool show);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const float		 LOCAL_SCALE;				///< Local scale
		static const std::string OGRE_AXIS_MATERIAL_NAME;	///< Name of OGRE material used for our manual axis object
		static const std::string OGRE_BONE_MATERIAL_NAME;	///< Name of OGRE material used for our manual bone object
		static const std::string OGRE_AXIS_MESH_NAME;		///< Name of OGRE axis mesh
		static const std::string OGRE_BONE_MESH_NAME;		///< Name of OGRE bone mesh

		struct BoneTagPoint
		{
			std::string		  boneName;			///< Name of the bone the tag point is attached to
			Ogre::v1::Entity* ogreEntity;		///< The attached OGRE entity
			Ogre::SceneNode*  ogreSceneNode;	///< The OGRE scene node managing the attached OGRE entity
			Ogre::Quaternion  offsetOrientation;
			Ogre::Vector3	  scale;

			BoneTagPoint(const std::string& _boneName, Ogre::v1::Entity* _ogreEntity, Ogre::SceneNode* _ogreSceneNode, const Ogre::Quaternion& _offsetOrientation, const Ogre::Vector3& _scale) :
				boneName(_boneName),
				ogreEntity(_ogreEntity),
				ogreSceneNode(_ogreSceneNode),
				offsetOrientation(_offsetOrientation),
				scale(_scale)
			{
				// Nothing here
			}

		};

		typedef std::vector<BoneTagPoint> BoneTagPoints;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    This method gets called before a compositor workspace gets rendered
		*
		*  @param[in] cameraComponent
		*    Used camera component
		*/
		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);

		void updateBoneTagPoints(const BoneTagPoints& boneTagPoints);

		void createAxisMaterial();
		void createBoneMaterial();
		void createAxisMesh();
		void createBoneMesh();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Map&								mMap;
		Ogre::v1::Entity&					mOgreEntity;
		GridSceneManager&					mGridSceneManager;
		bool								mShowAxis;
		bool								mShowBones;
		bool								mShowNames;
		BoneTagPoints						mAxisEntities;
		BoneTagPoints						mBoneEntities;
		std::vector<SkeletonBoneDebugText*> mTextOverlays;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
