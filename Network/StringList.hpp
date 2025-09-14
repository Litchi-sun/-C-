//
//  StringList.hpp
//
//  Created by 孙李智 on 2025/7/12.
//

//------------------------------------------------------------------------------
//【文件名】StringList.hpp
//【功能模块和目的】字符串列表特化类声明
//【开发者及日期】孙李智 2025/7/12
//【更改记录】
//------------------------------------------------------------------------------

#ifndef StringList_hpp
#define StringList_hpp

//std::string所属头文件
#include <string>
//DynamicContainer所属头文件
#include "DynamicContainer.hpp"

//使用类声明
using StringList = DynamicContainer<std::string>;

#endif /* StringList_hpp */
