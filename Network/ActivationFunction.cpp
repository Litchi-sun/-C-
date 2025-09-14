//
//  ActivationFunction.cpp
//
//  Created by 孙李智 on 2025/7/13.
//

//------------------------------------------------------------------------------
//【文件名】ActivationFunction.cpp
//【功能模块和目的】激活函数类的实现
//【开发者及日期】孙李智 2024/7/13
//【更改记录】
//------------------------------------------------------------------------------

// 激活函数类头文件
#include"ActivationFunction.hpp"
// 数学函数头文件
#include<cmath>    
// 异常基类头文件   
#include<stdexcept>   


//------------------------------------------------------------------------------
// 函数名：createAF
// 功能：根据激活函数类型码创建对应的激活函数对象
// 入口参数：激活函数的类型码
// 出口参数：无
// 返回值：指向 ActivationFunction 基类的指针
std::shared_ptr<ActivationFunction> ActivationFunction::createAF(int type)
{
switch (type) {
        // 线性激活
        case 0: 
            return std::make_shared<LinearActivation>();
        // Sigmoid激活
        case 1:  
            return std::make_shared<SigmoidActivation>();
        // Tanh激活
        case 2:  
            return std::make_shared<TanhActivation>();
        // ReLU激活
        case 3:  
            return std::make_shared<ReLUActivation>();
        // 非法处理
        default: 
            return NULL; 
    }
}

// 线性激活函数的实现（类型码0）
// 函数名：operator
// 功能：计算线性激活值，输出等于输入
// 入口参数：输入值
// 出口参数：无
// 返回值：激活后的值
double LinearActivation::operator()(double x) const {
   // 直接返回输入值
    return x;
}

// Sigmoid激活函数的实现（类型码1）
// 函数名：operator
// 功能：计算Sigmoid激活值
// 入口参数：输入值
// 出口参数：无
// 返回值：激活后的值
double SigmoidActivation::operator()(double x) const {
   // Sigmoid变换：1/(1+e^(-x))
    return 1.0 / (1.0 + std::exp(-x)); 
}



// Tanh激活函数的实现（类型码2）
// 函数名：operator
// 功能：计算Tanh激活值
// 入口参数：输入值
// 出口参数：无
// 返回值：激活后的值
double TanhActivation::operator()(double x) const {
    // Tanh变换：(e^x - e^(-x))/(e^x + e^(-x))
    return std::tanh(x);  
}


// ReLU激活函数的实现（类型码3）
// 函数名：operator
// 功能：计算ReLU激活值
// 入口参数：输入值
// 出口参数：无
// 返回值：激活后的值
double ReLUActivation::operator()(double x) const {
    // ReLU变换：max(0, x)
    return (x > 0.0) ? x : 0.0;
}
