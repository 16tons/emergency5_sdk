// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/NetworkTypes.h"

#include <qsf/asset/BaseAsset.h>
#include <qsf/base/BoostPtreeHelper.h>
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
				class QSF_EDITOR_BASE_API_EXPORT AssetEditStore : public qsf::packet::BinaryPacket<AssetEditStore>
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
					AssetEditStore();

					/**
					*  @brief
					*    Destructor
					*/
					virtual ~AssetEditStore();

					GlobalAssetId getGlobalAssetId() const;

					NetworkTransferId getNetworkTransferId() const;
					void setNetworkTransferId(NetworkTransferId transferId);

					const BaseAsset& getAsset() const;
					void setAsset(const BaseAsset& asset);

					// TODO(ca) Remove me: This is another asset-db-lite hack
					const boost::property_tree::ptree& getAssetDbLiteAttachment() const;
					void setAssetDbLiteAttachment(const boost::property_tree::ptree& data);


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
					BaseAsset mBaseAsset;
					boost::property_tree::ptree mAssetDbLiteAttachment;		///< TODO(ca) Remove me: This is another asset-db-lite hack


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
