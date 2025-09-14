//
//  StaticContainer.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】StaticContainer.hpp
//【功能模块和目的】静态容器类模版定义及实现，用于存储不同数据类型元素、长度固定的容器类模版
//【开发者及日期】孙李智 2025/7/12
//------------------------------------------------------------------------------

#ifndef StaticContainer_hpp
#define StaticContainer_hpp

//Container基类模板所属头文件
#include "Container.hpp"
//size_t所属头文件
#include <cstddef>
//std异常基类所属头文件
#include <stdexcept>
//std::string字符串类所属头文件
#include <string>
//std::array长度固定数组类模版所属头文件
#include <array>
//std::find函数所属头文件
#include <algorithm>

//------------------------------------------------------------------------------
//【类模版名】StaticContainer
//【功能】静态容器模版（固定长度），public继承于Container模版
//【接口说明】
//    所有Container基类模版接口（不包括拷贝构造函数）
//    拷贝构造函数
//    赋值运算符overload
//    虚析构（可能做基类）
//    静态数据成员Count(从概念出发，应存在的类属性)
//【开发者及日期】 孙李智 2025/7/12
//------------------------------------------------------------------------------
template<class ET, size_t N>
class StaticContainer : public Container<ET, std::array<ET, N>>{
public:
    //--------------------------------------------------------------------------
    //必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //继承基类构造函数（不包括拷贝构造函数）
    using Container<ET, std::array<ET, N>>::Container;
    //拷贝构造函数
    //派生类的拷贝构造函数时，可以在初始化列表中显示调用基类的拷贝构造函数
    //如果不在初始化列表中调用基类的拷贝构造函数，则会默认调用基类默认构造函数
    inline StaticContainer(const StaticContainer& Source);
    //赋值运算符overload，因需手动调用基类赋值运算符，不可默认实现
    inline StaticContainer& operator=(const StaticContainer& Source);
    //虚析构函数，因可为基类，声明为虚；因无成员动态构造，默认实现
    inline virtual ~StaticContainer() = default;
    //--------------------------------------------------------------------------
    //静态数据成员
    //--------------------------------------------------------------------------
    //静态容器中元素数量数据成员
    static constexpr size_t COUNT{N};
};

//------------------------------------------------------------------------------
//必要的构造、析构、赋值行为
//------------------------------------------------------------------------------

//函数名：StaticContainer
//功能：拷贝构造函数，手动调用基类拷贝构造函数
//入口参数：const StaticContainer& Source
//出口参数：无
//返回值：无
template<class ET, size_t N>
StaticContainer<ET, N>::StaticContainer(const StaticContainer& Source)
: Container<ET, std::array<ET, N>>(Source) {
}
//------------------------------------------------------------------------------

//函数名：operator=
//功能：赋值运算符overload，手动调用基类赋值运算符
//入口参数：const StaticContainer& Source
//出口参数：无
//返回值：静态容器自身的引用
template<class ET, size_t N>
StaticContainer<ET, N>& StaticContainer<ET, N>::operator=(
const StaticContainer<ET, N>& Source) {
    if (this != &Source) {
        //手动调用基类赋值运算符
        Container<ET, std::array<ET, N>>::operator=(Source);
    }
    //返回对象自身的引用
    return *this;
}
//------------------------------------------------------------------------------

#endif /* StaticContainer_hpp */
