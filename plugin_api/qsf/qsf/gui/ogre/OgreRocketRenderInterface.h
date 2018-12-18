// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <OGRE/OgreBlendMode.h>

#include <Rocket/Core/RenderInterface.h>

#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GuiContext;
	class HlmsBlueprint;
	class MaterialBlueprint;
	namespace detail
	{
		struct RocketOgreTexture;
		struct RocketOgreCompiledGeometry;
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
	*    OGRE render interface for libRocket
	*
	*  @note
	*    - Based on the libRocket SDK OGRE example written by Peter Curry
	*/
	class OgreRocketRenderInterface : public Rocket::Core::RenderInterface
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class OgreCompositorPassGui;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] windowWidth
		*    Initial window width
		*  @param[in] windowHeight
		*    Initial window height
		*/
		OgreRocketRenderInterface(uint32 windowWidth, uint32 windowHeight);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreRocketRenderInterface();

		/**
		*  @brief
		*    Return the currently rendered GUI context
		*
		*  @return
		*    The currently rendered GUI context, can be a null pointer, don't destroy the instance
		*/
		GuiContext* getCurrentGuiContext() const;

		/**
		*  @brief
		*    Set the "pixels per inch" conversion value, used for units like "pt", "in", "cm"
		*
		*  @param[in] ppi
		*    Pixels per inch, default is 100.0f
		*/
		void setPixelsPerInch(float ppi);


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Core::RenderInterface methods  ]
	//[-------------------------------------------------------]
	public:
		virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int numberOfVertices, int* indices, int numberOfIndices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation) override;
		virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int numberOfertices, int* indices, int numberOfIndices, Rocket::Core::TextureHandle texture) override;
		virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation) override;
		virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry) override;
		virtual void EnableScissorRegion(bool enable) override;
		virtual void SetScissorRegion(int x, int y, int width, int height) override;
		virtual bool LoadTexture(Rocket::Core::TextureHandle& textureHandle, Rocket::Core::Vector2i& textureDimensions, const Rocket::Core::String& source) override;
		virtual bool GenerateTexture(Rocket::Core::TextureHandle& textureHandle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions) override;
		virtual void ReleaseTexture(Rocket::Core::TextureHandle texture) override;
		virtual float GetHorizontalTexelOffset() override;
		virtual float GetVerticalTexelOffset() override;
		virtual float GetPixelsPerInch() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32, detail::RocketOgreTexture*> RocketOgreTextures;
		typedef std::vector<detail::RocketOgreCompiledGeometry*>	   RocketOgreCompiledGeometries;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::RenderSystem*			  mOgreRenderSystem;			///< OGRE render system instance, always valid, do not destroy the instance
		HlmsBlueprint*				  mHlmsBlueprint;
		MaterialBlueprint*			  mMaterialBlueprint;
		GuiContext*					  mCurrentGuiContext;			///< Current GUI context, always valid during rendering, do not destroy the instance
		bool						  mScissorEnabled;				///< "true" if the scissor test is enabled, else "false"
		int							  mScissorLeft;					///< Left coordinate of the scissor rectangle
		int							  mScissorTop;					///< Top coordinate of the scissor rectangle
		int							  mScissorRight;				///< Right coordinate of the scissor rectangle
		int							  mScissorBottom;				///< Bottom coordinate of the scissor rectangle
		float						  mPixelsPerInch;				///< Value returned by "GetPixelsPerInch()"
		detail::RocketOgreTexture*    mDefaultRocketOgreTexture;
		// Currently not used Rocket OGRE compiled geometry instances
		RocketOgreTextures			  mRocketOgreTextures;	// TODO(co) OGRE 2.1: Looks like we could remove this sanity management later on, not now keep it until we know everything is fine.
		RocketOgreCompiledGeometries  mRocketOgreCompiledGeometries;
		// Ugly cross class data
		Ogre::GpuProgramParametersSharedPtr mVertexOgreGpuProgramParametersSharedPtr;	///< Set by "qsf::OgreCompositorPassGui"
		Ogre::GpuProgramParametersSharedPtr mFragmentOgreGpuProgramParametersSharedPtr;	///< Set by "qsf::OgreCompositorPassGui"


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
