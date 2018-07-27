// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/entity/EntitySelectionOperation.h>


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
		*    Set current entity (the focus of attention) operation class
		*/
		class SetCurrentEntityOperation : public base::EntitySelectionOperation
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
			friend class EntitySelectionManager;	///< Only the entity selection manager is allowed to create instances of this operation in order to guarantee that entity locks are respected by the client


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::base::SetCurrentEntityOperation" unique operation ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SetCurrentEntityOperation();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SetCurrentEntityOperation();

			/**
			*  @brief
			*    Return the unique entity identifier previously set as current ID
			*
			*  @return
			*    The unique entity identifier previously set as current ID
			*/
			uint64 getPreviousEntityId() const;
			void setPreviousEntityId(uint64 previousEntityId);


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getId() const override;
			virtual bool isLocal() const override;
			virtual size_t getMemoryConsumption() const override;
			virtual void sealImpl() override;
			virtual void undoImpl() override;
			virtual void redoImpl() override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] entityId
			*    The unique entity identifier to set as current ID
			*/
			explicit SetCurrentEntityOperation(uint64 entityId);

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] id
			*    The unique entity identifier to set as current ID
			*  @param[in] previousEntityId
			*    The unique entity identifier previously set as current ID
			*/
			SetCurrentEntityOperation(uint64 id, uint64 previousEntityId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint64 mPreviousEntityId;	///< The unique entity identifier previously set as current ID


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::SetCurrentEntityOperation)
