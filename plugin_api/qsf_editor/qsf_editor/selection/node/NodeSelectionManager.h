// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/selection/SelectionManagerTemplate.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	template <typename T>
		class ClassBuilder;
}
namespace qsf
{
	class NodesComponent;
	namespace editor
	{
		class NodeSelectionMarkerManager;
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
		*    QSF editor node selection manager class
		*
		*  @note
		*    - The currently selected nodes can only be changed by using the "qsf::editor::SelectNodeOperation",
		*      "qsf::editor::AddNodesToSelectionOperation" and "qsf::editor::RemoveNodesFromSelectionOperation" operations
		*    - We need to inherit from qsf::EntitySelectionManager publically otherwise compiler error: qsf/plugin/pluginable/PluginableManager-inl.h:38:61: error: ‘qsf::PluginableBase’ is an inaccessible base of ‘qsf::editor::NodeSelectionManager’
		*      due private inheritance the information of that base class gets "lost" for the outer world
		*
		*  @todo
		*    - TODO(co) Add support for visual highlighting
		*/
		class QSF_EDITOR_API_EXPORT NodeSelectionManager : public QObject, public SelectionManagerTemplate<unsigned int>
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
			friend class SelectNodeOperation;
			friend class NodeSelectionMarkerManager;
			friend class AddNodesToSelectionOperation;
			friend class RemoveNodesFromSelectionOperation;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NodeSelectionManager" unique pluginable selection manager ID

			typedef boost::container::flat_set<unsigned int> IdSet;	///< Node ID set


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted after an selection change has occurred
			*
			*  @param[in] id
			*    The ID of the new selected node, "qsf::getUninitialized<unsigned int>()" is there's no current ID
			*/
			void change(unsigned int id);


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
			explicit NodeSelectionManager(qsf::SelectionSystem* selectionSystem);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodeSelectionManager();

			/**
			*  @brief
			*    Return whether or not the node selection marker is enabled
			*
			*  @return
			*    "true" if the node selection marker is enabled, else "false"
			*/
			bool isNodeSelectionMarkerEnabled() const;

			/**
			*  @brief
			*    Set whether or not the node selection marker is enabled
			*
			*  @param[in] nodeSelectionMarkerEnabled
			*    "true" if the node selection marker is enabled, else "false"
			*/
			void setNodeSelectionMarkerEnabled(bool nodeSelectionMarkerEnabled);

			/**
			*  @brief
			*    Return the used nodes component instance
			*
			*  @return
			*    The used nodes component instance, null pointer on error, do not destroy the instance
			*/
			NodesComponent* getNodesComponent() const;

			/**
			*  @brief
			*    Return the local space position of the current node
			*
			*  @return
			*    The local space position of the current node, null position on error
			*/
			glm::vec3 getCurrentNodePosition() const;

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
			unsigned int getSelectedId() const;

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
			bool isIdSelected(unsigned int id) const;

			/**
			*  @brief
			*    Return the current ID
			*
			*  @return
			*    The current ID, "qsf::getUninitialized<unsigned int>()" is there's no current ID
			*
			*  @remarks
			*    Usually the current ID is equal to the last selected ID. It's possible to explicitly set this current
			*    ID, although it's recommended to not do so if it can be avoided. Do also don't use the information about
			*    the current ID if you don't have to and use the general selected ID set whenever possible.
			*/
			unsigned int getCurrentId() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::SelectionManagerTemplate methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onCurrentChange(unsigned int id) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			NodeSelectionMarkerManager* mNodeSelectionMarkerManager;	///< Node selection marker manager instance, can be a null pointer, destroy the instance if you no longer need it


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NodeSelectionManager)
