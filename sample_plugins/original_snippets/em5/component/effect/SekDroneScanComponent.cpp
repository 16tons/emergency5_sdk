// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/effect/SekDroneScanComponent.h"
#include "em5/plugin/Jobs.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SekDroneScanComponent::COMPONENT_ID = qsf::StringHash("em5::SekDroneScanComponent");


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	SekDroneScanComponent::SekDroneScanComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype)
	{
		// Nothing to do in here
	}

	SekDroneScanComponent::~SekDroneScanComponent()
	{
		// Nothing to do in here
	}

	bool SekDroneScanComponent::onStartup()
	{
		if (isActive())
		{
			show();
		}

		// Done
		return true;
	}

	void SekDroneScanComponent::onShutdown()
	{
		if (isActive())
		{
			hide();
		}
	}

	void SekDroneScanComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			if (active)
			{
				show();
			}
			else
			{
				hide();
			}
		}
	}

	void SekDroneScanComponent::show()
	{
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_HIGHLIGHT, boost::bind(&SekDroneScanComponent::updateSimulation, this, _1));
	}

	void SekDroneScanComponent::hide()
	{
		mSimulationJobProxy.unregister();
	}

	void SekDroneScanComponent::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// TODO(pv): Detect gangsters in camera view frustum. (how to approach this the best way?)
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
