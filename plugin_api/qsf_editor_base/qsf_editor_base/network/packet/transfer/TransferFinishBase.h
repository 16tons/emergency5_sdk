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
				class QSF_EDITOR_BASE_API_EXPORT TransferFinishBase
				{


				//[-------------------------------------------------------]
				//[ Public virtual qsf::editor::base::packet::TransferFinishBase methods ]
				//[-------------------------------------------------------]
				public:
					inline virtual ~TransferFinishBase()
					{
						// Nothing to do in here
					}

					virtual void getAssetTransferInfo(TransferInfo& transferInfo) const = 0;
					virtual void setAssetTransferInfo(const TransferInfo& transferInfo) = 0;


				};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
