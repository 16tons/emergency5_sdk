// Copyright (C) 2012-2018 Promotion Software GmbH


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
		*    Terrain QActions category
		*/
		class TerrainQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainQActions" unique pluginable QActions ID


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
			explicit TerrainQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainQActions();

			//[-------------------------------------------------------]
			//[ Modeling QAction                                      ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the modeling raise Qt action
			*
			*  @return
			*    The modeling raise Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getModelingRaiseQAction() const;

			/**
			*  @brief
			*    Return the modeling smooth Qt action
			*
			*  @return
			*    The modeling smooth Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getModelingSmoothQAction() const;

			/**
			*  @brief
			*    Return the modeling set Qt action
			*
			*  @return
			*    The modeling set Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getModelingSetQAction() const;

			/**
			*  @brief
			*    Return the modeling flatten Qt action
			*
			*  @return
			*    The modeling flatten Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getModelingFlattenQAction() const;

			//[-------------------------------------------------------]
			//[ Painting QAction                                      ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the painting color Qt action
			*
			*  @return
			*    The painting color Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPaintingColorQAction() const;

			/**
			*  @brief
			*    Return the painting cave Qt action
			*
			*  @return
			*    The painting cave Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPaintingCaveQAction() const;

			//[-------------------------------------------------------]
			//[ Texturing QAction                                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the texturing paint Qt action
			*
			*  @return
			*    The texturing paint Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getTexturingPaintQAction() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Modeling
			QAction* mModelingRaiseQAction;		///< Modeling raise Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mModelingSmoothQAction;	///< Modeling smooth Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mModelingSetQAction;		///< Modeling set Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mModelingFlattenQAction;	///< Modeling flatten Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			// Painting
			QAction* mPaintingColorQAction;		///< Painting color Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mPaintingCaveQAction;		///< Painting cave Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			// Texturing
			QAction* mTexturingPaintQAction;	///< Texturing paint Qt action instance, can be a null pointer, Qt will automatically destroy the instance


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
#include "qsf_editor/qaction/edit/TerrainQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainQActions)
