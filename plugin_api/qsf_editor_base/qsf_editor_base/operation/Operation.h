// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Defines                                               ]
//[-------------------------------------------------------]
#define QSF_OPERATION_CHECK_SEAL QSF_ASSERT(!isSealed(), "Setter was called after Operation has already been sealed!", QSF_REACT_NONE)


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class CompoundOperation;
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
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Abstract operation base class
			*
			*  @note
			*    - Base class for undo/redo and cooperative work operations
			*    - This class is using the command design pattern as described within the book
			*      "Gamma, Erich; Helm, Richard; Johnson, Ralph; Vlissides, John (1994). Design Patterns: Elements of Reusable Object-Oriented Software. Addison-Wesley. ISBN 0-201-63361-2."
			*
			*  @todo
			*    - TODO(co) Optimization: We might want to use "qsf::PagedAllocator" in order to avoid overusing the C++ memory heap
			*/
			class QSF_EDITOR_BASE_API_EXPORT Operation : public Object
			{


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
			friend class CompoundOperation;	// Needs to write mParentOperation when operation is added


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~Operation();

				/**
				*  @brief
				*    Return the text of the operation
				*
				*  @return
				*    The UTF-8 text of the operation
				*/
				std::string getText() const;

				/**
				*  @brief
				*    Set the text of the operation
				*
				*  @param[in] text
				*    The UTF-8 text of the operation, do not localize the text because there's no reason to put that much effort into something the normal user will not see
				*/
				void setText(const std::string& text);

				/**
				*  @brief
				*    Return the ID of the user issuing the operation
				*
				*  @return
				*    The ID of the user issuing the operation
				*/
				uint32 getUserId() const;

				/**
				*  @brief
				*    Set the ID of the user issuing the operation
				*
				*  @param[in] userId
				*    The ID of the user issuing the operation
				*/
				void setUserId(uint32 userId);

				/**
				*  @note
				*    Use ease-of-use define for setters: QSF_OPERATION_CHECK_SEAL
				*
				*  @return
				*    true if the operation is sealed and no setters may be used anymore
				*/
				bool isSealed() const;

				/**
				*  @brief
				*    Seal the operation after constructing it using the default constructor and explicitly using setters for all parameters
				*
				*  @note
				*    - May be called multiple times and ensures to not change any property after seal has been placed
				*    - This method barely contains some boilerplate code see/override sealImpl() for actual implementation
				*/
				void seal();

				/**
				*  @brief
				*    Undo the operation
				*
				*  @note
				*    This method barely contains some boilerplate code see/override undoImpl() for actual implementation
				*/
				void undo();

				/**
				*  @brief
				*    Redo the operation
				*
				*  @note
				*    This method barely contains some boilerplate code see/override redoImpl() for actual implementation
				*/
				void redo();


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::Operation methods         ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Return the unique operation identifier
				*
				*  @return
				*    The unique operation identifier
				*
				*  @note
				*    - The unique operation identifier is generated by "qsf::StringHash" by using the class name
				*/
				virtual uint32 getId() const = 0;

				/**
				*  @brief
				*    Return whether or not the operation will end up in the undo/redo stack
				*
				*  @return
				*    "true" if the operation will end up in the undo/redo stack, else "false"
				*
				*  @note
				*    - The default implementation returns "true"
				*/
				virtual bool isReversible() const;

				/**
				*  @brief
				*    Return whether or not the operation is local only
				*
				*  @return
				*    "true" if it's a local operation, else "false"
				*
				*  @remarks
				*    Inside the QSF editor, operations have to use cases:
				*    - Undo/redo
				*    - cooperative work
				*
				*    For example the select layer operation is considered to be a local operation. While it is necessary
				*    that this operation will end up on the undo/redo stack, it does not make any sense to commit this
				*    operation to the server for cooperative work. Most of the operations are not local only.
				*
				*  @note
				*    - The default implementation returns "false"
				*/
				virtual bool isLocal() const;

				/**
				*  @brief
				*    Return whether or not the operation is replicated by the server to all other relevant clients
				*
				*  @return
				*    "true" if it's a replicated operation, else "false"
				*
				*  @remarks
				*    Some non-local operations are not meant to be replicated by the server. (e.g. EntitySelectOperation)
				*
				*  @note
				*    - The default implementation returns "true"
				*/
				virtual bool isReplicated() const;

				/**
				*  @brief
				*    Return the memory consumption of this operation
				*
				*  @return
				*    The memory consumption in bytes of this operation
				*
				*  @note
				*    - The memory consumption of cooperative work operations is usually lower because
				*      there's no need to store data required for undoing an operation
				*    - The default implementation returns 0
				*/
				virtual size_t getMemoryConsumption() const;

				/**
				*  @brief
				*    Seal the operation after constructing it using the default constructor and explicitly
				*    using setters for all parameters.
				*
				*  @note
				*    Must ensure to call it's parent's implementation.
				*    Will only be called once.
				*/
				virtual void sealImpl();

				/**
				*  @brief
				*    Undo the operation (common actions to always be executed)
				*
				*  @note
				*    Must ensure to call it's parent's implementation.
				*/
				virtual void undoImpl();

				/**
				*  @brief
				*    Redo the operation (common actions to always be executed)
				*
				*  @note
				*    Must ensure to call it's parent's implementation.
				*/
				virtual void redoImpl();


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Default constructor
				*/
				Operation();


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				CompoundOperation* mParentOperation;	///< Parent compound operation, can be a null pointer, do not destroy the instance


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::string	mText;		///< The UTF-8 text of the operation
				uint32		mUserId;	///< The ID of the user issuing the operation (set server side)
				bool		mSealed;	///< The sealing state of the operation


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::Operation)
