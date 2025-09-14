//
//  Network.hpp
//
//  Created by 孙李智 on 2025/7/14
//

//------------------------------------------------------------------------------
//【文件名】      Network.hpp
//【功能模块】    神经网络核心管理
//【功能描述】    实现神经网络的创建、修改、验证和推理功能
//【开发者及日期】孙李智 2025-07-15
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Network_hpp
#define Network_hpp

// Layer类头文件
#include "Layer.hpp"
// Neuron类头文件
#include "Neuron.hpp"
// Synapse类头文件
#include "Synapse.hpp"
// std::vector所需头文件
#include <vector>
// std::shared_ptr所需头文件
#include <memory>
// std::cout所需头文件
#include <iostream>
// std::vector所需头文件
#include <algorithm>
// 异常基类所需头文件
#include <stdexcept>
// StringList所需头文件
#include"StringList.hpp"

//------------------------------------------------------------------------------
//【类名】Network
//【功能】神经网络管理类
//【接口说明】
//    默认构造函数
//    拷贝构造函数
//    赋值运算符overload
//    虚析构函数（可能做基类）
//    以多种参数形式添加一个Layer
//    删除一个Layer
//    以多种参数形式修改一个Layer
//    删除所有Layer
//    删除一个Neuron
//    修改一个Neuron
//    添加输入连接
//    添加输出连接
//    设置输入神经元
//    设置输出神经元
//    获取输入神经元列表
//    获取输出神经元列表
//【开发者及日期】 孙李智 2025/7/14
//------------------------------------------------------------------------------
class Network {
public:
    // 默认构造函数
    Network() = default;
    // 赋值运算符overload
    Network& operator=(const Network& other);
    // 添加⼀个Layer对象，添加后内部没有Neuron对象
    bool AddLayer(size_t LayerIndex);
    // 添加一个层到层列表
    void AddLayer(std::shared_ptr<Layer> layer);
    // 删除一个Layer对象，传入对应的层索引
    bool RemoveLayer(size_t LayerIndex);
    // 删除所有的Layer
    bool RemoveAllLayer();
    // 获取层列表
    const std::vector<std::shared_ptr<Layer>>& GetLayers() const;
    // 删除一个Neuron
    bool RemoveNeuron(size_t NeuronIndex);
    // 修改一个Neuron的偏置
    bool ModifyNeuronBias(size_t NeuronIndex, double Bias);
    // 神经网络名称
    std::string Name{std::string{"NO NAME"}};
    // 模型注释
    StringList Comments;
    // 添加输入连接
    void AddInputConnection(size_t targetIdx, double weight);
    // 添加输出连接
    void AddOutputConnection(size_t sourceIdx, double weight);
    // 设置输入神经元
    void SetInputMarker(std::shared_ptr<Neuron> marker);
    // 设置输出神经元
    void SetOutputMarker(std::shared_ptr<Neuron> marker);
    // 获取输入神经元列表
    std::vector<std::shared_ptr<Neuron>> GetInputMarker() const;
    // 获取输出神经元列表
    std::vector<std::shared_ptr<Neuron>> GetOutputMarker() const;
    // 判断是否合理
    bool IsValid() const;
    // 获取输出值列表
    std::vector<double> Inference(const std::vector<double>& input);
private:
    // --------------------------------------------------------------------------
    // 私有数据成员
    // --------------------------------------------------------------------------
    // 层列表
    std::vector<std::shared_ptr<Layer>> m_Layers;
    // 输入神经元列表
    std::vector<std::shared_ptr<Neuron>> inputMarkers;
    // 输出神经元列表
    std::vector<std::shared_ptr<Neuron>> outputMarker;
    // 输入连接：目标神经元索引和权重
    std::vector<std::pair<size_t, double>> inputConnections;  
    // 输出连接：源神经元索引和权重
    std::vector<std::pair<size_t, double>> outputConnections; 
    // 神经网络全局索引
    size_t m_Network_Index;
    // 神经网络输出值列表
    std::vector<double> output;
};

#endif /* Network_hpp */