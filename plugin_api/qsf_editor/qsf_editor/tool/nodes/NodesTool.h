// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/Tool.h"

#include <qsf/reflection/object/ObjectProxy.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Node;
	class Entity;
	class ParameterGroup;
	class NodesComponent;
	namespace editor
	{
		class EditMode;
		class PropertiesWidget;
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
		*    Abstract nodes tool class
		*/
		class NodesTool : public QObject, public Tool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NodesTool" unique pluginable tool ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] toolManager
			*    The tool manager this tool instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] editedComponentId
			*    The ID of the component edited by this nodes tool, "qsf::getUninitialized<unsigned int>()" if invalid
			*/
			explicit NodesTool(ToolManager* toolManager, unsigned int editedComponentId);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodesTool();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(ToolboxView& toolboxView) override;
			virtual void onShutdown(ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::NodesTool methods      ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Called when there's a node change
			*
			*  @param[in] node
			*    Changed node
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void onNodeChange(Node& node);


		//[-------------------------------------------------------]
		//[ Protected Qt slots (MOC)                              ]
		//[-------------------------------------------------------]
		protected Q_SLOTS:
			// qsf::editor::EditModeManager
			virtual void onChangeEditMode(EditMode* newEditMode, EditMode* previousEditMode);
			// qsf::editor::EntitySelectionManager
			virtual void onChangeEntity(Entity* entity);
			// qsf::editor::NodeSelectionManager
			virtual void onChangeNode(unsigned int id);
			// qsf::editor::PropertiesModel
			virtual void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
			// qsf::editor::OperationManager
			virtual void onUndoOperationExecuted(const base::Operation& operation);
			virtual void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Set the inspected nodes component instance
			*
			*  @param[in] nodesComponent
			*    Nodes component instance to inspect, can be a null pointer, must stay valid as long as it's inspected
			*  @param[in] propertiesWidget
			*    Properties widget instance which will be used as nodes component properties view, must stay valid as long as it's inspected
			*/
			void setInspectedNodesComponent(NodesComponent* nodesComponent, PropertiesWidget& propertiesWidget);

			/**
			*  @brief
			*    Select the provided edit mode
			*
			*  @param[in] editMode
			*    Edit mode to select, can be a null pointer
			*  @param[in] checked
			*    Checked state of the UI element belonging to the provided edit mode
			*/
			void selectEditMode(EditMode* editMode, bool checked);

			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

			/**
			*  @brief
			*    Return the ID of the entity selected inside this tool
			*
			*  @return
			*    The ID of the entity selected inside this tool, "qsf::getUninitialized<uint64>()" if invalid
			*/
			uint64 getSelectedEntityId() const;

			/**
			*  @brief
			*    Return the currently used nodes component instance
			*
			*  @return
			*    The currently used nodes component instance, can be a null pointer, do not destroy the instance
			*/
			NodesComponent* getNodesComponent() const;

			/**
			*  @brief
			*    Return the inspected object instance
			*
			*  @return
			*    Currently inspected object, can be a null pointer, do not destroy the instance
			*/
			Object* getInspectedObject() const;

			/**
			*  @brief
			*    Set the inspected object instance
			*
			*  @param[in] object
			*    Object instance to inspect, can be a null pointer, must stay valid as long as it's inspected
			*  @param[in] propertiesWidget
			*    Properties widget instance which will be used as object properties view
			*/
			void setInspectedObject(Object* object, PropertiesWidget& propertiesWidget);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Update the GUI if required
			*
			*  @param[in] operation
			*    The executed operation
			*/
			void updateGuiForOperation(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			unsigned int	  mEditedComponentId;		///< The ID of the component edited by this nodes tool, "qsf::getUninitialized<unsigned int>()" if invalid
			uint64			  mSelectedEntityId;		///< ID of the entity selected inside this tool, "qsf::getUninitialized<uint64>()" if invalid
			NodesComponent*	  mNodesComponent;			///< Nodes component instance to inspect, can be a null pointer, must stay valid as long as it's inspected
			PropertiesWidget* mPropertiesWidget;		///< Properties widget instance which will be used as nodes component properties view, must stay valid as long as it's inspected, can be a null pointer
			ObjectProxy		  mInspectedObjectProxy;	///< Object proxy wrapping up the inspected object, is automatically updated in case the wrapped up object gets destroyed


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NodesTool)
