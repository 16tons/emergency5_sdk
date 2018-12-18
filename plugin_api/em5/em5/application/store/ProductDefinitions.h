// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace store
	{

		// Only relevant for Mac version so far
		struct Products
		{
			// See cpp for details about the different products
			static const qsf::NamedIdentifier EM5_IAP_EM2016;
			static const qsf::NamedIdentifier EM5_IAP_UPGRADE_EM2016_TO_EM2017;
			static const qsf::NamedIdentifier EM5_IAP_BUNDLE_EM2016_EM2017;
			static const qsf::NamedIdentifier EM5_IAP_UPGRADE_EM2017_TO_EM20;
			static const qsf::NamedIdentifier EM2017_IAP_UPGRADE_EM2017_TO_EM20;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // store
} // em5
