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

#include <qsf/asset/BaseAsset.h>
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
				class QSF_EDITOR_BASE_API_EXPORT EditorAssetUpdate : public qsf::packet::BinaryPacket<EditorAssetUpdate>
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
					EditorAssetUpdate();

					/**
					*  @brief
					*    Destructor
					*/
					virtual ~EditorAssetUpdate();

					ProjectId getProjectId() const;
					void setProjectId(ProjectId projectId);

					const std::string& getAssetPackageName() const;
					void setAssetPackageName(const std::string& assetPackageName);

					GlobalAssetId getGlobalAssetId() const;

					const BaseAsset& getAsset() const;
					void setAsset(const BaseAsset& asset);

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
					ProjectId mProjectId;
					std::string mAssetPackageName;
					BaseAsset mBaseAsset;
					AssetRevision mCachedAssetRevision;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
