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
		// For pipeline rasterizer state property usage
		enum class FillMode
		{
			WIREFRAME = 2,
			SOLID     = 3
		};
		enum class CullMode
		{
			NONE  = 1,
			FRONT = 2,
			BACK  = 3
		};
		enum class ConservativeRasterizationMode
		{
			OFF	= 0,
			ON	= 1
		};

		// For pipeline depth stencil state property usage
		enum class DepthWriteMask
		{
			ZERO = 0,
			ALL  = 1
		};
		enum class StencilOp
		{
			KEEP	 = 1,
			ZERO	 = 2,
			REPLACE	 = 3,
			INCR_SAT = 4,
			DECR_SAT = 5,
			INVERT	 = 6,
			INCREASE = 7,
			DECREASE = 8
		};

		// For pipeline depth stencil state and sampler state property usage
		enum class ComparisonFunc
		{
			NEVER		  = 1,
			LESS		  = 2,
			EQUAL		  = 3,
			LESS_EQUAL	  = 4,
			GREATER		  = 5,
			NOT_EQUAL	  = 6,
			GREATER_EQUAL = 7,
			ALWAYS		  = 8
		};

		// For pipeline blend state property usage
		enum RenderTargetWriteMask
		{
			ZERO  = 0,
			RED	  = 1,
			GREEN = 2,
			BLUE  = 4,
			ALPHA = 8,
			ALL	  = RED | GREEN | BLUE | ALPHA
		};
		enum class Blend
		{
			ZERO		     = 1,
			ONE			     = 2,
			SRC_COLOR	     = 3,
			INV_SRC_COLOR    = 4,
			SRC_ALPHA	     = 5,
			INV_SRC_ALPHA    = 6,
			DEST_ALPHA	     = 7,
			INV_DEST_ALPHA   = 8,
			DEST_COLOR	     = 9,
			INV_DEST_COLOR   = 10,
			SRC_ALPHA_SAT    = 11,
			BLEND_FACTOR     = 14,
			INV_BLEND_FACTOR = 15,
			SRC_1_COLOR	     = 16,
			INV_SRC_1_COLOR  = 17,
			SRC_1_ALPHA	     = 18,
			INV_SRC_1_ALPHA  = 19
		};
		enum class BlendOp
		{
			ADD			 = 1,
			SUBTRACT	 = 2,
			REV_SUBTRACT = 3,
			MIN			 = 4,
			MAX			 = 5
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // Renderer
} // qsf
