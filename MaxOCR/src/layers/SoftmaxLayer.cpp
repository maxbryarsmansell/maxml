#include "SoftmaxLayer.h"

void SoftmaxLayer::forwardPropagate(const Tensor<float>& input, Tensor<float>& output)
{
	float max = input(0, 0, 0);
	float denom = 0.0f;

	for (int i = 0; i < input.c_; i++)
		if (input(i, 0, 0) > max)
			max = input(i, 0, 0);
		
	for (int i = 0; i < input.c_; i++)
		denom += exp(input(i, 0, 0) - max);

	for (int i = 0; i < output.c_; i++)
		output(i, 0, 0) = exp(input(i, 0, 0) - max) / denom;
}

void SoftmaxLayer::backwardPropagate(const Tensor<float>& input, Tensor<float>& dinput, const Tensor<float>& output, const Tensor<float>& doutput)
{
	dinput.setTo(0.0f);

	/*for (int p = 0; p < input.sX; p++)
		for (int i = 0; i < output.sX; i++)
		{
			if (p == i)
				dinput(p, 0, 0) += dout(i, 0, 0) * output(p, 0, 0) * (1 - output(p, 0, 0));
			else
				dinput(p, 0, 0) += dout(i, 0, 0) * -output(i, 0, 0) * output(p, 0, 0);
		}*/
}

void SoftmaxLayer::updateParameters(float learningRate)
{

}
