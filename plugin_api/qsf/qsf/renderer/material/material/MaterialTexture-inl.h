// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MaterialTexture::MaterialTexture(uint32 _rootParameterIndex, const MaterialProperty& _materialProperty, Ogre::TextureType _ogreTextureType, uint32 _numberOfMipmaps, bool _hardwareGammaCorrection) :
		rootParameterIndex(_rootParameterIndex),
		materialProperty(_materialProperty),
		ogreTextureType(_ogreTextureType),
		numberOfMipmaps(_numberOfMipmaps),
		hardwareGammaCorrection(_hardwareGammaCorrection),
		textureUnit(getUninitialized<uint16>()),
		mStreamedTexture(nullptr)
	{
		// Nothing here
	}

	inline MaterialTexture::MaterialTexture(uint32 _rootParameterIndex, const MaterialProperty& _materialProperty, Ogre::TextureType _ogreTextureType, uint32 _numberOfMipmaps, bool _hardwareGammaCorrection, const Ogre::TexturePtr& _ogreTexturePtr, uint16 _textureUnit) :
		rootParameterIndex(_rootParameterIndex),
		materialProperty(_materialProperty),
		ogreTextureType(_ogreTextureType),
		numberOfMipmaps(_numberOfMipmaps),
		hardwareGammaCorrection(_hardwareGammaCorrection),
		ogreTexturePtr(_ogreTexturePtr),
		textureUnit(_textureUnit),
		mStreamedTexture(nullptr)
	{
		// Nothing here
	}

	inline MaterialTexture::MaterialTexture(uint32 _rootParameterIndex, Ogre::TextureType _ogreTextureType, uint32 _numberOfMipmaps, bool _hardwareGammaCorrection, const Ogre::TexturePtr& _ogreTexturePtr, uint16 _textureUnit) :
		rootParameterIndex(_rootParameterIndex),
		ogreTextureType(_ogreTextureType),
		numberOfMipmaps(_numberOfMipmaps),
		hardwareGammaCorrection(_hardwareGammaCorrection),
		ogreTexturePtr(_ogreTexturePtr),
		textureUnit(_textureUnit),
		mStreamedTexture(nullptr)
	{
		// Nothing here
	}

	inline MaterialTexture::~MaterialTexture()
	{
		resetStreamedTexture();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
