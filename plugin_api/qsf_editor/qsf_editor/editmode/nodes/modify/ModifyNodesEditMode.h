// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/nodes/NodesEditMode.h"

#include <qsf/math/Plane.h>

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace NodeSelectionManager
	{
		typedef boost::container::flat_set<unsigned int> IdSet;
	}
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class NodeSelectionRectangle;
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
		*    Modify nodes edit mode class
		*/
		class ModifyNodesEditMode : public NodesEditMode
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ModifyNodesEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static Plane getNodesComponentEditingPlane(const NodesComponent& nodesComponent);


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
			explicit ModifyNodesEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ModifyNodesEditMode();

			// TODO(co) What's the difference between "AddNode" and "InsertNode"? What are those methods about in the first place? Why so many possible states?
			bool getAddNode() const;
			void setAddNode(bool value);

			bool getInsertNode() const;
			void setInsertNode(bool value);

			bool getInsertNodeOnSeqment() const;
			void setInsertNodeOnSeqment(bool value);

			bool getDeleteNode() const;
			void setDeleteNode(bool value);

			bool getSplitNode() const;
			void setSplitNode(bool value);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void addNode(const glm::vec2& mousePosition);

			void splitNode();
			void startRectangleDrag(QMouseEvent& qMouseEvent);

			/**
			*  @brief
			*    Update the gizmo
			*/
			void updateGizmo();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseButtonDblClick(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;
			virtual void keyPressEvent(QKeyEvent& qKeyEvent) override;
			virtual void focusInEvent(QFocusEvent& qFocusEvent) override;
			virtual void focusOutEvent(QFocusEvent& qFocusEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ModifyNodesEditMode methods ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Filter node selection
			*
			*  @param[in] selection
			*    Node selection set
			*/
			virtual void nodeSelectionFilter(NodeSelectionManager::IdSet& selection);

			/**
			*  @brief
			*    Insert a node
			*
			*  @param[in] mousePosition
			*    Mouse position inside the render view
			*  @param[in] insertOnSeqment
			*    If True, the point is insert on the nearest seqment, in case of the first or the last node it insert normaly
			*/
			virtual void onInsertNode(const glm::vec2& mousePosition, bool insertOnSeqment = false);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ModifyNodesEditMode Qt slots (MOC) ]
		//[-------------------------------------------------------]
		protected Q_SLOTS:
			virtual void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredDelete();
			void onCustomContextMenuRequested(const QPoint& qPoint);
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
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
			bool					mInsertNode;
			bool					mInsertNodeOnSeqment;
			bool					mDeleteNode;
			bool					mSplitNode;
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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ModifyNodesEditMode)
