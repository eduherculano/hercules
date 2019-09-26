#ifndef GLOBAL_DATABASE_H
#define GLOBAL_DATABASE_H

#include <QString>
#include <QtSql>

class global_database
{
public:
    static bool database_status;

    global_database();
    ~global_database();

    static void abrir_conexao();
    static void fechar_conexao();

private:
    static QSqlDatabase db;
    static QString host_name;
    static int port;
    static QString database_name;
    static QString user_name;
    static QString password;
};

#endif // GLOBAL_DATABASE_H
