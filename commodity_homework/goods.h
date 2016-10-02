#ifndef GOODS_H_INCLUDED
#define GOODS_H_INCLUDED

#include <string>

#include "my_database.h"

namespace Good {

enum Good_Type { // 商品类型
    Elec_App = 1,
    Clothing = 2,
    Food = 3,
};

struct Good {
    unsigned int good_code;    // 商品编号
    std::string good_name;     // 商品名称
    unsigned int unit_price;   // 单价
    int good_num;              // 商品数量
    std::string origin;        // 原产地
};

void ViewGoodListTable(sql::ResultSet*& res);              // 遍历整个GoodList

void ReadGoodList(sql::Connection*& conn, Good_Type type); // 查看相应type的GoodList

} // namespace
#endif // GOODS_H_INCLUDED
