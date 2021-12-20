#include "model/Sequential.h"
#include "maths/Tensor.h"

#include "MnistLoader.h"

#include <iostream>

static void RegressionExample()
{
	std::vector<std::pair<mocr::DTensor, mocr::DTensor>> data =
	{
		{{1.00000000000000}, {1.00000000000000}},
		{{0.98999583159649}, {0.99651180089436}},
		{{0.97999166319299}, {0.99328127385289}},
		{{0.96998749478949}, {0.99058382007424}},
		{{0.95998332638599}, {0.98606418724558}},
		{{0.94997915798249}, {0.98167057064152}},
		{{0.93997498957899}, {0.97746413091426}},
		{{0.92997082117548}, {0.97364491233801}},
		{{0.91996665277198}, {0.97117764560335}},
		{{0.90996248436848}, {0.96923602987958}},
		{{0.89995831596498}, {0.96699240304967}},
		{{0.88995414756148}, {0.96339118476318}},
		{{0.87994997915798}, {0.95992709987336}},
		{{0.86994581075448}, {0.95680774455266}},
		{{0.85994164235097}, {0.95358181995561}},
		{{0.84993747394747}, {0.95081947819300}},
		{{0.83993330554397}, {0.94949890076125}},
		{{0.82992913714047}, {0.94794136226520}},
		{{0.81992496873697}, {0.94444460650234}},
		{{0.80992080033347}, {0.94096295453595}},
		{{0.79991663192997}, {0.93542647319546}},
		{{0.78991246352646}, {0.92766603885435}},
		{{0.77990829512296}, {0.92034371184549}},
		{{0.77032096706961}, {0.91266835790293}},
		{{0.76031679866611}, {0.90315400330014}},
		{{0.75031263026260}, {0.88940173732553}},
		{{0.74030846185910}, {0.87607069224977}},
		{{0.73030429345560}, {0.86553880849196}},
		{{0.72030012505210}, {0.85459455219674}},
		{{0.71029595664860}, {0.84403972104000}},
		{{0.70029178824510}, {0.83334723790132}},
		{{0.69028761984160}, {0.82145199342563}},
		{{0.68028345143809}, {0.79706850219928}},
		{{0.67027928303459}, {0.77713764906490}},
		{{0.66027511463109}, {0.76001590760365}},
		{{0.65027094622759}, {0.74503378421082}},
		{{0.64026677782409}, {0.73098919819260}},
		{{0.63026260942059}, {0.71845612622625}},
		{{0.62025844101709}, {0.71012602594806}},
		{{0.61025427261358}, {0.69352779633340}},
		{{0.60025010421008}, {0.68154918927293}},
		{{0.59024593580658}, {0.67309595740081}},
		{{0.58024176740308}, {0.66519119429219}},
		{{0.57023759899958}, {0.65814812546004}},
		{{0.56023343059608}, {0.65117078731294}},
		{{0.55022926219258}, {0.64585065327160}},
		{{0.54022509378907}, {0.63234040456782}},
		{{0.53022092538557}, {0.61816153737201}},
		{{0.52021675698207}, {0.60596687471660}},
		{{0.51021258857857}, {0.59692546989498}},
		{{0.50020842017507}, {0.58963299395167}},
		{{0.49020425177157}, {0.58304799798529}},
		{{0.48020008336807}, {0.57702268518923}},
		{{0.47019591496456}, {0.56381486894215}},
		{{0.46019174656106}, {0.54746345351223}},
		{{0.45018757815756}, {0.53172050068112}},
		{{0.44018340975406}, {0.51716893272231}},
		{{0.43017924135056}, {0.50658032869880}},
		{{0.42017507294706}, {0.50034625291339}},
		{{0.41017090454355}, {0.49575291615091}},
		{{0.40016673614005}, {0.48490544083476}},
		{{0.39016256773655}, {0.47285137961512}},
		{{0.38015839933305}, {0.45818663041755}},
		{{0.37015423092955}, {0.44256068339169}},
		{{0.36015006252605}, {0.43073483210963}},
		{{0.35014589412255}, {0.42179066202921}},
		{{0.34014172571904}, {0.41407793753445}},
		{{0.33013755731554}, {0.39769835644116}},
		{{0.32013338891204}, {0.38004691252146}},
		{{0.31012922050854}, {0.36374012783775}},
		{{0.30012505210504}, {0.34912866327836}},
		{{0.29012088370154}, {0.33578118553134}},
		{{0.28011671529804}, {0.32435675167251}},
		{{0.27011254689453}, {0.31531430968033}},
		{{0.26010837849103}, {0.29749416348465}},
		{{0.25010421008753}, {0.28147755968025}},
		{{0.24010004168403}, {0.26590185060400}},
		{{0.23009587328053}, {0.25070030081187}},
		{{0.22009170487703}, {0.23855386658810}},
		{{0.21008753647353}, {0.22719853421868}},
		{{0.20008336807002}, {0.21685800843154}},
		{{0.19007919966652}, {0.19746327524854}},
		{{0.18007503126302}, {0.18165433243974}},
		{{0.17007086285952}, {0.16822235770262}},
		{{0.16006669445602}, {0.15809769304094}},
		{{0.15006252605252}, {0.14801574684222}},
		{{0.14005835764902}, {0.13894838034471}},
		{{0.13005418924551}, {0.13180976217354}},
		{{0.12005002084201}, {0.11586423696495}},
		{{0.11004585243851}, {0.10036346346258}},
		{{0.10004168403501}, {0.08707941517840}},
		{{0.09003751563151}, {0.07529754641095}},
		{{0.08003334722801}, {0.06417509124670}},
		{{0.07002917882451}, {0.05756066556532}},
		{{0.06002501042100}, {0.05139899725052}},
		{{0.05002084201750}, {0.04180092353557}},
		{{0.04001667361400}, {0.03073830014873}},
		{{0.03001250521050}, {0.02047374710605}},
		{{0.02000833680702}, {0.01143276363498}},
		{{0.01000416840350}, {0.00470214243787}},
		{{0.00000000000000}, {0.00000000000000}}
	};

	mocr::Sequential seq(1, 1, 1, mocr::LossFunc::MSE, 0.1);
	seq.addFullyConnectedLayer(16, mocr::ActivationFunc::SIGMOID);
	seq.addFullyConnectedLayer(1, mocr::ActivationFunc::SIGMOID);

	const int iterations = 10000;

	for (int i = 0; i < iterations; i++)
	{
		int choice = rand() % data.size();

		auto inp = data[choice].first;
		auto exp = data[choice].second;

		auto out = seq.feedForward(inp);
		auto err = seq.feedBackward(exp);

		std::cout << "Iteration (" << i << "), Error = " << err << std::endl;
	}

	int points = 50;

	std::ostringstream ss;
	ss << std::setprecision(3) << std::fixed;

	for (int i = 0; i <= points; i++)
	{
		double x = (double)i / (double)points;

		mocr::DTensor inp = {x};
		mocr::DTensor out = seq.feedForward(inp);

		if (i < points)
			ss << "(" << inp[0] << ", " << out[0] << "),";
		else
			ss << "(" << inp[0] << ", " << out[0] << ")" << std::endl;
	}

	std::cout << ss.str();
}

static void MnistExample()
{
	int numTrainImages;
	int trainImageSize;
	int numTrainLabels;
	
	unsigned char **trainImages = read_mnist_images("../res/train-images.idx3-ubyte", numTrainImages, trainImageSize);
	unsigned char  *trainLabels = read_mnist_labels("../res/train-labels.idx1-ubyte", numTrainLabels);

	int imageWidth = static_cast<int>(std::sqrt(trainImageSize));

	std::vector<std::pair<mocr::DTensor, mocr::DTensor>> trainData;

	for (int i = 0; i < numTrainImages; i++)
	{
		mocr::DTensor imageTensor(1, imageWidth, imageWidth);
		for (int j = 0; j < trainImageSize; j++)
		{
			imageTensor[j] = trainImages[i][j];
		}

		mocr::DTensor labelTensor(1, 10, 1);
		labelTensor(0, static_cast<int>(trainLabels[i]), 0) = 1.0;

		trainData.push_back({mocr::DTensor::resize(imageTensor, 1, trainImageSize, 1), labelTensor});
	}

	for (int i = 0; i < numTrainImages; i++)
	{
		delete[] trainImages[i];
	}
	delete[] trainImages;
	delete[] trainLabels;

	mocr::Sequential seq(1, trainImageSize, 1, mocr::LossFunc::MSE, 0.01);
	seq.addFullyConnectedLayer(512, mocr::ActivationFunc::TANH);
	seq.addFullyConnectedLayer(256, mocr::ActivationFunc::TANH);
	seq.addFullyConnectedLayer(64, mocr::ActivationFunc::TANH);
	seq.addFullyConnectedLayer(10, mocr::ActivationFunc::SIGMOID);

	int numIterations = 10000;

	for (int i = 0; i < numIterations; i++)
	{
		int choice = rand() % trainData.size();

		auto inp = trainData[choice].first;
		auto exp = trainData[choice].second;

		auto out = seq.feedForward(inp);
		auto err = seq.feedBackward(exp);

		std::cout << "Iteration (" << i << "), Error = " << err << std::endl;
	}

	trainData.clear();

	int numTestImages;
	int testImageSize;
	int numTestLabels;

	unsigned char **testImages = read_mnist_images("../res/t10k-images.idx3-ubyte", numTestImages, testImageSize);
	unsigned char  *testLabels = read_mnist_labels("../res/t10k-labels.idx1-ubyte", numTrainLabels);

	std::vector<std::pair<mocr::DTensor, mocr::DTensor>> testData;

	for (int i = 0; i < numTestImages; i++)
	{
		mocr::DTensor imageTensor(1, imageWidth, imageWidth);
		for (int j = 0; j < trainImageSize; j++)
		{
			imageTensor[j] = testImages[i][j];
		}

		mocr::DTensor labelTensor(1, 10, 1);
		labelTensor(0, static_cast<int>(testLabels[i]), 0) = 1.0;

		testData.push_back({mocr::DTensor::resize(imageTensor, 1, trainImageSize, 1), labelTensor});
	}

	for (int i = 0; i < numTestImages; i++)
	{
		delete[] testImages[i];
	}
	delete[] testImages;
	delete[] testLabels;

	int numTests = 100;

	for (int i = 0; i <= numTests; i++)
	{
		int choice = rand() % testData.size();

		auto inp = testData[choice].first;
		auto exp = testData[choice].second;
		auto out = seq.feedForward(inp);

		double currentMax = -INFINITY;
		int expected = 0;

		for (int j = 0; j < exp.size(); j++)
		{
			if (exp[j] > currentMax)
			{
				currentMax = exp[j];
				expected = j;
			}
		}

		currentMax = -INFINITY;
		int predicted = 0;

		for (int j = 0; j < out.size(); j++)
		{
			if (out[j] > currentMax)
			{
				currentMax = out[j];
				predicted = j;
			}
		}

		// std::cout << "Input = " << std::endl << mocr::DTensor::resize(inp, 1, imageWidth, imageWidth).str() << std::endl;
		
		std::cout << "Testing " << expected << ", Got " << predicted << "... " 
		          << ((expected == predicted) ? "Correct" : "Incorrect")
		          << std::endl;
	}

	testData.clear();
}

int main(void)
{
	// RegressionExample();
	MnistExample();

	return 0;
}