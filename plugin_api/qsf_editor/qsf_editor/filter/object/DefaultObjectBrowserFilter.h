// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/filter/object/ObjectBrowserFilter.h"


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
		*    Default object browser filter class
		*/
		class DefaultObjectBrowserFilter : public ObjectBrowserFilter
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::DefaultObjectBrowserFilter" unique pluginable prototype fabricator ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] objectBrowserFilterManager
			*    The object browser filter manager this prototype fabricator is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit DefaultObjectBrowserFilter(ObjectBrowserFilterManager* objectBrowserFilterManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DefaultObjectBrowserFilter();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::ObjectBrowserFilter methods    ]
		//[-------------------------------------------------------]
		protected:
			virtual bool acceptsEntity(const QRegExp& filter, const Entity& entity) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::DefaultObjectBrowserFilter)