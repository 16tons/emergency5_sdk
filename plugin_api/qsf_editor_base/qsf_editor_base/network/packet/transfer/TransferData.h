// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/NetworkTypes.h"

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
				class QSF_EDITOR_BASE_API_EXPORT TransferData : public qsf::packet::BinaryPacket<TransferData>
				{


				//[-------------------------------------------------------]
				//[ Public definitions                                    ]
				//[-------------------------------------------------------]
				public:
					static const uint32 PACKET_ID;

					enum Flags : uint32
					{
						FLAG_CANCEL = 1 << 0
					};


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
				public:
					/**
					*  @brief
					*    Default constructor
					*/
					TransferData();

					/**
					*  @brief
					*    Destructor
					*/
					virtual ~TransferData();

					NetworkTransferId getTransferId() const;
					void setTransferId(NetworkTransferId transferId);

					bool getCancel() const;
					void setCancel(bool cancel);

					uint32 getFileSize() const;
					void setFileSize(uint32 fileSize);

					uint32 getFileDataOffset() const;
					void setFileDataOffset(uint32 fileDataOffset);

					uint32 getFileDataLength() const;

					const std::vector<char>& getFileData() const;
					void setFileData(const std::vector<char>& data);
					void setFileData(std::vector<char>&& data);


				//[--------------------------------------------------------]
				//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
				//[--------------------------------------------------------]
				public:
					virtual void serialize(BinarySerializer& serializer) override;


				//[-------------------------------------------------------]
				//[ Private data                                          ]
				//[-------------------------------------------------------]
				private:
					NetworkTransferId mTransferId;
					uint32 mFileSize;
					uint32 mFileDataOffset;
					uint32 mFlags;
					std::vector<char> mFileData;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
