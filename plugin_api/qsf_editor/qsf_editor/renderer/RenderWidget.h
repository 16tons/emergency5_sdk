// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qwidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class InputButton;
	class InputSystem;
	class RenderWindow;
	namespace editor
	{
		class MainWindow;
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
		*    Render widget class, connection between "QWidget" and "qsf::RenderWindow"
		*/
		class RenderWidget : public QWidget
		{


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
			explicit RenderWidget(QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~RenderWidget();

			/**
			*  @brief
			*    In case this widget had the input focus, lose the input focus
			*
			*  @return
			*    "true" in case the widget had the input focus, else "false"
			*/
			bool loseInputFocus();

			/**
			*  @brief
			*    Give this widget the input focus, if it had it before
			*
			*  @note
			*    - Do only call this method if you called "loseInputFocus()" some time ago
			*/
			void restoreInputFocus();

			/**
			*  @brief
			*    Return the main window
			*
			*  @return
			*    The main window, do no destroy the returned instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			MainWindow& getMainWindow() const;

			/**
			*  @brief
			*    Set the mouse cursor visibility
			*
			*  @param[in] visible
			*    "true" if the mouse cursor should be visible, else "false"
			*
			*  @note
			*    - Also backups/restores the cursor position
			*/
			void setMouseCursorVisible(bool visible);

			//[-------------------------------------------------------]
			//[ Internal                                              ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the encapsulated QSF render window instance
			*
			*  @return
			*    The encapsulated QSF render window instance, do no destroy the returned instance
			*
			*  @note
			*    - Don't access the encapsulated QSF render window instance if you don't have to
			*/
			inline RenderWindow& getRenderWindow() const;


		//[-------------------------------------------------------]
		//[ Protected virtual QPaintDevice methods                ]
		//[-------------------------------------------------------]
		protected:
			virtual QPaintEngine* paintEngine() const override;


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void focusInEvent(QFocusEvent* qFocusEvent) override;
			virtual void focusOutEvent(QFocusEvent* qFocusEvent) override;
			virtual void paintEvent(QPaintEvent* qPaintEvent) override;
			virtual void resizeEvent(QResizeEvent* qResizeEvent) override;
			virtual void mousePressEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent* qMouseEvent) override;
			virtual void wheelEvent(QWheelEvent* qMouseEvent) override;
			virtual void keyPressEvent(QKeyEvent* qEvent) override;
			virtual void keyReleaseEvent(QKeyEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			InputButton* mapQtMouseButtonToInputButton(Qt::MouseButton qtMouseButton);
			InputButton* mapQtKeyToInputButton(int qtKey, bool isNumpad);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			RenderWindow* mRenderWindow;		///< QSF render window instance, always valid, we're responsible for destroying the instance in case we no longer need it
			bool		  mHadInputFocus;		///< Had the widget the input focus before e.g. it was moved around?
			bool		  mPositionSizeDirty;	///< Has the position or size of the render widget been changed since the last render request?
			InputSystem&  mInputSystem;			///< QSF input system instance, always valid, do not destroy the instance
			glm::vec2	  mLastMousePos;
			bool		  mLastMousePosValid;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/renderer/RenderWidget-inl.h"
