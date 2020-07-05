#pragma once

#include "Tensor.h"

class ConvolutionLayer
{

public:
	ConvolutionLayer() = delete;
	ConvolutionLayer(int iN, int iWidth, int iHeight, int kSize, int kNum);
	~ConvolutionLayer();

	const Tensor<float>& forwardPropagate(const Tensor<float>& input);
	void backwardPropagate(const Tensor<float>& dout, float learningRate);

private:
	Tensor<float> input;
	Tensor<float> output;
	Tensor<float> kernel;

	Tensor<float > gradin;
};

