// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/localization/serializer/TranslatorSerializer.h"
#include "qsf/platform/EndianConversion.h"
#include "qsf/base/GetUninitialized.h"


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
	*    Qt binary QM translator serializer
	*
	*  @todo
	*    - TODO(ca) Currently only a subset of the QM file format is supported
	*/
	class QmTranslatorSerializer : public TranslatorSerializer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		QmTranslatorSerializer();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~QmTranslatorSerializer();


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
		template<typename T>
		inline void adjustByteorder(T& value) const;

		template<typename T>
		inline T read(std::istream& istream) const;

		template<typename T>
		inline T read(const char*& readPtr) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mIsLittleEndian;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/localization/serializer/QmTranslatorSerializer-inl.h"
