// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Application::~Application()
	{
		// Nothing to do in here
	}

	inline const std::vector<std::string>& Application::getProgramOptions() const
	{
		QSF_CHECK(nullptr != mProgramOptions, "QSF application program options pointer is invalid", QSF_REACT_THROW);
		return *mProgramOptions;
	}

	inline const boost::property_tree::ptree& Application::getParametersPTree() const
	{
		QSF_CHECK(nullptr != mParametersPTree, "QSF application parameters property tree pointer is invalid", QSF_REACT_THROW);
		return *mParametersPTree;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline Application::Application() :
		mProgramOptions(nullptr),
		mParametersPTree(nullptr)
	{
		// Nothing to do in here
	}

	inline void Application::initializeOptionsAndParameters(const std::vector<std::string>& programOptions, const boost::property_tree::ptree& parametersPTree)
	{
		mProgramOptions = &programOptions;
		mParametersPTree = &parametersPTree;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
