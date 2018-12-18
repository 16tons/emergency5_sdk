// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    A base class which adds GUI hot reloading support to an GUI document
	*
	*  @remarks
	*    That GUI hot reloading works a derived class has to implement onDocumentPreShow/onDocumentPreHide (If the derived class wants to react on pre show/hide)
	*    instead of the private virtual qsf::GuiDocument methods onPreShow/onPreHide
	*/
	class GuiHotReloadSupport : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*
		*  @param[in] guiContext
		*    The GUI context to which the GUI document belongs to
		*/
		explicit GuiHotReloadSupport(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GuiHotReloadSupport();


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GuiHotReloadSupport methods    ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Gets called when the GUI should be reloaded
		*/
		virtual void onReloadGui();

		// TODO(co) I would like to see those special methods removed, just overload methods as usual and call base implementations as usual (can't change it right now to not make too many untested changes inside a patch)
		// Due the private nature of onPreShow/onPreHide we have to define additional methods
		// These methods have the same meaning as qsf::GuiDocument::onPreShow/onPreHide
		virtual bool onDocumentPreShow();
		virtual void onDocumentPreHide();


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void disconnectSignals();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// TODO(sw) This is an fix for the crash when going from the game to the main menu and initiate then a GUI hot reloading -> The connection to the menubar was still there but the menubar instance was destroyed
		// Proper fix see QSF Bug "H301 GUI | "qsf::GuiDocument" | We need to guarantee that for each e.g. "onPostShow()" there's a "onPostHide()""
		boost::signals2::connection	mReloadGuiConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
