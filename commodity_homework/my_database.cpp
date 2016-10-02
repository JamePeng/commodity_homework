#include "my_database.h"

int ConnectToMySQLServer(sql::mysql::MySQL_Driver*& driver, sql::Connection*& conn, std::string host, std::string user, std::string pass) {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect(host, user, pass);
        if (!conn->isValid()) {
            conn->close();
            return EXIT_FAILURE;
        }
    } catch (sql::SQLException& e) {
        std::cout << "Error: SQLException in" << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

        return EXIT_FAILURE;

    } catch (std::runtime_error& e) {
        std::cout << "Error: runtime error in" << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int ConnectToMySQLDatabase(sql::Connection*& conn, std::string mydb) {
    try {
        /* Turn off the autocommit */
        conn->setAutoCommit(0);
        /* Select appropriate database schema */
        conn->setSchema(mydb);
    } catch (sql::SQLException& e) {
        std::cout << "Error: SQLException in" << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

        return EXIT_FAILURE;

    } catch (std::runtime_error& e) {
        std::cout << "Error: runtime error in" << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
