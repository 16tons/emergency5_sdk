// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/reflection/type/CampGlmQuat.h>
#include <qsf/asset/AssetSystemTypes.h>

#include <QtCore/qlist.h>
#include <QtCore/qnamespace.h>

#include <boost/noncopyable.hpp>

#include <limits>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMenu;
	class QImage;
	class QAction;
	class QMenuBar;
	class QComboBox;
QT_END_NAMESPACE
namespace camp
{
	class Property;
	class UserObject;
}
namespace qsf
{
	class Entity;
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
		*    Static helper class, containing functions used by the GUI or with Qt
		*/
		class QSF_EDITOR_API_EXPORT GuiHelper : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Get a combo box's best matching item index for a given float value.
			*
			*  @param[in] qComboBox
			*    The Qt combo box; every item must have a float user data value assigned
			*  @param[in] value
			*    The value that has to be matched
			*  @param[in] tolerance
			*    A maximum tolerance for the comparison of an item's user data value with the given value; may be 0.0f to allow only exact matches
			*
			*  @return
			*    The index, beginning with 0 for the first item; -1 is returned if no item was matched (e.g. when tolerance was too strict)
			*/
			static int getMatchingComboBoxIndexForItemValue(const QComboBox& qComboBox, float value, float tolerance = std::numeric_limits<float>::infinity());

			/**
			*  @brief
			*    Open operation system explorer with given path
			*
			*  @param[in] path
			*    Path to show inside the operation system explorer
			*
			*  @note
			*    - Unlike "QDesktopServices::openUrl()", in case of a file given this file will also be automatically selected inside the operation system explorer
			*/
			static void openExplorer(const QString& path);

			/**
			*  @brief
			*    Add a QMenu instance to a parent QMenuBar which takes over the ownership
			*
			*  @param[in] title
			*    QMenu title
			*  @param[in] qMenuBarParent
			*    Parent QMenuBar which takes over the ownership
			*
			*  @return
			*    The created QMenu instance, don't destroy it
			*/
			static QMenu* addMenu(const QString& title, QMenuBar& qMenuBarParent);
			static QMenu* addMenu(const QString& title, QMenu& qMenuParent);

			//[-------------------------------------------------------]
			//[ Asset system                                          ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Fill the given Qt combo box with available asset types
			*
			*  @param[in] qComboBox
			*    The Qt combo box to be filled, content is not cleared before new entries are added
			*  @param[in] assetTypeId
			*    The ID of the asset type to directly select, "qsf::getUninitialized<AssetTypeId>()" to not change the selection
			*  @param[in] sort
			*    Sort the content of the combo box?
			*/
			static void fillQComboBoxWithAssetTypes(QComboBox& qComboBox, bool sort = true);

			/**
			*  @brief
			*    Use a global asset ID and generate a text used e.g. inside tool tips
			*
			*  @param[in] globalAssetId
			*    Global asset ID which should be represented as text used e.g. inside tool tips
			*
			*  @return
			*    The generated text
			*/
			static QString globalAssetIdToText(GlobalAssetId globalAssetId);

			//[-------------------------------------------------------]
			//[ Reflection system                                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get a CAMP property value as human readable string
			*
			*  @param[in] campUserObject
			*    CAMP user object to use
			*  @param[in] campProperty
			*    CAMP property to return the value as human readable string from
			*
			*  @return
			*    CAMP property value as human readable string
			*/
			static QString getCampPropertyValueAsGuiString(const camp::UserObject& campUserObject, const camp::Property& campProperty);

			/**
			*  @brief
			*    Get a CAMP property value as even more human readable string
			*
			*  @param[in] campUserObject
			*    CAMP user object to use
			*  @param[in] campProperty
			*    CAMP property to return the value as human readable string from
			*
			*  @return
			*    CAMP property value as even more human readable string
			*/
			static QString getCampPropertyValueAsGuiDisplayString(const camp::UserObject& campUserObject, const camp::Property& campProperty);

			/**
			*  @brief
			*    Get a CAMP property value as QtVariant
			*
			*  @param[in] campUserObject
			*    CAMP user object to use
			*  @param[in] campProperty
			*    CAMP property to return the value as human readable string from
			*
			*  @return
			*    CAMP property value as QtVariant
			*/
			static QVariant getCampPropertyValueAsQtVariant(const camp::UserObject& campUserObject, const camp::Property& campProperty);

			/**
			*  @brief
			*    Get a CAMP property value as string by using a given human readable string
			*
			*  @param[in] guiString
			*    Human readable string
			*  @param[in] campUserObject
			*    CAMP user object to use
			*  @param[in] campProperty
			*    CAMP property to return the value from
			*
			*  @return
			*    CAMP property value as string
			*/
			static std::string guiStringToCampPropertyValueString(const QString& guiString, const camp::UserObject& campUserObject, const camp::Property& campProperty);

			//[-------------------------------------------------------]
			//[ Text formating                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return true if both value are equal enough with the float precision set in the editor settings
			*/
			static bool checkEqualityInFloatPrecision(float value1, float value2);

			/**
			*  @brief
			*    Convert UTC to local time
			*/
			static QString convertUtcToLocal(const std::string& utcTimeString);

			//[-------------------------------------------------------]
			//[ Texture helper                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Load QImage content by using a given global asset ID
			*/
			static bool loadQImageFromGlobalAssetId(QImage& qImage, GlobalAssetId globalAssetId);

			//[-------------------------------------------------------]
			//[ Sort                                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Sort a given QAction list alphabetically
			*/
			static void sortQActionListAlphabetically(QList<QAction*>& qActions);

			//[-------------------------------------------------------]
			//[ Entity helper                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Returns the object name of the entity (with all parent layers in which the entity is located in
			*/
			// TODO(sw) Is this the right place for this?
			static std::string getObjectName(const Entity& entity);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
