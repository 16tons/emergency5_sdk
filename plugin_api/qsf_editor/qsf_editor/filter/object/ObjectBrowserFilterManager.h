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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace editor
	{
		class ObjectBrowserFilter;
		class FilterConfiguration;
	}
}


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
		*    Object browser filter manager class
		*
		*  @remarks
		*    The object browser filter manager is automatically able to use all CAMP classes which are derived from "qsf::ObjectBrowserFilter".
		*/
		class QSF_EDITOR_API_EXPORT ObjectBrowserFilterManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			inline explicit ObjectBrowserFilterManager(const FilterConfiguration& filterConfiguration);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ObjectBrowserFilterManager();

			bool acceptsEntity(const QRegExp& filter, const Entity& entity) const;

			inline const FilterConfiguration& getFilterConfiguration() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const FilterConfiguration& mFilterConfiguration;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/filter/object/ObjectBrowserFilterManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ObjectBrowserFilterManager)
