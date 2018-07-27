// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/algorithm/string.hpp> // For boost::algorithm::erase_all_copy and boost::algorithm::is_any_of


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	template <typename T>
	T CampHelper::getCampTagValue(const camp::TagHolder& campTagHolder, camp::StringId tagId, const T& defaultValue)
	{
		// Get CAMP tag instance, if it exists
		const camp::Value* campValue = campTagHolder.tryGetTagById(tagId);
		if (nullptr != campValue)
		{
			// Get CAMP tag value
			try
			{
				return campValue->to<T>();
			}
			catch (const std::exception&)
			{
				// Nothing to do in here
			}
		}

		// Error! Return the default value instead...
		return defaultValue;
	}

	template<typename STORAGE, typename ENUM>
	std::string CampHelper::enumBitmaskToString(EnumBitmask<STORAGE, ENUM> enumBitmask)
	{
		// Get CAMP meta enum
		const camp::Enum& campMetaEnum = camp::enumByType<ENUM>();

		// Are all bits set?
		const STORAGE allBits = static_cast<STORAGE>(1 << campMetaEnum.size()) - 1;
		if (enumBitmask.allSet(allBits))
		{
			return "all";
		}

		// Build string consisting of all set enum names
		std::string result;
		for (size_t i = 0; i < campMetaEnum.size(); ++i)
		{
			const camp::Enum::Pair& campEnumPair = campMetaEnum.pair(i);

			if (enumBitmask.isSet(static_cast<ENUM>(campEnumPair.value)))
			{
				// Add plus character as separator
				if (!result.empty())
					result += ',';

				// Add name
				result += campEnumPair.name;
			}
		}

		// Done
		return result;
	}

	template<typename STORAGE, typename ENUM>
	void CampHelper::enumBitmaskFromString(const std::string& input, EnumBitmask<STORAGE, ENUM>& enumBitmask)
	{
		enumBitmask.clearAll();

		// Ignore empty strings, don't try to split them
		if (input.empty())
			return;

		// Get CAMP meta enum
		const camp::Enum& campMetaEnum = camp::enumByType<ENUM>();

		// Special handling for input string "all"
		if (input == "all")
		{
			// Set all bits
			const STORAGE allBits = static_cast<STORAGE>(1 << campMetaEnum.size()) - 1;
			enumBitmask.setMultiple(allBits);
			return;
		}

		// Split the string into its components
		std::vector<std::string> parts;

		// We have to use a named instance, because you cannot bind a temporary to a non const reference.
		// The second parameter of boost::split is a non const reference
		std::string inputString(boost::algorithm::erase_all_copy(input, " "));
		boost::split(parts, inputString, boost::is_any_of(","));

		for (const std::string& part : parts)
		{
			if (campMetaEnum.hasName(part))
			{
				enumBitmask.set(static_cast<ENUM>(campMetaEnum.value(part)));
			}
			else
			{
				QSF_WARN("Could not match value named \"" << part << "\" to enum entry name", QSF_REACT_NONE);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
