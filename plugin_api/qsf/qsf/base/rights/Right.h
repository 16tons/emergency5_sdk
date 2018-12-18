// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Rights;
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
	*    Fine granular right class
	*/
	class QSF_API_EXPORT Right : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rights
		*    The rights collection this right it a part of, must stay valid as long as this right instance exists
		*  @param[in] name
		*    The UTF-8 name of the right (not localized), use "qsf::StringHash" on this name in order to receive the unique right ID
		*  @param[in] description
		*    The UTF-8 description of the right
		*/
		Right(const Rights& rights, const std::string& name, const std::string& description);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~Right();

		/**
		*  @brief
		*    Return the rights collection this right it a part of
		*
		*  @return
		*    The rights collection this right it a part of, do not destroy the instance
		*/
		inline const Rights& getRights() const;

		/**
		*  @brief
		*    Return the name of the right
		*
		*  @return
		*    The UTF-8 name of the right (not localized), use "qsf::StringHash" on this name in order to receive the unique right ID
		*/
		inline std::string getName() const;

		/**
		*  @brief
		*    Return the description of the right
		*
		*  @return
		*    The UTF-8 description of the right, localize it before presenting the it to the user
		*/
		inline std::string getDescription() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Rights* mRights;		///< Rights collection this right is part of, always valid, do not destroy the instance
		std::string   mName;		///< UTF-8 name of the right (not localized), use "qsf::StringHash" on this name in order to receive the unique right ID
		std::string   mDescription;	///< UTF-8 description of the right


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/rights/Right-inl.h"
