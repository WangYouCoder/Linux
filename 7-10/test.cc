// #include <iostream>
// #include <mysql/mysql.h>
// #include <stdio.h>
// #include <stdlib.h>

// MYSQL *conn_ptr;
// MYSQL_RES *res_ptr;
// MYSQL_ROW sqlrow;
// unsigned int timeout = 7; //超时时间7秒
 
// void dispaly_row(MYSQL *ptr)
// {
//     unsigned int field_count = 0;
//     while (field_count < mysql_field_count(ptr)) //返回在结果结合中字段的数目
//     {
//         printf("%s", sqlrow[field_count]);
//         field_count++;
//     }
//     printf("\n");
// }
 
// void display_header()
// {
//     MYSQL_FIELD *field_ptr;
//     printf("Column details:\n");
//     while ((field_ptr = mysql_fetch_field(res_ptr)) != NULL) //返回列的定义字段信息
//     {
//         printf("\t Name:%s\n", field_ptr->name);
//         printf("\t Type:");
//         if (IS_NUM(field_ptr->type)) //若字段类型为数字
//         {
//             printf("Numeric field\n");
//         }
//         else
//         {
//             switch (field_ptr->type)
//             {
//             case FIELD_TYPE_VAR_STRING:
//                 printf("VACHAR\n");
//                 break;
//             case FIELD_TYPE_LONG:
//                 printf("LONG\n");
//                 break;
//             default:
//                 printf("Type is %d,check in mysql_com.h\n", field_ptr->type);
//             }
//         }
//         printf("\t Max width %ld \n", field_ptr->length);
//         if (field_ptr->flags & AUTO_INCREMENT_FLAG)
//             printf("\t Auto increments\n");
//         printf("\n");
//     }
// }
// int main()
// {
//     int ret = 0;
//     int first_row = 1;
//     conn_ptr = mysql_init(NULL); //初始化
//     if (!conn_ptr)
//     {
//         printf("mysql_init failed!\n");
//         return -1;
//     }
 
//     ret = mysql_options(conn_ptr, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout); //设置超时选项
//     if (ret)
//     {
//         printf("Options Set ERRO!\n");
//     }
//     conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "用户名密码", "数据库名", 3306, NULL, 0); //连接MySQL testdb数据库，端口为3306
//     if (conn_ptr)
//     {
//         printf("Connection Succeed!\n");
 
//         ret = mysql_query(conn_ptr, "SELECT * FROM user"); //执行SQL语句
//         if (!ret)
//         {
//             res_ptr = mysql_use_result(conn_ptr);
//             if (res_ptr)
//             {
//                 display_header();
//                 printf("Retrieved %lu rows\n", (unsigned long)mysql_num_rows(res_ptr)); //在结果集合中返回行的数量
//                 while ((sqlrow = mysql_fetch_row(res_ptr)))                             //返回store_result中得到的结构体，并从中检索单行
//                 {
//                     dispaly_row(conn_ptr);//打印表中的内容
//                 }
//             }
 
//             if (mysql_errno(conn_ptr))
//             {
//                 printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); //返回错误代码、错误消息
//                 return -2;
//             }
 
//             mysql_free_result(res_ptr);
//         }
//         else
//         {
//             printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); //返回错误代码、错误消息
//             return -3;
//         }
 
//         mysql_close(conn_ptr);
//         printf("Connection closed!\n");
//     }
//     else //错误处理
//     {
//         printf("Connection Failed!\n");
//         if (mysql_errno(conn_ptr))
//         {
//             printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); //返回错误代码、错误消息
//         }
//         return -2;
//     }
 
//     return 0;
// }


#include <iostream>
#include <mysql/mysql.h>

int main()
{
    std::cout << "mysql client version" << mysql_get_client_info() << std::endl;
    return 0;
}