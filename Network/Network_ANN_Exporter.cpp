//
//  Network_ANN_Exporter.cpp
//
//  Created by 孙李智 on 2025/7/16.
//

//------------------------------------------------------------------------------
//【文件名】Network_ANN_Exporter.cpp
//【功能模块和目的】导出Network到ANN文件的导出器类实现
//【开发者及日期】 2025/7/16
//【更改记录】
//------------------------------------------------------------------------------

//自身类头文件
#include "Network_ANN_Exporter.hpp"
//Exporter基类模版所属头文件
#include "Exporter.hpp"
//Network类所属头文件
#include "Network.hpp"
//size_t所属头文件
#include <cstddef>
//Layer类所属头文件
#include "Layer.hpp"
//Neuron类所属头文件
#include "Neuron.hpp"
//Synapse类所属头文件
#include "Synapse.hpp"
//std::ofstream所属头文件
#include <fstream>
//StringList所属头文件
#include "StringList.hpp"
//std::vector所属头文件
#include <vector>
// 用于控制浮点数输出格式
#include <iomanip> 
// 用于哈希映射
#include <unordered_map>  
// std::map所需头文件
#include <map> 
// std:set所需头文件
#include<set>
// std::numeric_limits<T>所需头文件
#include <limits>

//------------------------------------------------------------------------------
//必要的构造、析构、赋值行为
//------------------------------------------------------------------------------

//函数名：Network_ANN_Exporter
//功能：默认构造函数，手动触发基类构造函数，指定文件扩展名为"ANN"
//入口参数：无
//出口参数：无
//返回值：无
Network_ANN_Exporter::Network_ANN_Exporter()
: Exporter<Network>(std::string{"ANN"}){
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//受保护的 非静态Getter成员函数：需检查所有Getter是否有后置const
//------------------------------------------------------------------------------

//函数名：SaveToStream const
//功能：将Network类型对象导出到文件流的接口override
//入口参数：std::ofstream& Stream, const Network& ANetwork
//出口参数：std::ofstream& Stream
//返回值：无
void Network_ANN_Exporter::SaveToStream(std::ofstream& Stream, const Network& ANetwork) const {
    // 1. 导出原始注释
    for (size_t i = 0; i < ANetwork.Comments.Sum; ++i) {
        std::string comment = ANetwork.Comments[i];
        auto start = std::find_if(comment.begin(), comment.end(), [](int ch) { return !std::isspace(ch); });
        auto end = std::find_if(comment.rbegin(), comment.rend(), [](int ch) { return !std::isspace(ch); }).base();
        comment = (start < end) ? std::string(start, end) : "";
        Stream << "#" << (comment.empty() ? "" : " ") << comment << std::endl;
    }

    // 2. 导出网络名称
    std::string networkName = ANetwork.Name;
    auto nameStart = std::find_if(networkName.begin(), networkName.end(), [](int ch) { return !std::isspace(ch); });
    auto nameEnd = std::find_if(networkName.rbegin(), networkName.rend(), [](int ch) { return !std::isspace(ch); }).base();
    networkName = (nameStart < nameEnd) ? std::string(nameStart, nameEnd) : "";
    if (!networkName.empty()) {
        Stream << "G " << networkName << std::endl;
    }

    // 3. 导出神经元
    std::vector<std::shared_ptr<Neuron>> allNeurons;
    for (const auto& layer : ANetwork.GetLayers()) {
        const auto& neurons = layer->GetNeurons();
        allNeurons.insert(allNeurons.end(), neurons.begin(), neurons.end());
    }
    for (const auto& neuron : allNeurons) {
        Stream << "N " << std::fixed << std::setprecision(1) << neuron->GetBias() << " " << neuron->GetActivationType() << std::endl;
    }

    // 4. 导出层信息，并记录最小/最大索引层（输入/输出层）
    const auto& layers = ANetwork.GetLayers();
    if (layers.empty()) return;

    // 找到最小索引层（输入层）和最大索引层（输出层）
    size_t minLayerIdx = layers[0]->GetIndex();
    size_t maxLayerIdx = layers[0]->GetIndex();
    std::shared_ptr<Layer> inputLayer = layers[0];
    std::shared_ptr<Layer> outputLayer = layers[0];

    for (const auto& layer : layers) {
        if (layer->GetIndex() < minLayerIdx) {
            minLayerIdx = layer->GetIndex();
            inputLayer = layer;
        }
        if (layer->GetIndex() > maxLayerIdx) {
            maxLayerIdx = layer->GetIndex();
            outputLayer = layer;
        }
    }

    // 导出所有层信息
    for (const auto& layer : layers) {
        const auto& neurons = layer->GetNeurons();
        if (neurons.empty()) continue;
        size_t minIdx = neurons[0]->GetIndex(), maxIdx = neurons[0]->GetIndex();
        for (const auto& neuron : neurons) {
            size_t idx = neuron->GetIndex();
            if (idx < minIdx) minIdx = idx;
            if (idx > maxIdx) maxIdx = idx;
        }
        Stream << "L " << minIdx << " " << maxIdx << std::endl;
    }

    // 5. 导出输入层连接（最小索引层）
    const auto& inputNeurons = inputLayer->GetNeurons();
    if (!inputNeurons.empty()) {
        // 输入神经元索引范围
        size_t minInputIdx = inputNeurons[0]->GetIndex();
        size_t maxInputIdx = inputNeurons[0]->GetIndex();
        for (const auto& neuron : inputNeurons) {
            size_t idx = neuron->GetIndex();
            if (idx < minInputIdx) minInputIdx = idx;
            if (idx > maxInputIdx) maxInputIdx = idx;
        }
        // 输入连接注释
        Stream << "# Neuron " << minInputIdx << " to " << maxInputIdx << ": has one Dendrite" << std::endl;
        // 输入连接
        for (const auto& neuron : inputNeurons) {
            Stream << "S -1 " << neuron->GetIndex() << " 1.0" << std::endl;
        }
    }

    // 6. 导出输出层连接
    const auto& outputNeurons = outputLayer->GetNeurons();
    if (!outputNeurons.empty()) {
        // 输出神经元索引范围
        size_t minOutputIdx = outputNeurons[0]->GetIndex();
        size_t maxOutputIdx = outputNeurons[0]->GetIndex();
        for (const auto& neuron : outputNeurons) {
            size_t idx = neuron->GetIndex();
            if (idx < minOutputIdx) minOutputIdx = idx;
            if (idx > maxOutputIdx) maxOutputIdx = idx;
        }
        // 输出连接注释
        Stream << "# Neuron " << minOutputIdx << " to " << maxOutputIdx << ": has one Axon" << std::endl;
        // 输出连接
        for (const auto& neuron : outputNeurons) {
            Stream << "S " << neuron->GetIndex() << " -1 1.0" << std::endl;
        }
    }

    // 7. 导出层间连接
    std::map<size_t, std::vector<std::shared_ptr<Synapse>>> synapsesBySource;
    for (const auto& neuron : allNeurons) {
        for (const auto& axon : neuron->GetAxonOutputs()) {
            auto source = axon->GetSource();
            auto target = axon->GetTarget();
            if (!source || !target) continue;
            size_t sIdx = source->GetIndex();
            synapsesBySource[sIdx].push_back(axon);
        }
    }

    for (auto it = synapsesBySource.begin(); it != synapsesBySource.end(); ++it) {
        size_t sourceIdx = it->first;
        const auto& synapses = it->second;

        // 收集当前源神经元连接的所有目标神经元索引
        std::set<size_t> targetIndices;
        for (const auto& syn : synapses) {
            auto tgt = syn->GetTarget();
            if (tgt) {
                targetIndices.insert(tgt->GetIndex());
            }
        }

        // 计算目标神经元索引范围
        if (!targetIndices.empty()) {
            size_t minTgtIdx = *targetIndices.begin(); 
            size_t maxTgtIdx = *targetIndices.rbegin(); 

            // 生成带具体范围的注释
            Stream << "# Dendrites from Neuron " << sourceIdx 
               << " to Neuron " << minTgtIdx << "~" << maxTgtIdx << std::endl;
        } else {
            // 无目标神经元时的默认注释
            Stream << "# Dendrites from Neuron " << sourceIdx << " to Neuron (none)" << std::endl;
        }

        // 导出突触连接
        for (const auto& syn : synapses) {
            auto tgt = syn->GetTarget();
            size_t tgtIdx = tgt ? tgt->GetIndex() : -1;
            Stream << "S " << sourceIdx << " " << tgtIdx 
                << " " << std::fixed << std::setprecision(4) << syn->GetWeight() << std::endl;
            }
    }
}
//------------------------------------------------------------------------------

