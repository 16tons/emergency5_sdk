// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AdvancedAnimationComponent;
	class SkeletonHandle;
}


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
	*   Abstract animation node base class
	*
	*  @note
	*    - We need to use this pure virtual base class to create animation node modules
	*/
	class QSF_API_EXPORT AnimationNode
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class AdvancedAnimationComponent;	// We have prepared private methods for qsf::AdvancedAnimationComponent


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint16 MAXIMAL_NUMBER_OF_INPUTS = 6;	// Maximal number of inputs, we need this value to store all our input information in stack


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] advancedAnimationComponent
		*    A reference to an advanced animation component where this animation node lies
		*  @param[in] numberOfInputSlots
		*    Number of input slots we can use, can be 0, if higher then MAXIMAL_NUMBER_OF_INPUTS it will be corrected with no error !
		*  @param[in] readOnlyFirstInput
		*    If true we can only read the first input like the other input skeletons
		*    If false our first input and output are the same skeleton and we can access it with getOutputSkeleton
		*/
		explicit AnimationNode(AdvancedAnimationComponent& advancedAnimationComponent, uint16 numberOfInputSlots, bool readOnlyFirstInput);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AnimationNode();

		/**
		*  @brief
		*    Get the number of input slots of this node
		*/
		inline uint16 getNumberOfInputSlots() const;

		/**
		*  @brief
		*    Get if the first input slot is read only or can be changed and used as output
		*/
		inline bool isFirstInputReadOnly() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AnimationNode methods          ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Animation calculation method
		*
		*  @note
		*    - Overwrite this method and use the inputs and your own parameter to create the output
		*/
		virtual void calculate() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Get the output skeleton of this node
		*
		*  @return
		*    Return a pointer to a skeleton handle, can be null if we have no output
		*/
		inline SkeletonHandle* getOutputSkeleton() const;

		/**
		*  @brief
		*    Get a input skeleton of this node
		*
		*  @return
		*    Return a const pointer to a skeleton handle, can be null if we have no input connected
		*/
		inline const SkeletonHandle* getInputSkeleton(uint16 inputId) const;

		/**
		*  @brief
		*    Give back a regerence to the advanced animation component where this node is created
		*/
		inline AdvancedAnimationComponent& getAdvancedAnimationComponent();
		inline const AdvancedAnimationComponent& getAdvancedAnimationComponent() const;

		/**
		*  @brief
		*    Give back the number of bones we will have in skeletons
		*/
		uint16 getBoneNumber() const;

		/*
		*  @brief
		*    Add the offset from a key frame skeleton file to our output
		*
		*  @param[in] animationName
		*    Key frame skeleton file name, where we read our key frames
		*  @param[in] progress
		*    Should be a value between 0.0 (start of animation) and 1.0 (end of animation)
		*  @param[in] retarget
		*    If we want to retarget an animation from the skeleton data file skeleton to our skeleton
		*/
		void addOffsetFromKeyFrameSkeleton(const std::string& animationName, float progress, bool retarget = false);

		/*
		*  @brief
		*    Add the offset from a key frame skeleton file to the binding pose and give back the result
		*
		*  @param[in] animationName
		*    Key frame skeleton file name, where we read our key frames
		*  @param[in] toSkeletonId
		*    ID of our skeleton where we want our result
		*  @param[in] progress
		*    Should be a value between 0.0 (start of animation) and 1.0 (end of animation)
		*  @param[in] retarget
		*    If we want to retarget an animation from the skeleton data file skeleton to our skeleton
		*/
		void addOffsetToBindingPoseFromKeyFrameSkeleton(const std::string& animationName, float progress, bool retarget = false);

		/*
		*  @brief
		*    Give back the time  length of a key frame skeleton file
		*
		*  @param[in] animationName
		*    Key frame skeleton file name
		*
		*  @return
		*    Give back the time length in seconds
		*/
		float getKeyFrameSkeletonTimeLength(const std::string& animationName, bool retarget = false) const;

		/*
		*  @brief
		*    Set if we need to calculate a certain input
		*
		*  @param[in] inputId
		*    ID of the input slot, nothing will happen for invalid input IDs
		*  @param[in] calculationNeeded
		*    If we need our input calculated or not
		*
		*  @note
		*    - We will get null pointer for input skeleton on this slot if we want to take that skeleton
		*/
		void setInputNodeCalculationNeeded(uint16 inputId, bool calculationNeeded);

		/**
		*  @brief
		*    Calculate another sub node with the output and input skeletons from this node
		*
		*  @param[in] subNode
		*    Reference to another animation node, don't use nodes from other components
		*/
		void calculateSubNode(AnimationNode& subNode);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum CalculateState		// Internal state of tryCalculate which is called recursive
		{
			CALCULATE_READY,
			CALCULATE_STARTED,
			CALCULATE_FINISHED,
		};


	//[-------------------------------------------------------]
	//[ Private methods for qsf::AdvancedAnimationComponent   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Try to start the calculation of the animation and try to get a result
		*
		*  @note
		*    - The calculation cannot fail but we can immediatly return when we already have a result
		*/
		CalculateState tryCalculate();

		/**
		*  @brief
		*    Prepare this node for calculation, only call this once per animation update, before we want to calculate the node
		*/
		void resetForCalculation();

		/**
		*  @brief
		*    Connect a node to a input and take the output of this node as input
		*
		*  @param[in] inputId
		*    ID of an input slot, with an undefined or invalid ID nothing will be connected
		*  @param[in] animationNode
		*    Reference to an animation node
		*/
		void connectNode(uint16 inputId, AnimationNode& animationNode);

		/**
		*  @brief
		*    Disconnect the given animation node from its slot
		*
		*  @param[in] animationNode
		*    Reference to the animation node we want to remove
		*/
		void disconnectNode(AnimationNode& animationNode);

		/**
		*  @brief
		*    Disconnect an animation node from the given slot
		*
		*  @param[in] inputId
		*    ID of an input slot, with an undefined or invalid ID we will disconnect nothing
		*/
		void disconnectNode(uint16 inputId);

		/**
		*  @brief
		*    Give back the node connected to a input slot
		*
		*  @param[in] inputId
		*    ID of an input slot, with an undefined or invalid ID nothing will be connected
		*/
		inline AnimationNode* getConnectedNode(uint16 inputId) const;

		/**
		*  @brief
		*    Check if we are connected with an animation node
		*
		*  @param[in] animationNode
		*    Reference to the animation node we want to remove
		*  @param[out] inputId
		*    Giving back the ID of the input slot if we are connected
		*
		*  @return
		*    "true" if we are connected and "false" if not
		*/
		bool isConnected(AnimationNode& animationNode, uint16& inputId) const;
		bool isConnected(AnimationNode& animationNode) const;

		/**
		*  @brief
		*    Check if all output connections are used
		*/
		inline bool isOutputConnectionAllUsed() const;

		/**
		*  @brief
		*    Register a skeleton ID
		*/
		void registerSkeletonId();

		/**
		*  @brief
		*    Unregister the skeleton ID, only call this if we don't need the skeleton id and mOutputSkeleton anymore
		*/
		void unregisterSkeletonId();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Clear a skeleton handle if necessary
		*/
		void clearSkeleton(SkeletonHandle*& skeletonHandle);

		/**
		*  @brief
		*    Set our input skeleton handle to a output handle of an other node
		*/
		void setSkeleton(SkeletonHandle*& skeletonHandle, uint16 SkeletonId);

		/**
		*  @brief
		*    Prepare all inputs and outputs for calculation and calculate them if necessary
		*/
		void prepareForCalculation();

		/**
		*  @brief
		*    Unregister input skeletons who are already used with all connections
		*/
		void unregisterUnusedInputSkeletons();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AdvancedAnimationComponent& mAdvancedAnimationComponent;					///< Nodes cannot live without this component, only the base class is a friend

		AnimationNode*		mInputNodes[MAXIMAL_NUMBER_OF_INPUTS];					///< Nodes with output connected to our input, fixed length for placing in stack
		uint16				mNumberOfInputSlots;									///< Number of input slots

		uint16				mOutputConnectionNumber;								///< Number of outputs connections to other nodes
		uint16				mOutputConnectionsUsed;									///< How many outputs got already used
		bool				mOutputCouldNotBeReused;								///< We did need to copy the output (We need to hold this information to check if coul optimized the structure)

		bool				mReadOnlyFirstInput;									///< If we want to read only the first input or change it and use as output

		CalculateState		mCalculationState;										///< State of "tryCalculate" function

		SkeletonHandle*		mOutputSkeleton;										///< Skeleton instance handle for output, allocation is handled in this class
		SkeletonHandle*		mInputSkeletons[MAXIMAL_NUMBER_OF_INPUTS];				///< Skeleton instance handle for input skeletons, allocation is handled in this class

		uint16				mRegisteredSkeletonId;									///< Registered skeleton to this node, remember it to prevent multiple registration per node

		bool				mInputNodeCalculationNeeded[MAXIMAL_NUMBER_OF_INPUTS];	///< We try to remember which inputnodes we need to be calculated


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AnimationNode-inl.h"
