#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(){

        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;

        rc = sqlite3_open("test.db", &db);

        if(rc){
                fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(db));
                return 0;
        }else{
                fprintf(stderr, "Opened database successfully\n");
        }
        sqlite3_close(db);

        return 0;
}       
