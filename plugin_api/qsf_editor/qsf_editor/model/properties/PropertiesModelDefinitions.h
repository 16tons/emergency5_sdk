// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/reflection/object/Object.h>

#include <QtCore/QMetaType> // For Q_DECLARE_METATYPE


//[-------------------------------------------------------]
//[ Qt metatype                                           ]
//[-------------------------------------------------------]
Q_DECLARE_METATYPE(const camp::Property*)
Q_DECLARE_METATYPE(qsf::Object*)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		enum PropertiesModelRole
		{
			PROPERTIESMODELROLE_PROPERTY = Qt::UserRole + 1,	/**< This role holds the const camp::Property* for an entry in the model. */
			PROPERTIESMODELROLE_OBJECT,						/**< This role holds the qsf::Object* for an entry in the model. */
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
