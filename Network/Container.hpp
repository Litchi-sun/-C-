//
//  Container.hpp
//
//  Created by 孙李智 on 2025/7/13.
//

//------------------------------------------------------------------------------
//【文件名】Container.hpp
//【功能模块和目的】容器类模版定义及实现，用于存储不同数据类型元素的容器类模版
//【开发者及日期】孙李智 2025/7/13
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Container_h
#define Container_h

// size_t所属头文件
#include <cstddef>
// std异常基类所属头文件
#include <stdexcept>
// std::string字符串类所属头文件
#include <string>
// std::find函数所属头文件
#include <algorithm>

//------------------------------------------------------------------------------
//【类模版名】Container
//【功能】容器基类模版
//【接口说明】
//    默认构造函数
//    以"CT类型常引用"为参数，进行构造
//    拷贝构造函数
//    赋值运算符overload
//    虚析构
//    返回元素引用的[]下标运算符overload
//    返回元素值的[]下标运算符overload，const版本
//    两容器全部元素==相等关系运算符overload
//    两容器任一元素!=不等关系运算符overload
//    判断容器是否为空的IsEmpty函数
//    判断某元素是否存在的Exists函数
//【开发者及日期】 孙李智 2025/7/13
//【更改记录】
//------------------------------------------------------------------------------


//ET为元素类型，CT为stl容器类型
template<class ET, class CT>
class Container{
public:
    //--------------------------------------------------------------------------
    // 内嵌异常类
    //--------------------------------------------------------------------------
    // 在超下标范围访问元素时，需抛出此异常
    class INDEX_OUTOFRANGE : std::out_of_range{
    public:
        INDEX_OUTOFRANGE(size_t Index, size_t MaxIndex) : std::out_of_range(
            std::string("Index ")
            + std::to_string(Index)
            + std::string(" is out of range 0-")
            + std::to_string(MaxIndex)) {
        }
    };
    //--------------------------------------------------------------------------
    // 必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    // 默认构造函数
    Container() = default;
    // 以CT为参数的构造函数
    Container(const CT& Source);
    // 拷贝构造函数，因无引用类型成员，默认实现
    Container(const Container& Source) = default;
    // 赋值运算符overload，因无引用类型成员，默认实现
    Container& operator=(const Container& Source) = default;
    // 虚析构函数，默认实现
    virtual ~Container() = default;
    //--------------------------------------------------------------------------
    // 非静态Setter成员函数
    //--------------------------------------------------------------------------
    // 返回元素引用的[]下标运算符overload
    ET& operator[](size_t Index);
    //--------------------------------------------------------------------------
    // 非静态Getter成员函数：需检查所有Getter是否有后置const
    //--------------------------------------------------------------------------
    // 返回元素值的[]下标运算符overload
    ET operator[](size_t Index) const;
    // 两容器全部元素==相等关系运算符overload
    bool operator==(const Container& AContainer) const;
    // 两容器任一元素!=不等关系运算符overload
    bool operator!=(const Container& AContainer) const;
    // 判断容器是否为空，空返回true，非空返回false
    bool IsEmpty() const;
    // 判断某元素是否存在，存在返回true，不存在返回false
    bool Exists(const ET& Element) const;
protected:
    //--------------------------------------------------------------------------
    // 受保护数据成员
    //--------------------------------------------------------------------------
    // 元素的实际存储
    CT m_Elements;
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 必要的构造、析构、赋值行为
//------------------------------------------------------------------------------

// 函数名：Container
// 功能：带参数构造函数，初始化m_Elements
// 入口参数：const CT& Source，预期为stl容器
// 出口参数：无
// 返回值：无
template<class ET, class CT>
Container<ET, CT>::Container(const CT& Source){
    m_Elements = Source;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 非静态Setter成员函数
//------------------------------------------------------------------------------

// 函数名：operator[]
// 功能：返回元素引用的下标运算符overload，越界抛出异常
// 入口参数：Index指定下标
// 出口参数：无
// 返回值：指定下标元素的引用
template<class ET, class CT>
ET& Container<ET, CT>::operator[](size_t Index){
    //超下标范围访问，则抛出异常
    if (Index >= m_Elements.size()) {
        throw INDEX_OUTOFRANGE(Index, m_Elements.size() - 1);
    }
    return m_Elements[Index];
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 非静态Getter成员函数：需检查所有Getter是否有后置const
//------------------------------------------------------------------------------

// 函数名：operator[] const
// 功能：返回元素值的下标运算符overload，越界抛出异常
// 入口参数：Index指定下标
// 出口参数：无
// 返回值：指定下标元素值
template<class ET, class CT>
ET Container<ET, CT>::operator[](size_t Index) const{
    // 超下标范围访问，则抛出异常
    if (Index >= m_Elements.size()) {
        throw INDEX_OUTOFRANGE(Index, m_Elements.size() - 1);
    }
    return m_Elements[Index];
}
//------------------------------------------------------------------------------

// 函数名：operator== const
// 功能：比较两个容器是否相等，通过CT对象的比较行为实现
//      如CT为stl::array或vector，则逐一元素顺序比较
// 入口参数：const Container& AContainer
// 出口参数：无
// 返回值：全部元素均相等，返回true；任一元素不等，返回false
template<class ET, class CT>
bool Container<ET, CT>::operator==(const Container& AContainer) const{
    return m_Elements == AContainer.m_Elements;
}
//------------------------------------------------------------------------------

// 函数名：operator!= const
// 功能：比较两个容器是否相等，通过CT对象的比较行为实现
//      如CT为stl::array或vector，则逐一元素顺序比较
// 入口参数：const Container& AContainer
// 出口参数：无
// 返回值：全部元素均相等，返回false；任一元素不等，返回true
template<class ET, class CT>
bool Container<ET, CT>::operator!=(const Container& AContainer) const{
    //调用operator==是为了降低代码修改出现错误的风险，只需要改一处
    return !(*this == AContainer);
}
//------------------------------------------------------------------------------

//函数名：IsEmpty const
//功能：判断容器是否为空
//入口参数：无
//出口参数：无
//返回值：容器为空，返回true；容器不为空，返回false
template<class ET, class CT>
bool Container<ET, CT>::IsEmpty() const {
    return m_Elements.empty();
}
//------------------------------------------------------------------------------

//函数名：Exists const
//功能：判断容器中是否存在指定元素
//入口参数：const ET& Element
//出口参数：无
//返回值：存在1个及更多，返回true；不存在，返回false
template<class ET, class CT>
bool Container<ET, CT>::Exists(const ET& Element) const {
    //搜索第一个同值元素的迭代器
    auto it = std::find(m_Elements.begin(), m_Elements.end(), Element);
    //迭代器不为尾迭代器，则返回true，否在返回false
    bool Res = (it != m_Elements.end());
    return Res;
}
//------------------------------------------------------------------------------

#endif /* Container_h */
