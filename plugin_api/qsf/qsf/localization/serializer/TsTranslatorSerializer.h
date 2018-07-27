// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/localization/serializer/TranslatorSerializer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
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
	*    Qt XML ts translator serializer
	*
	*  @todo
	*    - TODO(co) Currently only a subset of the TS file format is supported
	*/
	class TsTranslatorSerializer : public TranslatorSerializer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline TsTranslatorSerializer();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~TsTranslatorSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::TranslatorSerializer methods      ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(Translator& translator, std::istream& istream) const override;
		virtual void serialize(Translator& translator, std::ostream& ostream) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load a Qt XML ts translator
		*
		*  @param[in] translator
		*    Translator instance to fill
		*  @param[in] tsPTree
		*    Boost TS ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadTranslator(Translator& translator, const boost::property_tree::ptree& tsPTree) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/localization/serializer/TsTranslatorSerializer-inl.h"
