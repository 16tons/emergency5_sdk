// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor/listener/EditorListener.h>

#include <qsf/message/MessageProxy.h>


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
		*    Updates light state of all dynamic lighting in the main map, when the according message gets emitted by the editor
		*/
		class TimeOfDayEditorListener : public qsf::editor::EditorListener
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit TimeOfDayEditorListener(qsf::editor::EditorListenerManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TimeOfDayEditorListener();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onTimeOfDayUpdated(const qsf::MessageParameters& parameters);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::MessageProxy mMessageProxy;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::TimeOfDayEditorListener)
