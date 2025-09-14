//
//  main.cpp
//
//  Created by 孙李智 on 2025/7/17.
//

//------------------------------------------------------------------------------
//【文件名】main.cpp
//【功能模块和目的】程序入口
//【开发者及日期】孙李智 2025/7/17
//【更改记录】
//------------------------------------------------------------------------------

//View类所需头文件
#include "./View/View.hpp"
//Importer所需头文件
#include "./Network/Importer.hpp"
//Layer所需头文件
#include "./Network/Layer.hpp"
//Neuron所需头文件
#include "./Network/Neuron.hpp"
//FilePorter所需头文件
#include "./Network/FilePorter.hpp"
//Network_ANN_Importer所需头文件
#include "./Network/Network_ANN_Importer.hpp"
//Network_ANN_Exporter所需头文件
#include "./Network/Network_ANN_Exporter.hpp"
//StaticContainer所需头文件
#include "./Network/StaticContainer.hpp"
//cassert头文件
#include <cassert>
//sstream头文件
#include <sstream>
//filesystem头文件
#include <filesystem>
//type_traits头文件
#include <type_traits>
//Controller所需头文件
#include "./Controller/Controller.hpp"


using Importer_test = FilePorter<FilePorterType::IMPORTER>;
using Exporter_test = FilePorter<FilePorterType::EXPORTER>;

int main() {
    {
        //创建层
        auto layer = std::make_shared<Layer>(1);
        assert(layer->GetIndex() == 1);
        assert(layer->GetNeuronCount() == 0);
        //设置层索引
        layer->SetIndex(2);
        //添加一个神经元
        auto neuron = std::make_shared<Neuron>(101);
        neuron->SetBias(0.5);
        layer->AddNeuron(neuron);
        assert(layer->GetNeuronCount() == 1);
        assert(neuron->GetLayer() == layer);
        //获取神经元
        const auto& neurons = layer->GetNeurons();

    }
    //静态容器
    {
        StaticContainer<int,5> sc;
        assert(sc.COUNT == 5);
    }
    
    {
        // 1.注册导入器
        Network_Importer::Register<Network_ANN_Importer>();
        // 2.获取导入器
        auto importer = Network_Importer::GetIstanceByExtName("ANN");
        // 3.导入网络模型
        Network network = importer->LoadFromFile("simple.ANN");
        std::cout<<"name:"<<network.Name<<std::endl;
    }
    
    {
        //导入扩展名
        assert(Importer_test::GetExtName("simple.ANN") == "ANN");
        //导出扩展名
        assert(Exporter_test::GetExtName("simple.ANN") == "ANN");
    }
    
    {   
        //获取单例
        auto controller = Controller::GetInstance();
        //创建新网络
        auto result = controller->CreateAndSetToCurrent();
        if(result == Controller::RES::OK){
            std::cout<<"创建网络成功\n";
        }else{
            std::cerr<<"错误："
                     << Controller::RES_STR[static_cast<int>(result)]
                     << "\n";
        }
        //添加层
        controller->AppendLayerToCurrentNetwork();
        //显示统计信息
        controller->ShowStatisticsForCurrentNetwork();
        //验证指定网络的合理性
        controller->ValidateCurrentNetwork();
    }
    {
        //测试
        View v;
        auto controller = Controller::GetInstance();
        std::cout<<v.MainMenu()<<std::endl;
    }
    return 0;
}
