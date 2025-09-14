//
//  Importer.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】Importer.hpp
//【功能模块和目的】导入器基类模版声明与实现
//【开发者及日期】孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Importer_hpp
#define Importer_hpp

//FilePorter基类所属头文件
#include "FilePorter.hpp"
//std::ifstream所属头文件
#include <fstream>
//std::vector所属头文件
#include <vector>
//std::vector操作所属头文件
#include <algorithm>
//std::vector操作所属头文件
#include <functional>
//std::shared_ptr所属头文件
#include <memory>
//std::string所属头文件
#include <string>

//------------------------------------------------------------------------------
//【类名】Importer
//【功能】导入器基类模版
//【接口说明】
//    继承得到全部基类接口
//    虚析构函数（可能做基类）
//    根据文件名导入到T类型对象
//    静态：注册具体导入器
//    静态：根据文件名获取具体导入器实例指针
//    静态：根据文件扩展名获取具体导入器实例指针
//    派生类接口：从文件流导入到T类型对象接口，纯虚函数
//【开发者及日期】 孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

template<class T>
class Importer : public FilePorter<FilePorterType::IMPORTER>{
protected:
    //--------------------------------------------------------------------------
    //受保护的 必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //带参构造函数
    Importer(const std::string& ExtName);
    //拷贝构造函数，默认实现
    Importer(const Importer& Source) = delete;
    //赋值运算符重载
    Importer& operator=(const Importer& Source) = delete;
    //虚析构函数（可能做基类），无动态构造成员，默认实现
    virtual ~Importer() = default;
public:
    //--------------------------------------------------------------------------
    //非静态Getter成员函数：需检查所有Getter是否有后置const
    //--------------------------------------------------------------------------
    //根据执行文件名导入到T类型对象
    T LoadFromFile(const std::string& FileName) const;
    //根据执行文件名导入到T类型对象
    void LoadFromFile(const std::string& FileName, T& AT) const;
    //从文件流导入到T类型对象接口，纯虚函数
    virtual T LoadFromStream(std::ifstream& Stream) const = 0;
    //--------------------------------------------------------------------------
    //静态Setter成员函数
    //--------------------------------------------------------------------------
    //DERIVED是要注册的具体导入器类型
    template<class DERIVED>
    //注册具体导入器
    static void Register();
    //根据文件名获取具体导入器实例指针
    static std::shared_ptr<Importer<T>>
        GetIstanceByFileName(const std::string& FileName);
    //根据文件扩展名获取具体导入器实例指针
    static std::shared_ptr<Importer<T>>
        GetIstanceByExtName(const std::string& ExtName);
private:
    //--------------------------------------------------------------------------
    //私有 非静态数据成员
    //--------------------------------------------------------------------------
    //文件扩展名
    std::string m_ExtName;
    //--------------------------------------------------------------------------
    //私有 静态数据成员
    //--------------------------------------------------------------------------
    //所有以Importer<T>为基类的实例指针列表
    static std::vector<std::shared_ptr<Importer<T>>> m_pImporters;
public:
    //--------------------------------------------------------------------------
    //非静态Getter成员：常引用
    //--------------------------------------------------------------------------
    const std::string& ExtName{m_ExtName};
};
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//私有 静态数据成员
//--------------------------------------------------------------------------
//所有以Importer<T>为基类的实例指针列表
template<class T>
std::vector<std::shared_ptr<Importer<T>>> Importer<T>::m_pImporters{};

//--------------------------------------------------------------------------
//受保护的 必要的构造、析构、赋值行为
//--------------------------------------------------------------------------

//函数名：Importer
//功能：带参构造函数，自动触发基类默认构造函数。文件扩展名不可重复
//入口参数：const std::string& ExtName
//出口参数：无
//返回值：无
template<class T>
Importer<T>::Importer(const std::string& ExtName)
: FilePorter<FilePorterType::IMPORTER>(), m_ExtName(ExtName){
    //扩展名为空，抛出异常
    if (ExtName == "") {
        throw FilePorter<FilePorterType::IMPORTER>::
            INVALID_FILE_TYPE("'EMPTY'");
    }
}
//------------------------------------------------------------------------------

//函数名：LoadFromFile const
//功能：从文件导入到T类型对象
//入口参数：const std::string& FileName
//出口参数：无
//返回值：T类型对象
template<class T>
T Importer<T>::LoadFromFile(const std::string& FileName) const{
    //测试文件扩展名，不一致抛出异常
    FileTypeTest(FileName, m_ExtName);
    //测试文件打开，打开失败抛出异常
    FileOpenTest(FileName);
    //构造文件对象
    std::ifstream InFile(FileName);
    //调用LoadFromStream，返回结果，完成执行
    return LoadFromStream(InFile);
    //自动关闭文件对象
}
//------------------------------------------------------------------------------

//函数名：LoadFromFile const
//功能：从文件导入到T类型对象
//入口参数：const std::string& FileName, T& AT
//出口参数：T& AT
//返回值：无
template<class T>
void Importer<T>::LoadFromFile(const std::string& FileName, T& AT) const{
    //生成临时对象
    T Temp;
    //导入到临时对象
    Temp = LoadFromFile(FileName);
    //赋值给出口参数
    AT = Temp;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//静态Setter成员函数
//------------------------------------------------------------------------------

//函数名：Register(静态)
//功能：注册具体导入器类，实例化对象，保存实体指针
//入口参数：无
//出口参数：无
//返回值：无
template<class T>
template<class DERIVED>
void Importer<T>::Register(){
    //静态断言，DERIVED必须是Importer<T>的public派生类
    static_assert(
        std::is_base_of<Importer<T>, DERIVED>::value,
        "Need IS-A Type");
    //动态构建一个DERIVED对象，由pDerivedImporter指向实体
    std::shared_ptr<Importer<T>> pDerivedImporter(new DERIVED());
    //遍历所有已存在导入器实例，检查拓展名的唯一性
    for (auto pImporter : m_pImporters){
        //DERIVED对象指定文件扩展名已存在，则抛出异常
        if (pImporter->ExtName == pDerivedImporter->ExtName){
            throw FilePorter<FilePorterType::IMPORTER>::
            INVALID_FILE_TYPE(pDerivedImporter->ExtName);
        }
    }
    //添加DERIVED对象到所有导入器队列
    m_pImporters.push_back(pDerivedImporter);
}
//------------------------------------------------------------------------------

//函数名：GetIstanceByFileName(静态)
//功能：根据指定文件名，获取导入器实例指针，不存在相关导入器实例，抛出异常
//入口参数：const std::string& FileName
//出口参数：无
//返回值：导入器实例指针
template<class T>
std::shared_ptr<Importer<T>> 
Importer<T>::GetIstanceByFileName(const std::string& FileName){
    //截取扩展名作为参数，GetIstanceByExtName返回调用结果
    return GetIstanceByExtName(GetExtName(FileName));
}
//------------------------------------------------------------------------------

//函数名：GetIstanceByExtName(静态)
//功能：根据指定文件名，获取导入器实例指针，不存在相关导入器实例，抛出异常
//入口参数：const std::string& ExtName
//出口参数：无
//返回值：导入器实例指针
template<class T>
std::shared_ptr<Importer<T>>
Importer<T>::GetIstanceByExtName(const std::string& ExtName){
    //遍历所有已存在导入器实例
    for (auto pImporter : m_pImporters){
        //找到指定文件扩展名，返回导入器指针
        if (pImporter->ExtName == ExtName){
            return pImporter;
        }
    }
    //未找到，抛出异常
    throw FilePorter<FilePorterType::IMPORTER>::INVALID_FILE_TYPE(ExtName);
}
//------------------------------------------------------------------------------

#endif /* Importer_hpp */
