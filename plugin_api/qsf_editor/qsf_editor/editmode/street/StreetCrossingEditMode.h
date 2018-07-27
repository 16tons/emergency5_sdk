// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/street/StreetEditMode.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class NodeSelectionRectangle;
	}
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
		*    Modify street crossing edit mode class
		*/
		class StreetCrossingEditMode : public StreetEditMode
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			// TODO(co) Looks like a somewhat poor solution. "Usually" we have selection managers for this purpose. We might want to add an edge selection manager.
			/**
			*  @brief
			*    Qt signal emitted after an edge selection change has occurred
			*
			*  @param[in] id
			*    The ID of the new selected edge, "qsf::getUninitialized<unsigned int>()" is there's no current ID
			*/
			void edgeSelectionChange(unsigned int id);


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::StreetCrossingEditMode" unique pluginable edit mode ID


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
			explicit StreetCrossingEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~StreetCrossingEditMode();

			bool getAddNode() const;
			void setAddNode(bool value);

			bool getInsertNode() const;
			void setInsertNode(bool value);

			bool getDeleteNode() const;
			void setDeleteNode(bool value);

			bool getAddOrExpandEdge() const;
			void setAddOrExpandEdge(bool value);

			StreetCrossingComponent* getStreetCrossingComponent() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;
			virtual void keyPressEvent(QKeyEvent& qKeyEvent) override;
			virtual void focusInEvent(QFocusEvent& qFocusEvent) override;
			virtual void focusOutEvent(QFocusEvent& qFocusEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onTriggeredDelete();
			void onTriggeredAddEdge(const QPoint& mousePosition);
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Insert a node
			*
			*  @param[in] mousePosition
			*    Mouse position inside the render view
			*/
			void onInsertNode(const glm::vec2& mousePosition);

			/**
			*  @brief
			*    Simply add a node
			*
			*  @param[in] mousePosition
			*    Mouse position inside the render view
			*/
			void addNode(const glm::vec2& mousePosition);

			/**
			*  @brief
			*    Insert a centered node on the lane
			*
			*  @param[in] mousePosition
			*    Mouse position inside the render view
			*/
			void addCenteredNode(const glm::vec2& mousePosition);

			/**
			*  @brief
			*    Update the gizmo
			*/
			void updateGizmo();

			/**
			*  @brief
			*    Reset the all flags for the different edit mode states
			*/
			void resetFlags();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					mAddNode;
			bool					mAddOrExpandEdge;
			bool					mDeleteNode;
			NodeSelectionRectangle* mSelectionRectangle;	///< Selection rectangle, can be a null pointer, we're responsible for destroying it


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::StreetCrossingEditMode)
