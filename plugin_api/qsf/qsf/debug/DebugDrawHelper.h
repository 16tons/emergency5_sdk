// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawProxy.h"
#include "qsf/debug/request/TextDebugDrawRequest.h"
#include "qsf/QsfHelper.h"

#include <sstream>


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Macro for quick'n'dirty debug text output to a fixed screen position
*
*  @param[in] text
*    Text which should be written onto the screen; you may build it using the stream operator <<
*  @param[in] positionX
*    X-coordinate of the 2D window position, in pixels from the left window border
*  @param[in] positionY
*    Y-coordinate of the 2D window position, in pixels from the upper window border
*
*  @remarks
*    Usage example:
*    @code
*    const std::string myName("Fred");
*    QSF_DEBUG_QUICK_TEXT_OUTPUT("Hi, my name is " << myName << ", nice to meet you.", 320, 80);
*    @endcode
*
*  @note
*    - Use this macro only temporarily for debugging (for the following reasons)
*    - The debug draw request created by this macro has an unlimited lifetime, there's no possibility to remove it
*    - Fixed screen text outputs may be useful for you, but annoy other people; that's why at least in retail builds, this macro is empty and produces no output at all
*/
#ifndef RETAIL
	#define QSF_DEBUG_QUICK_TEXT_OUTPUT(text, positionX, positionY) \
		{ \
			std::ostringstream stream; \
			stream << text; \
			static qsf::DebugDrawProxy debugDrawProxy; \
			debugDrawProxy.registerAt(QSF_DEBUGDRAW); \
			debugDrawProxy.addRequest(qsf::TextDebugDrawRequest(stream.str().c_str(), glm::vec2((positionX), (positionY)))); \
		}
#else
	#define QSF_DEBUG_QUICK_TEXT_OUTPUT(text, positionX, positionY) {}
#endif
