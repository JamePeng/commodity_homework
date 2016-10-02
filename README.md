# commodity_homework
A test sample for MySQL Connector C++

Note:

1.Install and deploy MySQL database.

   http://dev.mysql.com/downloads/installer/

	 My default config:

		host: 127.0.0.1:3036

		user: root

		pass: 123456

2.Install Connector/C++.

   http://dev.mysql.com/downloads/connector/cpp/

3.Compile a Boost libiary.

   http://www.boost.org/

4.Prepare your database for mysql

   (bla bla bla)

5.Open the .sln file of the project, then configure the project property:

	(1) [Configure property] -> [VC++ dir] -> [Include dir]:

		append <your boost libiary dir> and <your mysql connector dir>/include/

	(2) [Configure property] -> [VC++ dir] -> [Lib dir]:

		append <your mysql connector dir>/lib/opt/

	(3) [Configure property] -> [Linker]->[Input]->[Additional Dependencies]:

		append "mysqlcppconn.lib"