//
// Network.cpp
//
// Created by 孙李智 on 2025/7/13
//
//---------------------------------------------------------------------
//【文件名】      Network.cpp
//【功能模块】    神经网络模型核心实现
//【功能描述】    实现神经网络的结构管理、模型导入导出、验证推理等功能
//【开发者及日期】孙李智 2025-07-15
//【更改记录】
//---------------------------------------------------------------------

// Network类所属头文件
#include "Network.hpp"
// Neuron类所属头文件
#include "Neuron.hpp"
// Synapse类所属头文件
#include "Synapse.hpp"
// 异常基类所属头文件
#include <stdexcept>
// std::vector所属头文件
#include <vector>
// StringList类所属头文件
#include "StringList.hpp"
//哈希映射头文件，用于存储神经元和层的索引映射
#include <unordered_map>
// std::queue所需头文件
#include <queue>      
// std::unordered_set所需头文件
#include<unordered_set>

// 函数名：operator=
// 功能：赋值运算符重载
// 入口参数：const Network& other
// 出口参数：无
// 返回值：引用
Network& Network::operator=(const Network& other){
// 自赋值检查
    if (this == &other) {
        return *this;
    }

    // 清除当前网络的所有内容
    this->RemoveAllLayer();
    inputMarkers.clear();
    outputMarker.clear();
    inputConnections.clear();
    outputConnections.clear();

    // 复制基础属性
    Name = other.Name;
    Comments = other.Comments; // 假设StringList有赋值运算符

    // 创建神经元映射表（旧指针到新指针）
    std::unordered_map<std::shared_ptr<Neuron>, std::shared_ptr<Neuron>> neuronMap;

    // 复制层结构
    for (const auto& layerPtr : other.m_Layers) {
        if (!layerPtr) continue;
        
        // 创建新层
        auto newLayer = std::make_shared<Layer>(layerPtr->GetIndex());
        
        // 复制层中的神经元
        for (const auto& neuronPtr : layerPtr->GetNeurons()) {
            if (!neuronPtr) continue;
            
            // 创建新神经元（深拷贝）
            auto newNeuron = std::make_shared<Neuron>(
                neuronPtr->GetBias(),
                neuronPtr->GetActivationType()
            );
            newNeuron->SetIndex(neuronPtr->GetIndex());
            
            // 保存映射关系
            neuronMap[neuronPtr] = newNeuron;
            
            // 添加到层
            newLayer->AddNeuron(newNeuron);
        }
        
        // 添加新层到网络
        m_Layers.push_back(newLayer);
    }

    // 复制连接关系
    for (const auto& layerPtr : other.m_Layers) {
        if (!layerPtr) continue;
        
        for (const auto& neuronPtr : layerPtr->GetNeurons()) {
            if (!neuronPtr) continue;
            
            auto newNeuron = neuronMap[neuronPtr];
            if (!newNeuron) continue;

            // 复制树突连接
            for (const auto& dendrite : neuronPtr->GetDendrites()) {
                if (!dendrite) continue;
                
                auto source = dendrite->GetSource();
                auto target = dendrite->GetTarget();
                
                if (source && target && neuronMap.count(source) && neuronMap.count(target)) {
                    auto newSynapse = std::make_shared<Synapse>(
                        neuronMap[source],
                        neuronMap[target],
                        dendrite->GetWeight()
                    );
                    newNeuron->AddDendrite(newSynapse);
                    neuronMap[source]->AddAxonOutput(newSynapse);
                }
            }
        }
    }

    // 复制输入/输出标记
    for (const auto& marker : other.inputMarkers) {
        if (marker && neuronMap.count(marker)) {
            inputMarkers.push_back(neuronMap[marker]);
        }
    }
    for (const auto& marker : other.outputMarker) {
        if (marker && neuronMap.count(marker)) {
            outputMarker.push_back(neuronMap[marker]);
        }
    }

    // 复制输入/输出连接
    inputConnections = other.inputConnections;
    outputConnections = other.outputConnections;

    return *this;
}


// 函数名：AddLayer
// 功能：添加空层
// 入口参数：size_t LayerIndex（层索引）
// 出口参数：无
// 返回值：添加成功返回true
bool Network::AddLayer(size_t LayerIndex) {
    auto newLayer = std::make_shared<Layer>(LayerIndex);
    m_Layers.push_back(newLayer);
    return true;
}

// 函数名：RemoveLayer
// 功能：删除一个Layer对象
// 入口参数：层索引，size_t LayerIndex
// 出口参数：无
// 返回值：删除成功返回true
bool Network::RemoveLayer(size_t LayerIndex){
    // 在 m_Layers 容器中查找指定索引的 Layer 对象
    auto it = std::find_if(m_Layers.begin(), m_Layers.end(),
            [LayerIndex](const std::shared_ptr<Layer>& layer) {
            return layer->GetIndex() == LayerIndex;
            });
        // 如果找到了该 Layer 对象
        if (it != m_Layers.end()) {
            // 清空该层中的所有神经元及其连接
            (*it)->Clear();
            // 从 m_Layers 容器中移除该 Layer 对象
            m_Layers.erase(it);
            // 返回 true 表示删除成功
            return true; 
        }
        // 未找到该 Layer 对象，返回 false
        return false; 
}

// 函数名：RemoveAllLayer
// 功能：删除所有的Layer
// 入口参数：无
// 出口参数：无
// 返回值：删除成功返回true
bool Network::RemoveAllLayer(){
    // 遍历每一层
    for (const auto& layer : m_Layers) {
        if (layer) {
            // 调用Clear方法清空层内的所有神经元和连接
            layer->Clear();
        }
    }
    // 清空层列表
    m_Layers.clear();
    return true;
}

// 函数名：GetLayers
// 功能：获取层列表
// 入口参数：无
// 出口参数：无
// 返回值：层列表指针
const std::vector<std::shared_ptr<Layer>>& Network::GetLayers() const {
    return m_Layers;
}


// 函数名：RemoveNeuron
// 功能：删除指定索引的神经元
// 入口参数：神经元索引size_t NeuronIndex
// 出口参数：无
// 返回值：删除成功返回true
bool Network::RemoveNeuron(size_t NeuronIndex){
    // 遍历网络中的每一层
    for (const auto& layer : m_Layers) {
        if (layer) {
            // 在当前层中查找指定索引的神经元
            auto neuronToFind = layer->GetNeuron(NeuronIndex);
            if (neuronToFind) {
                // 若找到该神经元，则调用层的 RemoveNeuron 方法删除它
                layer->RemoveNeuron(NeuronIndex);
                return true;
            }
        }
    }
    // 若遍历完所有层都未找到指定索引的神经元，则返回 false
    return false;
}

// 函数名：ModifyNeuronBias
// 修改指定索引Neuron的偏置
// 入口参数：索引值，更新的Bias
// 出口参数：无
// 返回值：修改成功返回true
bool Network::ModifyNeuronBias(size_t NeuronIndex, double Bias){
    // 遍历网络中的每一层
    for (const auto& layer : m_Layers) {
        if (layer) {
            // 在当前层中查找指定索引的神经元
            auto neuronToFind = layer->GetNeuron(NeuronIndex);
            if (neuronToFind) {
                // 若找到该神经元，则修改其偏置
                neuronToFind->SetBias(Bias);
                return true;
            }
        }
    }
    // 若遍历完所有层都未找到指定索引的神经元，则返回 false
    return false;
}

// 函数名：AddInputConnection
// 功能：添加输入连接
// 入口参数：size_t targetIdx, double weight
// 出口参数：无
// 返回值：无
void Network::AddInputConnection(size_t targetIdx, double weight) {
        inputConnections.emplace_back(targetIdx, weight);
}

// 函数名：AddOutputConnection
// 功能：添加输出连接
// 入口参数：size_t sourceIdx, double weight
// 出口参数：无
// 返回值：无
void Network::AddOutputConnection(size_t sourceIdx, double weight) {
        outputConnections.emplace_back(sourceIdx, weight);
}
    
// 函数名：SetInputMarker
// 功能：设置输入神经元
// 入口参数：std::shared_ptr<Neuron> marker
// 出口参数：无
// 返回值：无
void Network::SetInputMarker(std::shared_ptr<Neuron> marker) {
    if (marker) {
        inputMarkers.push_back(marker);
    }
}

// 函数名：SetOutputMarker
// 功能：设置输出神经元
// 入口参数：std::shared_ptr<Neuron> marker
// 出口参数：无
// 返回值：无
void Network::SetOutputMarker(std::shared_ptr<Neuron> marker) {
    if (marker) {
        outputMarker.push_back(marker);
    }
}
    
// 函数名：GetInputMarker
// 功能：获取输入神经元列表
// 入口参数：无
// 出口参数：无
// 返回值：std::vector<std::shared_ptr<Neuron>>
std::vector<std::shared_ptr<Neuron>> Network::GetInputMarker() const {
    return inputMarkers;
}

// 函数名：GetOutputMarker
// 功能：获取输神经元列表
// 入口参数：无
// 出口参数：无
// 返回值：std::vector<std::shared_ptr<Neuron>>
std::vector<std::shared_ptr<Neuron>> Network::GetOutputMarker() const { 
    return outputMarker;
}

// 函数名：IsVaild
// 功能：判断网络是否合理
// 入口参数：无
// 出口参数：无
// 返回值：合理则返回true
bool Network::IsValid() const {
    // 检查网络基本结构
    if (m_Layers.empty()) return false;
    if (inputMarkers.empty()) return false; // 必须有输入神经元
    if (outputMarker.empty()) return false; // 必须有输出神经元

    // 使用unordered_set存储唯一神经元，避免重复
    std::unordered_set<std::shared_ptr<Neuron>> uniqueNeurons;

    // 1. 从所有层收集神经元
    for (const auto& layer : m_Layers) {
        for (const auto& neuron : layer->GetNeurons()) {
            uniqueNeurons.insert(neuron);
        }
    }

    // 2. 从输入标记添加神经元（若未包含在层中）
    for (const auto& input : inputMarkers) {
        uniqueNeurons.insert(input);
    }

    // 3. 从输出标记添加神经元（若未包含在层中）
    for (const auto& output : outputMarker) {
        uniqueNeurons.insert(output);
    }

    // 转换为vector便于处理
    std::vector<std::shared_ptr<Neuron>> allNeurons(uniqueNeurons.begin(), uniqueNeurons.end());

    // 创建入度计数器和依赖图
    std::unordered_map<std::shared_ptr<Neuron>, size_t> inDegree;
    std::unordered_map<std::shared_ptr<Neuron>, std::vector<std::shared_ptr<Neuron>>> graph;

    for (const auto& neuron : allNeurons) {
        inDegree[neuron] = 0; // 初始化入度为0
    }

    // 构建依赖图和计算入度
    for (const auto& neuron : allNeurons) {
        for (const auto& dendrite : neuron->GetDendrites()) {
            auto source = dendrite->GetSource();
            if (source && uniqueNeurons.count(source)) { // 仅处理网络中的神经元
                graph[source].push_back(neuron);
                inDegree[neuron]++;
            }
        }
    }

    // 拓扑排序检测环状依赖
    std::queue<std::shared_ptr<Neuron>> q;
    for (const auto& input : inputMarkers) {
        if (inDegree[input] == 0) {
            q.push(input);
        } else {
            return false; // 输入神经元不能有入度
        }
    }

    size_t processedCount = 0;
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        processedCount++;
        if (graph.find(current) != graph.end()) {
            for (const auto& next : graph[current]) {
                if (--inDegree[next] == 0) {
                    q.push(next);
                }
            }
        }
    }

    // 检查所有神经元是否被处理（无环且连通）
    if (processedCount != allNeurons.size()) return false;

    // 检查输出神经元是否无后续连接
    for (const auto& output : outputMarker) {
        if (!output->GetAxonOutputs().empty()) {
            return false; // 输出神经元不能有输出连接
        }
    }

    return true;
}


// 函数名：Inference
// 功能：获取输出值列表
// 入口参数：输入值列表
// 出口参数：无
// 返回值：输出值列表
std::vector<double> Network::Inference(const std::vector<double>& input) {
    if (!IsValid()) {
        throw std::runtime_error("Network is not valid for inference");
    }
    // 1. 检查输入数量与输入神经元数量匹配
    if (input.size() != inputMarkers.size()) {
        return {}; // 输入不匹配
    }
    // 2. 为每个输入神经元设置输入值（关键步骤）
    for (size_t i = 0; i < inputMarkers.size(); ++i) {
        inputMarkers[i]->InputMarkSetOutput(input[i]); // 输入值+偏置
    }
    // 3. 执行前向传播（仅对非输入层的神经元计算）
    // 3.1 收集输入层的索引（包含输入神经元的层）
    std::unordered_set<size_t> inputLayerIndices;
    for (const auto& inputNeuron : inputMarkers) {
        if (auto layer = inputNeuron->GetLayer()) {
            inputLayerIndices.insert(layer->GetIndex());
        }
    }
    // 3.2 遍历层，跳过输入层
    for (const auto& layer : m_Layers) {
        if (inputLayerIndices.count(layer->GetIndex())) {
            continue; // 跳过输入层，保留其输出
        }
        // 对非输入层的神经元执行前向传播
        for (const auto& neuron : layer->GetNeurons()) {
            neuron->Forward(); 
        }
    }
    // 4. 收集输出神经元的结果
    std::vector<double> outputs;
    for (const auto& outputNeuron : outputMarker) {
        outputs.push_back(outputNeuron->GetOutput());
    }
    return outputs;
}





// 函数名：AddLayer
// 功能：添加层到层列表中
// 入口参数：新添加的层的智能指针
// 出口参数：无
// 返回值：无
void Network::AddLayer(std::shared_ptr<Layer> layer){
    if (layer) {
        m_Layers.push_back(layer);
    }
}