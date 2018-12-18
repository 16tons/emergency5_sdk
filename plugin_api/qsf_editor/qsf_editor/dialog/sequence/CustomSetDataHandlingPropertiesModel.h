// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/model/properties/GenericPropertiesModel.h"


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
		*    CAMP properties Qt model
		*
		*  @remarks
		*    This is a generic Qt model which shows the properties of any CAMP reflection class.
		*    When properties are changed, a signal is emitted to handle these changes
		*/
		class CustomSetDataHandlingPropertiesModel : public GenericPropertiesModel
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
			*  @param[in] parent
			*    Optional parent, can be a null pointer, in case there's a parent it will take over the (memory) control of this model
			*  @param[in] campUserObject
			*    CAMP user object to list the properties from, must be valid, the instance the user object is wrapping up must stay valid as long as this properties model exist (not the "camp::UserObject"-instance itself)
			*/
			CustomSetDataHandlingPropertiesModel(QObject* parent, const camp::UserObject& campUserObject);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CustomSetDataHandlingPropertiesModel();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::GenericPropertiesModel methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onPropertyValueChanged(const camp::UserObject& campUserObject, const camp::Property& campProperty, const std::string& newPropertyValue) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
