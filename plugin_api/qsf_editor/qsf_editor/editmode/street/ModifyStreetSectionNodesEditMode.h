// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/nodes/modify/ModifyNodesEditMode.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace NodeSelectionManager
	{
		typedef boost::container::flat_set<unsigned int> IdSet;
	}
	class StreetSectionComponent;
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
		*    Modify street section nodes edit mode class
		*/
		class ModifyStreetSectionNodesEditMode : public ModifyNodesEditMode
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ModifyStreetSectionNodesEditMode" unique pluginable edit mode ID


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
			explicit ModifyStreetSectionNodesEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ModifyStreetSectionNodesEditMode();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ModifyNodesEditMode methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void nodeSelectionFilter(NodeSelectionManager::IdSet& selection) override;
			virtual void onInsertNode(const glm::vec2& mousePosition, bool insertOnSeqment = false) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ModifyNodesEditMode Qt slots (MOC) ]
		//[-------------------------------------------------------]
		protected Q_SLOTS:
			virtual void onRedoOperationExecuted(const base::Operation& operation) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredDelete();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void createDrawRequest();
			void clearDrawRequest();

			/**
			*  @brief
			*    Get the street section component
			*/
			StreetSectionComponent* getStreetSectionComponent() const;

			/**
			*  @brief
			*    Get tier of a node
			*
			*  @param[in] nodeIndex
			*    From which node we want to know the tier
			*  @param[in] nodeCount
			*    The number of nodes
			*
			*  @return
			*    Tier of the node
			*/
			int getTier(int nodeIndex, int nodeCount) const;

			/**
			*  @brief
			*    Get node selectable
			*
			*  @param[in] nodeIndex
			*    From which node we want to know whether or not it's selectable
			*  @param[in] nodeCount
			*    The number of nodes
			*
			*  @return
			*    "true" if the node is selectable, else "false"
			*/
			bool isNodeSelectable(int nodeIndex, int nodeCount) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<unsigned int>	mDebugDrawRequestIds;	///< Debug draw request IDs


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ModifyStreetSectionNodesEditMode)
