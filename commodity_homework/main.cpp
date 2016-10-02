#include "goods.h"
#include "consumer.h"

enum Menu_Type { // 菜单类型
    Exit       = 0,
    UserConfig = 4,
};

void print_menu();                       // 输出菜单页面

void print_userconfig();                 // 输出用户设置页面

void userconfig(sql::Connection*& conn); // 分配用户输入选项处理

int main(int argc, char* argv[]) {
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* conn;

    const std::string host(argc > 2 ? argv[2] : HOST);
    const std::string user(argc > 3 ? argv[3] : USER);
    const std::string pass(argc > 4 ? argv[4] : PASS);
    const std::string mydb(argc > 5 ? argv[5] : MYDB);

    if (ConnectToMySQLServer(driver, conn, host, user, pass)) {
        std::cout << "Connect to MySQL Server failed!" << std::endl << std::endl;
        delete conn;
        return EXIT_FAILURE;
    }
    if (ConnectToMySQLDatabase(conn, mydb)) {
        std::cout << "Connect to your database " << mydb << " failed!" << std::endl << std::endl;
        delete conn;
        return EXIT_FAILURE;
    }

    int choose = 0;
    while(1){
        print_menu();
        std::cin >> choose;

        switch(choose){
        case Good::Good_Type::Elec_App:
            std::cout << "------------------电器类列表---------------------" << std::endl;
            Good::ReadGoodList(conn, Good::Good_Type::Elec_App);
            break;
        case Good::Good_Type::Clothing:
            std::cout << "------------------服装类列表---------------------" << std::endl;
            Good::ReadGoodList(conn, Good::Good_Type::Clothing);
            break;
        case Good::Good_Type::Food:
            std::cout << "------------------食品类列表---------------------" << std::endl;
            Good::ReadGoodList(conn, Good::Good_Type::Food);
            break;
        case Menu_Type::UserConfig:
            userconfig(conn);
            break;
        case Menu_Type::Exit:
            std::cout << "\n-----------------谢谢你的惠顾--------------------\n" << std::endl;
            User::Logout();
            goto _END;
            break;
        default:
            std::cout << "错误：非法输入参数, 有效范围是[0-4]" << std::endl;
            break;
        }
    }
_END:
    /* Clean up */
    conn->close();
    delete conn;

    return EXIT_SUCCESS;
}

void print_menu(){
    std::cout << "\n\n==================大傻鸭超市=====================" << std::endl;
    std::cout << "                  1、电器类                         " << std::endl;
    std::cout << "                  2、服装类                         " << std::endl;
    std::cout << "                  3、食品类                         " << std::endl;
    std::cout << "                  4、用户设置                       " << std::endl;
    std::cout << "                  0、退出                           " << std::endl;
    std::cout << "\n请输入您的选择: ";
}

void print_userconfig() {
    std::cout << "-------------------用户设置----------------------" << std::endl;
    std::cout << "                  1、用户登陆                         " << std::endl;
    std::cout << "                  2、注册用户                         " << std::endl;
    std::cout << "                  3、删除用户                         " << std::endl;
    std::cout << "                  4、查看用户信息                     " << std::endl;
    std::cout << "                  5、更改密码                         " << std::endl;
    std::cout << "                  6、用户注销                         " << std::endl;
    std::cout << "                  0、退出界面                         " << std::endl;
    std::cout << "\n请输入您的选择: ";

}

void userconfig(sql::Connection*& conn) {
    int choose =0 ;
    while (1) {
        print_userconfig();
        std::cin >> choose;
        switch (choose) {
        case User::User_Config::Login_User:
            User::Login(conn);
            break;
        case User::User_Config::Reg_User:
            User::RegUser(conn);
            break;
        case User::User_Config::Del_User:
            User::DelUser(conn);
            break;
        case User::User_Config::User_Info:
            User::ReadUserInfo(conn);
            break;
        case User::User_Config::Change_Pass:
            User::ChangePassword(conn);
            break;
        case User::User_Config::Logout_User:
            User::Logout();
            break;
        case Menu_Type::Exit:
            return;
            break;
        default:
            std::cout << "错误：非法输入参数, 有效范围是[0-5]" << std::endl;
            break;
        }
    }
}
