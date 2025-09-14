//
//  DynamicContainer.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】DynamicContainer.hpp
//【功能模块和目的】动态容器类模版定义及实现，用于存储不同数据类型元素、长度可变的容器类模版
//【开发者及日期】孙李智 2025/7/12
//【更改记录】
//-------------------------------------------------P-----------------------------

#ifndef DynamicContainer_hpp
#define DynamicContainer_hpp


//Container基类模板所属头文件
#include "Container.hpp"
//size_t所属头文件
#include <cstddef>
//std::string字符串类所属头文件
#include <string>
//std::vector可变长度数组类模版所属头文件
#include <vector>
//std::find函数所属头文件
//std::remove函数所属头文件
#include <algorithm>

//------------------------------------------------------------------------------
//【类模版名】DynamicContainer
//【功能】动态容器模版（可变长度），public继承于Container模版
//【接口说明】
//    所有Container基类模版接口（不包括拷贝构造函数）
//    拷贝构造函数
//    赋值运算符重载
//    虚析构（可能做基类）
//    添加元素到末尾
//    插入元素到指定位置
//    删除指定位置元素
//    删除第一个指定值元素
//    删除末一个指定值元素
//    删除全部指定值元素
//    非静态数据成员Sum，表示容器中元素总数
//【开发者及日期】 孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

//ET为元素类型，继承自Container<ET, std::vector<ET>>的容器类模版
template<class ET>
class DynamicContainer: public Container<ET, std::vector<ET>>{
public:
    //--------------------------------------------------------------------------
    //必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //继承基类构造函数（不含拷贝构造函数）
    using Container<ET, std::vector<ET>>::Container;
    //拷贝构造函数
    DynamicContainer(const DynamicContainer& Source);
    //赋值运算符overload
    DynamicContainer& operator=(const DynamicContainer& Source);
    //虚析构函数
    virtual ~DynamicContainer() = default;
    //添加元素到末尾
    void AddToEnd(const ET& Element);
    //插入元素到指定位置
    void InsertAT(size_t Index, const ET& Element);
    //删除指定位置元素
    void RemoveAT(size_t Index);
    //删除第一个指定值元素
    void RemoveFirst(const ET& Element);
    //删除末一个指定值元素
    void RemoveLast(const ET& Element);
    //删除所有指定值元素
    void RemoveAll(const ET& Element);
    //清空
    void Clear();
private:
    //--------------------------------------------------------------------------
    //私有数据成员
    //--------------------------------------------------------------------------
    //元素总个数，初始化为0
    size_t m_Sum{0};
public:
    //--------------------------------------------------------------------------
    //非静态Getter成员：常引用类型
    //--------------------------------------------------------------------------
    //元素总个数常引用
    const size_t& Sum{m_Sum};
};

//------------------------------------------------------------------------------
//必要的构造、析构、赋值行为
//------------------------------------------------------------------------------

//函数名：DynamicContainer
//功能：拷贝构造函数，手动调用基类拷贝构造函数，并初始化m_Sum
//入口参数：const DynamicContainer& Source
//出口参数：无
//返回值：无
template<class ET>
DynamicContainer<ET>::DynamicContainer(const DynamicContainer& Source)
: Container<ET, std::vector<ET>>(Source){
    this->Container<ET, std::vector<ET>>::operator==(Source);
    this->m_Sum = Source.m_Sum;
}
//------------------------------------------------------------------------------

//函数名：operator=
//功能：赋值运算符重载，手动调用基类赋值运算符，并赋值m_Sum
//入口参数：const DynamicContainer& Source
//出口参数：无
//返回值：动态容器自身的引用
template<class ET>
DynamicContainer<ET>& DynamicContainer<ET>::operator=
(const DynamicContainer<ET>& Source){
    if (this != &Source) {
        this->Container<ET, std::vector<ET>>::operator=(Source);
        this->m_Sum = Source.m_Sum;
    }
    return *this;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//非静态Setter成员函数
//------------------------------------------------------------------------------

//函数名：InsertAT
//功能：在指定位置，插入指定元素
//入口参数：size_t Index, const ET& Element
//出口参数：无
//返回值：无，超范围插入抛出异常
template<class ET>
void DynamicContainer<ET>::InsertAT(size_t Index, const ET& Element){
    //插入到末尾
    if (Index == this->m_Elements.size()) {
        //实际插入
        this->m_Elements.insert(this->m_Elements.end(), Element);
    }
    //插入到其他为止，检查越界
    else {
        //通过访问指定Index位置元素的引用，越界引发基类部分抛出异常
        ET Temp = Container<ET, std::vector<ET>>::operator[](Index);
        //回写，放置编译器优化掉上调只读行为语句
        Container<ET, std::vector<ET>>::operator[](Index) = Temp;
        //实际插入
        this->m_Elements.insert(this->m_Elements.begin() + Index, Element);
    }
    //更新容器元素总个数
    this->m_Sum = this->m_Elements.size();
}
//------------------------------------------------------------------------------

//函数名：AddToEnd
//功能：在末尾添加指定元素
//入口参数：const ET& Element
//出口参数：无
//返回值：无
template<class ET>
void DynamicContainer<ET>::AddToEnd(const ET& Element){
    //实际添加
    this->m_Elements.push_back(Element);
    //更新容器元素总个数
    this->m_Sum = this->m_Elements.size();
}
//------------------------------------------------------------------------------

//函数名：RemoveAT
//功能：删除指定位置元素
//入口参数：size_t Index
//出口参数：无
//返回值：无。超范围删除抛出异常
template<class ET>
void DynamicContainer<ET>::RemoveAT(size_t Index){
    //通过访问指定Index位置元素的引用，越界引发基类部分抛出异常
    ET Temp = Container<ET, std::vector<ET>>::operator[](Index);
    //回写，放置编译器优化掉上调只读行为语句
    Container<ET, std::vector<ET>>::operator[](Index) = Temp;
    //实际删除
    this->m_Elements.erase(this->m_Elements.begin() + Index);
    //更新容器元素总个数
    this->m_Sum = this->m_Elements.size();
}
//------------------------------------------------------------------------------

//函数名：RemoveFirst
//功能：删除第一个指定值元素
//入口参数：const ET& Element
//出口参数：无
//返回值：无
template<class ET>
void DynamicContainer<ET>::RemoveFirst(const ET& Element){
    //搜索第一个指定值元素迭代器
    auto firstE = std::find(this->m_Elements.begin(),
                        this->m_Elements.end(),
                        Element);
    //如找到指定值元素，删除
    if (firstE != this->m_Elements.end()) {
        this->m_Elements.erase(firstE);
    }
    //更新容器元素总个数
    this->m_Sum = this->m_Elements.size();
}
//------------------------------------------------------------------------------

//函数名：RemoveLast
//功能：删除末一个与给定值相同的元素
//入口参数：const ET& Element
//出口参数：无
//返回值：无
template<class ET>
void DynamicContainer<ET>::RemoveLast(const ET& Element){
    //末一个指定值元素迭代器（反向迭代器）
    auto lastE = std::find(this->m_Elements.rbegin(), 
                        this->m_Elements.rend(),
                        Element);
    //如找到指定值元素，删除
    if (lastE != this->m_Elements.end()) {
        this->m_Elements.erase(lastE);
    }
    //更新容器元素总个数
    this->m_Sum = this->m_Elements.size();
}
//------------------------------------------------------------------------------

//函数名：RemoveAll
//功能：删除所有与给定值相同的元素
//入口参数：const ET& Element
//出口参数：无
//返回值：无
template<class ET>
void DynamicContainer<ET>::RemoveAll(const ET& Element){
    //实际通过std::remove删除全部指定值元素
    this->m_Elements.erase(std::remove(this->m_Elements.begin(),
                                       this->m_Elements.end(), 
                                       Element),
                           this->m_Elements.end()
    );
    //更新容器元素总个数
    this->m_Sum = this->m_Elements.size();
}
//------------------------------------------------------------------------------

//函数名：Clear
//功能：清空容器
//入口参数：无
//出口参数：无
//返回值：无
template<class ET>
void DynamicContainer<ET>::Clear(){
    //实际清空
    this->m_Elements.clear();
    //更新容器元素总个数
    this->m_Sum = this->m_Elements.size();
}
//------------------------------------------------------------------------------

#endif /* DynamicContainer_hpp */
