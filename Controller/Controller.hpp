//
//  Controller.hpp
//
//  Created by 孙李智 on 2025/7/16.
//

//------------------------------------------------------------------------------
//【文件名】Controller.hpp
//【功能模块和目的】控制器类声明
//【开发者及日期 孙李智 2025/7/16
//【更改记录】
//------------------------------------------------------------------------------

#ifndef Controller_hpp
#define Controller_hpp

//Network类所属头文件
#include "../Network/Network.hpp"
//size_t所属头文件
#include <cstddef>
//Layer类所属头文件
#include "../Network/Layer.hpp"
//Neuron类所属头文件
#include "../Network/Neuron.hpp"
//Synapse类所属头文件
#include "../Network/Synapse.hpp"
//std::vector所属头文件
#include <vector>
//std::shared_ptr所属头文件
#include <memory>
//std::string所属头文件
#include <string>

//marco trick
#define enum_to_string(x) #x

//------------------------------------------------------------------------------
//【类名】Controller
//【功能】控制器类，具有用户可调用的全部层级功能，单例模式
//【接口说明】
//    静态：获取控制器实例指针
//    虚析构函数（可能做基类）
//    创建空网络并设为当前网络
//    导入指定文件到当前网络
//    导出当前网络到指定文件
//    列出当前网络中所有层对象信息
//    （显示层的全局索引和内部神经元数量、全局索引）
//    删除当前网路中的指定层
//    向当前网络中添加层对象（内部无神经元对象）
//    列出当前网络中指定层的所有神经元信息
//    （显示神经元全局索引和偏置）
//    修改当前网络指定神经元对象的偏置
//    列出当前网络指定神经元对象的突触连接关系
//    删除当前网络指定神经元对象
//    连接当前网络中指定神经元对象
//    显示统计信息（显示网络中层总数，神经元总数和突触总数）
//    验证网络的合理性
//    执行推理
//    当前网络索引（只读）
//【开发者及日期】 孙李智 2025/7/16
//【更改记录】
//------------------------------------------------------------------------------
class Controller{
public:
    //--------------------------------------------------------------------------
    //与数据交换相关的内嵌类
    //--------------------------------------------------------------------------
    //所有控制器公开成员函数返回值的枚举类型
    enum class RES : int {
        //操作成功
        OK = 0,
        //网络索引错误
        NETWORK_INDEX_ERROR = 1,
        //层索引错误
        LAYER_INDEX_ERROR = 2,
        //神经元索引错误
        NEURON_INDEX_ERROR = 3,
        //操作不允许
        OPERATION_NOT_ALLOWED = 4,
        //导入器错误
        IMPORTER_ERROR = 5,
        //导出器错误
        EXPORTER_ERROR = 6,
        //文件类型错误
        FILE_TYPE_ERROR = 7,
        //文件打开错误
        FILE_OPEN_ERROR = 8,
        //层索引重复
        DUPLICATE_LAYER_INDEX_ERROR = 9,
        //神经元索引重复
        DUPLICATE_NEURON_INDEX_ERROR = 10,
        //连接重复
        DUPLICATE_CONNECTION_ERROR = 11,
        //网络验证失败
        NETWORK_VALIDATION_ERROR = 12,
        //导入器重复注册
        DUPLICATE_IMPORTER_ERROR = 13,
        //导出器重复注册
        DUPLICATE_EXPORTER_ERROR = 14,
        // 添加失败
        ADD_FAILED = 15,
        //未知错误
        UNKNOWN_ERROR = 16
    };

// 神经网络信息
class NetworkInfo {
public:
    std::string Name;              // 网络名称
    size_t LayerCount;             // 层数量
    size_t NeuronCount;            // 神经元总数
    size_t SynapseCount;           // 突触连接总数
};

// 层信息
class LayerInfo {
public:
    size_t Index;                  // 层索引
    size_t NeuronCount;            // 层内神经元数量
    size_t StartNeuronIndex;       // 起始神经元索引
    size_t EndNeuronIndex;         // 结束神经元索引
};

// 神经元信息
class NeuronInfo {
public:
    size_t GlobalIndex;            // 神经元全局索引
    double Bias;                   // 偏置值
    int ActivationType;            // 激活函数类型
    size_t DendriteCount;          // 输入连接数
    size_t AxonOutputCount;        // 输出连接数
};

// 突触连接信息
class SynapseInfo {
public:
    size_t SourceIndex;            // 源神经元索引
    size_t TargetIndex;            // 目标神经元索引
    double Weight;                 // 连接权重
};

// 信息列表类型定义
using NetworkInfoList = std::vector<NetworkInfo>;
using LayerInfoList = std::vector<LayerInfo>;
using NeuronInfoList = std::vector<NeuronInfo>;
using SynapseInfoList = std::vector<SynapseInfo>;



    // 获取控制器单例
    static std::shared_ptr<Controller> GetInstance();

    // 创建空网络并设为当前网络
    RES CreateAndSetToCurrent();
    // 删除当前网络
    RES RemoveNetwork();
    // 导入指定文件到当前网络
    RES ImportCurrentNetwork(const std::string& FileName);
    // 导出当前网络到指定文件
    RES ExportCurrentNetwork(const std::string& FileName);
    // 向当前网络添加层（内部无神经元）
    RES AppendLayerToCurrentNetwork();
    // 移除当前网络中的指定层
    RES RemoveLayerFromCurrentNetwork(size_t LayerIndex);
    // 列出当前网络指定层中的所有神经元信息
    RES ListNeuronsInLayerOfCurrentNetwork(size_t LayerIndex);
    // 修改当前网络指定神经元的偏置
    RES ModifyNeuronBiasInCurrentNetwork(size_t NeuronIndex, 
                                         double Bias);
    // 列出当前网络指定神经元的突触连接关系信息
    RES ListSynapsesOfNeuronInCurrentNetwork(size_t NeuronIndex);
    // 删除当前网络指定神经元
    RES RemoveNeuronFromCurrentNetwork(size_t NeuronIndex);
    // 连接当前网络中相邻层的两个神经元
    RES ConnectNeuronsInCurrentNetwork(size_t FromNeuronIndex, 
                                       size_t ToNeuronIndex, 
                                       double Weight);
    // 显示统计信息（层总数，神经元总数，突触总数）
    RES ShowStatisticsForCurrentNetwork();
    // 验证指定网络的合理性
    RES ValidateCurrentNetwork();
    // 对指定网络执行推理
    RES InferenceOnCurrentNetwork(const std::vector<double>& input);
    //获取神经网络列表的指针
    std::vector<std::shared_ptr<Network>> GetNetworks();

    //--------------------------------------------------------------------------
    //非静态Getter数据成员：常引用
    //--------------------------------------------------------------------------
    //当前网络索引值（只读）
    const size_t& CurrentNetworkIndex;
    //修改当前神经网络的索引值
    size_t SetCurrentNetworkIndex(size_t Index);
    //--------------------------------------------------------------------------
    //静态Getter数据成员
    //--------------------------------------------------------------------------
    //所有控制器公开成员函数返回值，对应的字符串
    static const std::string RES_STR[];

private:
    //--------------------------------------------------------------------------
    //私有 必要的构造、析构、赋值行为
    //--------------------------------------------------------------------------
    //单例模式，使用私有默认构造函数
    Controller();
    
    //--------------------------------------------------------------------------
    //私有 非静态数据成员
    //--------------------------------------------------------------------------
    //当前网络索引
    size_t m_CurrentNetworkIndex;
    //所有网络实例指针列表
    std::vector<std::shared_ptr<Network>> m_Networks;

    // 注册导入导出器
    void RegisterImportersAndExporters();
    
    //--------------------------------------------------------------------------
    //私有 静态数据成员
    //--------------------------------------------------------------------------
    //控制器实例指针
    static std::shared_ptr<Controller> m_pControllerIntance;
};

#endif /* Controller_hpp */
