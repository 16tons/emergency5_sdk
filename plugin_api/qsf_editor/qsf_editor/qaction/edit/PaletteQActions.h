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
		*    Palette QActions category
		*/
		class PaletteQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::PaletteQActions" unique pluginable QActions ID


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
			explicit PaletteQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PaletteQActions();

			//[-------------------------------------------------------]
			//[ QAction                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the palette color 1 Qt action
			*
			*  @return
			*    The palette color 1 Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPaletteColor1QAction() const;

			/**
			*  @brief
			*    Return the palette color 2 Qt action
			*
			*  @return
			*    The palette color 2 Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPaletteColor2QAction() const;

			/**
			*  @brief
			*    Return the palette color 3 Qt action
			*
			*  @return
			*    The palette color 3 Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPaletteColor3QAction() const;

			/**
			*  @brief
			*    Return the palette color 4 Qt action
			*
			*  @return
			*    The palette color 4 Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPaletteColor4QAction() const;

			/**
			*  @brief
			*    Return the palette color 5 Qt action
			*
			*  @return
			*    The palette color 5 Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPaletteColor5QAction() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QAction* mPaletteColor1QAction;	///< Insert palette Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mPaletteColor2QAction;	///< Insert palette Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mPaletteColor3QAction;	///< Insert palette Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mPaletteColor4QAction;	///< Insert palette Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mPaletteColor5QAction;	///< Insert palette Qt action instance, can be a null pointer, Qt will automatically destroy the instance


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
#include "qsf_editor/qaction/edit/PaletteQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PaletteQActions)
