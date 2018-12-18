// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/overlay/OverlayComponent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Structs                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 building overlay provider struct
	*/
	struct EM5_API_EXPORT BuildingOverlayProvider
	{
		uint64 entityId;	///< The entity ID of the provider, "qsf::getUninitialized<uint64>()" if not set, yet
		float  progress;	///< Progress of the provider (range [0-1] where 1 is a full bar)

		BuildingOverlayProvider(uint64 _entityId, float _progress) :
			entityId(_entityId),
			progress(_progress)
		{
			// Nothing here
		}
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 building overlay component class
	*/
	class EM5_API_EXPORT BuildingOverlayComponent : public OverlayComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class BuildingOverlayWindowManager;	///< For GUI hot-reloading


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::BuildingOverlayComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Show progress of the given provider entity
		*
		*  @param[in] buildingEntity
		*    Overlay owner building entity
		*  @param[in] numberOfRequiredProviders
		*    Number of required providers which requested the visualization of a progress
		*  @param[in] providerEntity
		*    Overlay information providing entity (e.g. a firefighter entity)
		*  @param[in] progress
		*    Progress (range [0-1] where 1 is a full bar)
		*/
		static void showProgress(qsf::Entity& buildingEntity, uint32 numberOfRequiredProviders, const qsf::Entity& providerEntity, float progress);

		/**
		*  @brief
		*    Hide overlay of the given provider entity
		*
		*  @param[in] buildingEntity
		*    Overlay owner building entity
		*  @param[in] providerEntity
		*    Overlay information providing entity (e.g. a firefighter entity)
		*/
		static void hideOverlay(qsf::Entity& buildingEntity, const qsf::Entity& providerEntity);


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
		explicit BuildingOverlayComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BuildingOverlayComponent();

		inline const std::vector<BuildingOverlayProvider>& getRegisteredProviders() const;
		inline bool isEmpty() const;

		void registerOrUpdateProvider(uint64 providerEntityId, float progress);
		void unregisterProvider(uint64 providerEntityId);

		inline uint32 getNumberOfRequiredProviders() const;
		void setNumberOfRequiredProviders(uint32 numberOfRequiredProviders);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::OverlayComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual bool isContentEmpty() const override;
		virtual void updateContent(Rocket::Core::ElementDocument& document) override;
		virtual const glm::vec3& getContentWorldSpaceOffset() const override;
		virtual const glm::vec2& getContentWindowSize() const override;
		virtual const glm::vec2& getContentWindowOffset() const override;
		virtual void updateAnimation(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<BuildingOverlayProvider> Providers;
		Providers mRegisteredProviders;
		uint32    mNumberOfRequiredProviders;

		enum Dirty
		{
			GUI_NOT_DIRTY,		///< Nothing to update at all
			GUI_PROGRESS_DIRTY,	///< Negligible performance impact
			GUI_LAYOUT_DIRTY	///< Heavy performance impact: Needs to set in case the number of registered or required provider entities has changed
		};
		Dirty mDirty;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setDirtyState(Dirty dirty);
		std::string getPersonalInnerRmlString(const qsf::Entity& providerEntity) const;


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
#include "em5/component/overlay/BuildingOverlayComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuildingOverlayComponent)
