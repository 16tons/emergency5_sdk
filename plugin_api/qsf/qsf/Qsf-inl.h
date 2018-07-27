// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ApplicationContext& Qsf::getApplicationContext()
	{
		return *mApplicationContext;
	}

	inline const ApplicationContext& Qsf::getApplicationContext() const
	{
		return *mApplicationContext;
	}

	inline bool Qsf::isApplicationContextStackEmpty() const
	{
		return mApplicationContextStack.empty();
	}

	inline Application* Qsf::getApplication() const
	{
		return mApplication;
	}

	inline WorkerSystem& Qsf::getWorkerSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mWorkerSystem, "The worker system instance is invalid", QSF_REACT_THROW);
		return *mWorkerSystem;
	}

	inline PlatformSystem& Qsf::getPlatformSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPlatformSystem, "The platform system instance is invalid", QSF_REACT_THROW);
		return *mPlatformSystem;
	}

	inline FileSystem& Qsf::getFileSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mFileSystem, "The file system instance is invalid", QSF_REACT_THROW);
		return *mFileSystem;
	}

	inline LogSystem& Qsf::getLogSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mLogSystem, "The log system instance is invalid", QSF_REACT_THROW);
		return *mLogSystem;
	}

	inline LocalizationSystem& Qsf::getLocalizationSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mLocalizationSystem, "The localization system instance is invalid", QSF_REACT_THROW);
		return *mLocalizationSystem;
	}

	inline JobSystem& Qsf::getJobSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mJobSystem, "The job system instance is invalid", QSF_REACT_THROW);
		return *mJobSystem;
	}

	inline ClassSystem& Qsf::getClassSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mClassSystem, "The class system instance is invalid", QSF_REACT_THROW);
		return *mClassSystem;
	}

	inline PluginSystem& Qsf::getPluginSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPluginSystem, "The plugin system instance is invalid", QSF_REACT_THROW);
		return *mPluginSystem;
	}

	inline AssetSystem& Qsf::getAssetSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mAssetSystem, "The asset system instance is invalid", QSF_REACT_THROW);
		return *mAssetSystem;
	}

	inline WindowSystem& Qsf::getWindowSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mWindowSystem, "The window system instance is invalid", QSF_REACT_THROW);
		return *mWindowSystem;
	}

	inline ScriptSystem& Qsf::getScriptSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mScriptSystem, "The script system instance is invalid", QSF_REACT_THROW);
		return *mScriptSystem;
	}

	inline RendererSystem& Qsf::getRendererSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mRendererSystem, "The renderer system instance is invalid", QSF_REACT_THROW);
		return *mRendererSystem;
	}

	inline PhysicsSystem& Qsf::getPhysicsSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPhysicsSystem, "The physics system instance is invalid", QSF_REACT_THROW);
		return *mPhysicsSystem;
	}

	inline InputSystem& Qsf::getInputSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mInputSystem, "The input system instance is invalid", QSF_REACT_THROW);
		return *mInputSystem;
	}

	inline AudioSystem& Qsf::getAudioSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mAudioSystem, "The audio system instance is invalid", QSF_REACT_THROW);
		return *mAudioSystem;
	}

	inline NetworkSystem& Qsf::getNetworkSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mNetworkSystem, "The network system instance is invalid", QSF_REACT_THROW);
		return *mNetworkSystem;
	}

	inline MessageSystem& Qsf::getMessageSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMessageSystem, "The message system instance is invalid", QSF_REACT_THROW);
		return *mMessageSystem;
	}

	inline ComponentSystem& Qsf::getComponentSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mComponentSystem, "The component system instance is invalid", QSF_REACT_THROW);
		return *mComponentSystem;
	}

	inline PrototypeSystem& Qsf::getPrototypeSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPrototypeSystem, "The prototype system instance is invalid", QSF_REACT_THROW);
		return *mPrototypeSystem;
	}

	inline LinkSystem& Qsf::getLinkSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mLinkSystem, "The link system instance is invalid", QSF_REACT_THROW);
		return *mLinkSystem;
	}

	inline MapSystem& Qsf::getMapSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMapSystem, "The map system instance is invalid", QSF_REACT_THROW);
		return *mMapSystem;
	}

	inline GuiSystem& Qsf::getGuiSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGuiSystem, "The GUI system instance is invalid", QSF_REACT_THROW);
		return *mGuiSystem;
	}

	inline ActionExecutionSystem& Qsf::getActionExecutionSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mActionExecutionSystem, "The action execution system instance is invalid", QSF_REACT_THROW);
		return *mActionExecutionSystem;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::SystemContainer methods           ]
	//[-------------------------------------------------------]
	const char* Qsf::getSystemContainerName() const
	{
		return "QSF";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
