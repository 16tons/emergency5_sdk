// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/selection/SelectionRectangle.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
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
	*    Pixel space selection rectangle for selecting entities
	*/
	class QSF_API_EXPORT EntitySelectionRectangle : public SelectionRectangle
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
		explicit EntitySelectionRectangle(RenderWindow& renderWindow);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EntitySelectionRectangle();

		/**
		*  @brief
		*    Calculate and return the selected components inside the rectangle
		*
		*  @param[out] outComponents
		*    Array of component instances to be filled; is not automatically cleared
		*
		*  @return
		*    "true" if all went fine and there is at least one component in result, "false" on error or empty selection
		*/
		bool getComponentSelection(std::vector<Component*>& outComponents) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
