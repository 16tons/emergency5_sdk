// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline CompositingDebugGroup::CompositingDebugGroup(DebugGroupManager* debugGroupManager) :
			DebugGroup(debugGroupManager),
			// Material blueprint system
			mMonitorShaderFiles(false),
			mWriteShaderSourceCode(false),
			// Light
			mDebugShadows(false),
			mDisableDeferredLightRenderOperation(false)
		{
			// Nothing to do in here
		}

		inline CompositingDebugGroup::~CompositingDebugGroup()
		{
			// Nothing to do in here
		}

		inline bool CompositingDebugGroup::getMonitorShaderFiles() const
		{
			return mMonitorShaderFiles;
		}

		inline void CompositingDebugGroup::setMonitorShaderFiles(bool monitorShaderFiles)
		{
			assignAndPromoteChange(mMonitorShaderFiles, monitorShaderFiles, MONITOR_SHADER_FILES);
		}

		inline bool CompositingDebugGroup::getWriteShaderSourceCode() const
		{
			return mWriteShaderSourceCode;
		}

		inline void CompositingDebugGroup::setWriteShaderSourceCode(bool writeShaderSourceCode)
		{
			assignAndPromoteChange(mWriteShaderSourceCode, writeShaderSourceCode, WRITE_SHADER_SOURCE_CODE);
		}

		inline bool CompositingDebugGroup::getDebugShadows() const
		{
			return mDebugShadows;
		}

		inline void CompositingDebugGroup::setDebugShadows(bool debugShadows)
		{
			assignAndPromoteChange(mDebugShadows, debugShadows, DEBUG_SHADOWS);
		}

		inline bool CompositingDebugGroup::getDisableDeferredLightRenderOperation() const
		{
			return mDisableDeferredLightRenderOperation;
		}

		inline void CompositingDebugGroup::setDisableDeferredLightRenderOperation(bool disableDeferredLightRenderOperation)
		{
			assignAndPromoteChange(mDisableDeferredLightRenderOperation, disableDeferredLightRenderOperation, DISABLE_DEFERRED_LIGHT_RENDER_OPERATION);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf
