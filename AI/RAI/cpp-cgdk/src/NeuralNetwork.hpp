#pragma once

#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <vector>
#include <iostream>
#include <cstdio>
#include <string>

#include "MyStrategy.h"
#include "Utility.hpp"

using namespace model;
using std::vector;
using std::cerr;
using std::endl;
using std::string;

class Dendrite
{
public:
  Dendrite (double weight = 0.0, size_t targetNeuronIndex = 0) :
    weight(weight), targetNeuronIndex(targetNeuronIndex)
  {
  }

  void print(FILE* file)
  {
    fprintf(file, "%lf %lu\n", weight, targetNeuronIndex);
  }

  void read(FILE* file)
  {
    fscanf(file, "%lf %lu\n", &weight, &targetNeuronIndex);
  }

  double weight;
  size_t targetNeuronIndex;
};

class Neuron
{
public:

  Neuron(size_t id = 0, double activation = 0.0, double bias = 0.0) :
    id(id), activation(activation), bias(bias)
  {
    delta = 0.0;
  }

  void initDendrites(size_t dendriteNumber)
  {
    dendrites.resize(dendriteNumber);
    for (int i = 0; i < dendriteNumber; ++i)
    {
      dendrites[i].targetNeuronIndex = i;
    }
  }

  void randomizeDendrites()
  {
    for (int i = 0; i < dendrites.size(); ++i)
    {
      dendrites[i].weight = myRandSigned();
    }
  }

  void print(FILE* file)
  {
    fprintf(file, "%lu %lf %lf %lf\n", id, activation, bias, delta);
    fprintf(file, "%lu\n", dendrites.size());
    for (int i = 0; i < dendrites.size(); ++i)
    {
      dendrites[i].print(file);
    }
  }

  void read(FILE* file)
  {
    fscanf(file, "%lu %lf %lf %lf\n", &id, &activation, &bias, &delta);
    size_t dendritesSize;
    fscanf(file, "%lu\n", &dendritesSize);
    dendrites.resize(dendritesSize);
    for (int i = 0; i < dendrites.size(); ++i)
    {
      dendrites[i].read(file);
    }
  }
  vector<Dendrite> dendrites;

  size_t id;
  double activation;
  double bias;
  double delta;
};

class Layer
{
public:
  Layer(size_t neuronNumber = 1)
  {
    initialize(neuronNumber);
  }

  ~Layer()
  {
  }

  void initialize(size_t neuronNumber)
  {
    neurons.resize(neuronNumber);
  }

  Neuron& getNeuron(size_t index)
  {
    return neurons[index];
  }

  void setNeuron(const Neuron& neuron, size_t index)
  {
    neurons[index] = neuron;
  }

  size_t size()
  {
    return neurons.size();
  }

  void print(FILE* file)
  {
    fprintf(file, "%lu\n", neurons.size());
    for (int i = 0; i < neurons.size(); ++i)
    {
      neurons[i].print(file);
    }
  }

  void read(FILE* file)
  {
    size_t neuronsSize;
    fscanf(file, "%lu", &neuronsSize);
    neurons.resize(neuronsSize);
    for (int i = 0; i < neurons.size(); ++i)
    {
      neurons[i].read(file);
    }
  }

  vector<Neuron> neurons;
};

class Network
{
public:
  Network()
  {
  }

  int setNetworkParams(double learningRate, const vector<size_t>& layersSize)
  {
    size_t layerNumber = layersSize.size();

    if (layerNumber < 2)
      return -1;

    this->learningRate = learningRate;
    this->layers.resize(layerNumber);

    for (int i = 0; i < layerNumber; ++i)
    {
      this->layers[i].initialize(layersSize[i]);
    }
    return 0;
  }

  void randomizeDendrites()
  {
    for (int i = 0; i < layers.size(); ++i)
    {
      for (int j = 0; j < layers[i].size(); ++j)
      {
        if (i != (layers.size() - 1))
        {
          layers[i].neurons[j].initDendrites(layers[i + 1].size());
          layers[i].neurons[j].randomizeDendrites();
        }
        if (i != 0)
        {
          layers[i].neurons[j].bias = myRandSigned();
        }
      }
    }
  }

  void setInputs(const vector<double>& inputs)
  {
    for (int i = 0; i < layers[0].size(); ++i)
    {
      layers[0].neurons[i].activation = inputs[i];
    }
  }

  vector<double> getOutput()
  {
    vector<double> outputs(layers.back().size());

    for (int i = 1; i < layers.size(); ++i)
    {
      for (int j = 0; j < layers[i].size(); ++j)
      {
        layers[i].neurons[j].activation = 0.0;
        for (int k = 0; k < layers[i - 1].size(); ++k)
        {
          layers[i].neurons[j].activation +=
              layers[i - 1].neurons[k].activation * layers[i - 1].neurons[k].dendrites[j].weight;
        }
        layers[i].neurons[j].activation += layers[i].neurons[j].bias;
        layers[i].neurons[j].activation = activationFunction(layers[i].neurons[j].activation);
      }
    }
    for (int i = 0; i < outputs.size(); ++i)
    {
      outputs[i] = layers.back().neurons[i].activation;
    }

    return outputs;
  }

  vector<double> getReadyOutput()
  {
    vector<double> outputs(layers.back().size());
    for (int i = 0; i < outputs.size(); ++i)
    {
      outputs[i] = layers.back().neurons[i].activation;
    }
    return outputs;
  }

  double activationFunction(double activation)
  {
    //return tanh(value);   //Currently using tanh
    return (1.0/(1+exp(-activation)));
  }

  double sigmaWeightDelta(size_t layerIndex, size_t neuronIndex)
  {
    double result = 0.0;
    for (int i = 0; i < layers[layerIndex + 1].size(); ++i)
    {
      result +=
          layers[layerIndex].neurons[neuronIndex].dendrites[i].weight * layers[layerIndex + 1].neurons[i].delta;
    }
    return result;
  }

  void update()
  {
    getOutput();
  }

  void doBackPropagation(vector<double> outputs)
  {
    double target, actual, delta;
    for (int i = layers.size() - 1; i > 0; --i)
    {
      for (int j = 0; j < layers[i].size(); ++j)
      {
        if (i == (layers.size() - 1))
        {
          target = outputs[j];
          actual = layers[i].neurons[j].activation;
          delta = (target - actual) * actual * (1 - actual);
          layers[i].neurons[j].delta = delta;
          for (int k = 0; k < layers[i - 1].size(); ++k)
          {
            layers[i - 1].neurons[k].dendrites[j].weight +=
                delta * learningRate * layers[i - 1].neurons[k].activation;
          }

          layers[i].neurons[j].bias += delta * learningRate * 1;
        }
        else
        {
          actual = layers[i].neurons[j].activation;
          delta = actual * (1 - actual) * sigmaWeightDelta(i, j);

          for (int k = 0; k < layers[i - 1].size(); ++k)
          {
            layers[i - 1].neurons[k].dendrites[j].weight +=
                delta * learningRate * layers[i - 1].neurons[k].activation;
          }
          if (i != 0)
          {
            layers[i].neurons[j].bias += delta * learningRate * 1;
          }
        }
      }
    }
  }

  int train(vector<double> inputs, vector<double> outputs)
  {
    setInputs(inputs);
    update();
    doBackPropagation(outputs);

    return 0;
  }

  ~Network()
  {
  }

  void save(string fileName)
  {
    FILE* saveFile = fopen(fileName.c_str(), "w");
    fprintf(saveFile, "%lu\n", layers.size());
    for (int i = 0; i < layers.size(); ++i)
    {
      layers[i].print(saveFile);
    }
    fclose(saveFile);
  }

  void read(string fileName)
  {
    FILE* readFile = fopen(fileName.c_str(), "r");
    if (!readFile)
      return;
    size_t layersSize;
    fscanf(readFile, "%lu\n", &layersSize);
    layers.resize(layersSize);
    for (int i = 0; i < layers.size(); ++i)
    {
      layers[i].read(readFile);
    }
    fclose(readFile);
  }

  vector<Layer> layers;
  double learningRate;
};


#endif // NEURALNETWORK_HPP
