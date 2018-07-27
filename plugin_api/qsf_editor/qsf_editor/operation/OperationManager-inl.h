// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline const QUndoStack& OperationManager::getQUndoStack() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mQUndoStack, "QSF editor: The Qt undo stack instance is invalid", QSF_REACT_THROW);
			return *mQUndoStack;
		}

		inline bool OperationManager::getOperationExecutedSignalsEnabled() const
		{
			return mOperationExecutedSignalsEnabled;
		}

		inline void OperationManager::setOperationExecutedSignalsEnabled(bool operationExecutedSignalsEnabled)
		{
			mOperationExecutedSignalsEnabled = operationExecutedSignalsEnabled;
		}

		inline bool OperationManager::isUncoupledFromServer() const
		{
			return mDecoupledFromServer;
		}

		inline void OperationManager::setBlockServerCommunication(bool blockServerCommunication)
		{
			mBlockServerCommunication = blockServerCommunication;
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline QUndoStack& OperationManager::getQUndoStackWritable() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mQUndoStack, "QSF editor: The Qt undo stack instance is invalid", QSF_REACT_THROW);
			return *mQUndoStack;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::base::OperationManager methods ]
		//[-------------------------------------------------------]
		inline void OperationManager::onUndoOperationExecuted(const base::Operation& operation)
		{
			// Tell the world that an undo operation has been executed?
			if (mOperationExecutedSignalsEnabled)
			{
				Q_EMIT undoOperationExecuted(operation);
			}
		}

		inline void OperationManager::onRedoOperationExecuted(const base::Operation& operation)
		{
			// Tell the world that an redo operation has been executed?
			if (mOperationExecutedSignalsEnabled)
			{
				Q_EMIT redoOperationExecuted(operation);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
