// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/operation/OperationManager.h"

#include <qsf/network/layered/packet/BinaryPacket.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			namespace packet
			{
				//[-------------------------------------------------------]
				//[ Classes                                               ]
				//[-------------------------------------------------------]
				/**
				*  @brief
				*    TODO(co) Comment
				*/
				class QSF_EDITOR_BASE_API_EXPORT EditorOperationBulk : public qsf::packet::BinaryPacket<EditorOperationBulk>
				{


				//[-------------------------------------------------------]
				//[ Public definitions                                    ]
				//[-------------------------------------------------------]
				public:
					static const uint32 PACKET_ID;

					struct OperationAction
					{
						OperationAction(Operation* operation, bool isUndo) :
							operation(operation),
							isUndo(isUndo)
						{
							// Nothing to do here
						}

						Operation* operation;
						bool isUndo;
					};

					struct COperationAction
					{
						COperationAction(const Operation* operation, bool isUndo) :
							operation(operation),
							isUndo(isUndo)
						{
							// Nothing to do here
						}

						const Operation* operation;
						bool isUndo;
					};


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
				public:
					/**
					*  @brief
					*    Default constructor
					*/
					inline EditorOperationBulk();

					/**
					*  @brief
					*    Destructor
					*/
					inline virtual ~EditorOperationBulk();

					std::vector<OperationAction> deserializeOperations() const;
					void serializeOperations(const std::vector<COperationAction>& operations);


				//[--------------------------------------------------------]
				//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
				//[--------------------------------------------------------]
				public:
					virtual void serialize(BinarySerializer& serializer) override;


				//[-------------------------------------------------------]
				//[ Private data                                          ]
				//[-------------------------------------------------------]
				private:
					std::vector<std::pair<SerializedOperation, bool>> mSerializedOperations;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/network/packet/editor/EditorOperationBulk-inl.h"
