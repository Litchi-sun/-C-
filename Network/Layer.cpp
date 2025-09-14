//
//  Layer.cpp
//
//  Created by 孙李智 on 2025/7/14
//

//------------------------------------------------------------------------------
//【文件名】layer.cpp
//【功能模块和目的】神经网络层管理的实现
//【开发者及日期】孙李智 2024/7/14
//【更改记录】
//------------------------------------------------------------------------------

// Layer类头文件
#include "Layer.hpp"
// Neuron类头文件
#include "Neuron.hpp"
// Synapse类头文件
#include "Synapse.hpp"
// std::vector所属头文件
#include <algorithm>
// std::shared_ptr所属头文件
#include <memory>
// std::cout所属头文件
#include <iostream>

// 函数名:Layer
// 功能:含参构造函数，初始化层索引(全局唯一索引)
// 入口参数：size_t layerIndex
// 出口参数：无
// 返回值：无
Layer::Layer(size_t layerIndex)
    : m_LayerIndex(layerIndex) {
}

// 函数名:RemoveNeuron
// 功能:向层中添加指定神经元
// 入口参数：std::shared_ptr<Neuron> neuron
// 出口参数：无
// 返回值：无
void Layer::AddNeuron(std::shared_ptr<Neuron> neuron){
    if (neuron) {
        // 设置神经元所属层指针，存储于Neuron成员变量中
        neuron->SetLayer(shared_from_this());
        m_Neurons.push_back(neuron);
    }
}


// 函数名:RemoveNeuron
// 功能:删除指定索引（全局索引）的神经元
// 入口参数：size_t NeuronIndex
// 出口参数：无
// 返回值：无
void Layer::RemoveNeuron(size_t NeuronIndex) {
    // 在层中查找具有指定全局索引的神经元
    auto it = std::find_if(m_Neurons.begin(), m_Neurons.end(),
        [NeuronIndex](const std::shared_ptr<Neuron>& neuron) {
            return neuron->GetIndex() == NeuronIndex;
        });
    // 如果找到了指定神经元
    if (it != m_Neurons.end()) {
        std::shared_ptr<Neuron> NeuronToRemove = *it;
        // 处理输入连接（树突）
        // 遍历要删除神经元的每个输入突触
        for (auto& dendrite : NeuronToRemove->GetDendrites()) {
            // 获取源神经元
            if (auto source = dendrite->GetSource()) {
                // 从源神经元的轴突列表中移除该突触
                auto& outputs = source->ModifyAxonOutputs();
                auto new_end = std::remove(outputs.begin(), outputs.end(), dendrite);
                outputs.erase(new_end, outputs.end());
            }
        }
        // 处理输出连接（轴突）
        // 遍历要删除神经元的每个输出突触
        for (auto& axon : NeuronToRemove->GetAxonOutputs()) {
            // 获取目标神经元（输出信号的目标）
            if (auto target = axon->GetTarget()) {
                // 从目标神经元的树突列表中移除该突触
                auto& dendrites = target->ModifyDendrites();
                auto new_end = std::remove(dendrites.begin(), dendrites.end(), axon);
                dendrites.erase(new_end, dendrites.end());
            }
        }
        // 清除神经元自身的连接
        NeuronToRemove->ClearDendrites(); 
        NeuronToRemove->ClearAxonOutputs();
        // 从层中移除神经元并重置其层指针
        NeuronToRemove->SetLayer(nullptr);
        m_Neurons.erase(it);
    }
}

// 函数名:ConnectNeurons
// 功能:连接相邻层间的神经元
// 入口参数：std::shared_ptr<Neuron> source, std::shared_ptr<Neuron> target, double weight
// 出口参数：无
// 返回值：bool，连接是否成功
bool Layer::ConnectNeurons(std::shared_ptr<Neuron> source, 
                    std::shared_ptr<Neuron> target, 
                    double weight){
    if (!source || !target) {
        return false; 
    }
    // 获取神经元所在的层，获取原始指针
    Layer* sourceLayer = source->GetLayer().get();
    Layer* targetLayer = target->GetLayer().get();
    // 检查神经元所属层指针是否存在
    if (!sourceLayer || !targetLayer) {
        // 神经元未属于任何层
        return false;
    }
    // 获取当前层在网络中的全局索引
    size_t sourceIndex = sourceLayer->GetIndex();
    size_t targetIndex = targetLayer->GetIndex();
    // 检查是否为相邻层，根据索引判断，索引连续为相邻
    if (targetIndex - sourceIndex != 1 
        || sourceIndex - targetIndex != 1){
        return false;
        }
    // 检查是否已存在突触连接
    const auto& sourceOutputs = source->GetAxonOutputs();
    for (const auto& synapse : sourceOutputs) {
        if (synapse->GetTarget() == target) {
            return false;
        }
    } 
    // 创建突触连接
    auto synapse = std::make_shared<Synapse>(source,target, weight);
    // 添加到目标神经元的树突中
    target->AddDendrite(synapse);
    // 添加到源神经元的轴突输出中
    source->AddAxonOutput(synapse);
    return true;
}

// 函数名:GetNeuronCount
// 功能:获取层中神经元数量
// 入口参数：无
// 出口参数：无
// 返回值：size_t，神经元数量
size_t Layer::GetNeuronCount() const {
    return m_Neurons.size();
}

// 函数名:GetNeuron
// 功能:获取指定全局索引的神经元
// 入口参数：size_t index
// 出口参数：无
// 返回值：std::shared_ptr<Neuron>，神经元指针
std::shared_ptr<Neuron> Layer::GetNeuron(size_t NeuronIndex) {
    auto it = std::find_if(m_Neurons.begin(), m_Neurons.end(),
        [NeuronIndex](const std::shared_ptr<Neuron>& neuron) {
            return neuron->GetIndex() == NeuronIndex;
        });
    // 如果找到了指定神经元
    if (it != m_Neurons.end()) {
        std::shared_ptr<Neuron> NeuronToFind = *it;
        return *it;
    }
    // 未找到返回空指针
    return nullptr;
}

// 函数名:GetIndex
// 功能:获取层的全局索引
// 入口参数：无
// 出口参数：无
// 返回值：size_t，层的全局索引
size_t Layer::GetIndex() const {
    return m_LayerIndex;
}

// 函数名:SetIndex
// 功能:设置层的全局索引
// 入口参数：size_t index
// 出口参数：无
// 返回值：无
void Layer::SetIndex(size_t index) {
    m_LayerIndex = index;
}

// 函数名:ListNeurons
// 功能:列出层中所有神经元信息
// 入口参数：无
// 出口参数：无
// 返回值：无
void Layer::ListNeurons() const {
    for (const auto& neuron : m_Neurons) {
        if (neuron) {
            // 返回神经元信息，包括编号（全局索引）和偏置
            std::cout << "Neuron Index: " << neuron->GetIndex() 
                      << ", Bias: " << neuron->GetBias() 
                      << std::endl;
        }
    }
}

// 函数名:GetNeurons
// 功能:获取层中所有神经元
// 入口参数：无
// 出口参数：无
// 返回值：const std::vector<std::shared_ptr<Neuron>>&，神经元集合
const std::vector<std::shared_ptr<Neuron>>& Layer::GetNeurons() const {
    return m_Neurons;
}

// 函数名:Clear
// 功能:清空层，删除所有神经元和连接
// 入口参数：无
// 出口参数：无
// 返回值：无
void Layer::Clear() {
    // 复制神经元索引列表（防止迭代器失效）
    std::vector<size_t> neuronCopy;
    for (const auto& neuron : m_Neurons) {
        if (neuron) {
            neuronCopy.push_back(neuron->GetIndex());
        }
    }
    // 按全局索引删除所有神经元
    for (size_t index : neuronCopy) {
        // 调用RemoveNeuron处理每个神经元及其连接
        RemoveNeuron(index);
    }
}
