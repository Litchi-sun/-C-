//
//  Exporter.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】Exporter.hpp
//【功能模块和目的】导出器基类模版声明与实现
//【开发者及日期】孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Exporter_hpp
#define Exporter_hpp

//FilePorter基类所属头文件
#include "FilePorter.hpp"
//std::ofstream所属头文件
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
//【类名】Exporter
//【功能】导出器基类模版
//【接口说明】
//    继承得到全部基类接口
//    虚析构函数（可能做基类）
//    根据文件名导出T类型对象
//    静态：注册具体导入器
//    静态：根据文件名获取具体导出器实例指针
//    静态：根据文件扩展名获取具体导出器实例指针
//    派生类接口：从T类型对象导出到文件流接口，纯虚函数
//【开发者及日期】 孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------
template<class T>
class Exporter : public FilePorter<FilePorterType::EXPORTER>{
protected:
    //--------------------------------------------------------------------------
    //受保护的 必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //带参构造函数
    Exporter(const std::string& ExtName);
    //拷贝构造函数
    Exporter(const Exporter& Source) = delete;
    //赋值运算符overload
    Exporter& operator=(const Exporter& Source) = delete;
    //虚析构函数（可能做基类），无动态构造成员，默认实现
    virtual ~Exporter() = default;
public:
    //--------------------------------------------------------------------------
    //非静态Getter成员函数：需检查所有Getter是否有后置const
    //--------------------------------------------------------------------------
    //根据执行文件名导入到T类型对象
    void SaveToFile(const std::string& FileName, const T& AT) const;
    //从文件流导入到T类型对象接口，纯虚函数
    virtual void SaveToStream(std::ofstream& Stream, const T& AT) const = 0;
    //--------------------------------------------------------------------------
    //静态Setter成员函数
    //--------------------------------------------------------------------------
    template<class DERIVED>
    //注册具体导入器
    static void Register();
    //根据文件名获取具体导入器实例指针
    static std::shared_ptr<Exporter<T>>
        GetIstanceByFileName(const std::string& FileName);
    //根据文件扩展名获取具体导入器实例指针
    static std::shared_ptr<Exporter<T>>
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
    static std::vector<std::shared_ptr<Exporter<T>>> m_pExporters;
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
std::vector<std::shared_ptr<Exporter<T>>> Exporter<T>::m_pExporters{};

//--------------------------------------------------------------------------
//受保护的 必要的构造、析构、赋值行为
//--------------------------------------------------------------------------

//函数名：Exporter
//功能：带参构造函数，自动触发基类默认构造函数。文件扩展名不可重复
//入口参数：const std::string& ExtName
//出口参数：无
//返回值：无
template<class T>
Exporter<T>::Exporter(const std::string& ExtName)
: FilePorter<FilePorterType::EXPORTER>(), m_ExtName(ExtName){
    //扩展名为空，抛出异常
    if (ExtName == "") {
        throw FilePorter<FilePorterType::EXPORTER>::
            INVALID_FILE_TYPE("'EMPTY'");
    }
}
//------------------------------------------------------------------------------

//函数名：SaveToFile const
//功能：从T类型对象导出到文件
//入口参数：const std::string& FileName, const T& AT
//出口参数：无
//返回值：无
template<class T>
void Exporter<T>::SaveToFile(const std::string& FileName, const T& AT) const{
    //测试文件扩展名，不一致抛出异常
    FileTypeTest(FileName, m_ExtName);
    //测试文件打开，打开失败抛出异常
    FileOpenTest(FileName);
    //构造文件对象
    std::ofstream File(FileName);
    //调用LoadFromStream，返回结果，完成执行
    SaveToStream(File, AT);
    //关闭文件对象
    File.close();
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
void Exporter<T>::Register(){
    //静态断言，DERIVED必须是Importer<T>的public派生类
    static_assert(
        std::is_base_of<Exporter<T>, DERIVED>::value,
        "Need IS-A Type");
    //动态构建一个DERIVED对象，由pDerivedImporter指向实体
    std::shared_ptr<Exporter<T>> pDerivedExporter(new DERIVED());
    //遍历所有已存在导出器实例
    for (auto pExporter : m_pExporters){
        //DERIVED对象指定文件扩展名已存在，则抛出异常
        if (pExporter->ExtName == pDerivedExporter->ExtName){
            throw FilePorter<FilePorterType::EXPORTER>::
            INVALID_FILE_TYPE(pDerivedExporter->ExtName);
        }
    }
    //添加DERIVED对象到所有导出器队列
    m_pExporters.push_back(pDerivedExporter);
}
//------------------------------------------------------------------------------

//函数名：GetIstanceByFileName(静态)
//功能：根据指定文件名，获取导出器实例指针，不存在相关导出器实例，抛出异常
//入口参数：const std::string& FileName
//出口参数：无
//返回值：导出器实例指针
template<class T>
std::shared_ptr<Exporter<T>>
Exporter<T>::GetIstanceByFileName(const std::string& FileName){
    //截取扩展名作为参数，GetIstanceByExtName返回调用结果
    return GetIstanceByExtName(GetExtName(FileName));
}
//------------------------------------------------------------------------------

//函数名：GetIstanceByExtName(静态)
//功能：根据指定文件名，获取导出器实例指针，不存在相关导出器实例，抛出异常
//入口参数：const std::string& ExtName
//出口参数：无
//返回值：导出器实例指针
template<class T>
std::shared_ptr<Exporter<T>>
Exporter<T>::GetIstanceByExtName(const std::string& ExtName){
    //遍历所有已存在导入器实例
    for (auto pExporter : m_pExporters){
        //找到指定文件扩展名，返回导入器指针
        if (pExporter->ExtName == ExtName){
            return pExporter;
        }
    }
    //未找到，抛出异常
    throw FilePorter<FilePorterType::EXPORTER>::INVALID_FILE_TYPE(ExtName);
}
//------------------------------------------------------------------------------

#endif /* Exporter_hpp */
