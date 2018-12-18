// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/widget/PropertiesWidget.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Object;
	namespace editor
	{
		class PropertiesFilterModel;
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
		*    Properties widget class
		*/
		class CustomSetDataHandlingPropertiesWidget : public PropertiesWidget
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
			void propertyValueChanged(const camp::UserObject* campUserObject, const camp::Property* campProperty, const std::string& oldPropertyValue, const std::string& newPropertyValue);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this widget instance)
			*  @param[in] object
			*    Object to operate on, can be a null pointer
			*/
			CustomSetDataHandlingPropertiesWidget(QWidget* qWidgetParent = nullptr, Object* object = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CustomSetDataHandlingPropertiesWidget();


		//[-------------------------------------------------------]
		//[ Virtual protected qsf::editor::PropertiesWidget methods ]
		//[-------------------------------------------------------]
		protected:
			virtual GenericPropertiesModel* createPropertiesModelForObject(PropertiesFilterModel* filterModel, Object* forObject) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
