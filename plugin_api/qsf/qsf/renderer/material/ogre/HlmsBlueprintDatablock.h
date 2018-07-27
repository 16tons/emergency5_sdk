// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialTexture.h"

#include <OGRE/OgreHlmsDatablock.h>
#include <OGRE/OgreConstBufferPool.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Material;
	class HlmsBlueprint;
	class MaterialBlueprint;
	class MaterialBlueprintManager;
	namespace detail
	{
		class OgreHlmsManagerListener;
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
	*    QSF HLMS blueprint datablock implementation
	*/
	class HlmsBlueprintDatablock : public Ogre::HlmsDatablock, public Ogre::ConstBufferPoolUser
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class HlmsBlueprint;
		friend class Material;		///< "qsf::Material" must be able to call "qsf::HlmsBlueprintDatablock::scheduleConstBufferUpdate()"


	// TODO(co) Cleanup
	public:
		MaterialTextures mMaterialTextures[2];	///< For rare use-cases like GUI rendering, index 0 for default pass, index 1 for shadow map pass


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		HlmsBlueprintDatablock( Ogre::IdString name, HlmsBlueprint *creator, const Ogre::HlmsMacroblock *macroblock, const Ogre::HlmsBlendblock *blendblock, const Ogre::HlmsParamVec &params );
		virtual ~HlmsBlueprintDatablock();
		void destroyDatablock();

		MaterialBlueprint* getMaterialBlueprint() const;
		Material* getMaterial() const;
		void setMaterial(Material* material);

		// TODO(co) Remove this
		void flushRenderables();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::HlmsDatablock methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void calculateHash() override;


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::ConstBufferPoolUser methods   ]
	//[-------------------------------------------------------]
	protected:
		virtual void uploadToConstBuffer(char* dstPtr) override;
		virtual void uploadToExtraBuffer(char* dstPtr) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void scheduleConstBufferUpdate();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const MaterialBlueprintManager& mMaterialBlueprintManager;
		MaterialBlueprint*				mMaterialBlueprint;
		Material*						mMaterial;			///< The material instance which triggered the creation of this OGRE HLMS datablock, can be a null pointer, the instance will stay valid as long as this OGRE HLMS datablock instance exists, do not destroy the instance

		// TODO(co) OGRE 2.1: Dynamic material blueprint properties
		bool mYuvConversion;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
