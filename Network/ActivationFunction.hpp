//
//  ActivationFunction.hpp
//
//  Created by 孙李智 on 2025/7/13.
//

//------------------------------------------------------------------------------
//【文件名】ActivationFunction.hpp
//【功能模块和目的】激活函数类的声明
//【开发者及日期】孙李智 2025/7/13
//【更改记录】
//------------------------------------------------------------------------------

#ifndef ActivationFunction_hpp
#define ActivationFunction_hpp

//std::shared_ptr所属头文件 
#include <memory>      

//------------------------------------------------------------------------------
//【类名】ActivationFunction
//【功能】激活函数基类
//【接口说明】
//    ActivationFunction：抽象基类，定义激活函数接口
//    LinearActivation：实现线性激活函数
//    SigmoidActivation：实现Sigmoid激活函数
//    TanhActivation：实现Tanh激活函数
//    ReLUActivation：实现ReLU激活函数
//【开发者及日期】Lychee 2025/7/13
//------------------------------------------------------------------------------


// 激活函数抽象基类
class ActivationFunction {
public:
    // 默认构造函数
    ActivationFunction() = default;
    // 拷贝构造函数，因无引用类型成员，默认实现
    ActivationFunction(const ActivationFunction&) = delete;
    // 赋值运算符重载，因无引用类型成员，默认实现
    void operator=(const ActivationFunction&) = delete;
    // 虚析构函数，默认实现
    virtual ~ActivationFunction() = default;
    // 根据类型码创建对应的激活函数对象，静态函数
    static std::shared_ptr<ActivationFunction> createAF(int type);
    // 计算激活函数值，子类实现，纯虚函数，运算符()重载
    virtual double operator()(double x) const = 0;
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 使用final修饰的具体激活函数类，防止继续继承
// 线性激活函数的声明（类型码0）
class LinearActivation final : public ActivationFunction {
public:
    double operator()(double x) const override;
};

// Sigmoid激活函数的声明（类型码1）
class SigmoidActivation final : public ActivationFunction {
public:
    double operator()(double x) const override;
};

// Tanh激活函数的声明（类型码2）
class TanhActivation final : public ActivationFunction {
public:
    double operator()(double x) const override;
};

// ReLU激活函数的声明（类型码3）
class ReLUActivation final : public ActivationFunction {
public:
    double operator()(double x) const override;
};
//------------------------------------------------------------------------------

#endif /* ActivationFunction_hpp */
