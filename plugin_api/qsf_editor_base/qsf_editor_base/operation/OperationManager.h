// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/base/manager/Manager.h>

#include <boost/function.hpp>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Operation;
			class OperationData;
		}
	}
}


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
#define REGISTER_OPERATION_TYPE(classname) registerOperationType(classname::OPERATION_ID, boost::factory<classname*>())


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
			//[ Global definitions                                    ]
			//[-------------------------------------------------------]
			typedef Operation* OperationGenerator();
			typedef std::unordered_map<uint32, boost::function<OperationGenerator> > OperationTypeList;
			typedef std::vector<char> SerializedOperation;


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Abstract operation manager base class
			*/
			class QSF_EDITOR_BASE_API_EXPORT OperationManager : public Manager
			{


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
				friend class Operation;	// Must inform the operation manager about undo/redo


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~OperationManager();

				/**
				*  @brief
				*    Create an operation by using given data
				*
				*  @param[in] data
				*    Operation data, can be a null pointer (in this case a null pointer will be returned)
				*  @param[in] size
				*    Operation data size in bytes
				*
				*  @return
				*    The created operation, destroy the instance in case you no longer need it, null pointer on error
				*/
				Operation* createOperation(const SerializedOperation& operation) const;

				// Throws exceptions in case of an error
				SerializedOperation serializeOperation(const Operation& operation) const;

				bool isOperationOnline(uint32 operationId) const;


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::OperationManager methods ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Push an operation into the operation manager; in case there's an online editor server it will be told about the pushed operation
				*
				*  @param[in] operation
				*    Operation to push into the operation manager, can be a null pointer (in this case nothing happens at all), the operation manager takes over the control of the memory
				*  @param[in] allowUndo
				*    "true" if operation may be undone based on the operations "isReversible()"-method, "false" if it should not be undone in any case ignoring the operations setting
				*
				*  @note
				*    - For ease-of-use purposes, the operation is passed by pointer instead of by reference
				*    - Do not access the given operation after this method call
				*/
				virtual void push(Operation* operation, bool allowUndo = true) = 0;

				/**
				*  @brief
				*    Push an hidden operation into the operation manager; in case there's an online editor server it won't be told about the pushed operation
				*
				*  @param[in] operation
				*    Operation to push into the operation manager, can be a null pointer (in this case nothing happens at all), the operation manager takes over the control of the memory
				*  @param[in] isUndo
				*    "true" if operation is being undone, "false" otherwise
				*
				*  @remarks
				*    The editor adds some hidden temporary entities or components like a ghost previsualization of a new entity. While
				*    such elements are not saved, not put onto the undo-stack and not send to a server when using cooperative work,
				*    certain used Qt models need to be informed about the changes inside a map to be in a consistent state.
				*    Therefore, such special elements have also to be created by using operations, but those operations are not allowed
				*    to be put onto the undo stack or to be send to a server. Use "qsf::editor::OperationManager::pushLocalOnly()"
				*    instead of "qsf::editor::OperationManager::push()" to commit such operations. The same is true for operations
				*    received from a server (cooperative work).
				*
				*  @note
				*    - For ease-of-use purposes, the operation is passed by pointer instead of by reference
				*    - Do not access the given operation after this method call
				*/
				virtual void pushLocalOnly(Operation* operation, bool isUndo) = 0;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Default constructor
				*/
				OperationManager();

				inline void registerOperationType(uint32 operationId, boost::function<OperationGenerator> constructor);


			//[-------------------------------------------------------]
			//[ Protected virtual qsf::editor::base::OperationManager methods ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Method is called after an undo operation was executed
				*
				*  @param[in] operation
				*    The executed undo operation
				*
				*  @note
				*    - The default implementation is empty
				*/
				virtual void onUndoOperationExecuted(const Operation& operation);

				/**
				*  @brief
				*    Method is called after an redo operation was executed
				*
				*  @param[in] operation
				*    The executed redo operation
				*
				*  @note
				*    - The default implementation is empty
				*/
				virtual void onRedoOperationExecuted(const Operation& operation);


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				OperationTypeList mOperationTypeList;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/OperationManager-inl.h"
