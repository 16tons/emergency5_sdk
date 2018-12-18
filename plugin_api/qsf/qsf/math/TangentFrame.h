// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/fwd.hpp>

#include <boost/noncopyable.hpp>


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
	*    Static tangent frame class
	*/
	class QSF_API_EXPORT TangentFrame : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Calculate QTangent from given input vectors
		*
		*  @param[in] normal
		*    Normalized normal
		*  @param[in] tangent
		*    Normalized tangent
		*  @param[in] binormal
		*    Normalized binormal
		*  @param[out] tangentFrameQuaternion
		*    Receives the calculated QTangent
		*
		*  @remarks
		*    QTangent basing on http://dev.theomader.com/qtangents/ "QTangents" which is basing on
		*    http://www.crytek.com/cryengine/presentations/spherical-skinning-with-dual-quaternions-and-qtangents "Spherical Skinning with Dual-Quaternions and QTangents"
		*/
		static void calculateQTangent(const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& binormal, glm::quat& tangentFrameQuaternion);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
