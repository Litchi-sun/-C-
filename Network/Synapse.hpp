//
// Synapse.hpp
//
// Created by 孙李智 on 2025/7/13
//

//------------------------------------------------------------------------------
//【文件名】Synapse.hpp
//【功能模块和目的】突触类声明与实现，用于存储前驱神经元指针与权重
//【开发者及日期】孙李智 2025/7/13
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Synapse_hpp
#define Synapse_hpp


//std::shared_ptr所属头文件
#include <memory>

// 前向声明神经元类，防止循环依赖
class Neuron;

//------------------------------------------------------------------------------
//【类模版名】Synapse
//【功能】树突基类，存储前驱神经元指针与权重
//【接口说明】
//    默认构造函数
//    以指针与double值为参数，进行构造
//    拷贝构造函数
//    赋值运算符重载
//    虚析构
//    树突全部元素==相等关系运算符重载
//【开发者及日期】 孙李智 2025/7/13
//【更改记录】
//------------------------------------------------------------------------------

class Synapse {
    public:
    //--------------------------------------------------------------------------
    //必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //默认构造函数
    Synapse() = default;
    //以前驱神经元指针和权重为参数的构造函数
    inline Synapse(std::shared_ptr<Neuron> pPreNeuron, 
            std::shared_ptr<Neuron> pPostNeuron,
            double Weight);
    //拷贝构造函数（使用编译器生成的默认版本，按成员拷贝）
    Synapse(const Synapse& Sourse) = default;
    //赋值运算符重载
    inline Synapse& operator=(const Synapse& Source);
    //虚析构函数，默认实现
    inline virtual ~Synapse() = default;
    //--------------------------------------------------------------------------
    //非静态Setter成员函数
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //非静态Getter成员函数：需检查所有Getter是否有后置const
    //--------------------------------------------------------------------------
    // 获取前驱神经元指针
    inline std::shared_ptr<Neuron> GetSource() const;
    // 获取后继神经元指针
    inline std::shared_ptr<Neuron> GetTarget() const;
    // 获取连接权重
    inline double GetWeight() const;
    //树突全部元素==相等关系运算符重载
    inline bool operator==(const Synapse& ASynapse) const;
    //树突任一元素!=不等关系运算符重载
    inline bool operator!=(const Synapse& ASynapse) const;
    private:
    //--------------------------------------------------------------------------
    //受保护数据成员
    //--------------------------------------------------------------------------
    //前驱神经元指针
    std::shared_ptr<Neuron> source; 
    //后继神经元指针
    std::shared_ptr<Neuron> target;
    //连接权重
    double weight;                      
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//必要的构造、析构、赋值行为
//------------------------------------------------------------------------------

//函数名：Synapse
//功能：带参数构造函数，初始化前驱神经元指针和权重
//入口参数：std::shared_ptr<Neuron> pPreNeuron, double Weight
//出口参数：无
//返回值：无
inline Synapse::Synapse(std::shared_ptr<Neuron> pPreNeuron, 
                std::shared_ptr<Neuron> pPostNeuron,
                double Weight)
: source(pPreNeuron), target(pPostNeuron), weight(Weight) {  
}
//------------------------------------------------------------------------------

//函数名：operator=
//功能：赋值运算符重载
//入口参数：const Synapse& Source
//出口参数：无
//返回值：Synapse&，指向自身的引用
inline Synapse& Synapse::operator=(const Synapse& Source) {
    if (this != &Source) {
        source = Source.source;
        target = Source.target;
        weight = Source.weight;
    }
    return *this;
}
//------------------------------------------------------------------------------

//函数名：operator==
//功能：树突全部元素==相等关系运算符重载
//入口参数：const Synapse& ASynapse
//出口参数：无
//返回值：如果前驱神经元指针和权重都相等，返回true；否则返回false
inline bool Synapse::operator==(const Synapse& ASynapse) const {
    return (source == ASynapse.source) 
            && (target == ASynapse.target)
            && (weight == ASynapse.weight);
}
//------------------------------------------------------------------------------

//函数名：operator!=
//功能：树突任一元素!=不等关系运算符重载
//入口参数：const Synapse& ASynapse
//出口参数：无
//返回值：如果前驱神经元指针或权重不相等，返回true；否则返回false
inline bool Synapse::operator!=(const Synapse& ASynapse) const {
    return !(*this == ASynapse);
}
//------------------------------------------------------------------------------

//函数名：GetSource
//功能：获取前驱神经元指针
//入口参数：无
//出口参数：无
//返回值：std::shared_ptr<Neuron>，前驱神经元指针
inline std::shared_ptr<Neuron> Synapse::GetSource() const {
    return source;
}
//------------------------------------------------------------------------------

//函数名：GetTarget
//功能：获取后继神经元指针
//入口参数：无
//出口参数：无
//返回值：std::shared_ptr<Neuron>，后继神经元指针
inline std::shared_ptr<Neuron> Synapse::GetTarget() const {
    return target;
}
//------------------------------------------------------------------------------

//函数名：GetWeight
//功能：获取连接权重
//入口参数：无
//出口参数：无
//返回值：double，连接权重
inline double Synapse::GetWeight() const {
    return weight;
}
//------------------------------------------------------------------------------

#endif /* Synapse.hpp */
// ...existing code...