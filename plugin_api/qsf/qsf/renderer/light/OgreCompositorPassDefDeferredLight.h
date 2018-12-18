// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/Compositor/Pass/OgreCompositorPassDef.h>


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
	*    Deferred light OGRE compositor pass definition
	*/
	class OgreCompositorPassDefDeferredLight : public Ogre::CompositorPassDef
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/// Same implementation as in "Ogre::CompositorPassQuadDef"
		struct TextureSource
		{
			size_t		   texUnitIdx;	///< Index of texture unit state to change
			Ogre::IdString textureName;	///< Name of the texture (can come from input channel, local textures, or global ones)
			size_t		   mrtIndex;	///< Index in case of MRT. Ignored if textureSource isn't mrt

			TextureSource(size_t _texUnitIdx, Ogre::IdString _textureName, size_t _mrtIndex) :
				texUnitIdx(_texUnitIdx),
				textureName(_textureName),
				mrtIndex(_mrtIndex)
			{
				// Nothing here
			}
		};
		typedef Ogre::vector<TextureSource>::type TextureSources;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		OgreCompositorPassDefDeferredLight(Ogre::uint32 rtIndex, Ogre::CompositorNodeDef* parentNodeDef);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreCompositorPassDefDeferredLight();

		/**
		*  @brief
		*    Return the texture sources
		*
		*  @return
		*    The texture sources
		*/
		const TextureSources& getTextureSources() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addTextureSource(size_t texUnitIdx, const Ogre::String& textureName, size_t mrtIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::CompositorNodeDef* mParentNodeDef;
		TextureSources			 mTextureSources;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
