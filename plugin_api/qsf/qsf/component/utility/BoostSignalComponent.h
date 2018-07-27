// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"

#include <boost/signals2.hpp>


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
	*    Boost signal component class for spying on component property value changes
	*
	*  @remarks
	*    Component instances of prototypes and entities can react on property value changes of other components
	*    of the same prototype and entity by using the virtual method "qsf::Component::onComponentPropertyChange()".
	*    While this intra prototype and entity component communication system is simple and does not add per instance
	*    memory overhead, there are situations were other program parts need to be informed as in case of a component
	*    property value changes. This Boost signal component can be added to a prototype and entity in order to enable
	*    other program parts to react on component property value changes. For instance, a GUI can update the displayed
	*    property value.
	*
	*  @note
	*    - Do not overuse the Boost signal component
	*/
	class QSF_API_EXPORT BoostSignalComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::BoostSignalComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const Component& component, uint32 propertyId)> ComponentPropertyChange;	///< This Boost signal is emitted as soon as a component property value was changed, component instance which had a property value change as first parameter, unique class property ID the component property which values was changed as second parameter


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
		inline explicit BoostSignalComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BoostSignalComponent();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		inline virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/utility/BoostSignalComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BoostSignalComponent)
