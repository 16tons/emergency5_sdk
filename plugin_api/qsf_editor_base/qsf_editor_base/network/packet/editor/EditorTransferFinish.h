// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/network/packet/transfer/TransferFinishBase.h"

#include <qsf/asset/BaseCachedAsset.h>
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
				class QSF_EDITOR_BASE_API_EXPORT EditorTransferFinish : public qsf::packet::BinaryPacket<EditorTransferFinish>, public TransferFinishBase
				{


				//[-------------------------------------------------------]
				//[ Public definitions                                    ]
				//[-------------------------------------------------------]
				public:
					static const uint32 PACKET_ID;


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
				public:
					/**
					*  @brief
					*    Default constructor
					*/
					EditorTransferFinish();

					/**
					*  @brief
					*    Destructor
					*/
					virtual ~EditorTransferFinish();

					const BaseCachedAsset& getBaseCachedAsset() const;
					void setBaseCachedAsset(const BaseCachedAsset& baseCachedAsset);


				//[-------------------------------------------------------]
				//[ Public virtual qsf::editor::base::packet::TransferFinishBase methods ]
				//[-------------------------------------------------------]
				public:
					virtual void getAssetTransferInfo(TransferInfo& transferInfo) const override;
					virtual void setAssetTransferInfo(const TransferInfo& transferInfo) override;


				//[--------------------------------------------------------]
				//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
				//[--------------------------------------------------------]
				public:
					virtual void serialize(BinarySerializer& serializer) override;


				//[-------------------------------------------------------]
				//[ Private data                                          ]
				//[-------------------------------------------------------]
				private:
					TransferInfo mTransferInfo;
					BaseCachedAsset mBaseCachedAsset;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
