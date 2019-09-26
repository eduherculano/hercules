#include "fm_login.h"
#include "ui_fm_login.h"

#include "global_database.h"
#include "global_variaveis.h"
#include "fm_main.h"
#include <QApplication>

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStatusBar>


fm_login::fm_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_login)
{
    ui->setupUi(this);

    this->setFixedSize(350, 475);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);  // não exibe a seta de redimencionamento
    this->setStyleSheet("background-color: white");


    ui->label_footer->setText("CONECTADO AO BANCO DE DADOS HÉRCULES");

    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->addWidget(ui->label_footer);
    statusBar->setSizeGripEnabled(false); // desabilita o sizeGrip

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(statusBar);  // stretch (estender) = 1
    layout->setAlignment(statusBar, Qt::AlignLeft | Qt::AlignBottom);
    layout->setMargin(1);  // no caso, esquerda e inferior
    setLayout(layout);

    global_database *banco_de_dados = new global_database;  // abertura da conexão com o banco de dados
    banco_de_dados->abrir_conexao();


    QPixmap pc_login(":/img/pc_login.jpg");
    int largura = pc_login.width();
    int altura = pc_login.height();
    ui->label_pc_login->setPixmap(pc_login.scaled(largura, altura, Qt::KeepAspectRatio));

    ui->lineEdit_cpf->setFocus();

    // SOMENTE PARA TESTE
    ui->lineEdit_cpf->setText("32727562802");
    ui->lineEdit_senha->setText("789");


}

fm_login::~fm_login()
{
    global_database::fechar_conexao();

    delete ui;
}

void fm_login::on_pushButton_login_clicked()
{
    QString cpf = ui->lineEdit_cpf->text();
    QString senha = ui->lineEdit_senha->text();

    global_database::abrir_conexao();  // abertura da conexão com o banco de dados
    QSqlQuery query;

    query.prepare("SELECT id, nome, cpf, senha, nivel FROM u566464314_hrcls.dp_pessoal WHERE cpf = '"+ cpf +"' AND senha = '"+ senha +"'");

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.first();
            global_variaveis::id_usuario_ativo = query.value(0).toString();
            global_variaveis::nome_usuario_ativo = query.value(1).toString();
            global_variaveis::nivel_usuario_ativo = query.value(4).toInt();

            qDebug() << "NÍVEL DO USUÁRIO: " << global_variaveis::nivel_usuario_ativo;

            fm_main *janela_main = new fm_main();
            janela_main->setWindowState(Qt::WindowMaximized);
            janela_main->show();

            this->close();

        }
        else
        {
            ui->label_footer->setText("Erro ao realizar o login. Usuário ou senha não econtrado.");
        }
    }
    else
    {
        ui->label_footer->setText("Erro ao acessar o banco de dados. Verifique sua conexão.");
    }
}
