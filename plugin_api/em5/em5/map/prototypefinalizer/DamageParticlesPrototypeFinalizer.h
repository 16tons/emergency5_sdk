// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/prototype/finalizer/PrototypeFinalizer.h>


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
	*    Prototype finalizer class for hiding damage and fire particles
	*/
	class DamageParticlesPrototypeFinalizer : public qsf::PrototypeFinalizer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		DamageParticlesPrototypeFinalizer(qsf::PrototypeFinalizerManager* prototypeFinalizerManager);

		/**
		*  @brief
		*    Destructor
		*/
		~DamageParticlesPrototypeFinalizer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PrototypeFinalizer methods        ]
	//[-------------------------------------------------------]
	public:
		virtual void processPrototypes(qsf::BasePrototypeManager& prototypeManager, const qsf::PrototypeFinalizerSettings& settings) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DamageParticlesPrototypeFinalizer)
