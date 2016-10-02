// Standard Lib
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

// MySQL Connector Lib
#include "mysql_driver.h"
#include "cppconn\connection.h"
#include "cppconn\driver.h"
#include "cppconn\exception.h"
#include "cppconn\prepared_statement.h"
#include "cppconn\resultset.h"
#include "cppconn\statement.h"

#include "goods.h"

// 默认连接信息
#define HOST "localhost"
#define USER "root"
#define PASS "123456"
#define MYDB "commodity_homework"

int ConnectToMySQLServer(sql::mysql::MySQL_Driver*& driver, sql::Connection*& conn, std::string host, std::string user, std::string pass); // 连接到Mysql服务器

int ConnectToMySQLDatabase(sql::Connection*& conn, std::string mydb); // 连接到数据库
