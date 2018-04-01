#include <iostream>
#include<winsqlite\winsqlite3.h>
#include<Windows.h>
#include<vector>
#pragma comment(lib,"winsqlite3.lib")
using namespace std;
bool test(sqlite3* db);
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)NotUsed);
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
string UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), strUTF8.length(), NULL, 0);
	WCHAR* wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (const char*)strUTF8.c_str(), strUTF8.length(), wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, len, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, len, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}
string Sqlite_getdata(const char * filename,string table,int index)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open(filename, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else {
		fprintf(stdout, "Opened database successfully\n");
	}

	string strSql = "select * from "+ table;
	char** pResult;
	int nRow = 0;
	int nCol = 0;
	int nResult = sqlite3_get_table(db, strSql.c_str(), &pResult, &nRow, &nCol, &zErrMsg);
	if (nResult != SQLITE_OK)
	{
		sqlite3_close(db);
		cout << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		return 0;
	}

	string strOut;
	strOut = pResult[index];
	sqlite3_free_table(pResult);  //使用完后务必释放为记录分配的内存，否则会内存泄漏
	//cout << pResult[1][2] << endl;
	sqlite3_close(db);
	return UTF8ToGBK(strOut);
}
bool Iskey(string inputstring, string srcstring)
{
	std::vector<std::string> resVec;
	int pos = srcstring.find("|", 0);
	 srcstring = srcstring + "|";
	while (pos!=string::npos)
	{
		string splitstring = srcstring.substr(0,pos);
		resVec.push_back(splitstring);
		srcstring = srcstring.substr(pos+1, srcstring.size());
		//cout << srcstring.c_str();
		pos = srcstring.find("|", 0);
		
	}
	for (size_t i = 0; i < resVec.size(); i++)
	{
		//cout << resVec[i].c_str() << endl;;
		if( strstr(inputstring.c_str(),resVec[i].c_str()))
			return 1;
	}
	return false;
}
int main()
{
    // Create SQL statement
   /* 
    sql = "CREATE TABLE person("  \
        "id INT PRIMARY KEY     NOT NULL," \
        "name           TEXT    NOT NULL," \
        "age            INT     NOT NULL," \
        "address        CHAR(50)," \
        "salary         REAL );";
        */

    /* Create SQL statement */
    /*
    sql = "INSERT INTO person (id,name,age,address,salary) "  \
        "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
        "INSERT INTO person (id,name,age,address,salary) "  \
        "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
        "INSERT INTO person (id,name,age,address,salary)" \
        "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
        "INSERT INTO person (id,name,age,address,salary)" \
        "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
        */

    /* Create SQL statement */
    
   //sql = "SELECT * from person";
    

    /* Create merged SQL statement */
    /*
    sql = "UPDATE person set salary = 25000.00 where id=1; " \
        "SELECT * from person";
        */

    /* Create merged SQL statement */
    /*sql = "DELETE from person where id=2; " \
        "SELECT * from person";*/

    /* Execute SQL statement */


	/*for (int i = 0; i<nRow; i++)
	{
		for (int j = 0; j<nCol; j++)
		{
			strOut += pResult[j];
			strOut += ":";
			strOut += pResult[nIndex];
			strOut += "\n";
			++nIndex;
		}
	}*/
	string result= Sqlite_getdata("test.db", "first", 2);
	cout<< result.c_str();
	cout<<Iskey("你好A", result);
	while (true);
	return 0;
}
