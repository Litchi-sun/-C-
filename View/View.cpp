//
//  View.cpp
//
//  Created by 孙李智 on 2025/7/16
//

//------------------------------------------------------------------------------
//【文件名】View.cpp
//【功能模块和目的】界面类实现
//【功能描述】实现用户界面功能，包括菜单显示和用户交互
//【开发者及日期】孙李智 2025/7/16
//【更改记录】
//------------------------------------------------------------------------------

// View类所需头文件
#include "View.hpp"
// Controller类所属头文件
#include "../Controller/Controller.hpp"
// std::cout所需头文件
#include <iostream>
//std::shared_ptr所需头文件
#include<memory>
// std::string所需头文件
#include <string>
using namespace std;

//函数名：View
//功能：默认构造函数，调用显示主菜单
//入口参数：无
//出口参数：无
//返回值：无
View::View() {
    std::cout << MainMenu() << std::endl;
}

//函数名：MainMenu
//功能：显示主菜单
//入口参数：无
//出口参数：无
//返回值：开发者信息字符串
string View::MainMenu() const {
    //获取控制器实例指针
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    //用户所键入的选择
    string Selection;
    //非预期选择，循环显示菜单，要求用户再次键入选择
    while (Selection != "1" && Selection != "2" && Selection != "3" 
           && Selection != "4" && Selection != "5" && Selection != "6" 
           && Selection != "7" && Selection != "8" && Selection != "9") {
        std::cout << "---Neural network management system---" << std::endl;
        std::cout << "1. Networks" << std::endl;
        std::cout << "2. Create" << std::endl;
        std::cout << "3. Import" << std::endl;
        std::cout << "4. Export" << std::endl;
        std::cout << "5. Select" << std::endl;
        std::cout << "6. Modify" << std::endl;
        std::cout << "7. Valid" << std::endl;
        std::cout << "8. Inference" << std::endl;
        std::cout << "9. Exit"   << std::endl << "(1/2/3/4/5/6/7/8/9):";

        //读取用户键入
        getline(cin, Selection);

        //根据选择触发下一级菜单显示
        if (Selection == "1") {
            std::cout << ListNetworksMenu() << std::endl;
        }
        else if (Selection == "2") {
            std::cout << CreateNetworkMenu() << std::endl;
        }
        else if (Selection == "3") {
            std::cout << ImportNetworkMenu() << std::endl;
        }
        else if (Selection == "4") {
            std::cout << ExportNetworkMenu() << std::endl;
        }
        else if (Selection == "5") {
            std::cout << SelectNetworkMenu() << std::endl;
        }
        else if (Selection == "6") {
            std::cout << ModifyNetworkMenu() << std::endl;
        }
        else if (Selection == "7") {
            std::cout << ValidateNetworkMenu() << std::endl;
        }
        else if (Selection == "8") {
            std::cout << InferenceMenu() << std::endl;
        }
        else if (Selection == "9") {
            break;
        }
        //清楚用户键入选择
        Selection.clear();
    }
    //返回开发者信息
    return "slz23@mails.tsinghua.edu.cn 2025.07.25";
}

//------------------------------------------------------------------------------


//函数名：ListNetworksMenu
//功能：显示列出所有网络信息菜单
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ListNetworksMenu() const {
    //获取控制器实例指针
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    //用户键入选择
    string Selection;
    //非预期选择，循环显示菜单，要求用户再次键入选择
    while (Selection != "Y" && Selection != "y") {
        //获取神经网络的数量
        size_t networkcount = Ctrller->GetNetworks().size();
        
        auto networks = Ctrller->GetNetworks();
        //显示神经网络信息,层总数，神经元总数，突触总数
        std::cout << endl << "---List All Networks---" << std::endl;
        std::cout << networkcount << " Network(s)" << std::endl;
        size_t i = 0;
        for (i; i < networkcount; i++) {
            std::cout << "Network " << i + 1 << " Information:" << std::endl;
            size_t layerCount = networks[i]->GetLayers().size();
            size_t neuronCount = 0;
            size_t synapseCount = 0;
            
            for (const auto& layer : networks[i]->GetLayers()) {
                neuronCount += layer->GetNeuronCount();
                for (const auto& neuron : layer->GetNeurons()) {
                    synapseCount += neuron->GetDendriteCount();
                }
            }

            std::cout << "Layers: " << layerCount << std::endl;
            std::cout << "Neurons: " << neuronCount << std::endl;
            std::cout << "Synapses: " << synapseCount << std::endl;
            std::cout << "-----------------------" << std::endl;
        }
        //清空用户键入选择
        Selection.clear();
        //非预期选择，重新显示提示，要求输入输入
        while (Selection != "Y" && Selection != "N"
            && Selection != "y" && Selection != "n") {
            cout << endl << "Back to main menu? (Y/N):";
            //获取用户键入的选择
            getline(cin, Selection);
        }
    }
    return "";
}

//函数名：CreateNetworkMenu
//功能：显示创建模型菜单
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::CreateNetworkMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string Selection;
    while (Selection != "Y" && Selection != "y") {
        cout << endl << "---Create Empty Network---" << endl;
        Ctrller->CreateAndSetToCurrent();
        cout << "1 empty network was created" << endl;
        cout << "N[" << Ctrller->CurrentNetworkIndex << "] is current" << endl;
        Selection.clear();
        while (Selection != "Y" && Selection != "N" 
            && Selection != "y" && Selection != "n") {
            cout << endl <<  "Back to main menu? Y/N?" << endl 
            << "(N for create another empty Network):";
            getline(cin, Selection);
        }
    }
    return "";
}

//函数名：ImportNetworkMenu
//功能：显示导入模型菜单
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ImportNetworkMenu() const {
    std::shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string Selection;
    while (Selection != "Y" && Selection != "y") {
        std::cout << std::endl << "---Import Network From File---" << std::endl;
        std::cout << "File name:";
        string FileName;
        getline(cin, FileName);
        
        Controller::RES Res = Ctrller->ImportCurrentNetwork(FileName);
        
        if (Res != Controller::RES::OK) {
            std::cout << Controller::RES_STR[static_cast<int>(Res)] << std::endl;
        }
        else {
            std::cout << "File: " << FileName 
                 << " has been imported successfully." << std::endl;
            std::cout << "Current network index: " << Ctrller->CurrentNetworkIndex << std::endl;
        }
        
        Selection.clear();
        while (Selection != "Y" && Selection != "N" 
            && Selection != "y" && Selection != "n") {
            std::cout << std::endl <<  "Back to main menu? Y/N?" << std::endl
                 << "(N for import another file):";
            getline(cin, Selection);
        }
    }
    return "";

}





//函数名：ExportNetworkMenu
//功能：显示导出神经网络菜单
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ExportNetworkMenu() const {
    std::cout << "请输入要导出的文件名: ";
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string Selection;
    while (Selection != "Y" && Selection != "y") {
        std::cout << std::endl << "---Export Network To File---" << std::endl;
        
        // 获取网络列表
        auto networks = Ctrller->GetNetworks();
        size_t networkCount = networks.size();
        
        // 检查是否有可导出的网络
        if (networkCount == 0) {
            std::cout << "No network can be exported, please create a network first" << std::endl;
            return "";
        }
        
        // 显示当前网络信息
        std::cout << "Current network index: " << Ctrller->CurrentNetworkIndex << std::endl;
        std::cout << "Available networks (1-" << networkCount << "):" << std::endl;
        
        // 获取文件名
        std::cout << "File name: ";
        string FileName;
        getline(cin, FileName);
        
        // 获取要导出的网络索引
        std::cout << "Network index(1~" << networkCount << "): ";
        string indexInput;
        getline(cin, indexInput);
        
        try {
            size_t Index = stoul(indexInput);
            Index--;
            if (Index >= networkCount) {
                std::cout << "Invalid network index" << std::endl;
                continue;
            }
            
            // 导出网络
            Controller::RES Res = Ctrller->ExportCurrentNetwork(FileName);
            if (Res != Controller::RES::OK) {
                std::cout << Controller::RES_STR[static_cast<int>(Res)] << std::endl;
            } else {
                std::cout << "File: " << FileName 
                     << " has been exported from network [" << Index << "]" << std::endl;
            }
        } catch (...) {
            std::cout << "Invalid input" << std::endl;
            continue;
        }
        
        // 询问用户是否返回主菜单
        Selection.clear();
        while (Selection != "Y" && Selection != "N" 
               && Selection != "y" && Selection != "n") {
            std::cout << std::endl << "Back to main menu? Y/N?" << std::endl
                 << "(N for export another network): ";
            getline(cin, Selection);
        }
    }
    return "";
}


//函数名：SelectNetworkMenu
//功能：显示选择神经网络菜单
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::SelectNetworkMenu() const {
    //获取控制器实例指针
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    //用户键入选择
    string Selection;
    //非预期选择，循环显示菜单，要求用户再次键入选择
    while (Selection != "Y" && Selection != "y") {
        //获取网络列表
        auto networks = Ctrller->GetNetworks();
        size_t networkCount = networks.size();

        std::cout << std::endl << "-Select Current Network-" << std::endl;
        if (networks.empty()) {
            std::cout << "No network available" << std::endl;
        } else {
            if (Ctrller->CurrentNetworkIndex >= networkCount) {
                std::cout << "No network is selected as current" << std::endl;
            } else {
                std::cout << "Network " << Ctrller->CurrentNetworkIndex + 1 
                     << " is current" << std::endl;
            }
            
            std::cout << "Network index(1~" << networkCount << "):";
            string indexInput;
            getline(cin, indexInput);
            
            try {
                size_t Index = stoul(indexInput);
                Index--;
                if (Index >= networkCount) {
                    std::cout << "Invalid network index" << std::endl;
                } else {
                    // 这里返回 空
                    size_t result = Ctrller->SetCurrentNetworkIndex(Index);
                     if (result == Index) {
                        std::cout << "Current network set to Network " << Index + 1 << std::endl;
                    } else {
                        std::cout << "Failed to set current network" << std::endl;
                    }
                }
            } catch (...) {
                std::cout << "Invalid input" << std::endl;
            }
        }
        
        Selection.clear();
        while (Selection != "Y" && Selection != "N" 
            && Selection != "y" && Selection != "n") {
            std::cout << std::endl << "Back to main menu? (Y/N):";
            getline(cin, Selection);
        }
    }
    return "";
}


//函数名：ModifyNetworkMenu
//功能：显示修改网络菜单
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ModifyNetworkMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string Sel;
    while (Sel != "1" && Sel != "2" && Sel != "3" && Sel != "4" 
           && Sel != "5" && Sel != "6" && Sel != "7" && Sel != "8" && Sel != "9") {
        std::cout << "---Modify Current Network---" << std::endl;
        //检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No network has been selected as current, please select network first" << std::endl;
            return "";
        }
        //显示修改网络的子菜单选项
        std:: cout << "1. List network details" << std::endl;
        std::cout << "2. Append layer" << std::endl;
        std::cout << "3. Remove layer" << std::endl;
        std::cout << "4. List neurons in layer" << std::endl;
        std::cout << "5. Modify neuron bias" << std::endl;
        std::cout << "6. List neuron synapses" << std::endl;
        std::cout << "7. Remove neuron" << std::endl;
        std::cout << "8. Connect neurons" << std::endl;
        std::cout << "9. Back to main menu" << std::endl << "(1/2/3/4/5/6/7/8/9):";
        
        getline(cin, Sel);
        
        //根据用户选择调用对应的子菜单
        if (Sel == "1") {
            std::cout << ListNetworkDetailsMenu() << std::endl;
        }
        else if (Sel == "2") {
            std::cout << AppendLayerMenu() << std::endl;
        }
        else if (Sel == "3") {
            std::cout << RemoveLayerMenu() << std::endl;
        }
        else if (Sel == "4") {
            std::cout << ListNeuronsInLayerMenu() << std::endl;
        }
        else if (Sel == "5") {
            std::cout << ModifyNeuronBiasMenu() << std::endl;
        }
        else if (Sel == "6") {
            std::cout << ListSynapsesMenu() << std::endl;
        }
        else if (Sel == "7") {
            std::cout << RemoveNeuronMenu() << std::endl;
        }
        else if (Sel == "8") {
            std::cout << ConnectNeuronsMenu() << std::endl;
        }
        else if (Sel == "9") {
            break;
        }
        Sel.clear();
    }
    return "";
}

//函数名：ListNetworkDetailsMenu
//功能：显示当前神经网络的详细信息（包含所有层和神经元的具体参数）
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ListNetworkDetailsMenu() const {
    // 获取控制器实例
    shared_ptr<Controller> ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否存在网络
        auto networks = ctrller->GetNetworks();
        if (networks.empty() || ctrller->CurrentNetworkIndex >= networks.size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        // 显示当前网络索引
        std::cout << std::endl << "---Network Details---" << std::endl;
        std::cout << "Current Network Index: " << ctrller->CurrentNetworkIndex << std::endl;
        
        // 调用控制器的统计信息函数显示层、神经元、突触数量
        Controller::RES res = ctrller->ShowStatisticsForCurrentNetwork();
        if (res != Controller::RES::OK) {
            std::cout << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }

        // 显示网络名称
        auto& currentNetwork = *networks[ctrller->CurrentNetworkIndex];
        std::cout << "Network Name: " << currentNetwork.Name << std::endl;

        // 显示网络注释 - 使用索引遍历代替迭代器
        std::cout << "Comments:" << std::endl;
        const auto& comments = currentNetwork.Comments;
        
        // 使用Sum属性和operator[]进行遍历
        for (size_t i = 0; i < comments.Sum; i++) {
            std::cout << "#" << i+1 << ": " << comments[i] << std::endl;
        }

        // 显示输入输出神经元信息
        auto inputNeurons = currentNetwork.GetInputMarker();
        std::cout << "Input Neurons Count: " << inputNeurons.size() << std::endl;
        
        auto outputNeurons = currentNetwork.GetOutputMarker();
        std::cout << "Output Neurons Count: " << outputNeurons.size() << std::endl;

        // 询问是否返回上一级菜单
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to modify menu? (Y/N):";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：AppendLayerMenu
//功能：向当前神经网络添加新层的交互界面
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::AppendLayerMenu() const {
    // 获取控制器实例
    shared_ptr<Controller> ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 显示菜单标题
        std::cout << std::endl << "---Append Layer to Current Network---" << std::endl;
        
        // 调用控制器添加层
        Controller::RES res = ctrller->AppendLayerToCurrentNetwork();
        
        // 显示操作结果
        if (res == Controller::RES::OK) {
            // 获取当前网络的最新层信息
            auto& network = *ctrller->GetNetworks()[ctrller->CurrentNetworkIndex];
            const auto& layers = network.GetLayers();
            
            if (!layers.empty()) {
                // 显示新添加的层索引
                std::cout << "Layer " << layers.back()->GetIndex() 
                     << " appended successfully" << std::endl;
            } else {
                std::cout << "Layer appended successfully" << std::endl;
            }
        } else {
            // 显示错误信息
            std::cout << "Failed to append layer: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否继续添加
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {

            std::cout << std::endl << "Back to modify menu? Y/N?" << std::endl
            << "(N for Append another layer)";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：RemoveLayerMenu
//功能：从当前神经网络删除指定层的交互界面
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::RemoveLayerMenu() const {
    // 获取控制器实例
    shared_ptr<Controller> ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否存在网络
        auto networks = ctrller->GetNetworks();
        if (networks.empty() || ctrller->CurrentNetworkIndex >= networks.size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        // 显示菜单标题
        std::cout << std::endl << "---Remove Layer from Current Network---" << std::endl;
        
        // 获取当前网络和层信息
        auto& network = *networks[ctrller->CurrentNetworkIndex];
        const auto& layers = network.GetLayers();
        
        if (layers.empty()) {
            std::cout << "No layers available to remove." << std::endl;
            return "";
        }
        
        // 显示现有层列表
        std::cout << "Existing Layers (by global index):" << std::endl;
        for (const auto& layer : layers) {
            if (layer) {
                std::cout << "Layer " << layer->GetIndex() 
                     << " (" << layer->GetNeuronCount() << " neurons)" << std::endl;
            }
        }
        
        // 获取用户输入要移除的层索引
        size_t layerIndex = 0;
        bool validInput = false;
        string input;
        
        while (!validInput) {
            std::cout << "Enter layer index to remove: ";
            getline(cin, input);
            
            if (input.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                layerIndex = stoul(input);
                
                // 检查层是否存在
                auto it = std::find_if(layers.begin(), layers.end(),
                    [layerIndex](const std::shared_ptr<Layer>& layer) {
                        return layer->GetIndex() == layerIndex;
                    });
                
                if (it != layers.end()) {
                    validInput = true;
                } else {
                    std::cout << "Layer " << layerIndex << " not found." << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid layer index." << std::endl;
            }
        }

        // 调用控制器移除层
        Controller::RES res = ctrller->RemoveLayerFromCurrentNetwork(layerIndex);
        
        // 显示操作结果
        if (res == Controller::RES::OK) {
            std::cout << "Layer " << layerIndex << " removed successfully" << std::endl;
        } else {
            std::cout << "Failed to remove layer: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否继续移除
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to modify menu? Y/N?" << std::endl
            << "(N for Remove another layer)";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：ListNeuronsInLayerMenu
//功能：显示当前神经网络中指定层的所有神经元信息
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ListNeuronsInLayerMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || 
            Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        auto& network = *Ctrller->GetNetworks()[Ctrller->CurrentNetworkIndex];
        const auto& layers = network.GetLayers();
        
        if (layers.empty()) {
            std::cout << "No layers available in the current network." << std::endl;
            return "";
        }

        // 显示菜单标题
        std::cout << std::endl << "---List Neurons in Layer---" << std::endl;
        
        // 显示可用层
        std::cout << "Available Layers (by global index):" << std::endl;
        for (const auto& layer : layers) {
            if (layer) {
                std::cout << "Layer " << layer->GetIndex() 
                     << " (" << layer->GetNeuronCount() << " neurons)" << std::endl;
            }
        }
        
        // 获取用户输入的层索引
        size_t layerIndex = 0;
        bool validInput = false;
        string input;
        
        while (!validInput) {
            cout << "Enter layer index to list neurons: ";
            getline(cin, input);
            
            if (input.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                layerIndex = stoul(input);
                
                // 检查层是否存在
                auto it = std::find_if(layers.begin(), layers.end(),
                    [layerIndex](const std::shared_ptr<Layer>& layer) {
                        return layer->GetIndex() == layerIndex;
                    });
                
                if (it != layers.end()) {
                    validInput = true;
                } else {
                    std::cout << "Layer " << layerIndex << " not found." << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid layer index." << std::endl;
            }
        }

        // 调用控制器列出神经元
        Controller::RES res = Ctrller->ListNeuronsInLayerOfCurrentNetwork(layerIndex);
        
        // 显示操作结果
        if (res == Controller::RES::OK) {
            std::cout << "Neurons in Layer " << layerIndex << " listed successfully" << std::endl;
        } else {
            std::cout << "Failed to list neurons: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否继续列出
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to modify menu? Y/N?" << std::endl
            << "(N for List neurons in another layer)";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：ModifyNeuronBiasMenu
//功能：修改当前神经网络中指定神经元偏置的交互界面
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ModifyNeuronBiasMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || 
            Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        auto& network = *Ctrller->GetNetworks()[Ctrller->CurrentNetworkIndex];
        
        // 显示菜单标题
        std::cout << std::endl << "---Modify Neuron Bias---" << std::endl;
        
        // 直接获取神经元全局索引
        size_t neuronIndex = 0;
        bool validNeuronInput = false;
        string neuronInput;
        
        while (!validNeuronInput) {
            std::cout << "Enter neuron global index to modify: ";
            getline(cin, neuronInput);
            
            if (neuronInput.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                neuronIndex = stoul(neuronInput);
                validNeuronInput = true;
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid neuron index." << std::endl;
            }
        }
        
        // 获取新的偏置值
        double newBias = 0.0f;
        bool validBias = false;
        string biasInput;
        
        while (!validBias) {
            std::cout << "Enter new bias value (double): ";
            getline(cin, biasInput);
            
            if (biasInput.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                newBias = stof(biasInput);
                validBias = true;
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid double value." << std::endl;
            }
        }
        
        // 调用控制器修改偏置
        Controller::RES res = Ctrller->ModifyNeuronBiasInCurrentNetwork(neuronIndex, newBias);
        
        // 显示操作结果
        if (res == Controller::RES::OK) {
            std::cout << "Bias for neuron " << neuronIndex 
                 << " updated successfully to " << newBias << std::endl;
        } else {
            std::cout << "Failed to modify neuron bias: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否继续修改
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to modify menu? Y/N?" << std::endl
            << "(N for Modify another neuron bias)";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：ListSynapsesMenu
//功能：显示当前神经网络中指定神经元的突触连接关系
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ListSynapsesMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || 
            Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        auto& network = *Ctrller->GetNetworks()[Ctrller->CurrentNetworkIndex];
        
        // 显示菜单标题
        std::cout << endl << "---List Neuron Synapse Connections---" << std::endl;
        
        // 直接获取神经元全局索引
        size_t neuronIndex = 0;
        bool validNeuronInput = false;
        string neuronInput;
        
        while (!validNeuronInput) {
            std::cout << "Enter neuron global index to view synapses: ";
            getline(cin, neuronInput);
            
            if (neuronInput.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                neuronIndex = stoul(neuronInput);
                validNeuronInput = true;
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid neuron index." << std::endl;
            }
        }
        
        // 调用控制器列出突触连接
        std::cout << "\nSynapse connections for Neuron " << neuronIndex << ":\n";
        Controller::RES res = Ctrller->ListSynapsesOfNeuronInCurrentNetwork(neuronIndex);
        
        // 显示操作结果
        if (res != Controller::RES::OK) {
            std::cout << "\nFailed to list synapses: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否继续查看
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to modify menu? Y/N?" << std::endl
            << "(N for View synapses for another neuron)";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：RemoveNeuronMenu
//功能：从当前神经网络删除指定神经元的交互界面
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::RemoveNeuronMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || 
            Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        // 显示菜单标题
        cout << std::endl << "---Remove Neuron---" << std::endl;
        
        // 获取神经元全局索引
        size_t neuronIndex = 0;
        bool validInput = false;
        string input;
        
        while (!validInput) {
            std::cout << "Enter neuron global index to remove: ";
            getline(cin, input);
            
            if (input.empty()) {
                std::cout << "Input cannot be empty." << endl;
                continue;
            }
            
            try {
                neuronIndex = stoul(input);
                validInput = true;
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid neuron index." << std::endl;
            }
        }
        
        // 调用控制器删除神经元
        Controller::RES res = Ctrller->RemoveNeuronFromCurrentNetwork(neuronIndex);
        
        // 显示操作结果
        if (res == Controller::RES::OK) {
            std::cout << "Neuron " << neuronIndex << " removed successfully." << std::endl;
        } else {
            std::cout << "Failed to remove neuron: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否继续删除
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to modify menu? Y/N?" << std::endl
            << "(N for Remove another neuron)";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：ConnectNeuronsMenu
//功能：在当前神经网络中连接两个神经元的交互界面
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ConnectNeuronsMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || 
            Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        // 显示菜单标题
        std::cout << std::endl << "---Connect Neurons---" << std::endl;
        
        // 获取前驱神经元索引
        size_t fromIndex = 0;
        bool validFromInput = false;
        string fromInput;
        
        while (!validFromInput) {
            std::cout << "Enter source neuron global index: ";
            getline(cin, fromInput);
            
            if (fromInput.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                fromIndex = stoul(fromInput);
                validFromInput = true;
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid neuron index." << std::endl;
            }
        }
        
        // 获取后继神经元索引
        size_t toIndex = 0;
        bool validToInput = false;
        string toInput;
        
        while (!validToInput) {
            std::cout << "Enter target neuron global index: ";
            getline(cin, toInput);
            
            if (toInput.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                toIndex = stoul(toInput);
                validToInput = true;
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid neuron index." << std::endl;
            }
        }
        
        // 获取连接权重
        double weight = 0.0;
        bool validWeight = false;
        string weightInput;
        
        while (!validWeight) {
            cout << "Enter connection weight: ";
            getline(cin, weightInput);
            
            if (weightInput.empty()) {
                std::cout << "Input cannot be empty." << std::endl;
                continue;
            }
            
            try {
                weight = stod(weightInput);
                validWeight = true;
            } catch (...) {
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
            }
        }
        
        // 调用控制器连接神经元
        Controller::RES res = Ctrller->ConnectNeuronsInCurrentNetwork(
            fromIndex, toIndex, weight);
        
        // 显示操作结果
        if (res == Controller::RES::OK) {
            std::cout << "Neurons connected successfully!" << std::endl;
        } else {
            std::cout << "Failed to connect neurons: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否继续连接
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to modify menu? Y/N?" << std::endl
            << "(N for Connect another pair of neurons)";
            getline(cin, selection);
        }
    }
    return "";
}

//------------------------------------------------------------------------------
// 网络分析与执行
//------------------------------------------------------------------------------

//函数名：ValidateNetworkMenu
//功能：验证当前神经网络有效性的交互界面
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::ValidateNetworkMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || 
            Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        // 显示菜单标题
        std::cout << std::endl << "---Validate Network Structure---" << std::endl;
        
        // 调用控制器验证网络
        Controller::RES res = Ctrller->ValidateCurrentNetwork();
        
        // 显示验证结果
        if (res == Controller::RES::OK) {
        std::cout << "Network validation successful! No issues found." << std::endl;
        } else {
                std::cout << "Network validation issue: " 
                << Controller::RES_STR[static_cast<int>(res)] << std::endl;
                }

        
        // 询问用户是否重新验证
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to main menu? Y/N?" << std::endl
            << "(N for Re-validate network)";
            getline(cin, selection);
        }
    }
    return "";
}

//函数名：InferenceMenu
//功能：对当前神经网络执行推理的交互界面
//入口参数：无
//出口参数：无
//返回值：空字符串
string View::InferenceMenu() const {
    shared_ptr<Controller> Ctrller = Controller::GetInstance();
    string selection;

    while (selection != "Y" && selection != "y") {
        // 检查是否有选中的网络
        if (Ctrller->GetNetworks().empty() || 
            Ctrller->CurrentNetworkIndex >= Ctrller->GetNetworks().size()) {
            std::cout << "No current network available. Please create or select a network first." << std::endl;
            return "";
        }

        // 显示菜单标题
        std::cout << std::endl << "---Perform Network Inference---" << std::endl;
        
        // 获取当前网络
        auto& network = *Ctrller->GetNetworks()[Ctrller->CurrentNetworkIndex];
        size_t inputNeuronCount = network.GetInputMarker().size();
        
        // 检查输入神经元数量
        if (inputNeuronCount == 0) {
            std::cout << "Network has no input neurons defined. Cannot perform inference." << std::endl;
            return "";
        }
        
        // 获取输出神经元数量
        size_t outputNeuronCount = network.GetOutputMarker().size();
        
        // 准备输入数据
        std::vector<double> inputValues;
        inputValues.reserve(inputNeuronCount);
        
        std::cout << "Enter " << inputNeuronCount << " input values (one per line):" << std::endl;
        
        // 获取用户输入的每个值
        for (size_t i = 0; i < inputNeuronCount; i++) {
            std::string inputStr;
            bool validInput = false;
            
            while (!validInput) {
                std::cout << "Input value " << (i + 1) << ": ";
                std::getline(std::cin, inputStr);
                
                try {
                    double value = std::stod(inputStr);
                    inputValues.push_back(value);
                    validInput = true;
                } catch (const std::exception& e) {
                    std::cout << "Invalid input. Please enter a valid number." << std::endl;
                }
            }
        }
        
        // 执行推理
        Controller::RES res = Ctrller->InferenceOnCurrentNetwork(inputValues);
        
        // 处理推理结果
        if (res == Controller::RES::OK) {
            // 获取并显示输出结果
            auto outputValues = network.Inference(inputValues); // 通过Network对象获取输出
            
            std::cout << std::endl << "Inference Result (" << outputNeuronCount << " outputs):" << std::endl;
            for (size_t i = 0; i < outputValues.size(); i++) {
                std::cout << "Output " << (i + 1) << ": " << outputValues[i] << std::endl;
            }
        } else {
            std::cout << "Inference failed: " 
                 << Controller::RES_STR[static_cast<int>(res)] << std::endl;
        }
        
        // 询问用户是否重新推理
        selection.clear();
        while (selection != "Y" && selection != "N" 
               && selection != "y" && selection != "n") {
            std::cout << std::endl << "Back to main menu? Y/N?" << std::endl
            << "(N for Perform another inference)";
            getline(cin, selection);
        }
    }
    return "";
}