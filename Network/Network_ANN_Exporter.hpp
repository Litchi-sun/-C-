//
//  Network_ANN_Exporter.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】Network_ANN_Exporter.hpp
//【功能模块和目的】导出Network到ANN文件的导出器类声明
//【开发者及日期】孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Network_ANN_Exporter_hpp
#define Network_ANN_Exporter_hpp

//std::0fstream所属头文件
#include <fstream>
//Exporter基类模版所属头文件
#include "Exporter.hpp"
//Network类所属头文件
#include "Network.hpp"

using Network_Exporter = Exporter<Network>;

//------------------------------------------------------------------------------
//【类名】Network_ANN_Exporter
//【功能】导出Network到ANN文件的导出器
//【接口说明】
//    继承得到全部基类接口
//    默认构造函数，手动触发基类构造函数，指定文件扩展名为"ANN"
//    虚析构函数（可能做基类）
//    静态：是否压缩导出
//    派生类接口：将Network类型对象导出到文件流的接口override
//【特殊使用说明】需要手动调用Network_Exporter::Registr<Network_ANN_Exporter>()，注册类
//             之后可以通过Network_Exporter::GetInstanceXXX获取具体导出器实例
//【开发者及日期】 孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------
class Network_ANN_Exporter : public Exporter<Network>{
public:
    //--------------------------------------------------------------------------
    //必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //默认构造函数，手动触发基类构造函数，指定文件扩展名为"ANN"
    Network_ANN_Exporter();
    //无拷贝构造，因为基类没有
    Network_ANN_Exporter(const Network_ANN_Exporter&) = delete;
    //无赋值运算符，因为基类没有
    Network_ANN_Exporter& operator=(const Network_ANN_Exporter&) = delete;
    //虚析构函数（可能做基类），无动态构造成员，默认实现
    virtual ~Network_ANN_Exporter() = default;
    //--------------------------------------------------------------------------
    //非静态Getter成员函数：需检查所有Getter是否有后置const
    //--------------------------------------------------------------------------
    //将Network类型对象导出到文件流的接口override
    virtual void SaveToStream
         (std::ofstream& Stream, const Network& ANetwork) const override;
};

#endif /* Network_ANN_Exporter_hpp */
