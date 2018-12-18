// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{

		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		template <typename T> T* OperationMessageHandlerManager::getHandler(bool onlyHost)
		{
			return static_cast<T*>(OperationMessageHandlerManager::getHandlerById(T::HANDLER_ID, onlyHost));
		}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		template <typename T>
		T* OperationMessageHandlerManager::getHandler()
		{
			return static_cast<T*>(getHandlerById(T::HANDLER_ID));
		}

		inline OperationMessageHandler* OperationMessageHandlerManager::getHandlerById(OperationMessage id)
		{
			auto iterator = mHandlerMap.find(id);
			if (iterator == mHandlerMap.end())
			{
				return nullptr;
			}
			return iterator->second.get();
		}

		inline const std::map<OperationMessage, std::unique_ptr<OperationMessageHandler>>& OperationMessageHandlerManager::getAllHandler() const
		{
			return mHandlerMap;
		}

		inline void OperationMessageHandlerManager::registerHandler(OperationMessage id, OperationMessageHandler& operationMessageHandler)
		{
			mHandlerMap.emplace(id, std::unique_ptr<OperationMessageHandler>(&operationMessageHandler));
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
