// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/WeakPtr.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GuiContext;
}
namespace em5
{
	class OverlayWindow;
}


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
	*    EMERGENCY 5 overlay window manager base class
	*/
	class OverlayWindowManager : public qsf::WeakPtrTarget
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit OverlayWindowManager(qsf::GuiContext& guiContext);
		virtual ~OverlayWindowManager();
		qsf::GuiContext& getGuiContext() const;


	//[-------------------------------------------------------]
	//[ Public virtual em5::OverlayWindowManager methods      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Set the visibility state of all overlays
		*
		*  @param[in] visible
		*    "true" to make all active overlays visible, else "false"
		*/
		virtual void setAllOverlaysVisible(bool visible) = 0;

		/**
		*  @brief
		*    Return a new or a resuable status overlay window instance
		*
		*  @param[in] entityId
		*    The entity which orders the status overlay window
		*/
		virtual OverlayWindow* getOverlayWindowInstance(uint64 entityId) = 0;

		/**
		*  @brief
		*    Return the status overlay window instance
		*
		*  @param[in] entityId
		*    The entity which release the overlay window
		*/
		virtual void releaseOverlayWindowInstance(uint64 entityId) = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::GuiContext& mGuiContext;	///< Used GUI context, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
