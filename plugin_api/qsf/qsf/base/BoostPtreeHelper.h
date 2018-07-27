// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/algorithm/string/predicate.hpp>
// #include <boost/lexical_cast.hpp>	// TODO(co) See commented code below
#include <boost/property_tree/ptree.hpp>


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
	*    Custom translator for bool (only supports std::string)
	*
	*  @remarks
	*    boost::ptree by default produces invalid JSON output (bools are converted to strings)
	*    and fails to parse the values it just wrote ("strings cannot be converted to boolean")
	*/
	struct BoolTranslator
	{
		typedef std::string internal_type;
		typedef bool        external_type;

		// Converts a string to bool
		boost::optional<external_type> get_value(const internal_type& str)
		{
			using boost::algorithm::iequals;
			if (iequals(str, "true") || iequals(str, "yes") || atoi(str.c_str()) != 0)
			{
				return boost::optional<external_type>(true);
			}
			else if (iequals(str, "false") || iequals(str, "no") || atoi(str.c_str()) == 0)
			{
				return boost::optional<external_type>(false);
			}
			else
			{
				return boost::optional<external_type>(boost::none);
			}
		}

		// Converts a bool to string
		boost::optional<internal_type> put_value(const external_type& b)
		{
			return boost::optional<internal_type>(b ? "true" : "false");
		}
	};

	struct Uft8WStringTranslator
	{
		typedef std::wstring internal_type;
		typedef std::string        external_type;

		// Converts a wstring to string
		boost::optional<external_type> get_value(const internal_type& str)
		{
			return boost::optional<external_type>("");
		}

		// Converts a string to wstring
		boost::optional<internal_type> put_value(const external_type& b)
		{
			return boost::optional<internal_type>(L"");
		}
	};

	struct Uft8StringTranslator
	{
		typedef std::string internal_type;
		typedef std::string        external_type;

		// Converts a wstring to string
		boost::optional<external_type> get_value(const internal_type& str)
		{
			return boost::optional<external_type>("");
		}

		// Converts a string to wstring
		boost::optional<internal_type> put_value(const external_type& b)
		{
			return boost::optional<internal_type>("");
		}
	};

	struct WBoolTranslator
	{
		typedef std::wstring internal_type;
		typedef bool        external_type;

		// Converts a string to bool
		boost::optional<external_type> get_value(const internal_type& str)
		{
			using boost::algorithm::iequals;
			if (iequals(str, L"true") || iequals(str, L"yes") || wcstol(str.c_str(), nullptr, 10) != 0)
			{
				return boost::optional<external_type>(true);
			}
			else if (iequals(str, L"false") || iequals(str, L"no") || wcstol(str.c_str(), nullptr, 10) == 0)
			{
				return boost::optional<external_type>(false);
			}
			else
			{
				return boost::optional<external_type>(boost::none);
			}
		}

		// Converts a bool to string
		boost::optional<internal_type> put_value(const external_type& b)
		{
			return boost::optional<internal_type>(b ? L"true" : L"false");
		}
	};

	// TODO(co) Why is this commented?
	/**
	*  @brief
	*    Custom translator for numbers (only supports std::string)
	*
	*  @remarks
	*    boost::ptree by default produces invalid JSON output (numbers are converted to strings)
	*/
	/*
	template<typename Number>
	struct NumberTranslator
	{
		typedef std::string internal_type;
		typedef Number      external_type;

		// Converts a string to Number
		boost::optional<external_type> get_value(const internal_type& str)
		{
			try
			{
				return boost::optional<external_type>(boost::lexical_cast<external_type>(str));
			}
			catch (const std::exception&)
			{
				return boost::optional<external_type>(boost::none);
			}
		}

		// Converts a Number to string
		boost::optional<internal_type> put_value(const external_type& i)
		{
			return boost::optional<internal_type>(boost::lexical_cast<internal_type>(i));
		}
	};
	*/

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

/*  Specialize translator_between so that it uses our custom translator for
bool value types. Specialization must be in boost::property_tree
namespace. */
namespace boost {
	namespace property_tree {

		template<>
		struct translator_between<std::string, bool>
		{
			typedef qsf::BoolTranslator type;
		};

		template<>
		struct translator_between<std::wstring, bool>
		{
			typedef qsf::WBoolTranslator type;
		};

		// TODO(co) Why is this commented?
		/*
		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, int8>
		{
			typedef qsf::NumberTranslator<int8> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, uint8>
		{
			typedef qsf::NumberTranslator<uint8> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, int16>
		{
			typedef qsf::NumberTranslator<int16> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, uint16>
		{
			typedef qsf::NumberTranslator<uint16> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, int32>
		{
			typedef qsf::NumberTranslator<int32> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, uint32>
		{
			typedef qsf::NumberTranslator<uint32> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, int64>
		{
			typedef qsf::NumberTranslator<int64> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, uint64>
		{
			typedef qsf::NumberTranslator<uint64> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, float>
		{
			typedef qsf::NumberTranslator<float> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, double>
		{
			typedef qsf::NumberTranslator<double> type;
		};

		template<typename Ch, typename Traits, typename Alloc>
		struct translator_between<std::basic_string< Ch, Traits, Alloc >, long double>
		{
			typedef qsf::NumberTranslator<long double> type;
		};
		*/

	} // namespace property_tree
} // namespace boost
