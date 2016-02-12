#ifndef NET_H
#define NET_H
#include <math.h>
#include <vector>
#include <random>
#include <stdio.h>

using namespace std;

namespace sj
{
typedef struct
{
    int numNeuron;
    int numActivation;
    int numWeight;
} LayerInfo;

class Net
{
public:

    bool m_firstInput;
    int m_inputNum;
    float m_eta;
    float m_alpha;
    float m_weightRange;

    float *m_input;



    float m_error;
    float m_recentAverageError;
    float m_recentAverageSmoothingFactor;


    vector<float**> m_layers;           // neuron == weights
    vector<float**> m_layers_tmp;

    vector<float**> m_layersDeltaWeight;

    vector<float*> m_layersActivation;
    vector<float*> m_layersActivation_diff;
    vector<LayerInfo*> m_layersInfo;

    Net();
    Net(int _inputNum);

    ~Net();

    void push_back_layer(int _numNeuron);
    void setWeight();

    void feedforward(float *_input);
    void backprob(float *_target);
    float activate(float _x);
    float activate_diff(float _x);

    void save(char* _filePath);
    void load(char* _filePath);
    float getRecentAverageError();
};

}

#endif // NET_H
