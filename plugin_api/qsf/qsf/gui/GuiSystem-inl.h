// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GuiSystem::GuiSystem() :
		mRunning(false),
		mRocketSystemInterface(nullptr),
		mRocketFileInterface(nullptr),
		mOgreRocketRenderInterface(nullptr),
		mDebuggerGuiContext(nullptr),
		mRocketCoreContextDebugger(nullptr)
	{
		// Nothing to do in here
	}

	inline GuiSystem::~GuiSystem()
	{
		// Nothing to do in here
	}

	inline GuiContext* GuiSystem::getDebuggerGuiContext() const
	{
		return mDebuggerGuiContext;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* GuiSystem::getName() const
	{
		return "GUI system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
