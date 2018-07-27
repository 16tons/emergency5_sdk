// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheBase.h"

#include <qsf/base/WeakPtr.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class MeshComponent;
	namespace game
	{
		class BitStream;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 entity with a mesh componet data cache for a single entity for multiplayer
		*/
		class MeshData: public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			MeshData(const qsf::Entity& targetEntity);
			virtual ~MeshData();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool includeForPrepare(const int32 currentTickCount, bool entityIsVisible) override;
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void writeBoundingBoxPartToStream(qsf::game::BitStream& bitStream, const glm::vec3& boundingBoxPart);
			void readBoundingBoxPartFromStream(const qsf::game::BitStream& bitStream, glm::vec3& boundingBoxPart);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::WeakPtr<qsf::MeshComponent> mComponent;
			bool		mCustomBoundingBoxSet;
			bool		mCustomBoundingBoxChanged;
			glm::vec3	mCustomBoundBoxMin;
			glm::vec3	mCustomBoundBoxMax;
			bool		mCustomBoundingBoxExtendsChanged;
			glm::vec3	mCustomBoundingBoxExtends;
			uint32		mTintPaletteLocalAssetId;
			bool		mTintPaletteLocalAssetIdChanged;
			int32		mTintPaletteIndex;
			bool		mTintPaletteIndexChanged;

			uint32		mMeshLocalAssetId;
			bool		mMeshLocalAssetIdChanged;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				bool		mCustomBoundingBoxSet;
				bool		mCustomBoundingBoxChanged;
				glm::vec3	mCustomBoundBoxMin;
				glm::vec3	mCustomBoundBoxMax;
				bool		mCustomBoundingBoxExtendsChanged;
				glm::vec3	mCustomBoundingBoxExtends;
				uint32		mTintPaletteLocalAssetId;
				bool		mTintPaletteLocalAssetIdChanged;
				int32		mTintPaletteIndex;
				bool		mTintPaletteIndexChanged;

				uint32		mMeshLocalAssetId;
				bool		mMeshLocalAssetIdChanged;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
