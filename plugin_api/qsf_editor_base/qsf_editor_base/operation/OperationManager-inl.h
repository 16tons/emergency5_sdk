// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline OperationManager::~OperationManager()
			{
				// Nothing to do in here
			}


			//[-------------------------------------------------------]
			//[ Protected virtual qsf::editor::base::OperationManager methods ]
			//[-------------------------------------------------------]
			inline void OperationManager::onUndoOperationExecuted(const Operation& operation)
			{
				// Nothing to do in here
			}

			inline void OperationManager::onRedoOperationExecuted(const Operation& operation)
			{
				// Nothing to do in here
			}


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			inline void OperationManager::registerOperationType(uint32 operationId, boost::function<OperationGenerator> constructor)
			{
				mOperationTypeList[operationId] = constructor;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
