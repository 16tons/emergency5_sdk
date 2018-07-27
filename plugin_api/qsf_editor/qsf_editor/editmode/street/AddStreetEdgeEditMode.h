// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/street/StreetEditMode.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class StreetCrossingComponent;
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
		*    Edit mode for creating an new street edge or expand an existing edge, between to points of the street crossing component
		*
		*  @todo
		*    - TODO(mk) Finish this class, works with street crossing component
		*/
		class AddStreetEdgeEditMode : public StreetEditMode
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::AddStreetEdgeEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit AddStreetEdgeEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AddStreetEdgeEditMode();

			/**
			*  @brief
			*    Give the initial position for creating a street
			*
			*  @param[in] streetCrossingComponent
			*    Street crossing component instance to operate on, can be a null pointer, must stay valid as long as this edit mode operates on the instance
			*/
			void setBeginPosition(const glm::vec3& beginNode, int firstNodeIndex, StreetCrossingComponent* streetCrossingComponent);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void stopDrawing();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					 mFirstNodeInsert;			///< Has a first node already been inserted?
			glm::vec3				 mFirstNodePosition;		///< World space position of the first node
			unsigned int			 mDebugDrawRequestId;		///< Debug draw request ID, "qsf::getUninitialized<unsigned int>()" if invalid
			StreetCrossingComponent* mStreetCrossingComponent;	///< Street crossing component instance to operate on, can be a null pointer, do not destroy the instance
			int						 mFirstNodeIndex;			///< TODO(co) It looks like -1 means "not set", use "qsf::getUninitialized" instead of -1, please, do also document, please or we will lose the control over the code as it grows :/


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::AddStreetEdgeEditMode)
