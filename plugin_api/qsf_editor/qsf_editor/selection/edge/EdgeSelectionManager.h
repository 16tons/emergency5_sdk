// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/component/nodes/Node.h>
#include <qsf/component/street/crossing/StreetEdge.h>
#include <qsf/selection/SelectionManagerTemplate.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <QtCore/qobject.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
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
		*    QSF editor edge edge manager class
		*/
		// We need to inherit from qsf::EntitySelectionManager publically otherwise compiler error: qsf/plugin/pluginable/PluginableManager-inl.h:38:61: error: ‘qsf::PluginableBase’ is an inaccessible base of ‘qsf::editor::EdgeSelectionManager’
		// Due private inheritance the information of that base class gets "lost" for the outer world
		class QSF_EDITOR_API_EXPORT EdgeSelectionManager : public QObject, public SelectionManagerTemplate<uint32>
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			template <typename T>
				friend class camp::ClassBuilder;
			friend class SelectEdgeOperation;
			friend class UnselectEdgeOperation;
			friend class AddEdgesToSelectionOperation;
			friend class RemoveEdgesFromSelectionOperation;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::EdgeSelectionManager" unique pluginable selection manager ID

			typedef boost::container::flat_set<uint32> IdSet;			///< Edge ID set


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] selectionSystem
			*    The QSF selection system this selection manager instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit EdgeSelectionManager(qsf::SelectionSystem* selectionSystem);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EdgeSelectionManager();

			//[-------------------------------------------------------]
			//[ Mirrored qsf::SelectionManagerTemplate getter methods ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the first selected ID
			*
			*  @return
			*    The selected ID, in case of multiple selected IDs the first one will be returned
			*/
			uint32 getSelectedId() const;

			/**
			*  @brief
			*    Return whole selection as set of IDs
			*
			*  @return
			*    Whole selection as set of IDs, do not destroy the instance
			*/
			const IdSet& getSelectedIdSet() const;

			/**
			*  @brief
			*    Return the number of currently selected IDs
			*
			*  @return
			*    The number of currently selected IDs
			*/
			size_t getSelectionSize() const;

			/**
			*  @brief
			*    Check whether or not the given ID is currently selected
			*
			*  @return
			*    "true" in case the given ID is currently selected, else "false"
			*/
			bool isIdSelected(uint32 id) const;

			/**
			*  @brief
			*    Return the current ID
			*
			*  @return
			*    The current ID, "qsf::getUninitialized<uint32>()" is there's no current ID
			*
			*  @remarks
			*    Usually the current ID is equal to the last selected ID. It's possible to explicitly set this current
			*    ID, although it's recommended to not do so if it can be avoided. Do also don't use the information about
			*    the current ID if you don't have to and use the general selected ID set whenever possible.
			*/
			uint32 getCurrentId() const;

			//[-------------------------------------------------------]
			//[ Edge                                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the current street edge
			*/
			const StreetEdge* getCurrentEdge() const;

			/**
			*  @brief
			*    Add IDs to the current selection
			*
			*  @param[in] idSet
			*    ID to add to the current selection, already selected IDs will be deselected
			*/
			void addIdToSelection(uint32 id);

			/**
			*  @brief
			*    Remove an ID from the current selection
			*
			*  @param[in] id
			*    ID to remove from the current selection, not selected IDs or "qsf::getUninitialized<IdType>()" are silently ignored
			*/
			void removeIdFromSelection(uint32 id);

			/**
			*  @brief
			*    Clear the current selection
			*/
			void clearSelection();

			/**
			*  @brief
			*    Set the highlighted edge
			*
			*  @param[in] entityId
			*    The ID of the edge to highlight, "qsf::getUninitialized<uint64><>()" to highlight nothing
			*/
			void setHighlightedEdgeById(uint32 id);

			/**
			*  @brief
			*    Return the highlighted ID
			*
			*  @return
			*    The highlighted ID
			*/
			uint32 getHighlightedId() const;

			/**
			*  @brief
			*    Check whether or not the given ID is currently highlighted
			*
			*  @return
			*    "true" in case the given ID is currently selected, else "false"
			*/
			bool isIdHighlighted(uint32 id) const;

			/**
			*  @brief
			*    Getter to our nodes and edges
			*
			*  @return
			*    The vector of nodes/edges
			*/
			inline const std::vector<StreetEdge>& getEdges() const;
			inline const std::vector<Node>& getNodes() const;

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			bool onStartup(Entity* entity, const std::vector<StreetEdge>* edges, const std::vector<Node>* nodes);
			bool onShutdown();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateSelectionDebugDraw();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Entity*							mEntity;
			const std::vector<StreetEdge>*	mEdges;
			const std::vector<Node>*		mNodes;
			uint32							mHighlightedEdge;

			// Visualization
			DebugDrawProxy					mDebugDrawProxySelection;
			DebugDrawProxy					mDebugDrawProxyHighlight;


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
#include "qsf_editor/selection/edge/EdgeSelectionManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::EdgeSelectionManager)
