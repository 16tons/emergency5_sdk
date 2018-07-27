// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <glm/vec3.hpp>

#include <QtWidgets/qwidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Property;
}

QT_BEGIN_NAMESPACE
	class QDoubleSpinBox;
QT_END_NAMESPACE


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
		*    Editor widget for editing the thre components of an glm::vec3 individually
		*
		*  @note
		*    When the camp property has no StepSize tag set it defaults to a step size of 0.01
		*/
		class GlmVec3Editor : public QWidget
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			void valueChanged(glm::vec3 newValue);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit GlmVec3Editor(QWidget* parentWidget, const camp::Property* campProperty);
			void setValue(const glm::vec3& data);
			const glm::vec3& getValue() const;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onVec3XValueChanged(double value);
			void onVec3YValueChanged(double value);
			void onVec3ZValueChanged(double value);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			QDoubleSpinBox* createSpinbox(const camp::Property* campProperty);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QDoubleSpinBox* mVec3XEditor;
			QDoubleSpinBox* mVec3YEditor;
			QDoubleSpinBox* mVec3ZEditor;
			glm::vec3 mCurrentValue;
			bool mChangedInternal;

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
