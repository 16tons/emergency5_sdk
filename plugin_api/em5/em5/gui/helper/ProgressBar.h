// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Element;
	}
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
	*    Progress bar class
	*/
	class EM5_API_EXPORT ProgressBar
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ProgressBar(Rocket::Core::Element* element, Rocket::Core::Element* elementBg);

		/**
		*  @brief
		*    Returns progress value in interval 0 to 1
		*/
		float getValue() const;

		/**
		*  @brief
		*    Sets the currently shown value of the progress bar
		*/
		void setValue(float value);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int						mMaxWidth;
		float					mValue;
		Rocket::Core::Element*	mElement;		///< Check if the element is set before use
		Rocket::Core::Element*	mElementBg;		///< Check if the element is set before use


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
