//
// Layer.hpp
//
// Created by 孙李智 on 2025/7/14
//

//---------------------------------------------------------------------
//【文件名】      Layer.hpp
//【功能模块】    神经网络层管理
//【功能描述】    实现层的创建、删除、神经元管理及连接处理
//【开发者及日期】孙李智 2025-07-14
//【更改记录】    待更新，神经元的索引不一定从0开始
//---------------------------------------------------------------------

#ifndef Layer_hpp
#define Layer_hpp

// Neuron类所属头文件
#include "Neuron.hpp"
// Synapse类所属头文件
#include "Synapse.hpp"
// std::vector所属头文件
#include <vector>
// std::shared_ptr所属头文件
#include <memory>
// std::vector所属头文件
#include <algorithm>

// 前向声明，需要在Network中清除突触连接
class Network;

//------------------------------------------------------------------------------
//【类名】Layer
//【功能】神经网络层类
//【接口说明】
//    默认构造函数
//    含参构造函数
//    默认析构函数
//    添加神经元（要求中未提到该功能）
//    删除指定神经元
//    连接神经元（相邻层间连接）
//    获取层中神经元数量
//    获取指定神经元
//    获取层的全局索引
//    设置层的全局索引
//    列出层中所有神经元信息（编号和偏置）
//    获取所有神经元
//    清空层（删除所有神经元和连接）
//【开发者及日期】 孙李智 2025/7/14
//【更改记录】
//------------------------------------------------------------------------------
class Layer : public std::enable_shared_from_this<Layer> {
public:
    // 构造函数，默认实现
    Layer() = default;
    // 含参构造函数
    Layer(size_t layerIndex = 0);
    // 析构函数，默认实现
    ~Layer() = default;
    // 向层中添加指定神经元
    void AddNeuron(std::shared_ptr<Neuron> neuron);
    // 删除指定神经元
    void RemoveNeuron(size_t NeuronIndex);
    // 连接神经元（相邻层间连接）
    bool ConnectNeurons(std::shared_ptr<Neuron> source, 
                        std::shared_ptr<Neuron> target, 
                        double weight);
    // 获取层中神经元数量
    size_t GetNeuronCount() const;
    // 获取指定神经元，输入全局索引
    std::shared_ptr<Neuron> GetNeuron(size_t NeuronIndex);
    // 获取层的全局索引
    size_t GetIndex() const;
    // 设置层的全局索引
    void SetIndex(size_t index);
    // 列出层中所有神经元信息
    void ListNeurons() const;
    // 获取所有神经元
    const std::vector<std::shared_ptr<Neuron>>& GetNeurons() const;
    // 清空层（删除所有神经元和连接）
    void Clear();
private:
    //------------------------------------------------------------------------
    // 私有成员函数
    //------------------------------------------------------------------------
    // 层索引，全局索引（区别于Network中的局部索引）
    size_t m_LayerIndex;
    // 层神经元集合，vector容器存储
    std::vector<std::shared_ptr<Neuron>> m_Neurons;
};

#endif /* Layer.hpp */