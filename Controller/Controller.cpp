//
//  Controller.cpp
//
//  Created by 孙李智 on 2025/7/16.
//

//------------------------------------------------------------------------------
//【文件名】Controller.cpp
//【功能模块和目的】控制器类实现
//【开发者及日期 孙李智 2025/7/27
//【更改记录】
//------------------------------------------------------------------------------

// Controller类所属头文件
#include "Controller.hpp"
// Network_ANN_Importer类所属头文件
#include "../Network/Network_ANN_Importer.hpp"
// Network_ANN_Exporter类所属头文件
#include "../Network/Network_ANN_Exporter.hpp"

// 单例实例初始化
std::shared_ptr<Controller> Controller::m_pControllerIntance = nullptr;

// RES枚举的字符串描述
const std::string Controller::RES_STR[] = {
    "OK",
    "NETWORK_INDEX_ERROR",
    "LAYER_INDEX_ERROR",
    "NEURON_INDEX_ERROR",
    "OPERATION_NOT_ALLOWED",
    "IMPORTER_ERROR",
    "EXPORTER_ERROR",
    "FILE_TYPE_ERROR",
    "FILE_OPEN_ERROR",
    "DUPLICATE_LAYER_INDEX_ERROR",
    "DUPLICATE_NEURON_INDEX_ERROR",
    "DUPLICATE_CONNECTION_ERROR",
    "NETWORK_VALIDATION_ERROR",
    "DUPLICATE_IMPORTER_ERROR",
    "DUPLICATE_EXPORTER_ERROR",
    "ADD_FAILED",
    "UNKNOWN_ERROR"
};

// 函数名：GetInstance
// 功能：获取控制器单例实例，确保整个系统中只有一个Controller对象
// 入口参数：无
// 出口参数：无
// 返回值：std::shared_ptr<Controller>
std::shared_ptr<Controller> Controller::GetInstance() {
    if (!m_pControllerIntance) {
        m_pControllerIntance = std::shared_ptr<Controller>(new Controller());
        m_pControllerIntance->RegisterImportersAndExporters();
    }
    return m_pControllerIntance;
}

// 函数名：Controller
// 功能：构造函数，初始化当前网络索引为0
// 入口参数：无
// 出口参数：无
// 返回值：无
Controller::Controller() 
    : CurrentNetworkIndex(m_CurrentNetworkIndex), 
      m_CurrentNetworkIndex(0) {}



// 函数名：RegisterImportersAndExporters
// 功能：注册导入导出器
// 入口参数：无
// 出口参数：无
// 返回值：无
void Controller::RegisterImportersAndExporters() {
    try {
        //导出器注册
        Network_Exporter::Register<Network_ANN_Exporter>();
        Network_Importer::Register<Network_ANN_Importer>();
    } catch (...) {
        // 重复注册会抛出异常，但这里不做处理
    }
}


// 函数名：CreateAndSetToCurrent
// 功能：创建空网络
// 入口参数：无
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::CreateAndSetToCurrent() {
    auto newNetwork = std::make_shared<Network>();
    m_Networks.push_back(newNetwork);
    m_CurrentNetworkIndex = m_Networks.size() - 1;
    return RES::OK;
}



// 函数名：RemoveNetwork
// 功能：删除当前网络，并更新当前网络索引
// 入口参数：无
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::RemoveNetwork() {
if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    // 删除当前网络
    m_Networks.erase(m_Networks.begin() + m_CurrentNetworkIndex);
    
    // 更新当前网络索引
    if (m_Networks.empty()) {
        m_CurrentNetworkIndex = 0;
    } else if (m_CurrentNetworkIndex >= m_Networks.size()) {
        m_CurrentNetworkIndex = m_Networks.size() - 1;
    }
    
    return RES::OK;
}


// 函数名：ImportCurrentNetwork
// 功能：将指定文件导入网络
// 入口参数：const std::string& FileName - 导入文件的路径和名称
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::ImportCurrentNetwork(const std::string& FileName) {
    try {
        auto importer = Importer<Network>::GetIstanceByFileName(FileName);
        auto network = importer->LoadFromFile(FileName);
        m_Networks.push_back(std::make_shared<Network>(network));
        m_CurrentNetworkIndex = m_Networks.size() - 1;
        return RES::OK;
    } catch (const FilePorter<FilePorterType::IMPORTER>::INVALID_FILE_TYPE&) {
        return RES::FILE_TYPE_ERROR;
    } catch (const FilePorter<FilePorterType::IMPORTER>::FILE_OPEN_FAIL&) {
        return RES::FILE_OPEN_ERROR;
    } catch (...) {
        return RES::IMPORTER_ERROR;
    }
}


// 函数名：ExportCurrentNetwork
// 功能：导出当前网络到指定文件
// 入口参数：const std::string& FileName
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::ExportCurrentNetwork(const std::string& FileName) {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    try {
        auto exporter = Exporter<Network>::GetIstanceByFileName(FileName);
        exporter->SaveToFile(FileName, *m_Networks[m_CurrentNetworkIndex]);
        return RES::OK;
    } catch (const FilePorter<FilePorterType::EXPORTER>::INVALID_FILE_TYPE&) {
        return RES::FILE_TYPE_ERROR;
    } catch (const FilePorter<FilePorterType::EXPORTER>::FILE_OPEN_FAIL&) {
        return RES::FILE_OPEN_ERROR;
    } catch (...) {
        return RES::EXPORTER_ERROR;
    }
}



// 函数名：AppendLayerToCurrentNetwork
// 功能：向当前网络添加层（内部无神经元）
// 入口参数：无
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::AppendLayerToCurrentNetwork() {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    size_t newIndex = 0;
    if (!network.GetLayers().empty()) {
        newIndex = network.GetLayers().back()->GetIndex() + 1;
    }
    return network.AddLayer(newIndex) ? RES::OK : RES::ADD_FAILED;
}



// 函数名：RemoveLayerFromCurrentNetwork
// 功能：移除当前网络中的指定层
// 入口参数：size_t LayerIndex
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::RemoveLayerFromCurrentNetwork(size_t LayerIndex) {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    return network.RemoveLayer(LayerIndex) ? RES::OK : RES::LAYER_INDEX_ERROR;
}




// 函数名：ListNeuronsInLayerOfCurrentNetwork
// 功能：列出当前网络指定层中的所有神经元信息
// 入口参数：size_t LayerIndex
// 出口参数：无
// 返回值：Controller::RES

Controller::RES Controller::ListNeuronsInLayerOfCurrentNetwork(size_t LayerIndex) {
if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    const auto& layers = network.GetLayers();
    
    // 查找指定层
    auto it = std::find_if(layers.begin(), layers.end(),
        [LayerIndex](const std::shared_ptr<Layer>& layer) {
            return layer->GetIndex() == LayerIndex;
        });
    
    if (it == layers.end()) {
        return RES::LAYER_INDEX_ERROR;
    }
    
    // 调用层的ListNeurons方法
    (*it)->ListNeurons();
    return RES::OK;
}



// 函数名：ModifyNeuronBiasInCurrentNetwork
// 功能：修改指定神经元偏置
// 入口参数： size_t NeuronIndex，double Bias
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::ModifyNeuronBiasInCurrentNetwork(size_t NeuronIndex, double Bias) {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    return network.ModifyNeuronBias(NeuronIndex, Bias) ? 
        RES::OK : RES::NEURON_INDEX_ERROR;
}




// 函数名：ListSynapsesOfNeuronInCurrentNetwork
// 功能：列出当前网络指定神经元的突触连接关系信息
// 入口参数：要查询的神经元的全局索引
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::ListSynapsesOfNeuronInCurrentNetwork(size_t NeuronIndex) {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    
    // 遍历所有层查找神经元
    for (const auto& layer : network.GetLayers()) {
        auto neuron = layer->GetNeuron(NeuronIndex);
        if (neuron) {
            neuron->ListSynapses();
            return RES::OK;
        }
    }
    
    return RES::NEURON_INDEX_ERROR;
}





// 函数名：RemoveNeuronFromCurrentNetwork
// 功能：删除当前网络指定神经元
// 入口参数：要删除的神经元的全局索引
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::RemoveNeuronFromCurrentNetwork(size_t NeuronIndex) {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    return network.RemoveNeuron(NeuronIndex) ? 
        RES::OK : RES::NEURON_INDEX_ERROR;
}



// 函数名：ConnectNeuronsInCurrentNetwork
// 功能：连接当前网络中相邻层的两个神经元
// 入口参数：源神经元的全局索引，目标神经元的全局索引，突触连接的权重值
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::ConnectNeuronsInCurrentNetwork(size_t FromNeuronIndex, 
                                                           size_t ToNeuronIndex, 
                                                           double Weight) {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    
    // 查找源神经元
    std::shared_ptr<Neuron> fromNeuron;
    for (const auto& layer : network.GetLayers()) {
        fromNeuron = layer->GetNeuron(FromNeuronIndex);
        if (fromNeuron) break;
    }
    
    // 查找目标神经元
    std::shared_ptr<Neuron> toNeuron;
    for (const auto& layer : network.GetLayers()) {
        toNeuron = layer->GetNeuron(ToNeuronIndex);
        if (toNeuron) break;
    }
    
    if (!fromNeuron || !toNeuron) {
        return RES::NEURON_INDEX_ERROR;
    }
    
    // 调用层的连接函数
    if (fromNeuron->GetLayer() && toNeuron->GetLayer()) {
        if (fromNeuron->GetLayer()->ConnectNeurons(fromNeuron, toNeuron, Weight)) {
            return RES::OK;
        }
        return RES::DUPLICATE_CONNECTION_ERROR;
    }
    
    return RES::OPERATION_NOT_ALLOWED;
}




// 函数名：ShowStatisticsForCurrentNetwork
// 功能：显示统计信息（层总数，神经元总数，突触总数）
// 入口参数：无
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::ShowStatisticsForCurrentNetwork() {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    auto& network = *m_Networks[m_CurrentNetworkIndex];
    
    size_t layerCount = network.GetLayers().size();
    size_t neuronCount = 0;
    size_t synapseCount = 0;
    
    // 计算神经元和突触总数
    for (const auto& layer : network.GetLayers()) {
        neuronCount += layer->GetNeuronCount();
        for (const auto& neuron : layer->GetNeurons()) {
            synapseCount += neuron->GetDendriteCount();
        }
    }
    
    std::cout << "Layers: " << layerCount << std::endl;
    std::cout << "Neurons: " << neuronCount << std::endl;
    std::cout << "Synapses: " << synapseCount << std::endl;
    
    return RES::OK;
}



// 函数名：ValidateCurrentNetwork
// 功能：验证指定网络的合理性
// 入口参数：无
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::ValidateCurrentNetwork() {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    return m_Networks[m_CurrentNetworkIndex]->IsValid() ? 
        RES::OK : RES::NETWORK_VALIDATION_ERROR;
}


// 函数名：InferenceOnCurrentNetwork
// 功能：执行推理
// 入口参数：输入层神经元的输入值
// 出口参数：无
// 返回值：Controller::RES
Controller::RES Controller::InferenceOnCurrentNetwork(const std::vector<double>& input) {
    if (m_Networks.empty() || m_CurrentNetworkIndex >= m_Networks.size()) {
        return RES::NETWORK_INDEX_ERROR;
    }
    
    try {
        auto& network = *m_Networks[m_CurrentNetworkIndex];
        auto output = network.Inference(input);
        
        return RES::OK;
    } catch (const std::exception& e) {
        std::cerr << "Inference Error: " << e.what() << std::endl;
        return RES::NETWORK_VALIDATION_ERROR;
    }
}

// 函数名：GetNetworks
// 功能：获取神经网络列表的指针
// 入口参数：输入层神经元的输入值
// 出口参数：无
// 返回值：std::vector<std::shared_ptr<Network>>
std::vector<std::shared_ptr<Network>> Controller::GetNetworks() {
        return m_Networks;
    }

// 函数名：SetCurrentNetworkIndex
// 功能：修改当前神经网络的索引值
// 入口参数：新的网络索引
// 出口参数：无
// 返回值：设置后的当前网络索引
size_t Controller::SetCurrentNetworkIndex(size_t Index) {
    m_CurrentNetworkIndex = Index;
    return m_CurrentNetworkIndex;
}