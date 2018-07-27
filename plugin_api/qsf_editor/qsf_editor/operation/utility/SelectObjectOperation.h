// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/Operation.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Object;
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
		*    Select object operation class
		*
		*  @note
		*    - Usually selection is not performed generic objects (this operation is primarily a broadcast)
		*    - Local atomic operation: This operation is primarily used for developer features to allow inspecting properties
		*/
		class SelectObjectOperation : public base::Operation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::SelectObjectOperation" unique operation ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] object
			*    Object to select, can be a null pointer
			*/
			explicit SelectObjectOperation(Object* object);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SelectObjectOperation();

			/**
			*  @brief
			*    Return the selected object
			*
			*  @return
			*    The selected object, can be a null pointer, do not destroy the instance
			*/
			Object* getObject() const;


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getId() const override;
			virtual bool isLocal() const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Object* mObject;	///< The selected object, can be a null pointer, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
