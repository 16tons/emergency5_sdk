// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/job/JobProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroup;
	class TextureStreamer;
	class RendererSettingsGroup;
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
	*    Texture streaming manager
	*
	*  @remarks
	*    The texture streaming manager is completely on-top of the existing
	*    OGRE texture resource architecture. You do not have to use it
	*    as it does not come without complexity. If you don't activate it
	*    the default resource handling of OGRE will still work as expected,
	*    and when enabled it will give you the added benefit of controlled background
	*    loading of textures.
	*/
	class QSF_API_EXPORT TextureStreamingManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererSystem;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		TextureStreamer& getTextureStreamer() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		TextureStreamingManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureStreamingManager();

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		void onStartup();
		void onShutdown();

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		void onAssetsMounted(const Assets& assets);
		void onAssetsUnmounted(const Assets& assets);
		void onSettingsPropertyChanged(const ParameterGroup& parameterGroup, uint32 property);
		void onUpdate(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		TextureStreamer*	   mTextureStreamer;
		JobProxy			   mUpdateJobProxy;			///< Job proxy
		RendererSettingsGroup* mRendererSettingsGroup;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
