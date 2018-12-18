// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/view/developer/DeveloperView.h"

#include <qsf/localization/LocalizationMessage.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QTreeWidgetItem;
QT_END_NAMESPACE
namespace Ui
{
	class LocalizationView;
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
		*    Localization view class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*
		*  @todo
		*    - TODO(co) According to plans there will be no localization view in the first public released QSF editor version, that's what this view is marked as developer view
		*/
		class LocalizationView : public DeveloperView
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::LocalizationView" unique pluginable view ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] viewManager
			*    Optional pointer to the view manager this view should be registered to, can be a null pointer
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this view instance)
			*/
			LocalizationView(ViewManager* viewManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LocalizationView();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Select a localization context
			*
			*  @param[in] localizationContext
			*    Localization context to select, can be a null pointer
			*/
			void selectContext(LocalizationContext* localizationContext);

			/**
			*  @brief
			*    Save the currently selected localization context, but only if there's currently a selected localization context
			*/
			void saveSelectContext() const;

			/**
			*  @brief
			*    Set the translation type of the currently selected localization message
			*
			*  @param[in] row
			*    Row of the localization message inside the tree view
			*  @param[in] translationType
			*    New translation type
			*/
			void setMessageTranslationType(int row, LocalizationMessage::TRANSLATION_TYPE translationType) const;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onClickedFile(const QModelIndex&);
			void onClickedMessage(int row, int column);
			void onTranslationChanged();
			void onTranslatorCommentChanged();
			void onTranslationStateChanged(bool checked);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::LocalizationView* mUiLocalizationView;	///< UI localization instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			LocalizationContext*  mSelectedContext;		///< Pointer to the user selected localization context, can be a null pointer, do not delete the instance
			LocalizationMessage*  mSelectedMessage;		///< Pointer to the user selected localization message, can be a null pointer, do not delete the instance


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::LocalizationView)
