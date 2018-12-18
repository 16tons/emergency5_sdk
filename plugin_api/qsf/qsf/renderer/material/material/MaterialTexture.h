// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialProperty.h"

#include <OGRE/OgreTexture.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	struct StreamedTexture;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint64	   GlobalAssetId;
	typedef StringHash MaterialPropertyId;	///< Material property identifier, internally just a POD "uint32", result of hashing the property name


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material texture
	*/
	class QSF_API_EXPORT MaterialTexture
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend struct StreamedTexture;	// Must be able to access "qsf::MaterialTexture::mStreamedTexture"


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		// TODO(co) Make those private
		// Read from material blueprint
		uint32			   rootParameterIndex;
		MaterialProperty   materialProperty;
		Ogre::TextureType  ogreTextureType;
		uint32			   numberOfMipmaps;
		bool			   hardwareGammaCorrection;
		// Derived data
		Ogre::TexturePtr ogreTexturePtr;
		uint16			 textureUnit;	///< Texture unit read from the material blueprint root signature (for renderer APIs which don't have root signatures, unlike e.g. Direct3D 12)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline MaterialTexture(uint32 _rootParameterIndex, const MaterialProperty& _materialProperty, Ogre::TextureType _ogreTextureType, uint32 _numberOfMipmaps, bool _hardwareGammaCorrection);
		inline MaterialTexture(uint32 _rootParameterIndex, const MaterialProperty& _materialProperty, Ogre::TextureType _ogreTextureType, uint32 _numberOfMipmaps, bool _hardwareGammaCorrection, const Ogre::TexturePtr& _ogreTexturePtr, uint16 _textureUnit);
		inline MaterialTexture(uint32 _rootParameterIndex, Ogre::TextureType _ogreTextureType, uint32 _numberOfMipmaps, bool _hardwareGammaCorrection, const Ogre::TexturePtr& _ogreTexturePtr, uint16 _textureUnit);
		inline ~MaterialTexture();
		StreamedTexture* getStreamedTexture() const;
		void resetStreamedTexture();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Derived data
		mutable StreamedTexture* mStreamedTexture;


	};


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::vector<MaterialTexture> MaterialTextures;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialTexture-inl.h"
