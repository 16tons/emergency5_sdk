// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <QtCore/qsize.h>
#include <QtCore/qpoint.h>
#include <QtGui/qcolor.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class UserObject;
	class UserProperty;
}
namespace qsf
{
	class Color3;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static type conversion class, for conversions between GLM and Qt types
		*/
		class QSF_EDITOR_API_EXPORT Convert : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			// QSize <-> glm::vec2
			static QSize convertGlmVec2ToQSize(const glm::vec2& glmVec2);
			static glm::vec2 convertQSizeToGlmVec2(const QSize& qSize);

			// QPoint <-> glm::vec2
			static QPoint convertGlmVec2ToQPoint(const glm::vec2& glmVec2);
			static glm::vec2 convertQPointToGlmVec2(const QPoint& qPoint);

			static double roundDoubleToEditorPrecision(double value);

			// QColor -> QString
			static QString convertColor3ToQString(const QColor& color);
			static QString convertColor4ToQString(const QColor& color);

			// glm::vec3 -> QString
			static QString convertGlmVec3ToQString(const glm::vec3& value);

			// QColor <-> qsf::Color3
			static QColor convertColor3ToQColor(const Color3& color3);
			static Color3 convertQColorToColor3(const QColor& qColor);
			static QColor campUserPropertyValueToQColor(const camp::UserObject& campUserObject, const camp::UserProperty& campUserProperty);

			// Quaternion to Euler angles in degree (glm::quat -> QString)
			static QString convertQuaternionToEulerAnglesQString(const glm::quat& quaternion);

			// Euler angles in degree to quaternion (QString -> glm::quat)
			static glm::quat convertEulerAnglesQStringToQuaternion(const QString& string);
			static QString convertEulerAnglesQStringToQuaternionQString(const QString& string);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
