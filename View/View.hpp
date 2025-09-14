//
//  View.hpp
//
//  Created by 孙李智 on 2025/7/16.
//

//------------------------------------------------------------------------------
//【文件名】View.hpp
//【功能模块和目的】界面类声明
//【开发者及日期】孙李智 2025/7/16
//【更改记录】
//------------------------------------------------------------------------------

#ifndef View_hpp
#define View_hpp

//std::string所属头文件
#include <string>

//------------------------------------------------------------------------------
//【类名】View
//【功能】界面类，显示菜单、与用户交换数据、调用Controller功能
//【接口说明】：除构造函数外，均为虚函数，为的是派生可以快速修改菜单实现或添加功能
//    默认构造函数
//    虚析构函数（可能做基类）
//主菜单：
//    显示主菜单

//网络管理菜单：
//    列出所有网络
//    创建新网络
//    导入网络
//    导出网络
//    选择当前网络

//网络编辑菜单：
//    修改网络菜单
//    列出网络信息
//    添加层
//    删除指定层
//    列出指定层中神经元
//    修改指定神经元偏置
//    列出指定神经元连接关系
//    删除指定神经元
//    连接指定神经元

//网络分析与执行
//    显示统计信息
//    验证网络
//    执行推理

//【开发者及日期】 孙李智 2024/7/16
//【更改记录】
//------------------------------------------------------------------------------
class View{
public:
    //默认构造函数
    View();
    //虚析构函数（可能做基类），无动态构造成员，默认实现
    virtual ~View() = default;
public:
    // 主菜单
    virtual std::string MainMenu() const;
    
    // 网络管理菜单
    // 列出所有网络
    virtual std::string ListNetworksMenu() const;  
    // 创建新网络
    virtual std::string CreateNetworkMenu() const;  
    // 导入网络
    virtual std::string ImportNetworkMenu() const; 
    // 导出网络
    virtual std::string ExportNetworkMenu() const; 
    // 选择当前网络
    virtual std::string SelectNetworkMenu() const;
    
    // 网络编辑菜单
    // 修改网络主菜单
    virtual std::string ModifyNetworkMenu() const; 
    // 列出网络信息
    virtual std::string ListNetworkDetailsMenu() const;
    // 添加层
    virtual std::string AppendLayerMenu() const;  
    // 删除指定层
    virtual std::string RemoveLayerMenu() const;     
    // 列出指定层中神经元信息
    virtual std::string ListNeuronsInLayerMenu() const; 
    // 修改指定神经元偏置
    virtual std::string ModifyNeuronBiasMenu() const;    
    // 列出指定神经元连接关系
    virtual std::string ListSynapsesMenu() const;     
    // 删除指定神经元   
    virtual std::string RemoveNeuronMenu() const;   
    // 连接指定神经元
    virtual std::string ConnectNeuronsMenu() const;     
    
    // 网络分析与执行     
    // 验证网络
    virtual std::string ValidateNetworkMenu() const;  
    // 执行推理
    virtual std::string InferenceMenu() const;   
};

#endif /* View_hpp */
