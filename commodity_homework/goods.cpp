#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "goods.h"

namespace Good {

std::string GetGoodTypeStr(Good_Type type) {
    switch (type) {
    case Good_Type::Elec_App:
        return "电器";
    case Good_Type::Clothing:
        return "服装";
    case Good_Type::Food:
        return "食品";
    }
    return "";
}

void ViewGoodListTable(sql::ResultSet*& res) {
    std::cout << "   商品标识    商品名称    商品类型   商品单价   商品产地  商品存量" << std::endl;
    while (res->next()) {
        std::cout << std::setw(8) << res->getInt("GoodID");
        std::cout << std::setw(15) << res->getString("GoodName");
        std::cout << std::setw(10) << res->getString("GoodType");
        std::cout << std::setw(9) << res->getInt("UnitPrice");
        std::cout << std::setw(12) << res->getString("Origin");
        std::cout << std::setw(8) << res->getInt("GoodNum") << std::endl;;
    }
}

void ReadGoodList(sql::Connection*& conn, Good_Type type) {
    sql::Statement* stmt;
    sql::ResultSet* res;
    try {
        stmt = conn->createStatement();
        std::stringstream ss_command;
        ss_command << "SELECT goodlist.GoodID, goodlist.GoodName, goodlist.GoodType, goodlist.UnitPrice, goodlist.Origin, goodstock.GoodNum";
        ss_command << " FROM goodlist";
        ss_command << " LEFT JOIN goodstock ON goodstock.GoodID = goodlist.GoodID";
        switch (type) {
        case Good_Type::Elec_App:
        case Good_Type::Clothing:
        case Good_Type::Food:
            ss_command << " WHERE goodlist.GoodType='" << GetGoodTypeStr(type) << "';";
            //std::cout << ss_command.str() << std::endl;
            stmt->execute("set names 'gbk';"); // Important!
            res = stmt->executeQuery(ss_command.str());
            ViewGoodListTable(res);
            delete stmt;
            delete res;
            break;
        default:
            std::cout << " -- Unknown Good Type! --" << std::endl;
            break;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: SQLException in" << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    catch (std::runtime_error& e) {
        std::cout << "Error: runtime error in" << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
    }
}

} // namespace