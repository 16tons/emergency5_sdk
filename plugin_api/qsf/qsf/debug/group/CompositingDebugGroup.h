// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/UniqueInstance.h"
#include "qsf/debug/group/DebugGroup.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Compositing debug group implementation
		*/
		class QSF_API_EXPORT CompositingDebugGroup : public DebugGroup, public UniqueInstance<CompositingDebugGroup>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;								///< "qsf::CompositingDebugGroup" unique pluginable debug group ID
			// Material blueprint system
			static const uint32 MONITOR_SHADER_FILES;						///< "MonitorShaderFiles" unique class property ID inside the class
			static const uint32 WRITE_SHADER_SOURCE_CODE;					///< "WriteShaderSourceCode" unique class property ID inside the class
			// Light
			static const uint32 DEBUG_SHADOWS;								///< "DebugShadows" unique class property ID inside the class
			static const uint32 DISABLE_DEFERRED_LIGHT_RENDER_OPERATION;	///< "DisableDeferredLightRenderOperation" unique class property ID inside the class


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] debugGroupManager
			*    Optional pointer to the debug group manager this debug group should be registered to, can be a null pointer
			*/
			explicit CompositingDebugGroup(DebugGroupManager* debugGroupManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CompositingDebugGroup();

			//[-------------------------------------------------------]
			//[ Material system                                       ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get whether or not to monitor shader files and to reload shaders if shader files have been modified since the last reload
			*
			*  @return
			*    "true" to monitor shader files, else "false"
			*/
			inline bool getMonitorShaderFiles() const;

			/**
			*  @brief
			*    Set whether or not to monitor shader files and to reload shaders if shader files have been modified since the last reload
			*
			*  @param[in] monitorShaderFiles
			*    "true" to monitor shader files, else "false"
			*/
			inline void setMonitorShaderFiles(bool monitorShaderFiles);

			inline bool getWriteShaderSourceCode() const;
			inline void setWriteShaderSourceCode(bool writeShaderSourceCode);

			//[-------------------------------------------------------]
			//[ Various tiny light triggers                           ]
			//[-------------------------------------------------------]
			inline bool getDebugShadows() const;
			inline void setDebugShadows(bool debugShadows);

			inline bool getDisableDeferredLightRenderOperation() const;
			inline void setDisableDeferredLightRenderOperation(bool disableDeferredLightRenderOperation);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			// Material blueprint system
			bool mMonitorShaderFiles;	///< "true" to monitor shader files and to reload shaders if shader files have been modified since the last reload, else "false"
			bool mWriteShaderSourceCode;
			// Light
			bool mDebugShadows;
			bool mDisableDeferredLightRenderOperation;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/group/CompositingDebugGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::CompositingDebugGroup)
