// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qdialog.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class AddPropertyDialog;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		class AddPropertyDialog : public QDialog
		{
			Q_OBJECT

		public:
			explicit AddPropertyDialog(QWidget* parent = nullptr);
			virtual ~AddPropertyDialog();

			int mType;
			QString mName;

		public Q_SLOTS:
			void accepted();
			void rejected();

		private:
			Ui::AddPropertyDialog* ui;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
