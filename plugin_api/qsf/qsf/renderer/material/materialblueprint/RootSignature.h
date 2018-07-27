// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/detail/RootSignatureTypes.h"

#include <boost/noncopyable.hpp>

#include <vector>


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
	*    Root signature
	*
	*  @remarks
	*    The DirectX 12 root signature fits perfectly into the material blueprint concept.
	*    We need it in the future anyway when supporting APIs like DirectX 12 - although OGRE
	*    itself currently doesn't has root signature support at this point in time.
	*
	*  @note
	*    - "qsf::Renderer::RootSignature" is the DirectX 12 style low level root signature while "qsf::RootSignature" manages the referenced memory
	*/
	class RootSignature : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class JsonMaterialBlueprintSerializer;	///< Reads the root signature


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline RootSignature();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~RootSignature();

		/**
		*  @brief
		*    Return the renderer root signature
		*
		*  @return
		*    The DirectX 12 style root signature, don't dare to delete any referenced memory
		*/
		inline const Renderer::RootSignature& getRendererRootSignature() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void initializeRendererRootSignature();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<Renderer::RootParameter>   RootParameters;
		typedef std::vector<Renderer::DescriptorRange> DescriptorRanges;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RootParameters			mRootParameters;		///< Memory management of the root parameters
		DescriptorRanges		mDescriptorRanges;		///< Memory management of the descriptor ranges
		Renderer::RootSignature	mRendererRootSignature;	///< The DirectX 12 style root signature


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/RootSignature-inl.h"
