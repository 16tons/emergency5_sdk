// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/shaderblueprint/ShaderSourceCodeBlueprint.h"


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
	*    Shader source code blueprint manager
	*/
	class ShaderSourceCodeBlueprintManager : public ElementManager<ShaderSourceCodeBlueprintId, ShaderSourceCodeBlueprint, detail::DirectAllocator<ShaderSourceCodeBlueprint>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderBlueprintManager;	///< The shader blueprint manager is the master of this class


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		ShaderSourceCodeBlueprintManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ShaderSourceCodeBlueprintManager();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
