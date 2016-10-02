#include <string>

#include "consumer.h"

namespace User {

static bool is_login = false;
static Consumer* co = nullptr;

bool isLogin() {
    return is_login;
}

int Login(sql::Connection*& conn) {
    sql::Statement* stmt = conn->createStatement();
    sql::ResultSet* res;
    std::stringstream ss_command;

    if (is_login == true || co != nullptr) {
        std::cout << "系统发现有用户正在登陆使用! 账户名称:" << co->GetUsername().c_str() << std::endl;
        std::cout << "您是否要注销这个账户登陆?[0-否, 非0-是 ]" << std::endl;
        int choose = 0;
        std::cin >> choose;
        if (choose) {
            delete co;
            is_login = false;
        }
        else {
            return EXIT_SUCCESS;
        }
    }
    try {
        std::string username;
        std::string password;

        std::cout << "请输入您的账户名: ";
        std::cin >> username;
        ss_command << "SELECT * FROM userinfo WHERE userinfo.UserName = \"" << username.c_str() << "\";";
        stmt->execute("set names 'gbk';");
        res = stmt->executeQuery(ss_command.str());
        if (res->next()) {
            while (1) {
                std::cout << "请输入您账户的密码: ";
                std::cin >> password;
                ss_command.clear();
                ss_command.str("");
                ss_command << "select * from userinfo where UserName='" << username.c_str() << "' and Password='" << password.c_str() << "';";
                res = stmt->executeQuery(ss_command.str());
                if (res->next()) {
                    co = new Consumer(res->getInt("UserID"), res->getString("UserName"), res->getString("Password"), res->getInt("Balance"), res->getBoolean("Alimit"));
                    is_login = true;

                    delete stmt;
                    delete res;
                    return EXIT_SUCCESS;
                }
                else {
                    std::cout << "账户密码输入错误，请重试！" << std::endl;
                }
            }

        }
        else {
            std::cout << "该用户名不存在！" << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: SQLException in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

    }
    return EXIT_FAILURE;
}

int Logout() {
    if (is_login == true || co != nullptr) {
        delete co;
        co = nullptr;
        is_login = false;
        std::cout << "注销成功" << std::endl;
    }
    else {
        std::cout << "注销失败: 并未发现有用户登录" << std::endl;
    }
    return EXIT_SUCCESS;
}

int RegUser(sql::Connection*& conn) {
    sql::Statement* stmt = conn->createStatement();
    sql::ResultSet* res;
    std::stringstream ss_command;

    int user_id;
    std::string user_name;
    std::string pass_word;
    std::cout << "请输入要创建的账户ID: ";
    std::cin >> user_id;
    std::cout << "请输入要创建的账户名: ";
    std::cin >> user_name;
    std::cout << "请输入该创建账户密码: ";
    std::cin >> pass_word;

    try {
        stmt->execute("set names 'gbk';");
        ss_command << "INSERT INTO userinfo(UserID,UserName,Password,Balance,Alimit) VALUES(" << user_id << ",'" << user_name.c_str() << "','" << pass_word.c_str() << "',0,0);";
        stmt->execute(ss_command.str());

        ss_command.clear();
        ss_command.str("");
        ss_command << "select * from userinfo where UserID=" << user_id << " and UserName='" << user_name.c_str() << "' and Password='" << pass_word.c_str() << "';";
        res = stmt->executeQuery(ss_command.str());;
        if (res->next()) {
            conn->commit(); // 记得提交
            std::cout << "用户注册成功！" << std::endl;

            delete stmt;
            delete res;
            return EXIT_SUCCESS;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: SQLException in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

    }
    return EXIT_FAILURE;
}

int DelUser(sql::Connection*& conn) {
    sql::Statement* stmt;
    sql::ResultSet* res;
    std::stringstream ss_command;

    if (co == nullptr || is_login == false) {
        std::cout << "错误：您还未登陆您的账户，无法显示你的信息" << std::endl;
        return EXIT_FAILURE;
    }
    else {
        if (!co->isAlimit()) {
            std::cout << "错误：您的账户非管理员权限，无法删除用户" << std::endl;
            return EXIT_FAILURE;
        }
    }

    int user_id;
    std::cout << "请输入要删除用户的ID: ";
    std::cin >> user_id;

    try {
        stmt = conn->createStatement();
        ss_command << "select * from userinfo where UserID=" << user_id << ";";
        res = stmt->executeQuery(ss_command.str());
        if (res->next()) {
            ss_command.clear();
            ss_command.str("");
            ss_command << "delete from userinfo where UserID=" << user_id << ";";
            stmt->execute(ss_command.str());

            ss_command.clear();
            ss_command.str("");
            ss_command << "select * from userinfo where UserID=" << user_id << ";";
            res = stmt->executeQuery(ss_command.str());
            if (!res->next()) {
                std::cout << "用户删除成功" << std::endl;
                conn->commit();
                delete stmt;
                delete res;
                return EXIT_SUCCESS;
            }
            else {
                std::cout << "用户删除失败" << std::endl;
                delete stmt;
                delete res;
                return EXIT_FAILURE;
            }
        }
        else {
            std::cout << "错误：您要删除的用户不存在，请检查您输入的用户ID" << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: SQLException in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

    }
    catch (std::runtime_error& e) {
        std::cout << "Error: runtime error in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    return EXIT_FAILURE;
}

void ViewUserInfoTable(sql::ResultSet*& res) {
    std::cout << "   账户标识    账户名称   账户密码   账户余额   管理员权限" << std::endl;
    while (res->next()) {
        std::cout << std::setw(8) << res->getInt("UserID");
        std::cout << std::setw(12) << res->getString("UserName");
        std::cout << std::setw(12) << res->getString("PassWord");
        std::cout << std::setw(8) << res->getInt("Balance");
        std::cout << "          " << (res->getBoolean("Alimit") ? "true" : "false") << std::endl;
    }
}

int ReadUserInfo(sql::Connection*& conn) {
    sql::Statement* stmt;
    sql::ResultSet* res;
    std::stringstream ss_command;

    if (co == nullptr || is_login == false) {
        std::cout << "错误：您还未登陆您的账户，无法显示你的信息" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        stmt = conn->createStatement();
        if (co->isAlimit()) { // 如果登陆账户为管理员，可查看全部用户信息
            ss_command << "select * from userinfo;";
        }
        else {
            ss_command << "select * from userinfo where UserName='" << co->GetUsername() << "' and UserID=" << co->GetUserID() << ";";
        }

        res = stmt->executeQuery(ss_command.str());
        ViewUserInfoTable(res);
        delete stmt;
        delete res;
    }
    catch (sql::SQLException& e) {
        std::cout << "Error: SQLException in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

    }
    catch (std::runtime_error& e) {
        std::cout << "Error: runtime error in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}

int ChangePassword(sql::Connection*& conn) {
    sql::Statement* stmt;
    sql::ResultSet* res;

    std::string confirm_pass;
    std::string new_password;
    std::string old_password;

    std::stringstream ss_command;

    int choose = 0;

    if (co == nullptr || is_login == false) {
        std::cout << "错误：您还未登陆您的账户，无法更改你的密码" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        stmt = conn->createStatement();

        while (1) {
            std::cout << "请输入您的旧密码：";
            std::cin >> old_password;
            ss_command << "select * from userinfo where UserID=\"" << co->GetUserID() << "\" and Password=\"" << old_password << "\";";
            res = stmt->executeQuery(ss_command.str());
            if (res->next()) {
                break;
            }
            else {
                std::cout << "错误：您输入的旧密码错误！是否重新输入？[0-是，非0-否]:" << std::endl;
                std::cin >> choose;
                if (choose) {
                    std::cout << "更改密码操作失败！" << std::endl;
                    delete stmt;
                    delete res;
                    return EXIT_FAILURE;
                }
            }
        }

        // 清除指令流数据
        ss_command.clear();
        ss_command.str("");

        while (1) {
            std::cout << "请输入您的新密码：";
            std::cin >> new_password;
            std::cout << "请确认您的新密码：";
            std::cin >> confirm_pass;
            if (new_password == confirm_pass) {
                ss_command << "update userinfo set Password=\"" << new_password << "\" where UserID=" << co->GetUserID() << ";";
                if (stmt->executeUpdate(ss_command.str()) == 1) {
                    std::cout << "更改密码操作成功！" << std::endl;
                    delete stmt;
                    delete res;
                    return EXIT_SUCCESS;
                }
                else {
                    std::cout << "更改密码操作失败！" << std::endl;
                    delete stmt;
                    delete res;
                    return EXIT_FAILURE;
                }
            }
            else {
                std::cout << "您两次输入的密码不一致，请重试！" << std::endl;
            }

        }
    }
    catch (sql::SQLException& e) {
        std::cout << "更改密码操作失败！" << std::endl;
        std::cout << "Error: SQLException in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

    }
    catch (std::runtime_error& e) {
        std::cout << "更改密码操作失败！" << std::endl;
        std::cout << "Error: runtime error in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
    }
}

} // namespace