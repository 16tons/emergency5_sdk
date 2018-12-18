// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf_editor_base/operation/OperationManager.h>

#include <QtCore/qobject.h>

#include <deque>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QUndoStack;
QT_END_NAMESPACE
namespace qsf
{
	class Map;
	namespace editor
	{
		class NetworkManager;
		namespace base
		{
			class User;
			class Operation;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Operation manager class
		*
		*  @note
		*    - Imagine the operation manager as the gateway to the QSF editor server and hence the other users
		*/
		class QSF_EDITOR_API_EXPORT OperationManager : public QObject, public base::OperationManager
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class UndoHistoryView;


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted after an undo operation was executed
			*
			*  @param[in] operation
			*    The executed undo operation
			*/
			void undoOperationExecuted(const base::Operation& operation);

			/**
			*  @brief
			*    Qt signal emitted after an redo operation was executed
			*
			*  @param[in] operation
			*    The executed redo operation
			*/
			void redoOperationExecuted(const base::Operation& operation);

			/**
			*  @brief
			*    Qt signal emitted after coupled to server
			*/
			void coupledToServer();

			/**
			*  @brief
			*    Qt signal emitted after decoupled to server
			*/
			void decoupledFromServer();


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			OperationManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OperationManager();

			//[-------------------------------------------------------]
			//[ Local                                                 ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the Qt undo stack instance
			*
			*  @return
			*    The Qt undo stack instance, do not destroy the returned instance
			*/
			inline const QUndoStack& getQUndoStack() const;

			/**
			*  @brief
			*    Mark the Qt undo stack as clean (might result in enable/disable save action, update window title etc.)
			*/
			void setUndoStackClean();

			/**
			*  @brief
			*    Undo everything
			*/
			void undoAll();

			/**
			*  @brief
			*    Flush undo stack
			*
			*  @param[in] userFeedback
			*    Provide the user with feedback so it's known what's going on?
			*
			*  @note
			*   - Before the undo stack is cleared, a hidden select operation will be committed: As soon as the undo stack is empty, logically there can't be any selection
			*   - Use this method if the contents of the QUndoStack has become invalid (e.g. because there is no valid current map)
			*   - When the editor is in online mode, tell the server about the flush
			*   - When the editor is in offline mode, save the current map
			*/
			void flushUndoStack(bool userFeedback = true);

			/**
			*  @brief
			*    Clear undo stack
			*
			*  @note
			*   - This is the un-safe counterpart of flushUndoStack(). No committing to the server or map saving is done here!
			*
			*  @warning
			*   - Only use clearing when you are absolutely sure you don't need the safety net logic of flushUndoStack()
			*   - A good example is the Sequence editor which has a custom undo stack which is operating in isolation, without any server synchronization and on a different set of operations than the main editor undo stack. This would be a good use case.
			*/
			void clearUndoStack();

			/**
			*  @brief
			*    Return whether or not operation executed signals are enabled
			*
			*  @return
			*    "true" in case operation executed signals are enabled, else "false"
			*/
			inline bool getOperationExecutedSignalsEnabled() const;

			/**
			*  @brief
			*    Set whether or not operation executed signals are enabled
			*
			*  @param[in] operationExecutedSignalsEnabled
			*    "true" in case operation executed signals are enabled, else "false"
			*/
			inline void setOperationExecutedSignalsEnabled(bool operationExecutedSignalsEnabled);

			/**
			*  @brief
			*    Push an hidden operation into the operation manager; in case there's an online editor server it won't be told about the pushed operation
			*
			*  @param[in] operation
			*    Operation to push into the operation manager, can be a null pointer (in this case nothing happens at all), the operation manager takes over the control of the memory
			*  @param[in] isUndo
			*    "true" if operation is being undone, "false" otherwise
			*  @param[in] map
			*    Map to commit the operation to
			*
			*  @see
			*    - "qsf::editor::base::OperationManager::pushLocalOnly()"
			*/
			void pushLocalOnlyToMap(base::Operation* operation, bool isUndo, const Map& map);

			//[-------------------------------------------------------]
			//[ Management                                            ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Inject an operation received from the server
			*
			*  @param[in] operation
			*    Operation to push into the operation manager, can be a null pointer (in this case nothing happens at all),
			*    the operation manager takes over the control of the memory
			*  @param[in] isUndo
			*    "true" if operation is being undone, "false" otherwise
			*/
			void injectFromServer(base::Operation* operation, bool isUndo);

			/**
			*  @brief
			*    Return whether or not the editor is currently uncoupled from the server.
			*
			*  @return
			*    "true" if decoupled, which means that operations from the server are not executed, but collected in a queue for later execution
			*
			*  @see
			*    - qsf::editor::OperationManager::coupleToServer
			*/
			inline bool isUncoupledFromServer() const;

			/**
			*  @brief
			*    Couple to the server
			*
			*  @remarks
			*    By default, the editor is coupled to the server.
			*    It can be decoupled e.g. for locally running a simulation preview that requires temporarily ignoring operations from the server.
			*    This method will immediately execute all operations received while the editor was decoupled.
			*/
			void coupleToServer();

			/**
			*  @brief
			*    Decouple from the server
			*
			*  @see
			*    - qsf::editor::OperationManager::coupleToServer
			*/
			void decoupleFromServer();

			/**
			*  @brief
			*    Block server communication
			*
			*  @remarks
			*    Unlike "coupleToServer()"/"decoupleFromServer()" which are used to temporarily disable server/client communication,
			*    blocking is irreversible (meaning data loss) while once enabled during map editing. This blocking functionality is
			*    primarily used for maps which are inside locked projects (read-only).
			*/
			inline void setBlockServerCommunication(bool blockServerCommunication);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::OperationManager methods ]
		//[-------------------------------------------------------]
		public:
			virtual void push(base::Operation* operation, bool allowUndo = true) override;
			virtual void pushLocalOnly(base::Operation* operation, bool isUndo) override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			//[-------------------------------------------------------]
			//[ Local                                                 ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the Qt undo stack instance
			*
			*  @return
			*    The Qt undo stack instance, do not destroy the returned instance
			*
			*  @remarks
			*   - Take care that your writing access to the QUndoStack will not disturb the regular online operation
			*/
			inline QUndoStack& getQUndoStackWritable() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::base::OperationManager methods ]
		//[-------------------------------------------------------]
		protected:
			inline virtual void onUndoOperationExecuted(const base::Operation& operation) override;
			inline virtual void onRedoOperationExecuted(const base::Operation& operation) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct OperationAction
			{
				base::Operation* operation;
				bool isUndo;

				inline OperationAction(base::Operation* operation, bool isUndo) :
					operation(operation),
					isUndo(isUndo)
				{
					// Nothing to do here
				}
			};


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void indexChangedQUndoStack(int newUndoStackIndex);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Submit an operation through the QSF editor client
			*
			*  @param[in] operation
			*    Operation to submit
			*    - For ease-of-use purposes, the operation is passed by reference instead of by pointer
			*  @param[in] isUndo
			*    "true" if operation is being undone, "false" otherwise
			*/
			void submitOperation(const base::Operation& operation, bool isUndo) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			NetworkManager&				 mNetworkManager;					///< Network manager instance, do not destroy the instance
			bool						 mOperationExecutedSignalsEnabled;	///< Are operation executed signals are enabled?
			QUndoStack*					 mQUndoStack;						///< Qt undo stack instance, always valid, we have to destroy the instance in case we no longer need it
			int							 mPreviousUndoStackIndex;			///< previous position in undo stack (used for replicating undo/redo history navigation to server)
			bool						 mDecoupledFromServer;				///< If "true", injected operations get enqueued for later execution
			bool						 mBlockServerCommunication;			///< If "true", server communication will be blocked in a none reversible way (meaning data loss)
			std::deque<OperationAction>	 mOperationQueue;					///< Queue of operations injected from the server while we are decoupled
			std::deque<OperationAction>	 mPushedOperationsQueue;			///< Operations pushed while a operation push is currently handled
			base::Operation*			 mCurrentlyPushedOperation;			///< Reference to the operation which is currently pushed


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/OperationManager-inl.h"
