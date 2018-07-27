// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/BoostPtreeHelper.h"

#include <boost/property_tree/ptree.hpp>

#include <camp/valuemapper.hpp>


// No ValueMapper provided (yet?)


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(boost::property_tree::ptree);
