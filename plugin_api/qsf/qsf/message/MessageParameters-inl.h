// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MessageParameters::MessageParameters() :
		mConfiguration(nullptr),
		mResponseReceiver(nullptr),
		mResponse(camp::Value::nothing)
	{
		// Nothing here
	}

	template<typename T>
	void MessageParameters::setParameter(const StringHash& identifier, const T& value)
	{
		mParameters.set(identifier, value);
	}

	template<typename T>
	bool MessageParameters::getParameter(const StringHash& identifier, T& value) const
	{
		return mParameters.get(identifier, value);
	}

	template<typename T>
	T MessageParameters::getParameterSafe(const StringHash& identifier) const
	{
		return mParameters.getSafe<T>(identifier);
	}

	template<typename T>
	void MessageParameters::respond(const T& response) const
	{
		if (nullptr == mResponseReceiver)
		{
			// Directly set response
			mResponse = response;
		}
		else
		{
			// Inform response receiver, which can then set the mResponse variable
			mResponseReceiver->onMessageResponseReceived(*this, camp::Value(response));
		}
	}

	template<typename T>
	bool MessageParameters::tryGetResponse(T& response) const
	{
		// Is response empty?
		if (camp::Value::nothing == mResponse)
			return false;

		// Is conversion possible?
		if (false == mResponse.isCompatible<T>())
			return false;

		// Do the conversion
		response = mResponse.to<T>();
		return true;
	}

	inline bool MessageParameters::hasResponse() const
	{
		return (camp::Value::nothing != mResponse);
	}

	inline const MessageConfiguration& MessageParameters::getConfiguration() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mConfiguration, "The message parameters configuration instance is invalid", QSF_REACT_THROW);
		return *mConfiguration;
	}

	inline uint32 MessageParameters::getNumberOfFilters() const
	{
		return getConfiguration().getNumberOfFilters();
	}

	template<typename T>
	T MessageParameters::getFilter(uint32 index) const
	{
		return getConfiguration().getFilter<T>(index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
