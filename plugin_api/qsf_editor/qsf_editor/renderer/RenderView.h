// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/renderer/RenderWidget.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroup;
	namespace editor
	{
		class Compass;
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
		*    Render view class
		*/
		class RenderView : public RenderWidget
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
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this widget instance)
			*/
			explicit RenderView(QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~RenderView();


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void mousePressEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent* qMouseEvent) override;
			virtual void dragEnterEvent(QDragEnterEvent* qDragEnterEvent) override;
			virtual void dragLeaveEvent(QDragLeaveEvent* qDragLeaveEvent) override;
			virtual void dropEvent(QDropEvent* qDropEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QObject methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void timerEvent(QTimerEvent* qTimerEvent) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			int		   mUpdateQtTimerId;			///< Update Qt timer ID, "0" means no timer
			QPoint	   mMouseClickInitialPosition;	///< Mouse position at the time a mouse button was pressed down
			bool	   mRightMouseButtonDown;		///< Is the right mouse button currently pressed down?
			Compass&   mCompass;					///< Compass visualization; always valid during RenderView's lifetime


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
