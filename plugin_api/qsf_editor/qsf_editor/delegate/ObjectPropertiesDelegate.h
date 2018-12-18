// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <QtWidgets/qstyleditemdelegate.h>

#include <boost/property_tree/ptree.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Object;
	namespace editor
	{
		class BaseModel;
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
		*    The delegate for using the specific display of the different values from the object properties view
		*
		*  @remarks
		*    Uses the Qt model/view principle to show a data model with a view. The delegate is a extra layer on the view
		*    for displaying different via own input style, like colors via color dialog, filenames via file dialog, etc.
		*    See Qt documentation for more information http://qt-project.org/doc/qt-4.8/model-view-programming.html#delegate-classes
		*    We derived from QStyledItemDelegate because it has more functionality see for more information http://qt-project.org/doc/qt-4.8/qstyleditemdelegate.html#qstyleditemdelegate-vs-qitemdelegate
		*
		*    We check the different property values for a style tag and display it via an extra "editor"-widget.
		*    We can use our own painting style by creating a "QStyleOptionViewItem" class.
		*
		*    We have two ways for editing and display the values:
		*    1. Create a editor widget and handle everything inside of this extra widget, use createEditor, setEditorData, setModelData
		*    2. Use paint/editorEvent function to display the values directly, handle input in editorEvent function
		*
		*    Workflow from 1.
		*        - createEditor() creates a widget for visualisation
		*        - setEditorData() sets the initial data
		*        - setModelData() gives the final value to the model
		*
		*  @todo
		*   - TODO(co) This class starts to get huge and therefore hard to maintain. It might be a good idea to write individual delegates as seen e.g. inside http://subversion.assembla.com/svn/neobot/trunk/Informatique/PC/Libraries/WidgetToolsLib/SpinBoxDelegate.cpp
		*/
		class QSF_EDITOR_API_EXPORT ObjectPropertiesDelegate : public QStyledItemDelegate
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ObjectPropertiesDelegate" unique pluginable view ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this delegate instance)
			*/
			explicit ObjectPropertiesDelegate(QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ObjectPropertiesDelegate();


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemDelegate methods          ]
		//[-------------------------------------------------------]
		public:
			virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
			virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
			virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
			virtual void destroyEditor(QWidget* editor, const QModelIndex& index) const override;
			virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
			virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
			virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
			virtual bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onValueChanged(int value);
			void onValueChanged(double value);
			void onValueChanged(glm::vec3 value);
			void onCurrentIndexChanged(int index);
			void onCurrentColorChanged(const QColor& color);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Display style
			*/
			enum DisplayStyle
			{
				DEFAULT_DISPLAY_STYLE,			///< Fallback to string representation for unknown data types and unknown semantics
				SPIN_BOX_DISPLAY_STYLE,			///< Data type is "integer" - User can write values in text field (only numbers accepted) and have upper/lower buttons on the right side
				DOUBLE_SPIN_BOX_DISPLAY_STYLE,	///< Data type is "real" - User can write values in text field (only floating point numbers accepted) and have upper/lower buttons on the right side
				CHECK_BOX_DISPLAY_STYLE,		///< Data type is "boolean" - Check box can be checked or unchecked
				ENUM_DISPLAY_STYLE,				///< Data type is "enumeration" - Combo box with all enum as type
				COLOR_DISPLAY_STYLE,			///< Data type is "qsf::Color3" or "qsf::Color4" - Opens a color picker dialog
				ASSET_DISPLAY_STYLE,			///< Data type is "qsf::AssetProxy" - Opens the asset browser dialog
				ASSET_TYPE_DISPLAY_STYLE,		///< Data type is "qsf::AssetTypeId" - Opens the asset browser dialog
				FILE_DISPLAY_STYLE,				///< Semantic: "Filename" - Opens a file browser dialog
				VECTOR2_DISPLAY_STYLE,			///< Data type is "glm::vec2"
				VECTOR3_DISPLAY_STYLE,			///< Data type is "glm::vec3"
				VECTOR4_DISPLAY_STYLE,			///< Data type is "glm::vec4"
				QUAT_DISPLAY_STYLE,				///< Data type is "glm::quat"
				TIME_DISPLAY_STYLE,				///< Semantic: "Time"
				QSF_AI_UNSIGNEDFLOAT_STYLE,		///< Semantic: "UnsignedFloat" has a special way of displaying in case it is negative
				QSF_AI_PERCENTAGE_STYLE,		///< Special data type for editing a percentage between 0 and 1, data type used inside the AI settings
				DYNAMIC_ENUM_DISPLAY_STYLE
			};


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			DisplayStyle getDisplayStyleFromIndex(const QModelIndex& index) const;
			void getCampPropertyAndObjectParentFromIndex(const QModelIndex& sourceIndex, const camp::Property** campProperty, Object** objectOut = nullptr) const;
			void restorePreviousCampPropertyValue(const QModelIndex& index) const;

			/**
			*  @brief
			*    Checks if the event is a mouse button released event and it isn't for the right mouse button
			*    Additional it checks if the mouse position is inside the area given by the style option view item
			*/
			bool isMouseButtonReleasedIgnoringRightMouseButton(QEvent* event, const QStyleOptionViewItem& option);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			mutable QModelIndex					mWorkingIndex;
			mutable boost::property_tree::ptree mPreviousCampValue;	///< Previously set CAMP value as boost property tree
			mutable bool						mPreviousCampValueAvailable;
			mutable bool						mChangingValueFromEditor;	// Indicates if we set a value to the model on value change of the editor (In this case ignore model data changes)


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
