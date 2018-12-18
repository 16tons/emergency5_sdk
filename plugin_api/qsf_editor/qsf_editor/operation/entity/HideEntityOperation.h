// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/entity/EntityOperation.h>


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
		*    Hide entity operation class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, it would be fatal if one user could hide the layer of another user
		*/
		class HideEntityOperation : public base::EntityOperation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::HideEntityOperation" unique operation ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			HideEntityOperation();

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] entityId
			*    The unique entity identifier of the entity to hide
			*  @param[in] hidden
			*    "true" if the entity is hidden, else "false"
			*/
			HideEntityOperation(uint64 entityId, bool hidden);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~HideEntityOperation();

			bool getHidden() const;
			void setHidden(bool hidden);

			bool getPreviousHidden() const;
			void setPreviousHidden(bool previousHidden);


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
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void hideLinkedChildren(const Entity& entity, bool hiddenState) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mPreviousHidden;	///< The previous hidden state of the entity
			bool mHidden;			///< The new hidden state of the entity


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
