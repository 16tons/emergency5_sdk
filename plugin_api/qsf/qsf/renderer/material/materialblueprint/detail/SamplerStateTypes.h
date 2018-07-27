// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	/**
	*  @brief
	*    Own renderer related definitions
	*
	*  @remarks
	*    Sadly, there's some naming chaos inside OGRE. Example for such an OGRE naming issue: "Ogre::HlmsMacroblock::mCullMode" with type "Ogre::CullingMode".
	*    Such naming is confusing and introduces name mismatch problems which could be avoided if it's always written exactly in the same way. Should be
	*    "Ogre::HlmsMacroblock::mCullingMode" to have a consistent naming. In order to fight such naming chaos we just stick to the Direct3D 12 terminology
	*    by introducing own definitions (we need to be Direct3D 12, Vulkan etc. ready anyway).
	*/
	namespace Renderer
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		// For sampler state property usage
		enum class FilterMode
		{
			DEFAULT         						   = -1,
			MIN_MAG_MIP_POINT						   = 0,
			MIN_MAG_POINT_MIP_LINEAR				   = 0x1,
			MIN_POINT_MAG_LINEAR_MIP_POINT			   = 0x4,
			MIN_POINT_MAG_MIP_LINEAR				   = 0x5,
			MIN_LINEAR_MAG_MIP_POINT				   = 0x10,
			MIN_LINEAR_MAG_POINT_MIP_LINEAR			   = 0x11,
			MIN_MAG_LINEAR_MIP_POINT				   = 0x14,
			MIN_MAG_MIP_LINEAR						   = 0x15,
			ANISOTROPIC								   = 0x55,
			COMPARISON_MIN_MAG_MIP_POINT			   = 0x80,
			COMPARISON_MIN_MAG_POINT_MIP_LINEAR		   = 0x81,
			COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT  = 0x84,
			COMPARISON_MIN_POINT_MAG_MIP_LINEAR		   = 0x85,
			COMPARISON_MIN_LINEAR_MAG_MIP_POINT		   = 0x90,
			COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
			COMPARISON_MIN_MAG_LINEAR_MIP_POINT		   = 0x94,
			COMPARISON_MIN_MAG_MIP_LINEAR			   = 0x95,
			COMPARISON_ANISOTROPIC					   = 0xd5
		};
		enum class TextureAddressMode
		{
			WRAP		= 1,
			MIRROR		= 2,
			CLAMP		= 3,
			BORDER		= 4,
			MIRROR_ONCE	= 5
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // Renderer
} // qsf
