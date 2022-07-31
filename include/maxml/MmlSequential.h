#pragma once

#include "maxml/MmlTensor.h"

#include <vector>
#include <variant>
#include <memory>

namespace maxml
{
	/*
	* TODO: Describe a network with a json or xml file.
	*       See the json file below for an idea of how this might work.
	*

	{
		"Name": "MnistNetwork",
		"LossFunc": "MSE",
		"LearningRate": 0.3,
		"Layers": [
			{
				"Kind": "Input",
				"Description": {
					"Channels": 1,
					"Rows": 28,
					"Cols": 28
				}
			},
			{
				"Kind": "Flatten"
				"Description": {
				}
			},
			{
				"Kind": "FullyConnected"
				"Description": {
					"NumOutputs": 256,
					"ActivationFunc": "Sigmoid"
				}
			},
			{
				"Kind": "FullyConnected"
				"Description": {
					"NumOutputs": 64,
					"ActivationFunc": "Sigmoid"
				}
			},
			{
				"Kind": "FullyConnected"
				"Description": {
					"NumOutputs": 10,
					"ActivationFunc": "Sigmoid"
				}
			}
		]
	}
	*/

	struct Layer;

	enum class ActivationFunc
	{
		None = -1,
		Sigmoid = 0,
		Tanh = 1,
		ReLU = 2,
		Softmax = 3
	};

	enum class PoolingFunc
	{
		Max = 0
	};

	enum class LossFunc
	{
		MSE = 0,
		CrossEntropy = 1
	};

	enum class LayerKind
	{
		Input = 0,
		FullyConnected = 1,
		Convolutional = 2,
		Polling = 3,
		Flatten = 4
	};

	struct InputDesc
	{
		size_t Channels = 0;
		size_t Rows = 0;
		size_t Cols = 0;
	};

	struct FullyConnectedDesc
	{
		size_t NumOutputs = 0;
		ActivationFunc ActivFunc = ActivationFunc::None;
	};

	struct ConvolutionalDesc
	{
		size_t NumKernels = 8;
		size_t KernelWidth = 3;
		size_t KernelHeight = 3;
		ActivationFunc ActivFunc = ActivationFunc::None;
	};

	struct PoolingDesc
	{
		size_t TileWidth = 2;
		size_t TileHeight = 2;
		PoolingFunc PoolFunc = PoolingFunc::Max;
	};

	struct FlattenDesc {};

	struct SequentialDesc
	{
		using LayerDesc = std::variant<
			InputDesc,          // Input
			FullyConnectedDesc, // FullyConnected
			ConvolutionalDesc,  // Convolutional
			PoolingDesc,        // Pooling
			FlattenDesc         // Flatten
		>;

		LossFunc ObjectiveFunc = LossFunc::MSE;
		float LearningRate = 0.1f;
		std::vector<LayerDesc> LayerDescs = {};

		static LayerKind getLayerKind(size_t index)
		{
			return static_cast<LayerKind>(index);
		}
	};

	InputDesc makeInput(size_t channels, size_t rows, size_t cols);
	FullyConnectedDesc makeFullyConnected(size_t numOutputs, ActivationFunc activFunc);
	ConvolutionalDesc makeConvolutional(size_t numKernels, size_t kernelWidth, size_t kernelHeight, ActivationFunc activFunc);
	PoolingDesc makePooling(size_t tileWidth, size_t tileHeight, PoolingFunc poolFunc);
	FlattenDesc makeFlatten();

	class Sequential
	{
	public:
		Sequential() = delete;
		Sequential(const SequentialDesc &sequentialDesc);

		Sequential(const Sequential &other) = delete;
		Sequential(const Sequential &&other) = delete;
		Sequential &operator=(const Sequential &other) = delete;
		Sequential &operator=(const Sequential &&other) = delete;

		const Tensor &feedForward(const Tensor &input);
		float feedBackward(const Tensor &expected);

	private:
		const Tensor &dataInputAt(size_t index) const;
		const Tensor &dataOutputAt(size_t index) const;
		const Tensor &deltaInputAt(size_t index) const;
		const Tensor &deltaOutputAt(size_t index) const;

		Tensor &dataInputAt(size_t index);
		Tensor &dataOutputAt(size_t index);
		Tensor &deltaInputAt(size_t index);
		Tensor &deltaOutputAt(size_t index);

	private:
		// Each layer has a pair of tensors for input and output, respectively
		std::vector<std::pair<std::shared_ptr<Tensor>, std::shared_ptr<Tensor>>> m_Data;
		std::vector<std::pair<std::shared_ptr<Tensor>, std::shared_ptr<Tensor>>> m_Delta;
		std::vector<std::shared_ptr<Layer>> m_Layers;

		// TODO: This should probably be specified with an 'output' layer
		LossFunc m_ObjectiveFunc;

		// TODO: This should probably be associated with some kind of 'optimizer' object
		float m_LearningRate;

		SequentialDesc m_SequentialDesc;
	};
}