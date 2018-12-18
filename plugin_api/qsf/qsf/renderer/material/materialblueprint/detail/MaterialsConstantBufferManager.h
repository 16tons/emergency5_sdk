// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/StringHash.h"
#include "qsf/renderer/material/materialblueprint/detail/RootSignatureTypes.h"

#include <OGRE/OgreConstBufferPool.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class CommandBuffer;
}
namespace qsf
{
	class MaterialBlueprint;
	class HlmsBlueprintDatablock;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::map<StringHash, std::string> DynamicShaderPieces;	// TODO(co) Unordered map might perform better


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Materials constant buffer manager
	*
	*  @note
	*    - Class name is plural because the properties of many materials are stored in the buffer
	*/
	class QSF_API_EXPORT MaterialsConstantBufferManager : public Ogre::ConstBufferPool
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/*
		*  @brief
		*    Fill dynamic shader pieces
		*
		*  @param[in] materialsConstantBufferManager
		*    Materials constant buffer manager, can be a null pointer
		*  @param[out] inOutDynamicShaderPieces
		*    Receives the dynamic shader pieces, not cleared before something is added
		*/
		static void fillDynamicShaderPieces(const MaterialsConstantBufferManager& materialsConstantBufferManager, DynamicShaderPieces inOutDynamicShaderPieces[Ogre::NumShaderTypes]);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		MaterialsConstantBufferManager(const MaterialBlueprint& materialBlueprint, uint32 numberOfBytesPerMaterial);
		virtual ~MaterialsConstantBufferManager();
		void resetLastBoundPool();
		uint32 getSlotsPerPool() const;

		/**
		*  @brief
		*    Bind the materials constant buffer to the renderer
		*
		*  @param[in] ogreCommandBuffer
		*    OGRE command buffer to fill with instructions
		*  @param[in] slotIndex
		*    Slot index
		*  @param[in] hlmsBlueprintDatablock
		*    Used HLMS blueprint datablock
		*  @param[in] casterPass
		*    Caster pass?
		*/
		void bindToRenderer(Ogre::CommandBuffer& ogreCommandBuffer, uint16 slotIndex, const HlmsBlueprintDatablock& hlmsBlueprintDatablock, bool casterPass);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Renderer::ShaderVisibility		   mShaderVisibility;
		const ConstBufferPool::BufferPool* mLastBoundPool;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
