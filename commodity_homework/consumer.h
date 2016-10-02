#ifndef CONSUMER_H_INCLUDED
#define CONSUMER_H_INCLUDED

#include <iostream>

#include "my_database.h"

namespace User {

enum User_Config {
    Login_User  = 1,
    Reg_User    = 2,
    Del_User    = 3,
    User_Info   = 4,
    Change_Pass = 5,
    Logout_User = 6,
};

class Consumer {
public:
    Consumer(int id, std::string name, std::string pass, int mon, bool alim) :userid(id), username(name), password(pass), money(mon), alimit(alim) {

    }
    ~Consumer() {
        userid = 0;
        username.clear();
        password.clear();
        money = 0;
    }
    int Recharge(int cash) {    // 充值
        if (cash <= 500) {
            std::cout << "Recharge failed! Valid cash number is [>=500]" << std::endl;
            return -1;
        }
        else {
            money += cash;
            std::cout << "Recharge succeed! Consumer Money: " << money << std::endl;
        }
        return 0;
    }
    int Consumption(int cash) { // 消费
        if (cash > money) {
            std::cout << "Consumption failed! The cash was bigger than user money" << std::endl;
            return -1;
        }
        else {
            money -= cash;
        }
        return 0;
    }
    void SetUsetname(std::string namestr) {
        username = namestr;
    }
    std::string GetUsername() {
        return username;
    }
    void SetPassword(std::string passstr) {
        password = passstr;
    }
    std::string GetPassword() {
        return password;
    }
    int GetUserID() {
        return userid;
    }
    bool isAlimit() {
        return alimit;
    }
private:
    int userid;           // 账户标识
    std::string username; // 账户名称
    std::string password; // 账户密码
    int money;            // 账户余额
    bool alimit;          // 管理员权限
};
bool isLogin();                      // 确认登陆状况

int Login(sql::Connection*& conn);   // 用户登陆

int Logout();                        // 用户登出

int RegUser(sql::Connection*& conn); // 用户注册

int DelUser(sql::Connection*& conn); // 用户删除（管理员）

void ViewUserInfoTable(sql::ResultSet*& res); // 查看用户信息

int ReadUserInfo(sql::Connection*& conn);   // 读取用户信息

int ChangePassword(sql::Connection*& conn); // 更改用户密码

} // namespace

#endif // CONSUMER_H_INCLUDED
