// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/prototype/finalizer/PrototypeFinalizer.h"


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
	*    Prototype finalizer class for deactivating ambient audio management components
	*/
	class AmbientPolygonPrototypeFinalizer : public PrototypeFinalizer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		AmbientPolygonPrototypeFinalizer(PrototypeFinalizerManager* prototypeFinalizerManager);

		/**
		*  @brief
		*    Destructor
		*/
		~AmbientPolygonPrototypeFinalizer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PrototypeFinalizer methods        ]
	//[-------------------------------------------------------]
	public:
		virtual void processPrototypes(BasePrototypeManager& prototypeManager, const PrototypeFinalizerSettings& settings) override;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AmbientPolygonPrototypeFinalizer)
