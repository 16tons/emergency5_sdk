// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline CommandSystem::CommandSystem() :
			mCommandManager(nullptr),
			mCommandMode(nullptr),
			mDefaultCommandMode(nullptr),
			mCommandDelegate(nullptr),
			mCommandEvaluation(nullptr)
		{
			// Nothing to do in here
		}

		inline CommandSystem::~CommandSystem()
		{
			// Nothing to do in here
		}

		inline CommandManager& CommandSystem::getCommandManager() const
		{
			return *mCommandManager;
		}

		inline CommandMode& CommandSystem::getCommandMode() const
		{
			return *mCommandMode;
		}

		template <typename T>
		void CommandSystem::switchToCommandMode()
		{
			T* commandMode = new T();
			setCommandMode(*commandMode);
		}

		inline CommandDelegate& CommandSystem::getCommandDelegate() const
		{
			return *mCommandDelegate;
		}

		template <typename T>
		void CommandSystem::createCommandDelegate()
		{
			T* commandDelegate = new T();
			setCommandDelegate(*commandDelegate);
		}

		inline CommandEvaluation& CommandSystem::getCommandEvaluation() const
		{
			return *mCommandEvaluation;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		inline const char* CommandSystem::getName() const
		{
			return "Command system";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
