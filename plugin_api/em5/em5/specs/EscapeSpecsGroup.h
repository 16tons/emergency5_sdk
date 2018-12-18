// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/specs/SpecsGroup.h"

#include <qsf/base/UniqueInstance.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Escape specs group implementation
	*/
	class EM5_API_EXPORT EscapeSpecsGroup : public SpecsGroup, public qsf::UniqueInstance<EscapeSpecsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;					///< "em5::EscapeSpecsGroup" unique pluginable specs group ID
		static const uint32 ESCAPE_TIME_FOOT_LONG;			///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 ESCAPE_TIME_CAR_LONG;			///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 RUN_TIME_FOOT_LONG_MIN;			///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 RUN_TIME_FOOT_LONG_MAX;			///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 RUN_TIME_FOOT_SHORT_MIN;		///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 RUN_TIME_FOOT_SHORT_MAX;		///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 BREATHE_TIME_FOOT_LONG_MIN;		///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 BREATHE_TIME_FOOT_LONG_MAX;		///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 BREATHE_TIME_FOOT_SHORT_MIN;	///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 BREATHE_TIME_FOOT_SHORT_MAX;	///< "EscapeTimeFootLong" unique class property ID inside the class
		static const uint32 HELICOPTER_FOLLOW_TIME;			///< "HelicopterFollowTime" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] specsGroupManager
		*    Optional pointer to the specs group manager this specs group should be registered to, can be a null pointer
		*/
		explicit EscapeSpecsGroup(SpecsGroupManager* specsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EscapeSpecsGroup();

		inline float getEscapeTimeFootLong() const;
		void setEscapeTimeFootLong(float time);

		inline float getEscapeTimeCarLong() const;
		void setEscapeTimeCarLong(float time);

		inline float getRunTimeFootLongMin() const;
		void setRunTimeFootLongMin(float time);

		inline float getRunTimeFootLongMax() const;
		void setRunTimeFootLongMax(float time);

		inline float getRunTimeFootShortMin() const;
		void setRunTimeFootShortMin(float time);

		inline float getRunTimeFootShortMax() const;
		void setRunTimeFootShortMax(float time);

		inline float getBreatheTimeFootLongMin() const;
		void setBreatheTimeFootLongMin(float time);

		inline float getBreatheTimeFootLongMax() const;
		void setBreatheTimeFootLongMax(float time);

		inline float getBreatheTimeFootShortMin() const;
		void setBreatheTimeFootShortMin(float time);

		inline float getBreatheTimeFootShortMax() const;
		void setBreatheTimeFootShortMax(float time);

		inline float getHelicopterFollowTime() const;
		void setHelicopterFollowTime(float time);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float mEscapeTimeFootLong;
		float mEscapeTimeCarLong;
		float mRunTimeFootLongMin;
		float mRunTimeFootLongMax;
		float mRunTimeFootShortMin;
		float mRunTimeFootShortMax;
		float mBreatheTimeFootLongMin;
		float mBreatheTimeFootLongMax;
		float mBreatheTimeFootShortMin;
		float mBreatheTimeFootShortMax;
		float mHelicopterFollowTime;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/specs/EscapeSpecsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::EscapeSpecsGroup)
