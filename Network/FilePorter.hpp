//
//  FilePorter.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】FilePorter.hpp
//【功能模块和目的】文件接口类声明
//【开发者及日期】孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

#ifndef FilePorter_hpp
#define FilePorter_hpp

//std::string所属头文件
#include <string>
//std异常基类所属头文件
#include <stdexcept>
//std::fstream所属头文件
#include <fstream>

//------------------------------------------------------------------------------
//【类名】FilePorter
//【功能】文件读写接口基类模版
//【接口说明】
//    静态：测试给定文件类型是否与自身一致
//    静态：测试给定文件能否正常打开
//    静态：获取指定文件的扩展名
//    读写接口类型枚举，导入器或导出器
//【开发者及日期】 孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//内嵌文件接口枚举类
//--------------------------------------------------------------------------
enum class FilePorterType{IMPORTER, EXPORTER};
//--------------------------------------------------------------------------
template<FilePorterType ENUM>
class FilePorter{
public:
    //--------------------------------------------------------------------------
    //内嵌异常类
    //--------------------------------------------------------------------------
    //遇到非法文件扩展名，需抛出此异常
    class INVALID_FILE_TYPE : public std::invalid_argument{
    public:
        INVALID_FILE_TYPE(const std::string& FileExtName)
        :  std::invalid_argument(
            FileExtName
          + std::string(" is invalid file extesion name")){
        };
    };
    //遇到文件打开失败，需抛出此异常
    class FILE_OPEN_FAIL : public std::invalid_argument{
    public:
        FILE_OPEN_FAIL(const std::string& FileName)
        : std::invalid_argument(FileName + std::string(" open fail")){
        };
    };
    //--------------------------------------------------------------------------
    //静态Getter成员函数
    //--------------------------------------------------------------------------
    //文件类型测试，如与FilePorter指定类型不同，抛出异常
    static void FileTypeTest
        (const std::string& FileName, const std::string ExtName);
    //文件打开测试，如与打开失败，抛出异常
    static void FileOpenTest(const std::string& FileName);
    //获取指定文件的扩展名（最后一个'.'之后的字符串），无扩展名返回空string
    static std::string GetExtName(const std::string& FileName);
protected:
    //--------------------------------------------------------------------------
    //受保护 必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //默认构造函数
    FilePorter() = default;
    //拷贝构造函数，默认实现
    FilePorter(const FilePorter& Source) = delete;
    //赋值运算符overload
    FilePorter& operator=(const FilePorter& Source) = delete;
    //虚析构函数（可能做基类），无动态构造成员，默认实现
    virtual ~FilePorter() = default;
public:
    //---------------------------------------------------------------
    //非静态Getter成员：常引用
    //---------------------------------------------------------------
    //文件接口类型
    static constexpr FilePorterType PORT_TYPE{ENUM};
};

//------------------------------------------------------------------------------
//受保护的 非静态Getter成员函数：需检查所有Getter是否有后置const
//------------------------------------------------------------------------------

//函数名：FileTypeTest
//功能：测试文件是否具有合法的扩展名（与自身对象所指扩展名一致），不一致会抛出异常
//入口参数：const std::string& FileName
//出口参数：无
//返回值：无
template<FilePorterType ENUM>
void FilePorter<ENUM>::FileTypeTest
(const std::string& FileName, const std::string ExtName){
    //获取文件名中的扩展名
    std::string ExtInFileName = FileName.substr(
        FileName.length()- ExtName.length()
    );
    //扩展名对比，不一致则抛出异常
    if (ExtInFileName != ExtName) {
        throw INVALID_FILE_TYPE(ExtName);
    }
}
//------------------------------------------------------------------------------

//函数名：FileOpenTest
//功能：测试文件能否成功打开，打开失败抛出异常
//入口参数：const std::string& FileName
//出口参数：无
//返回值：无
template<FilePorterType ENUM>
void FilePorter<ENUM>::FileOpenTest(const std::string& FileName){
    if (ENUM == FilePorterType::IMPORTER) {
        //导入时检查文件是否可读
        std::ifstream File(FileName);
        if (!File.is_open()) {
            throw FILE_OPEN_FAIL(FileName);
        }
    } else {
        //导出时使用写模式检查（允许创建新文件）
        std::ofstream File(FileName, std::ios::out);
        if (!File.is_open()) {
            throw FILE_OPEN_FAIL(FileName);
        }
    }
}
//------------------------------------------------------------------------------

//函数名：GetExtName
//功能：获取指定文件的扩展名（最后一个'.'之后的字符串），无扩展名返回空string
//入口参数：const std::string& FileName
//出口参数：无
//返回值：扩展名std::string
template<FilePorterType ENUM>
std::string FilePorter<ENUM>::GetExtName(const std::string& FileName){
    size_t Position = FileName.find_last_of('.');
    // 如果找不到点或点位于字符串的首位（没有扩展名），返回空字符串
    if (Position == std::string::npos || Position == FileName.length() - 1) {
        return std::string{""};
    }
    // 返回从点开始到字符串末尾的部分作为扩展名
    else {
        return FileName.substr(Position + 1);
    }
}
//------------------------------------------------------------------------------

#endif /* FilePorter_hpp */
