// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>


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
	*    EMERGENCY 5 gaper component class
	*/
	class EM5_API_EXPORT GaperComponent : public qsf::Component, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::GaperComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit GaperComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GaperComponent();

		bool getUseMobilephone() const;
		void setUseMobilephone(bool useMobilephone);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateAnimation(const qsf::JobArguments& jobArguments);
		void resetWaitTimer();
		bool preCheck();
		void pushAnimation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal
		qsf::JobProxy	mAnimatingJobProxy;
		qsf::Time		mTimeGaping;
		bool			mUseMobilephone;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GaperComponent)
