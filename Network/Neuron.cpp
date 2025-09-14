//
// Neuron.cpp
//
// Created by 孙李智 on 2025/7/13
//

//---------------------------------------------------------------------
//【文件名】      Neuron.cpp
//【功能模块】    神经元类的实现
//【功能描述】    实现神经元的构造、析构、赋值、激活函数设置等功能
//【开发者及日期】孙李智 2025-07-13
//【更改记录】
//---------------------------------------------------------------------

// Neuron类头文件
#include "Neuron.hpp"
// ActivationFunction类头文件
#include "ActivationFunction.hpp"
// Synapse类头文件
#include "Synapse.hpp"
// 异常基类头文件
#include <stdexcept>
// std::vector所属头文件
#include <algorithm>
// std::make_shared所属头文件
#include<memory>
// std::cout所属头文件
#include<iostream>

//------------------------------------------------------------------------------
// 函数名：Neuron
// 功能：带参数构造函数，初始化偏置和激活函数
// 入口参数：偏置值和激活函数指针
// 出口参数：无
// 返回值：无
Neuron::Neuron(double bias, int activationType)
 {
    // 初始化偏置值
    m_rbias = bias;
    // 设置激活函数
    m_ActivationType = activationType;
    SetActivation(activationType);
}

// 函数名：AddDendrite
// 功能：添加树突连接
// 入口参数：std::shared_ptr<Synapse> dendrite
// 出口参数：无
// 返回值：无
void Neuron::AddDendrite(std::shared_ptr<Synapse> dendrite) {
    if (!dendrite) {
        throw std::invalid_argument("Invalid dendrite pointer");
    }
    m_pdendrites.push_back(dendrite);
}

// 函数名：AddAxonOutput
// 功能：添加轴突连接
// 入口参数：std::shared_ptr<Synapse> axonOutputs
// 出口参数：无
// 返回值：无
void Neuron::AddAxonOutput(std::shared_ptr<Synapse> axonOutputs){
    if (!axonOutputs) {
        throw std::invalid_argument("Invalid axon output pointer");
    }
    m_paxonOutputs.push_back(axonOutputs);
}

// 函数名：SetAxonOutputs
// 功能：设置轴突输出连接
// 入口参数：const std::vector<std::shared_ptr<Synapse>>& outputs
// 出口参数：无
// 返回值：无
void Neuron::SetAxonOutputs(const std::vector<std::shared_ptr<Synapse>>& outputs) {
    if (outputs.empty()) {
        throw std::invalid_argument("Outputs cannot be empty");
    }
    m_paxonOutputs = outputs;
}

// 函数名：Forward
// 功能：前向传播计算
// 入口参数：无
// 出口参数：无
// 返回值：double 输出值
void Neuron::Forward() const{
    double sum = 0.0;
    // 遍历所有树突，计算输入加权和
    for (const auto& dendrite : m_pdendrites) {
        if (dendrite && dendrite->GetSource()) {
            sum += dendrite->GetWeight() * dendrite->GetSource()->GetOutput();
        }
    }
    // 加上偏置
    sum += m_rbias;
    // 应用激活函数
    m_routput = (*m_activation)(sum);
}

// 函数名：GetOutput
// 功能：获取输出值
// 入口参数：无
// 出口参数：无
// 返回值：double 输出值
double Neuron::GetOutput() const {
    return m_routput;
}

// 函数名：InputMarkSetOutput
// 功能：输入神经元设置输出值，输入值加偏置
// 入口参数：double input
// 出口参数：无
// 返回值：无
void Neuron::InputMarkSetOutput(double input){
    m_routput = input + m_rbias;
}

// 函数名：SetBias
// 功能：设置偏置
// 入口参数：double bias
// 出口参数：无
// 返回值：无
void Neuron::SetBias(double bias) {
    m_rbias = bias;
}

// 函数名：GetBias
// 功能：获取偏置
// 入口参数：无
// 出口参数：无
// 返回值：double 输出值
double Neuron::GetBias() const{
    return m_rbias;
}

// 函数名：SetActivation
// 功能：设置激活函数
// 入口参数：int activationType
// 出口参数：无
// 返回值：无
void Neuron::SetActivation(int activationType) {
    m_ActivationType = activationType;
    // 创建激活函数对象，静态调用createAF
    m_activation = ActivationFunction::createAF(m_ActivationType);
    if (!m_activation) {
        throw std::runtime_error("Failed to create activation function");
    }
}

// 函数名：GetActivationTtpe
// 获取激活函数类型
// 入口参数：无
// 出口参数：无
// 返回值：激活函数类型（int）
int Neuron::GetActivationType(){
    return m_ActivationType;
}

// 函数名：GetDendriteCount
// 功能：获取树突数量
// 入口参数：无
// 出口参数：无
// 返回值：size_t 树突数量
size_t Neuron::GetDendriteCount() const {
    return m_pdendrites.size();
}

// 函数名：GetDendrites
// 功能：获取树突列表
// 入口参数：无
// 出口参数：无
// 返回值：const std::vector<std::shared_ptr<Synapse>>& 树突列表
const std::vector<std::shared_ptr<Synapse>>& Neuron::GetDendrites() const {
    return m_pdendrites;
}

// 函数名：ModifyDendrites
// 功能：修改树突列表
// 入口参数：无
// 出口参数：无
// 返回值：std::vector<std::shared_ptr<Synapse>>& 树突列表
std::vector<std::shared_ptr<Synapse>>& Neuron::ModifyDendrites(){
    return m_pdendrites;
}

// 函数名：GetIndex
// 功能：获取神经元索引
// 入口参数：无
// 出口参数：无
// 返回值：size_t 神经元索引
size_t Neuron::GetIndex() const {
    return m_Index;
}

// 函数名：SetIndex
// 功能：设置神经元索引
// 入口参数：size_t index
// 出口参数：无
// 返回值：无
void Neuron::SetIndex(size_t index) {
    m_Index = index;
}

// 函数名：ClearDendrites
// 功能：清空树突列表
// 入口参数：无
// 出口参数：无
// 返回值：无
void Neuron::ClearDendrites() {
    m_pdendrites.clear();
}

// 函数名：ClearAxonOutputs
// 功能：清空轴突输出连接
// 入口参数：无
// 出口参数：无
// 返回值：无
void Neuron::ClearAxonOutputs() {
    m_paxonOutputs.clear();
}

// 函数名：SetLayer
// 功能：设置神经元所属Layer
// 入口参数：std::shared_ptr<Layer> player
// 出口参数：无
// 返回值：无
void Neuron::SetLayer(std::shared_ptr<Layer> player){
    m_player = player;
}

// 函数名：GetLayer
// 功能：获取神经元所属Layer
// 入口参数：无
// 出口参数：无
// 返回值：std::shared_ptr<Layer> player
std::shared_ptr<Layer> Neuron::GetLayer() const{
    return m_player;
}

// 函数名: GetAxonOutputs
// 功能: 获取轴突输出列表
// 入口参数：无
// 出口参数：无
// 返回值：std::vector<std::shared_ptr<Synapse>>& Neuron
const std::vector<std::shared_ptr<Synapse>>& Neuron::GetAxonOutputs() const {
    return m_paxonOutputs;
}

// 函数名：ModifyAxonOutputs
// 功能：修改轴突列表
// 入口参数：无
// 出口参数：无
// 返回值：std::vector<std::shared_ptr<Synapse>>& 轴突列表
std::vector<std::shared_ptr<Synapse>>& Neuron::ModifyAxonOutputs() {
    return m_paxonOutputs;
}

// 函数名：ListSynapses
// 功能：列出神经元中所有突触连接信息
// 入口参数：无
// 出口参数：无
// 返回值：无
void Neuron::ListSynapses() const{
    // 输出神经元信息
    std::cout << "Neuron " << GetIndex() << " connections:" << std::endl;
    
    // 判断是否为输入神经元（没有树突输入）
    bool isInputNeuron = m_pdendrites.empty();
    // 判断是否为输出神经元（没有轴突输出）
    bool isOutputNeuron = m_paxonOutputs.empty();

    // 输出前驱神经元（树突输入）
    std::cout << "  Inputs from:" << std::endl;
    if (isInputNeuron) {
        std::cout << "    INPUT" << std::endl;
    } else {
        for (const auto& dendrite : m_pdendrites) {
            if (auto source = dendrite->GetSource()) {
                std::cout << "    Neuron " << source->GetIndex() 
                          << " (Weight: " << dendrite->GetWeight() << ")" << std::endl;
            }
        }
    }

    // 输出后继神经元（轴突输出）
    std::cout << "  Outputs to:" << std::endl;
    if (isOutputNeuron) {
        std::cout << "    OUTPUT" << std::endl;
    } else {
        for (const auto& axon : m_paxonOutputs) {
            if (auto target = axon->GetTarget()) {
                std::cout << "    Neuron " << target->GetIndex() 
                          << " (Weight: " << axon->GetWeight() << ")" << std::endl;
            }
        }
    }
}

// 函数名：SetOutput
// 功能：不加偏置直接设置输出值
// 入口参数：double output
// 出口参数：无
// 返回值：无
void Neuron::SetOutput(double output) {
    m_routput = output;
}