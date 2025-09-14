//
//  Network_ANN_Importer.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】Network_ANN_Importer.hpp
//【功能模块和目的】导入ANN文件到Network的导入器类声明
//【开发者及日期】孙李智 2025/7/15
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Network_ANN_Importer_hpp
#define Network_ANN_Importer_hpp

//Network类所属头文件
#include "Network.hpp"
//Importer基类模版所属头文件
#include "Importer.hpp"
//std::ifstream所属头文件
#include <fstream>

using Network_Importer = Importer<Network>;

//------------------------------------------------------------------------------
//【类名】Network_ANN_Importer
//【功能】导入ANN文件到Network的导入器
//【接口说明】
//    继承得到全部基类接口
//    默认构造函数，手动触发基类构造函数，指定文件扩展名为"ANN"
//    虚析构函数（可能做基类）
//    派生类接口：从文件流导入到Network类型对象接口override
//【特殊使用说明】需要手动调用Network_Importer::Registr<Network_ANN_Importer>()，注册类
//             之后可以通过Network_Importer::GetInstanceXXX获取具体导入器实例
//【开发者及日期】 孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------
class Network_ANN_Importer : public Importer<Network>{
public:
    //--------------------------------------------------------------------------
    //必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //默认构造函数，手动触发基类构造函数，指定文件扩展名为"ANN"
    Network_ANN_Importer();
    //无拷贝构造，因为基类没有
    Network_ANN_Importer(const Network_ANN_Importer&) = delete;
    //无赋值运算符，因为基类没有
    Network_ANN_Importer& operator=(const Network_ANN_Importer&) = delete;
    //虚析构函数（可能做基类），无动态构造成员，默认实现
    virtual ~Network_ANN_Importer() = default;
    //--------------------------------------------------------------------------
    //受保护的 非静态Getter成员函数：需检查所有Getter是否有后置const
    //--------------------------------------------------------------------------
    //从文件流导入到Network对象接口override
    virtual Network LoadFromStream(std::ifstream& Stream) const override;
};

#endif /* Network_ANN_Importer_hpp */
