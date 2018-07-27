// Copyright (C) 2012-2017 Promotion Software GmbH


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
		*/
		class PropertiesModel : public GenericPropertiesModel
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


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
			*  @param[in] supportEntityMultiSelection
			*    Support entity multiSelection?
			*/
			PropertiesModel(QObject* parent, const camp::UserObject& campUserObject, bool supportEntityMultiSelection = true);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PropertiesModel();

			/**
			*  @brief
			*    Set the modified flag to the specified state
			*
			*  @param[in] isModified
			*    New modified flag state
			*/
			void setModified(bool isModified);

			/**
			*  @brief
			*    Return the current modified flag state
			*
			*  @return
			*    The current modified flag state
			*/
			bool isModified() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::GenericPropertiesModel methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onPropertyValueChanged(const camp::UserObject& campUserObject, const camp::Property& campProperty, const std::string& newPropertyValue) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mSupportEntityMultiSelection;
			bool mIsModified;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
