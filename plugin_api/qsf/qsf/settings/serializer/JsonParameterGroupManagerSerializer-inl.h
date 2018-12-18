// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline JsonParameterGroupManagerSerializer::JsonParameterGroupManagerSerializer(const std::string* formatTypeOverride) :
		mRequiredFormatType((nullptr != formatTypeOverride) ? *formatTypeOverride : FORMAT_TYPE)
	{
		// Nothing to do in here
	}

	inline JsonParameterGroupManagerSerializer::~JsonParameterGroupManagerSerializer()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
