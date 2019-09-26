#include "global_database.h"
#include "global_variaveis.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

bool global_database::database_status;

QSqlDatabase global_database::db = QSqlDatabase::addDatabase("QMYSQL");
QString global_database::host_name = "sql23.main-hosting.eu";
int global_database::port = 3306;
QString global_database::database_name = "u566464314_hrcls";
QString global_database::user_name = "u566464314_eduhs";
QString global_database::password = "lotr4hp1008HE*";

global_database::global_database()
{
    db.setHostName(global_database::host_name);
    db.setPort(global_database::port);
    db.setDatabaseName(global_database::database_name);
    db.setUserName(global_database::user_name);
    db.setPassword(global_database::password);
}

global_database::~global_database()
{
    db.close();
}

void global_database::abrir_conexao()
{
    if(db.open())
    {
        qDebug() << "A conexão com o banco de dados foi aberta com sucesso!";
        global_database::database_status = true;
    }
    else
    {
        qDebug() << db.lastError().text();
        global_database::database_status = false;
    }
}

void global_database::fechar_conexao()
{
    if(db.isOpen())
    {
        db.close();
        qDebug() << "A conexão com o banco de dados foi encerrada!";
        global_database::database_status = false;
    }
}
