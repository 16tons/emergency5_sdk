// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/asset/BaseAsset.h>
#include <qsf/base/GetUninitialized.h>
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
				class QSF_EDITOR_BASE_API_EXPORT AssetHistory : public qsf::packet::BinaryPacket<AssetHistory>
				{


				//[-------------------------------------------------------]
				//[ Public definitions                                    ]
				//[-------------------------------------------------------]
				public:
					static const uint32 PACKET_ID;

					class AssetHistoryEntry : public BaseAsset
					{
					public:
						inline AssetHistoryEntry() :
							BaseAsset(getUninitialized<GlobalAssetId>()),
							mHasData(false)
						{
							// Nothing to do in here
						}

						inline AssetHistoryEntry(const AssetHistoryEntry& x) :
							BaseAsset(x.getGlobalAssetId()),
							mHasData(x.mHasData)
						{
							operator=(x);
						}

						inline AssetHistoryEntry& operator=(const AssetHistoryEntry& x)
						{
							BaseAsset::operator=(x);
							mHasData = x.mHasData;
							return *this;
						}

						inline AssetHistoryEntry& operator=(const BaseAsset& x)
						{
							BaseAsset::operator=(x);
							return *this;
						}

						inline void setGlobalAssetId(GlobalAssetId globalAssetId)
						{
							mGlobalAssetId = globalAssetId;
						}

						inline bool hasData() const
						{
							return mHasData;
						}

						inline void setHasData(bool hasData)
						{
							mHasData = hasData;
						}


					private:
						bool mHasData;

						QSF_CAMP_RTTI();
					};


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
				public:
					/**
					*  @brief
					*    Default constructor
					*/
					AssetHistory();

					/**
					*  @brief
					*    Destructor
					*/
					virtual ~AssetHistory();

					GlobalAssetId getGlobalAssetId() const;
					void setGlobalAssetId(GlobalAssetId globalAssetId);

					const std::vector<AssetHistoryEntry>& getAssetHistory() const;
					void setAssetHistory(const std::vector<AssetHistoryEntry>& assetHistory);


				//[--------------------------------------------------------]
				//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
				//[--------------------------------------------------------]
				public:
					virtual void serialize(BinarySerializer& serializer) override;


				//[-------------------------------------------------------]
				//[ Protected data                                        ]
				//[-------------------------------------------------------]
				protected:
					GlobalAssetId mGlobalAssetId;
					std::vector<AssetHistoryEntry> mHistory;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf

QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::packet::AssetHistory::AssetHistoryEntry);
