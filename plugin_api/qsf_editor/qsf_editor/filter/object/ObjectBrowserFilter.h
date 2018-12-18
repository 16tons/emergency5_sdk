// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/plugin/pluginable/Pluginable.h>

#include <QtCore/qregexp.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace editor
	{
		class ObjectBrowserFilterManager;
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
		*    Abstract object browser filter class
		*/
		class QSF_EDITOR_API_EXPORT ObjectBrowserFilter : public Pluginable<ObjectBrowserFilterManager>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class ObjectBrowserFilterManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ObjectBrowserFilter();

			/**
			*  @brief
			*    Return the object browser filter manager
			*
			*  @return
			*    The object browser filter manager, do not destroy the instance
			*/
			inline ObjectBrowserFilterManager& getObjectBrowserFilterManager() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::ObjectBrowserFilter methods    ]
		//[-------------------------------------------------------]
		protected:
			virtual bool acceptsEntity(const QRegExp& filter, const Entity& entity) = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] objectBrowserFilterManager
			*    The object browser filter manager this prototype fabricator is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			inline explicit ObjectBrowserFilter(ObjectBrowserFilterManager* objectBrowserFilterManager);


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
#include "qsf_editor/filter/object/ObjectBrowserFilter-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ObjectBrowserFilter)
