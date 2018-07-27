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

#include <qsf/asset/AssetSystemTypes.h>
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
				class QSF_EDITOR_BASE_API_EXPORT EditorCachedDataRequest : public qsf::packet::BinaryPacket<EditorCachedDataRequest>
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
					EditorCachedDataRequest();

					/**
					*  @brief
					*    Destructor
					*/
					virtual ~EditorCachedDataRequest();

					NetworkTransferId getTransferId() const;
					void setTransferId(NetworkTransferId transferId);

					ProjectId getProjectId() const;
					void setProjectId(ProjectId projectId);

					const std::string& getAssetPackageName() const;
					void setAssetPackageName(const std::string& assetPackageName);

					GlobalAssetId getGlobalAssetId() const;
					void setGlobalAssetId(GlobalAssetId globalAssetId);

					AssetRevision getCachedAssetRevision() const;
					void setCachedAssetRevision(AssetRevision cachedAssetRevision);


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
					ProjectId mProjectId;
					std::string mAssetPackageName;
					GlobalAssetId mGlobalAssetId;
					AssetRevision mCachedAssetRevision;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
