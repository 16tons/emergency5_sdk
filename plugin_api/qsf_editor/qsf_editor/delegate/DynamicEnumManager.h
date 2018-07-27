// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/plugin/pluginable/PluginableManager.h>

#include <QtCore/qvariant.h>


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
		*    Dynamic enum manager implementation
		*
		*  @remarks
		*    The dynamic enum manager is automatically able to use all CAMP classes which are derived from "qsf::editor::DynamicEnumProvider".
		*/
		class QSF_EDITOR_API_EXPORT DynamicEnumManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef std::pair<QVariant, std::string> ValuePair;
			typedef std::vector<ValuePair> ValuePairList;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			DynamicEnumManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DynamicEnumManager();

			/**
			*  @brief
			*    Search for matching dynamic enum provider to CAMP property, return all items for display
			*
			*  @return
			*    List with item values and QVariant for display
			*/
			const ValuePairList& getItems(const camp::Property& campProperty, bool rebuildCachedList = true);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;
			virtual void createPluginableInstance(const camp::Class& added, camp::UserObject& campUserObject) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::DynamicEnumManager)
