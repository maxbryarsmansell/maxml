#include "MmlLayer.h"
#include "MmlActivations.h"
#include "MmlLog.h"

#include "maxml/MmlTensor.h"

namespace maxml
{
	FullyConLayer::FullyConLayer(Tensor &&weights, Tensor &&biases)
		: DeltaWeights(weights.channels(), weights.rows(), weights.cols()), DeltaBiases(weights.channels(), weights.rows(), 1), Weights(std::forward<Tensor>(weights)), Biases(std::forward<Tensor>(biases))
	{
	}

	void FullyConLayer::forward(const Tensor &input, Tensor &output)
	{
		Tensor::matMult(Weights, input, output);
		Tensor::add(output, Biases, output);
	}

	void FullyConLayer::backward(const Tensor &input, const Tensor &output, Tensor &inputDelta, const Tensor &outputDelta)
	{
		Tensor::matMult(Tensor::transpose(Weights), outputDelta, inputDelta);
		Tensor::matMult(outputDelta, Tensor::transpose(input), DeltaWeights);
		Tensor::copy(outputDelta, DeltaBiases);
	}

	void FullyConLayer::update(float learningRate)
	{
		Tensor::aMinusXMultB(Weights, DeltaWeights, learningRate, Weights);
		Tensor::aMinusXMultB(Biases, DeltaBiases, learningRate, Biases);
	}

	ConvLayer::ConvLayer(size_t inChannels, size_t outRows, size_t outCols, const Tensor &kernel)
		: KernelChannels(kernel.channels()), KernelRows(kernel.rows()), KernelCols(kernel.cols()), KernelWindowed(inChannels, kernel.channels(), kernel.rows() * kernel.cols()), InputWindowed(inChannels, kernel.rows() * kernel.cols(), outRows * outCols), DeltaKernelWindowed(inChannels, kernel.channels(), kernel.rows() * kernel.cols()), DeltaInputWindowed(inChannels, kernel.rows() * kernel.cols(), outRows * outCols)
	{
		for (size_t iChan = 0; iChan < inChannels; ++iChan)
		{
			KernelWindowed.fill(iChan, Tensor::resize(kernel, 1, kernel.channels(), kernel.rows() * kernel.cols()));
		}
	}

	void ConvLayer::forward(const Tensor &input, Tensor &output)
	{
		for (size_t winRow = 0; winRow < InputWindowed.rows(); ++winRow)
		{
			for (size_t winCol = 0; winCol < InputWindowed.cols(); ++winCol)
			{
				size_t origRow = winCol % output.rows() + winRow / KernelRows;
				size_t origCol = winCol / output.cols() + winRow % KernelRows;

				for (size_t chan = 0; chan < input.channels(); ++chan)
				{
					InputWindowed(chan, winRow, winCol) = input(chan, origRow, origCol);
				}
			}
		}

		Tensor result = Tensor::matMult(KernelWindowed, InputWindowed);
		result.resize(output.channels(), output.rows(), output.cols());
		result.transpose();

		Tensor::copy(result, output);
	}

	void ConvLayer::backward(const Tensor &input, const Tensor &output, Tensor &inputDelta, const Tensor &outputDelta)
	{
		Tensor deltaOutputWindowed = Tensor::transpose(outputDelta);
		deltaOutputWindowed.resize(inputDelta.channels(), KernelChannels, outputDelta.rows() * outputDelta.cols());

		Tensor::matMult(deltaOutputWindowed, Tensor::transpose(InputWindowed), DeltaKernelWindowed);
		Tensor::matMult(Tensor::transpose(KernelWindowed), deltaOutputWindowed, DeltaInputWindowed);

		for (size_t winRow = 0; winRow < DeltaInputWindowed.rows(); ++winRow)
		{
			for (size_t winCol = 0; winCol < DeltaInputWindowed.cols(); ++winCol)
			{
				size_t origRow = winCol % output.rows() + winRow / KernelRows;
				size_t origCol = winCol / output.cols() + winRow % KernelRows;

				for (size_t chan = 0; chan < input.channels(); ++chan)
				{
					inputDelta(chan, origRow, origCol) = DeltaInputWindowed(chan, winRow, winCol);
				}
			}
		}
	}

	void ConvLayer::update(float learningRate)
	{
		Tensor::aMinusXMultB(KernelWindowed, DeltaKernelWindowed, learningRate, KernelWindowed);
	}

	MaxPoolLayer::MaxPoolLayer(size_t tileWidth, size_t tileHeight)
		: TileWidth(tileWidth), TileHeight(tileHeight)
	{
	}

	void MaxPoolLayer::forward(const Tensor &input, Tensor &output)
	{
		for (size_t iChan = 0; iChan < output.channels(); ++iChan)
		{
			for (size_t iRow = 0; iRow < output.rows(); ++iRow)
			{
				for (size_t iCol = 0; iCol < output.cols(); ++iCol)
				{
					float max = -std::numeric_limits<float>::infinity();

					for (size_t tRow = 0; tRow < TileWidth; ++tRow)
					{
						for (size_t tCol = 0; tCol < TileHeight; ++tCol)
						{
							float val = input(iChan, iRow * TileWidth + tRow, iCol * TileHeight + tCol);

							if (val > max)
							{
								max = val;
							}
						}
					}

					output(iChan, iRow, iCol) = max;
				}
			}
		}
	}

	void MaxPoolLayer::backward(const Tensor &input, const Tensor &output, Tensor &inputDelta, const Tensor &outputDelta)
	{
		inputDelta.fill(0.0);

		for (size_t iChan = 0; iChan < output.channels(); ++iChan)
		{
			for (size_t iRow = 0; iRow < output.rows(); ++iRow)
			{
				for (size_t iCol = 0; iCol < output.cols(); ++iCol)
				{
					float max = output(iChan, iRow, iCol);

					for (size_t tRow = 0; tRow < TileWidth; ++tRow)
					{
						for (size_t tCol = 0; tCol < TileHeight; ++tCol)
						{
							float val = input(iChan, iRow * TileWidth + tRow, iCol * TileHeight + tCol);

							if (val >= max)
							{
								inputDelta(iChan, iRow * TileWidth + tRow, iCol * TileHeight + tCol) = outputDelta(iChan, iRow, iCol);
								break;
							}
						}
					}
				}
			}
		}
	}

	void FlattenLayer::forward(const Tensor &input, Tensor &output)
	{
		Tensor::copy(input, output);
	}

	void FlattenLayer::backward(const Tensor &input, const Tensor &output, Tensor &inputDelta, const Tensor &outputDelta)
	{
		Tensor::copy(outputDelta, inputDelta);
	}

	ActvLayer::ActvLayer(ActivationFunc activation)
		: Activation(activation)
	{
	}

	void ActvLayer::forward(const Tensor &input, Tensor &output)
	{
		switch (Activation)
		{
		case ActivationFunc::Sigmoid:
			Tensor::fastSig(input, output);
			break;
		case ActivationFunc::Tanh:
			Tensor::mapWith(
				input, [](float x)
				{ return tanh(x); },
				output);
			break;
		case ActivationFunc::ReLU:
			Tensor::fastRelu(input, output);
			break;
		}
	}

	void ActvLayer::backward(const Tensor &input, const Tensor &output, Tensor &inputDelta, const Tensor &outputDelta)
	{
		switch (Activation)
		{
		case ActivationFunc::Sigmoid:
			Tensor::fastSigDeriv(output, inputDelta);
			Tensor::mult(inputDelta, outputDelta, inputDelta);
			break;
		case ActivationFunc::Tanh:
			Tensor::zipWith(
				input, outputDelta, [](float x, float y)
				{ return (tanhPrime(x)) * y; },
				inputDelta);
			break;
		case ActivationFunc::ReLU:
			Tensor::fastReluDeriv(input, inputDelta);
			Tensor::mult(inputDelta, outputDelta, inputDelta);
			break;
		}
	}
}