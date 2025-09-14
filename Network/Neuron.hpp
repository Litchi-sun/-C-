//
//  Neuron.hpp
//
//  Created by 孙李智 on 2025/7/13
//

//------------------------------------------------------------------------------
//【文件名】Neuron.hpp
//【功能模块和目的】神经元类声明
//【开发者及日期】孙李智 2025/7/13
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Neuron_hpp
#define Neuron_hpp

// ActivationFunction类头文件
#include "ActivationFunction.hpp"
// Synapse类头文件
#include "Synapse.hpp"
// std::shared_ptr所属头文件
#include <memory>
// std::vector所属头文件
#include <vector>

//------------------------------------------------------------------------------
//【类名】Neuron
//【功能】神经元类
//【接口说明】
//    继承了突触类的全部接口
//    默认构造函数
//    带参数构造函数，初始化偏置和激活函数
//    拷贝构造函数
//    赋值运算符重载
//    虚析构函数（可能做基类）
//    添加树突连接
//    添加轴突连接
//    设置轴突输出连接
//    前向传播计算
//    获取输出值
//    设置偏置
//    获取偏置
//    设置激活函数
//    获取树突数量
//    获取树突列表
//    获取神经元的全局索引
//    设置神经元的全局索引
//    清空树突连接
//    清空轴突输出连接
//    设置神经元所属Layer
//    获取神经元所属Layer
//    获取轴突输出列表
//    修改轴突列表
//    列出该Neuron的突触连接关系信息
//【开发者及日期】 孙李智 2025/7/13
//【更改记录】
//------------------------------------------------------------------------------

// 前向声明层类，防止循环依赖
class Layer;

class Neuron{    
public:
    // 构造函数，默认实现
    Neuron() = default;
    // 带参数构造函数，初始化偏置和激活函数
    Neuron(double bias = 0.0, 
           int ActivationType = 0);
    // 拷贝构造函数，默认实现
    Neuron(const Neuron& Source) = default;
    // 赋值运算符重载，默认实现
    Neuron& operator=(const Neuron& Source) = default;
    // 虚析构函数，默认实现
    virtual ~Neuron() = default;
    // 添加树突连接
    void AddDendrite(std::shared_ptr<Synapse> dendrite);
    // 添加轴突连接
    void AddAxonOutput(std::shared_ptr<Synapse> axonOutputs);
    // 设置轴突输出连接
    void SetAxonOutputs(const std::vector<std::shared_ptr<Synapse>>& outputs);
    // 前向传播计算
    void Forward() const;
    // 获取输出值
    double GetOutput() const;
    // 输入神经元设置输出值，输入值加偏置
    void InputMarkSetOutput(double input);
    // 设置偏置
    void SetBias(double bias);
    // 获取偏置
    double GetBias() const;
    // 设置激活函数，参数为激活函数类型
    void SetActivation(int);
    // 获取激活函数类型
    int GetActivationType();
    // 获取树突数量
    size_t GetDendriteCount() const;
    // 获取树突列表
    const std::vector<std::shared_ptr<Synapse>>& GetDendrites() const;
    // 修改树突列表
    std::vector<std::shared_ptr<Synapse>>& ModifyDendrites();
    // 获取神经元的全局索引
    size_t GetIndex() const;
    // 设置神经元的全局索引
    void SetIndex(size_t index);
    // 清空树突连接
    void ClearDendrites();
    // 清空轴突输出连接
    void ClearAxonOutputs();
    // 设置神经元所属Layer
    void SetLayer(std::shared_ptr<Layer> player);
    // 获取神经元所属Layer
    std::shared_ptr<Layer> GetLayer() const;
    // 获取轴突输出列表
    const std::vector<std::shared_ptr<Synapse>>& GetAxonOutputs() const;
    // 修改轴突列表
    std::vector<std::shared_ptr<Synapse>>& ModifyAxonOutputs();
    // 列出神经元中所有突触连接信息
    void ListSynapses() const;
    // 直接设置输出值
    void SetOutput(double output);
private:
    // --------------------------------------------------------------------------
    // 私有数据成员
    // --------------------------------------------------------------------------
    // 偏置值
    double m_rbias;
    // 输出值
    mutable double m_routput;
    // 激活函数指针
    std::shared_ptr<ActivationFunction> m_activation;
    // 神经元所属Layer的指针
    std::shared_ptr<Layer> m_player;
    // 神经元的全局索引，用于唯一标识神经元在网络中的位置（区别在Layer中的局部索引）
    size_t m_Index;
    // 激活函数类型
    int m_ActivationType;
    // 输入树突列表
    std::vector<std::shared_ptr<Synapse>> m_pdendrites;
    // 轴突输出连接列表
    std::vector<std::shared_ptr<Synapse>> m_paxonOutputs;
};

#endif /* Neuron.hpp */
