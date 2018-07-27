// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawProxy.h"

#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RenderWindow;
	class Frustum;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Pixel space selection rectangle
	*/
	class QSF_API_EXPORT SelectionRectangle : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderWindow
		*    Render window to operate on
		*/
		explicit SelectionRectangle(RenderWindow& renderWindow);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SelectionRectangle();

		/**
		*  @brief
		*    Return the render window instance this selection rectangle operates on
		*
		*  @return
		*    The render window instance this selection rectangle operates on, don't destroy the instance
		*/
		inline RenderWindow& getRenderWindow() const;

		/**
		*  @brief
		*    Return whether or not drag is in progress
		*
		*  @return
		*    "true" if drag is in progress, else "false"
		*/
		inline bool isDragInProgress() const;

		/**
		*  @brief
		*    Start the selection rectangle drag
		*
		*  @param[in] position
		*    Pixel space position where to start the selection rectangle
		*
		*  @note
		*    - In case drag is already in progress, it's silently restarted
		*/
		void startDrag(const glm::vec2& position);

		/**
		*  @brief
		*    Update the selection rectangle drag
		*
		*  @param[in] position
		*    Current pixel space drag position
		*/
		void updateDrag(const glm::vec2& position);

		/**
		*  @brief
		*    Stop the selection rectangle drag
		*
		*  @note
		*    - In case drag is not in progress the method call is silently ignored
		*/
		void stopDrag();

		/**
		*  @brief
		*    Return whether or not the selection rectangle is active
		*
		*  @return
		*    "true" if the selection rectangle is active, else "false"
		*/
		inline bool hasSelectionRectangle() const;

		/**
		*  @brief
		*    Return the world space frustum associated with the selection rectangle
		*
		*  @param[out] outFrustum
		*    Frustum instance to be filled with frustum data
		*
		*  @return
		*    "true" if all went fine, "false" if no frustum could be created
		*/
		bool getWorldSpaceFrustum(Frustum& outFrustum) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RenderWindow*  mRenderWindow;				///< RenderWindow instance where to draw the selection rectangle; always valid
		float		   mClickReleaseRangeInPixels;	///< Distance in pixels we're allowed to dragged without switching to the selection rectangle
		bool		   mDragInProgress;				///< Drag in progress?
		bool		   mHasSelectionRectangle;		///< "true" if the selection rectangle is active, else "false"
		glm::vec2	   mDragStartPosition;			///< Pixel space position where the selection rectangle was started
		glm::vec2	   mDragEndPosition;			///< Pixel space position where the selection rectangle currently ends
		DebugDrawProxy mRectangleDebugDraw;			///< Debug draw proxy for the selection rectangle


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/selection/SelectionRectangle-inl.h"
