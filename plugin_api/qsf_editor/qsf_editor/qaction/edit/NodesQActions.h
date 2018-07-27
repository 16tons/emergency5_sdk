// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActions.h"


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
		*    Nodes QActions category
		*/
		class NodesQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NodesQActions" unique pluginable QActions ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qActionsManager
			*    The QtActions manager this QtActions instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit NodesQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodesQActions();

			//[-------------------------------------------------------]
			//[ QAction                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the insert node Qt action
			*
			*  @return
			*    The insert node Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getInsertNodeQAction() const;

			/**
			*  @brief
			*    Return the select all nodes Qt action
			*
			*  @return
			*    The select all nodes Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectAllNodesQAction() const;

			/**
			*  @brief
			*    Return the align to center Qt action
			*
			*  @return
			*    The align to center Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getAlignToCenterQAction() const;

			/**
			*  @brief
			*    Return the set center Qt action
			*
			*  @return
			*    The set center Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSetCenterQAction() const;

			//[-------------------------------------------------------]
			//[ Street Crossing QAction                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Add a node to the nearest street gateway
			*/
			inline QAction* getConnectNodeToGatewayQAction() const;

			/**
			*  @brief
			*    Remove a node from street gateway
			*/
			inline QAction* getRemoveNodeFromGatewayQAction() const;

			/**
			*  @brief
			*    Remove a node from street gateway
			*/
			inline QAction* getCreateGatewayQAction() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredSelectNodesAll();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QAction* mInsertNodeQAction;			///< Insert node Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSelectAllNodesQAction;		///< Select all nodes Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mAlignToCenterQAction;			///< Align to center Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSetCenterQAction;				///< Set center Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mConnectNodeToGatewayQAction;	///< Add a node to the nearest street gateway
			QAction* mRemoveNodeFromGatewayQAction;	///< Delete a node from the street gateway
			QAction* mCreateGatewayQAction;			///< Create a street gateway at node position


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
#include "qsf_editor/qaction/edit/NodesQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NodesQActions)
