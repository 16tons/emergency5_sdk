// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor/filter/object/ObjectBrowserFilter.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Event tag object browser filter class
		*/
		class EventTagObjectBrowserFilter : public qsf::editor::ObjectBrowserFilter
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "em5::editor::EventTagObjectBrowserFilter" unique pluginable prototype fabricator ID


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
			explicit EventTagObjectBrowserFilter(qsf::editor::ObjectBrowserFilterManager* objectBrowserFilterManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EventTagObjectBrowserFilter();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::ObjectBrowserFilter methods    ]
		//[-------------------------------------------------------]
		protected:
			virtual bool acceptsEntity(const QRegExp& filter, const qsf::Entity& entity) override;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::EventTagObjectBrowserFilter)
