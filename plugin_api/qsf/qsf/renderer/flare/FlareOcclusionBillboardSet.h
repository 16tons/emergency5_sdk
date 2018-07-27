// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/OgreBillboardSet.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class HardwareOcclusionQuery;
}
namespace qsf
{
	class FlareComponent;
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
	*    Flare occlusion billboard set
	*/
	class FlareOcclusionBillboardSet : public Ogre::v1::BillboardSet
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		FlareOcclusionBillboardSet(Ogre::IdType id, Ogre::ObjectMemoryManager& objectMemoryManager, Ogre::SceneManager& sceneManager, FlareComponent& flareComponent, Ogre::HardwareOcclusionQuery& ogreHardwareOcclusionQuery);
		virtual ~FlareOcclusionBillboardSet();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::Renderable methods               ]
	//[-------------------------------------------------------]
	public:
		virtual bool preRender(Ogre::SceneManager* sm, Ogre::CommandBuffer* commandBuffer) override;
		virtual void postRender(Ogre::SceneManager* sm, Ogre::CommandBuffer* commandBuffer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FlareComponent&				  mFlareComponent;
		Ogre::HardwareOcclusionQuery& mOgreHardwareOcclusionQuery;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
