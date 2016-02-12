#include "Net.h"
namespace sj
{
Net::Net()
{
    m_firstInput = true;
    m_inputNum = 0;
    m_eta = 0.0;
    m_alpha =0.0;
    m_weightRange=0.0;
    m_recentAverageError =0.0;
    m_recentAverageSmoothingFactor = 100.0;
}
Net::Net(int _inputNum)
{
    m_firstInput = true;
    m_inputNum = _inputNum;
    m_eta = 0.015;
    m_alpha = 0.5;
    m_weightRange = 0.3;
    m_recentAverageError =0.0;
    m_recentAverageSmoothingFactor = 100.0;
}

Net::~Net()
{
    // Net release start!
    printf("*************************************************\n");
    printf("Net release start!\n\n");

    // release m_layers
    printf("m_layer release...\n");
    for(int i = 0 ; i < m_layers.size() ; ++i)
    {
        for(int _y = 0 ; _y<m_layersInfo[i]->numNeuron ; ++_y)
        {
           delete m_layers[i][_y];
        }
        delete m_layers[i];
    }
    printf("m_layer release done!\n\n");

    printf("m_layer_tmp release...\n");
    for(int i = 0 ; i < m_layers_tmp.size() ; ++i)
    {
        for(int _y = 0 ; _y<m_layersInfo[i]->numNeuron ; ++_y)
        {
           delete m_layers_tmp[i][_y];
        }
        delete m_layers_tmp[i];
    }
    printf("m_layer_tmp release done!\n\n");

    printf("m_layersDeltaWeight release...\n");
    for(int i = 0 ; i < m_layersDeltaWeight.size() ; ++i)
    {
        for(int _y = 0 ; _y<m_layersInfo[i]->numNeuron ; ++_y)
        {
           delete m_layersDeltaWeight[i][_y];
        }
        delete m_layersDeltaWeight[i];
    }
    printf("m_layersDeltaWeight release done!\n\n");

    //release m_layerActivation
    printf("m_layersActivation release...\n");
    for(int i = 0 ; i < m_layersActivation.size(); ++i)
    {
        delete m_layersActivation[i];
    }
    printf("m_layersActivation release done!\n\n");

    //release m_layerActivation_diff
    printf("m_layerActivation_diff release...\n");
    for(int i = 0 ; i < m_layersActivation_diff.size(); ++i)
    {
        delete m_layersActivation_diff[i];
    }
    printf("m_layerActivation_diff release done!\n\n");

    //release m_layersInfo
    printf("m_layersInfo release...");
    for(int i = 0 ; i < m_layersInfo.size(); ++i)
    {
        delete m_layersInfo[i];
    }
    printf("m_layersInfo done!\n\n");

    printf("Net release done!\n");
    printf("*************************************************\n");
}

void Net::push_back_layer(int _numNeuron)
{
    if(this->m_firstInput)
    {
        // first layer generation
        float **neuron = new float*[_numNeuron];
        float **neuron_tmp = new float*[_numNeuron];
        float **deltaWeight = new float*[_numNeuron];
        for(int i =0 ; i < _numNeuron ; ++i)
        {
            neuron[i] = new float[this->m_inputNum + 1]; // + 1 for bias
            neuron_tmp[i] = new float[this->m_inputNum + 1]; // + 1 for bias
            deltaWeight[i] = new float[this->m_inputNum + 1]; // + 1 for bias
        }

        m_layers.push_back(neuron);
        m_layers_tmp.push_back(neuron_tmp);
        m_layersDeltaWeight.push_back(deltaWeight);

        // first layer activation generation
        float *activation = new float[_numNeuron+1];    // + 1 for bias
        float *activation_diff = new float[_numNeuron+1];    // + 1 for bias but bias doesn't have value
        // initialize
        for(int i = 0 ; i < _numNeuron ; ++i)
        {
            activation[i] = 0.0f;
            activation_diff[i] = 0.0f;
        }
        activation[_numNeuron] = 1.0f;
        m_layersActivation.push_back(activation);
        m_layersActivation_diff.push_back(activation_diff);

        // first layer info generation
        LayerInfo *layerInfo = new LayerInfo;
        layerInfo->numNeuron = _numNeuron;
        layerInfo->numActivation = _numNeuron + 1;      // + 1 for bias
        layerInfo->numWeight = m_inputNum + 1;          // + 1 for bias

        m_layersInfo.push_back(layerInfo);

        this->m_firstInput = false;
        return;
    }

    // layer generation
    float **neuron = new float*[_numNeuron];
    float **neuron_tmp = new float*[_numNeuron];
    float **deltaWeight = new float*[_numNeuron];
    for(int i =0 ; i < _numNeuron ; ++i)
    {
        neuron[i] = new float[m_layersInfo.back()->numActivation]; // Activation
        neuron_tmp[i] = new float[m_layersInfo.back()->numActivation]; // Activation
        deltaWeight[i] = new float[m_layersInfo.back()->numActivation];
    }

    m_layers.push_back(neuron);
    m_layers_tmp.push_back(neuron_tmp);
    m_layersDeltaWeight.push_back(deltaWeight);

    // layer activation generation
    float *activation = new float[_numNeuron+1];
    float *activation_diff = new float[_numNeuron+1];    // + 1 for bias
    // initialize
    for(int i = 0 ; i < _numNeuron ; ++i)
    {
        activation[i] = 0.0f;
        activation_diff[i] = 0.0f;
    }
    activation[_numNeuron] = 1.0f;
    m_layersActivation.push_back(activation);
    m_layersActivation_diff.push_back(activation_diff);

    // first layer info generation
    LayerInfo *layerInfo = new LayerInfo;
    layerInfo->numNeuron = _numNeuron;
    layerInfo->numActivation = _numNeuron + 1;                  // + 1 for bias
    layerInfo->numWeight = m_layersInfo.back()->numActivation;  // there is already  bias in m_layersInfo.back()->numActivation

    m_layersInfo.push_back(layerInfo);
}

void Net::setWeight()
{
    // generate random device for random weights
    random_device rd;
    mt19937_64 mt(rd());
    uniform_real_distribution<float > distribution((-1)*m_weightRange, m_weightRange);

    for(int num_layer = 0 ; num_layer <  this->m_layers.size(); ++num_layer)
    {
        for(int _y = 0 ; _y < m_layersInfo[num_layer]->numNeuron; ++_y)
        {
            for( int _x = 0 ; _x < m_layersInfo[num_layer]->numWeight; ++ _x)
            {
                m_layers[num_layer][_y][_x] = distribution(mt);
                m_layers_tmp[num_layer][_y][_x] = m_layers[num_layer][_y][_x];
                m_layersDeltaWeight[num_layer][_y][_x] = 0;
            }
        }
    }
}

void Net::feedforward(float *_input)
{
    m_input = _input;
    // calculate first layer
    for(int _y = 0 ; _y < m_layersInfo[0]->numNeuron; ++_y)
    {
        m_layersActivation[0][_y] = 0.0;
        for(int _x = 0 ; _x < m_layersInfo[0]->numWeight - 1; ++_x)
        {
            m_layersActivation[0][_y] += m_layers[0][_y][_x] * _input[_x];
        }
        m_layersActivation[0][_y] += m_layers[0][_y][m_layersInfo[0]->numWeight-1] * 1.0f;    // add bias at the end of input!
        m_layersActivation[0][_y] = activate(m_layersActivation[0][_y]);
    }

    for(int num_layer = 1 ; num_layer < m_layers.size(); ++num_layer )
    {
        for(int _y = 0 ; _y < m_layersInfo[num_layer]->numNeuron; ++_y)
        {
            m_layersActivation[num_layer][_y] =0.0;
            for(int _x = 0 ; _x < m_layersInfo[num_layer]->numWeight; ++_x)
            {
                m_layersActivation[num_layer][_y] += m_layers[num_layer][_y][_x] * m_layersActivation[num_layer-1][_x];
            }

            m_layersActivation[num_layer][_y] = activate(m_layersActivation[num_layer][_y]);
        }
    }
}

void Net::backprob(float *target)
{
    // error calculating algorithm
        // reference by : http://www.millermattson.com/dave/?p=54

        m_error = 0.0;

        printf("in bakcprob\n target, activation:");
        for(int i = 0 ; i < m_layersInfo.back()->numNeuron; ++ i)
        {
            float delta = target[i] - m_layersActivation.back()[i];

            printf("%f %f ",target[i], m_layersActivation.back()[i]);
            m_error += delta * delta;
        }
        printf("\n");

        m_error /= m_layersInfo.back()->numNeuron-1;
        m_error = sqrt(m_error);                    //RMS

        printf("m_error: %Lf\n", m_error);
        m_recentAverageError =
                (m_recentAverageError * m_recentAverageSmoothingFactor +m_error)
                /(m_recentAverageSmoothingFactor + 1.0);



    //tmp variables;
    float oldDeltaWeight = 0;

    // output backprob
    for(int _y = 0 ; _y < m_layersInfo.back()->numNeuron ; ++ _y)
    {
        m_layersActivation_diff.back()[_y] = (-1)*activate_diff( m_layersActivation.back()[_y] ) * (target[_y] - m_layersActivation.back()[_y]);

        for(int _x = 0 ; _x < m_layersInfo.back()->numWeight ; ++_x)
        {

            oldDeltaWeight = m_layersDeltaWeight.back()[_y][_x];
            m_layersDeltaWeight.back()[_y][_x] =(
                                                    (-1)*m_eta
                                                    * m_layersActivation_diff.back()[_y]
                                                    * m_layersActivation[m_layersActivation.size()-2][_x]  // m_layersActivation[m_layersActivation.size()-2] is closest hidden
                                                    + m_alpha
                                                    * oldDeltaWeight
                                                );

            m_layers_tmp.back()[_y][_x] = m_layers.back()[_y][_x]
                                            + m_layersDeltaWeight.back()[_y][_x];
        }
    }

    // hidden baccprob
    // start at second of end
    float sigma_weight_diff = 0;
    for(int num_layer = m_layers.size()-2 ; num_layer >= 1 ; --num_layer)
    {
        for(int _y = 0 ; _y < m_layersInfo[num_layer]->numNeuron ; ++ _y)
        {
            sigma_weight_diff =0;
            for(int num_layer_right = 0 ; num_layer_right < m_layersInfo[num_layer+1]->numNeuron ; ++num_layer_right)
            {
                sigma_weight_diff += m_layers[num_layer+1][num_layer_right][_y] * m_layersActivation_diff[num_layer+1][num_layer_right]; //vk
            }

            m_layersActivation_diff[num_layer][_y] = activate_diff(m_layersActivation[num_layer][_y]) * sigma_weight_diff;

            for(int _x = 0 ; _x < m_layersInfo[num_layer]->numWeight ; ++_x)
            {
                oldDeltaWeight = m_layersDeltaWeight[num_layer][_y][_x];
                m_layersDeltaWeight[num_layer][_y][_x] =(
                                                            (-1)*m_eta
                                                            * m_layersActivation_diff[num_layer][_y]
                                                            * m_layersActivation[num_layer-1][_x]
                                                            + m_alpha
                                                            * oldDeltaWeight
                                                        );
                m_layers_tmp[num_layer][_y][_x] = m_layers[num_layer][_y][_x]
                                                + m_layersDeltaWeight[num_layer][_y][_x];
            }
        }
    }

    // hidden to input
    for(int _y = 0 ; _y < m_layersInfo[0]->numNeuron ; ++ _y)
    {
        sigma_weight_diff =0;
        for(int num_layer_right = 0 ; num_layer_right < m_layersInfo[1]->numNeuron ; ++num_layer_right)
        {
            sigma_weight_diff += m_layers[1][num_layer_right][_y] * m_layersActivation_diff[1][num_layer_right] ;
        }
        m_layersActivation_diff[0][_y] = activate_diff(m_layersActivation[0][_y]) * sigma_weight_diff;

        for(int _x = 0 ; _x < m_layersInfo[0]->numWeight-1 ; ++_x)
        {
            oldDeltaWeight = m_layersDeltaWeight[0][_y][_x];
            m_layersDeltaWeight[0][_y][_x] =(
                                                        (-1)*m_eta
                                                        * m_layersActivation_diff[0][_y]
                                                        * m_input[_x]
                                                        + m_alpha
                                                        * oldDeltaWeight
                                                    );
            m_layers_tmp[0][_y][_x] = m_layers[0][_y][_x]
                                            + m_layersDeltaWeight[0][_y][_x];
        }

        oldDeltaWeight = m_layersDeltaWeight[0][_y][m_layersInfo[0]->numWeight-1];
        m_layersDeltaWeight[0][_y][m_layersInfo[0]->numWeight-1] =(
                                                                        (-1)*m_eta
                                                                        * m_layersActivation_diff[0][_y]
                                                                        * 1.0
                                                                        + m_alpha
                                                                        * oldDeltaWeight
                                                                    );

        m_layers_tmp[0][_y][m_layersInfo[0]->numWeight-1] = m_layers[0][_y][m_layersInfo[0]->numWeight-1]
                                        + m_layersDeltaWeight[0][_y][m_layersInfo[0]->numWeight-1];
    }


    for(int i = 0; i<m_layers.size(); ++i)
    {
        for(int _y = 0 ; _y < m_layersInfo[i]->numNeuron ; ++_y)
        {
            for(int _x = 0 ; _x < m_layersInfo[i]->numWeight ; ++_x)
            {
                //m_layers[i][_y][_x] = m_layers_tmp[i][_y][_x];
                m_layers[i][_y][_x] = (m_layers_tmp[i][_y][_x] > 3 )? 3 : m_layers_tmp[i][_y][_x];
                m_layers[i][_y][_x] = (m_layers_tmp[i][_y][_x] < -3 )? -3 : m_layers_tmp[i][_y][_x];
            }
        }
    }
}

float Net::activate(float _x)
{
    return tanh(_x);
}

float Net::activate_diff(float _x)
{
    return 1.0-_x*_x;
}


void Net::save(char *_filePath)
{
    FILE *fp = fopen(_filePath,"wt");

    printf("*************************************************\n");
    printf("Neural Net save start!\n\n");

    // write parameters
    printf("save parameters...\n");

    fprintf(fp, "m_inputNum: %d\n", m_inputNum);
    fprintf(fp, "m_eta: %f\n", m_eta);
    fprintf(fp, "m_alpha: %f\n", m_alpha);

    printf("save parameters...done!\n");

    printf("save Neural Net layers start...\n");
    fprintf(fp, "m_layers.size(): %d\n", m_layers.size() );

    // write Neural Net architecture
    for(int i = 0; i<m_layers.size(); ++i)
    {
        fprintf(fp,"\tm_layersInfo[%d]->numNeuron: %d\n",i, m_layersInfo[i]->numNeuron);
        fprintf(fp,"\tm_layersInfo[%d]->numActivation: %d\n",i, m_layersInfo[i]->numActivation);
        fprintf(fp,"\tm_layersInfo[%d]->numWeight: %d\n",i, m_layersInfo[i]->numWeight);
        fprintf(fp,"\n");
    }

    // write Neural Net
    for(int i = 0; i<m_layers.size(); ++i)
    {
        for(int _y = 0 ; _y < m_layersInfo[i]->numNeuron ; ++_y)
        {
            for(int _x = 0 ; _x < m_layersInfo[i]->numWeight ; ++_x)
            {
                fprintf(fp, "%f ", m_layers[i][_y][_x]);
                //printf("%f ",m_layers[i][_y][_x]);
            }
            fprintf(fp,"\n");
            //printf("\n");
        }
        fprintf(fp,"\n");
        //printf("\n");
    }

    printf("load Neural Net layers done!\n\n");

    printf("Neural Net load start!\n");
    printf("*************************************************\n");

    // release
    fclose(fp);
}

void Net::load(char *_filePath)
{
    FILE *fp = fopen(_filePath,"rt");
    if(fp == NULL){
        printf("file not found error!\n");
        exit(0);
    }

    int _inputNum                  = 0;
    float _eta                     = 0;
    float _alpha                   = 0;

    int _layers_size                = 0;
    int _layersInfo_numNeuron       = 0;
    int _layersInfo_numActivation   = 0;
    int _layersInfo_numWeight       = 0;

    int iter                        = 0;


    printf("*************************************************\n");
    printf("Neural Net load start!\n\n");

    printf("load parameters...\n");
    // read parameters
    fscanf(fp, "m_inputNum: %d\n", &_inputNum);
    fscanf(fp, "m_eta: %f\n", &_eta);
    fscanf(fp, "m_alpha: %f\n", &_alpha);

    printf("load parameters...done!\n");


    // set parameters
    m_inputNum  = _inputNum;
    m_eta       = _eta;
    m_alpha     = _alpha;


    printf("load Neural Net layers start...\n");
    // read m_layers size
    fscanf(fp, "m_layers.size(): %d\n", &_layers_size );

    // read neurons!
    for(int i = 0; i<_layers_size; ++i)
    {
        fscanf(fp,"\tm_layersInfo[%d]->numNeuron: %d\n", &iter, &_layersInfo_numNeuron);
        fscanf(fp,"\tm_layersInfo[%d]->numActivation: %d\n", &iter, &_layersInfo_numActivation);
        fscanf(fp,"\tm_layersInfo[%d]->numWeight: %d\n", &iter, &_layersInfo_numWeight);
        fscanf(fp,"\n");

        // make neurons
        this->push_back_layer(_layersInfo_numNeuron);
    }

    for(int i = 0; i<m_layers.size(); ++i)
    {
        for(int _y = 0 ; _y < m_layersInfo[i]->numNeuron ; ++_y)
        {
            for(int _x = 0 ; _x < m_layersInfo[i]->numWeight ; ++_x)
            {
                fscanf(fp, "%f ", &m_layers[i][_y][_x]);
            }
            fscanf(fp,"\n");
        }
        fscanf(fp,"\n");
    }
    printf("load Neural Net layers done!\n\n");

    printf("Neural Net load done!\n");
    printf("*************************************************\n");

    // release
    fclose(fp);


}

float Net::getRecentAverageError()
{
    return m_recentAverageError;
}

}

