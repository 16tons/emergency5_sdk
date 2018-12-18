// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

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
				*    TODO(co) Comment me
				*/
				class QSF_EDITOR_BASE_API_EXPORT EditorLoginResponse : public qsf::packet::BinaryPacket<EditorLoginResponse>
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
					inline EditorLoginResponse();

					/**
					*  @brief
					*    Destructor
					*/
					inline virtual ~EditorLoginResponse();

					inline bool isSuccess() const;
					inline void setSuccess(bool isSuccess);

					inline bool isServerEmpty() const;
					inline void setServerEmpty(bool isServerEmpty);

					inline const std::string& getServerUuid() const;
					inline void setServerUuid(const std::string& serverUuid);

					inline const boost::property_tree::ptree& getPluginVersionsPTree() const;
					inline void setPluginVersionsPTree(const boost::property_tree::ptree& pluginVersionsPTree);
					inline void setPluginVersionsPTree(boost::property_tree::ptree&& pluginVersionsPTree);


				//[--------------------------------------------------------]
				//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
				//[--------------------------------------------------------]
				public:
					virtual void serialize(BinarySerializer& serializer) override;


				//[-------------------------------------------------------]
				//[ Private data                                          ]
				//[-------------------------------------------------------]
				private:
					bool mIsSuccess;
					bool mIsServerEmpty;
					std::string mServerUuid;
					boost::property_tree::ptree mPluginVersionsPTree;


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
#include "qsf_editor_base/network/packet/editor/EditorLoginResponse-inl.h"
