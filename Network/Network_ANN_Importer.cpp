//
//  Network_ANN_Importer.cpp
//
//  Created by 孙李智 on 2024/7/21.
//

//------------------------------------------------------------------------------
//【文件名】Network_ANN_Importer.cpp
//【功能模块和目的】导入ANN文件到Network的导入器类实现
//【开发者及日期】孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

//自身类头文件
#include "Network_ANN_Importer.hpp"
//Importer基类模版所属头文件
#include "Importer.hpp"
//Network类所属头文件
#include "Network.hpp"
//Synapse类所属头文件
#include "Synapse.hpp"
//Layer类所属头文件
#include "Layer.hpp"
//Neuron类所属头文件
#include "Neuron.hpp"
//std::ifstream所属头文件
#include <fstream>
//字符串流头文件，用于解析行数据
#include<sstream>
//StringList所属头文件
#include "StringList.hpp"
//std::vector所属头文件
#include <vector>
//哈希映射头文件，用于存储神经元和层的索引映射
#include <unordered_map>

//------------------------------------------------------------------------------
//必要的构造、析构、赋值行为
//------------------------------------------------------------------------------

//函数名：Network_ANN_Importer
//功能：默认构造函数，手动触发基类构造函数，指定文件扩展名为"ANN"
//入口参数：无
//出口参数：无
//返回值：无
Network_ANN_Importer::Network_ANN_Importer()
: Importer<Network>(std::string("ANN")){
}

//--------------------------------------------------------------------------
//受保护的 非静态Getter成员函数：需检查所有Getter是否有后置const
//--------------------------------------------------------------------------

//函数名：LoadFromStream const
//功能：从文件流导入到Network对象接口override
//入口参数：std::ifstream& Stream
//出口参数：无
//返回值：Network对象
Network Network_ANN_Importer::LoadFromStream(std::ifstream& Stream) const{
    //临时，空神经网络对象
    Network TempNetwork;
    //
    std::string line;
    //存储神经元的映射
    std::unordered_map<size_t, std::shared_ptr<Neuron>> neuronMap;
    //存储层的映射
    std::unordered_map<size_t, std::shared_ptr<Layer>> layerMap;
    //临时存储神经元信息
    std::vector<std::shared_ptr<Neuron>> neurons;
    //临时存储层信息
    std::vector<std::shared_ptr<Layer>> layers;
    //层的数量
    size_t LayerCount = 0;

    while (std::getline(Stream, line)) {
        std::istringstream iss(line);
        char tag;
        iss >> tag;
        switch (tag) {
            // 注释行
            case '#': { 
                std::string comment = line.substr(line.find('#') + 1);
                TempNetwork.Comments.AddToEnd(comment);
                break;
            }
            // 网络名称
            case 'G': {  
                std::string name;
                std::getline(iss, name);  // 读取剩余部分
                TempNetwork.Name = name;
                break;
            }
            // 神经元信息，偏置与激活函数类型
            case 'N': { 
                double bias;
                int activationType;
                if (iss >> bias >> activationType) {
                    auto neuron = std::make_shared<Neuron>(bias, activationType);
                    // 设置全局索引
                    neuron->SetIndex(neurons.size()); 
                    neurons.push_back(neuron);
                    neuronMap[neurons.size() - 1] = neuron;
                }
                break;
            }
            
            // 层信息，此层中神经元最⼩和最⼤索引
            case 'L': {  
                size_t startIdx;
                size_t endIdx;
                if (iss >> startIdx >> endIdx) {
                    auto layer = std::make_shared<Layer>(LayerCount++);
                    for (size_t i = startIdx; i <= endIdx; ++i) {
                        if (neuronMap.find(i) != neuronMap.end()) {
                            layer->AddNeuron(neuronMap[i]);
                        }
                    }
                    TempNetwork.AddLayer(layer);
                }
                break;
            }
            
            case 'S': {  // 突触连接
                int sourceIdx;
                int targetIdx;
                double weight;
                if (iss >> sourceIdx >> targetIdx >> weight) {
                    std::shared_ptr<Neuron> source;
                    std::shared_ptr<Neuron> target;

                    // 处理源神经元索引为-1（输入连接）
                    if (sourceIdx == -1) {
                        if (neuronMap.find(targetIdx) != neuronMap.end()) {
                            target = neuronMap[targetIdx];
                            // 将目标神经元标记为输入神经元
                            TempNetwork.SetInputMarker(target); 
                            // 跳过创建突触（输入神经元无前置连接）
                            continue; 
                        } else {
                            continue;
                    }
                } 
                //处理正常神经元
                else if (neuronMap.find(sourceIdx) != neuronMap.end()) {
                            source = neuronMap[sourceIdx];
                } else {
                    // 如果找不到源神经元，跳过或报错
                    continue;
                }

                // 处理目标神经元索引为-1（输出连接）
                if (targetIdx == -1) {
                    // 将源神经元标记为输出神经元，不创建新神经元
                    TempNetwork.SetOutputMarker(source); 
                    // 不创建突触（输出神经元无后置连接）
                    continue;
                }
                else if (neuronMap.find(targetIdx) != neuronMap.end()) {
                    target = neuronMap[targetIdx];
                } else {
                    continue;
                }

                // 创建突触连接（确保源和目标都有效）
                if (source && target) {
                    auto synapse = std::make_shared<Synapse>(source, target, weight);
                    source->AddAxonOutput(synapse);
                    target->AddDendrite(synapse);
            
                // 关联到源神经元所在层
                if (auto sourceLayer = source->GetLayer()) {
                    sourceLayer->ConnectNeurons(source, target, weight);
                }
                }
                }
                break;  
            }
        }
    }
    return TempNetwork;
}
