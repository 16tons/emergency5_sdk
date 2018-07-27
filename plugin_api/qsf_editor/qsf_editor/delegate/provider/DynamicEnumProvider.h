// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/delegate/DynamicEnumManager.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/plugin/pluginable/Pluginable.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract dynamic enum provider base class
		*/
		class QSF_EDITOR_API_EXPORT DynamicEnumProvider : public Pluginable<DynamicEnumManager>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef DynamicEnumManager::ValuePairList ValuePairList;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit DynamicEnumProvider(DynamicEnumManager* dynamicEnumManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DynamicEnumProvider();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return the name of the dynamic enum provider
			*/
			virtual const std::string& getSemanticName() const = 0;

			/**
			*  @brief
			*    Return a list of human readable strings and internal values associated with them
			*/
			virtual const ValuePairList& getValues(bool rebuildCachedList) = 0;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			ValuePairList mCachedList;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::DynamicEnumProvider)
