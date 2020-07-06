#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#define BUF_SIZE 4096

struct path_data
{
    char base_path[1024];
    char local_path[1024];
    char platform[1024];
    char app_name[1024];
};

char *get_local_dir_from_platform(char *platform_alias)
{
    sqlite3 *db = NULL;
    char **azResult = NULL;
    int i = 0;
    int nrow = 0;
    int ncolumn = 0;
    char *zErrMsg = NULL;
    sqlite3_open("/opt/.program_list.db", &db);

    printf("platform_alias===%s\n", platform_alias);
    char sql[1024];
    sprintf(sql, "SELECT local_dir FROM plat_form_info WHERE is_default_localdir is true and platform_alias is '%s';", platform_alias);
    // sprintf(sql, "SELECT platform_alias  FROM plat_form_info where platform_alias= 'ubuntu2004_2';");

    int ret = sqlite3_get_table(db, sql, &azResult, &nrow, &ncolumn, &zErrMsg);

    if (NULL != zErrMsg)
    {
        printf("222err msg=%s", zErrMsg);
        sqlite3_free_table(azResult);
        zErrMsg = NULL;
        exit(1);
    }

    if (ncolumn == 0 && nrow == 0)
    {
        puts("platform_alias  is not exist!  (localdir not found)");
        exit(1);
    }
    return azResult[nrow];
}

char *get_baseos_dir_from_platform(char *platform_alias)
{
    sqlite3 *db = NULL;
    char **azResult = NULL;
    int i = 0;
    int nrow = 0;
    int ncolumn = 0;
    char *zErrMsg = NULL;
    sqlite3_open("/opt/.program_list.db", &db);

    char sql[1024];
    sprintf(sql, "SELECT baseos_dir FROM plat_form_info WHERE  platform_alias is '%s';", platform_alias);
    int ret = sqlite3_get_table(db, sql, &azResult, &nrow, &ncolumn, 0);
    if (nrow == 0 && ncolumn == 0)
    {
        puts("platform_alias  is not exist!  (basedir not found)");
        exit(1);
    }
    return azResult[nrow];
}

void list_app()
{
}

void list_platform()
{
}

void install_app(struct path_data app_data)
{
    char *work_dir_path = "/opt/worker";

    char mount_shell[BUF_SIZE];
    char chroot_install[BUF_SIZE];
    char umount_shell[BUF_SIZE];

    snprintf(mount_shell, BUF_SIZE, "mount -n -t overlay overlayfs:/overlay -o lowerdir=%s,upperdir=%s,workdir=%s %s",
             app_data.base_path, app_data.local_path, work_dir_path, app_data.local_path);
    puts(mount_shell);
    system(mount_shell);

    sprintf(chroot_install, "chroot %s apt-get install %s", app_data.local_path, app_data.app_name);

    system(chroot_install);

    sprintf(umount_shell, "umount %s", app_data.local_path);

    system(umount_shell);

    //操作app数据库
    sqlite3 *app_db = NULL;
    int len;
    char *zErrMsg = NULL;
    /* 打开数据库 */
    len = sqlite3_open("/opt/.program_list.db", &app_db);
    if (len)
    {
        /*  fprintf函数格式化输出错误信息到指定的stderr文件流中  */
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(app_db)); //sqlite3_errmsg(db)用以获得数据库打开错误码的英文描述。
        sqlite3_close(app_db);
        exit(1);
    }

    // insert database

    char *insert_app_info[1024];

    sprintf(insert_app_info, "INSERT INTO app_info VALUES ('%s','%s','/opt/%s','%s');",
            app_data.app_name, app_data.platform, app_data.app_name, app_data.local_path);
    sqlite3_exec(app_db, insert_app_info, NULL, NULL, &zErrMsg);
}

void remove_app(struct path_data app_data)
{
    printf("[%s %s] %s: %s: %d\n",
           __DATE__, __TIME__, __FILE__, __func__, __LINE__);
    char *work_dir_path = "/opt/worker";

    char mount_shell[BUF_SIZE];
    char chroot_install[BUF_SIZE];
    char umount_shell[BUF_SIZE];

    snprintf(mount_shell, BUF_SIZE, "mount -n -t overlay overlayfs:/overlay -o lowerdir=%s,upperdir=%s,workdir=%s %s",
             app_data.base_path, app_data.local_path, work_dir_path, app_data.local_path);

    system(mount_shell);

    sprintf(chroot_install, "chroot %s apt-get remove %s", app_data.local_path, app_data.app_name);

    system(chroot_install);

    sprintf(umount_shell, "umount %s", app_data.local_path);

    system(umount_shell);

    //操作app数据库
    sqlite3 *app_db = NULL;
    int len;
    char *zErrMsg = NULL;
    /* 打开数据库 */
    len = sqlite3_open("/opt/.program_list.db", &app_db);
    if (len)
    {
        /*  fprintf函数格式化输出错误信息到指定的stderr文件流中  */
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(app_db)); //sqlite3_errmsg(db)用以获得数据库打开错误码的英文描述。
        sqlite3_close(app_db);
        exit(1);
    }

    // deletes database

    char *delete_app_info[1024];

    sprintf(delete_app_info, "DELETE FROM  app_info where app_name is '%s' and platform_alias is '%s' and localdir is '%s';",
            app_data.app_name, app_data.platform, app_data.local_path);
    sqlite3_exec(app_db, delete_app_info, NULL, NULL, &zErrMsg);
}

void install_platform()
{
}

void remove_platform()
{
}

void init_database()
{
    sqlite3 *app_db = NULL;
    int len;
    char *zErrMsg = NULL;
    /* 打开数据库 */
    len = sqlite3_open("/opt/.program_list.db", &app_db);
    if (len)
    {
        /*  fprintf函数格式化输出错误信息到指定的stderr文件流中  */
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(app_db)); //sqlite3_errmsg(db)用以获得数据库打开错误码的英文描述。
        sqlite3_close(app_db);
        exit(1);
    }

    char *platform_sql_table = " CREATE TABLE if not exists platform_info(\
         platform_alias VARCHAR(255),\
         platform VARCHAR(255),\
         baseos_dir VARCHAR(255),\
         local_dir VARCHAR(255),\
         is_default_localdir BOOLEAN\
         );";
    sqlite3_exec(app_db, platform_sql_table, NULL, NULL, &zErrMsg);

    char *create_app_info_table = "CREATE TABLE if not exists app_info(\
        app_name VARCHAR(255),\
        platform_alias VARCHAR(255),\
        appdir VARCHAR(255),\
        localdir VARCHAR(255),\
        app_id int AUTOINCREMENT,\
        );";
    sqlite3_exec(app_db, create_app_info_table, NULL, NULL, &zErrMsg);

    char *create_depends_info_table = "CREATE TABLE if not exists depends_info(\
        depend_name VARCHAR(255),\
        depend_version VARCHAR(255),\
        is_auto_install BOOLEAN,\
        depend_id int AUTOINCREMENT,\
        )";

    sqlite3_exec(app_db, create_depends_info_table, NULL, NULL, &zErrMsg);

    char *create_app_depends_table = "CREATE TABLE if not exists app_depend_info(\
        app_id int AUTOINCREMENT,\
        depend_id int AUTOINCREMENT,\
        )";

    sqlite3_exec(app_db, create_app_depends_table, NULL, NULL, NULL);
}

int main(int argc, char *argv[])
{

    init_database();

    //帮助界面
    if (2 == argc && !strcmp(argv[1], "help"))
    {
        puts("kylin install app@platform [--localdir=custom localpath]     In the development platform to install the app, \
                                                                          you can specify the installation directory");
        puts("kylin list app                                               Show installed apps and information");
        puts("kylin list platform                                          Show installed platforms and infomations");
        puts("kylin remove app@platform  [--localdir=custom localpath]     In the development platform to remove the app, \
                                                                           you can specify the installed directory");
        exit(0);
    }

    //如果没加参数，提示并退出
    if (1 == argc || !strcmp(argv[1], "help") && !strcmp(argv[1], "list") && !strcmp(argv[1], "install") && !strcmp(argv[1], "remove"))
    {
        puts("The command line is not parameterized, please use 'kylin help'");
        exit(0);
    }
    if (3 == argc && (strcmp(argv[1], "list") == 0))
    {
        if (!strcmp(argv[2], "app"))
        {
            list_app();
        }
        else if (strcmp(argv[2], "platform") == 0)
        {
            list_platform();
        }
        else
        {
            puts("kylin list app/platform!");
            exit(0);
        }
    }
    struct path_data base_local;
    if (!strcmp(argv[1], "install") || !strcmp(argv[1], "remove"))
    {

        int opt;
        int digit_optind = 0;
        int option_index = 0;
        char *string = "a::b:c:d";
        char *ret;
        static struct option long_options[] =
            {
                {"localdir", required_argument, NULL, 'r'},
                {NULL, 0, NULL, 0},
            };
        opt = getopt_long_only(argc, argv, string, long_options, &option_index);

        char *str = argv[2];

        //指定platform
        if ((ret = strchr(argv[2], '@')) != NULL)
        {
            memset(base_local.app_name, 0, 1024);
            strncpy(base_local.app_name, argv[2], ret - &(str[0]));
            memset(base_local.platform, 0, 1024);
            strcpy(base_local.platform, ret + 1);
        }
        else
        {
            memset(base_local.app_name, 0, 1024);
            strcpy(base_local.app_name, argv[2]);
            memset(base_local.platform, 0, 1024);
            //获取默认平台
            strcpy(base_local.platform, getenv("DEFAULT_RUNTIME_OS"));
        }

        puts(base_local.platform);
        //指定localruntime path
        if (optarg != NULL)
        {
            strcpy(base_local.local_path, optarg);
        }
        else
        {
            puts("@2222");
            memset(base_local.local_path, 0, 1024);
            // char *local_dir = NULL;
            // memset(local_dir,0,1024);
            // local_dir = get_local_dir_from_platform(base_local.platform);
            // puts(local_dir);
            //获取平台对应的默认localruntime
            strncpy(base_local.local_path, get_local_dir_from_platform(base_local.platform), 1024);
            puts("@3333");
        }
        //获取baseruntime path
        strcpy(base_local.base_path, get_baseos_dir_from_platform(base_local.platform));

        printf("appname=%s\tplatform=%s\tbasedir=%s\tlocaldir=%s\n", base_local.app_name, base_local.platform, base_local.base_path, base_local.local_path);
    }
    if (argc >= 3 && (strcmp(argv[1], "install") == 0))
    {

        puts("this is install ");

        install_app(base_local);
    }
    else if (argc >= 3 && (strcmp(argv[1], "remove") == 0))
    {
        puts("this is remove");
        remove_app(base_local);
    }
    return 0;
}
