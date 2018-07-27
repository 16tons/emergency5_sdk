// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/EditorCampStructs.h"

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
				class QSF_EDITOR_BASE_API_EXPORT EditorAssetPackageEdit : public qsf::packet::BinaryPacket<EditorAssetPackageEdit>
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
					EditorAssetPackageEdit();

					/**
					*  @brief
					*    Destructor
					*/
					virtual ~EditorAssetPackageEdit();

					ProjectId getProjectId() const;
					void setProjectId(ProjectId projectId);

					const EditorAssetPackageProperties& getAssetPackageProperties() const;
					void setAssetPackageProperties(const EditorAssetPackageProperties& properties);


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
					EditorAssetPackageProperties mAssetPackageProperties;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
