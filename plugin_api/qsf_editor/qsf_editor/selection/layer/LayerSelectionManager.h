// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/selection/SelectionManager.h>

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
	class Layer;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
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
		*    QSF editor layer selection manager class
		*
		*  @note
		*    - The currently selected layer can only be changed by using the "qsf::editor::SelectLayerOperation"-operation
		*    - We need to inherit from selection publically otherwise compiler error: qsf/plugin/pluginable/PluginableManager-inl.h:38:61: error: ‘qsf::PluginableBase’ is an inaccessible base of ‘qsf::editor::SelectionManager’.
		*      Due private inheritance the information of that base class gets "lost" for the outer world.
		*/
		class QSF_EDITOR_API_EXPORT LayerSelectionManager : public QObject, public SelectionManager
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			template <typename T>
				friend class camp::ClassBuilder;
			friend class SelectLayerOperation;


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::LayerSelectionManager" unique pluginable selection manager ID


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted after an currently selected layer change has occurred
			*/
			void selectedLayerChanged();


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
			explicit LayerSelectionManager(qsf::SelectionSystem* selectionSystem);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LayerSelectionManager();

			/**
			*  @brief
			*    Return the ID of the currently selected layer
			*
			*  @return
			*    The ID of the currently selected layer, there's always a selected layer (e.g. ID 0 of the base layer)
			*/
			unsigned int getSelectedId() const;

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
			*    Return the currently selected layer
			*
			*  @return
			*    The currently selected layer, can be a null pointer, do not destroy the instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			Layer* getSelectedLayer() const;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Set the currently selected layer by using its ID
			*
			*  @param[in] id
			*    ID of the layer to select
			*
			*  @return
			*    "true" if all went fine, else "false" if the currently selected layer was not changed (e.g. layer ID is unknown)
			*
			*  @note
			*    - Only accessed by "qsf::editor::SelectLayerOperation" so operation listeners receive a valid
			*      result when asking "qsf::editor::LayerSelection" for the currently selected layer
			*/
			bool setSelectedLayerById(unsigned int id);

			/**
			*  @brief
			*    Call this method in case a layer has been destroyed
			*
			*  @param[in] layerId
			*    ID of the destroyed layer
			*
			*  @note
			*    - This method only exists for safety, usually "qsf::editor::LayerOperationHelper::buildDestroyLayerOperation" is
			*      used to destroy layers which takes care of switching the currently selected layer if required
			*/
			void layerDestroyed(unsigned int layerId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			unsigned int mSelectedLayerId;	///< ID of the currently selected layer, there's always a selected layer (e.g. ID 0 of the base layer)


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::LayerSelectionManager)
