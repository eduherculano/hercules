#include "fm_nuc_alvo.h"
#include "ui_fm_nuc_alvo.h"

#include "global_database.h"
#include "global_variaveis.h"
#include "local_variaveis_alvo.h"
#include "fm_nuc_alvo_pesquisar.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QPixmap>
#include <QStatusBar>
#include <QMessageBox>
#include <QDate>
#include <QVariant>

fm_nuc_alvo::fm_nuc_alvo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_nuc_alvo)
{
    ui->setupUi(this);

    this->setFixedSize(765, 680);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);  // não exibe a seta de redimencionamento

    this->setWindowTitle("MÓDULO ALVO");

    ui->pushButton_pesquisar_alvo->setText("Pesquisar\nAlvo");
    ui->pushButton_criar_alvo->setText("Criar Alvo");
    ui->pushButton_abrir_alvo->setText("Abrir Alvo");
    ui->pushButton_gerar_relatorio->setText("Gerar\nRelatório");
    ui->pushButton_salvar_alteracoes->setText("Salvar\nAlterações");
    ui->pushButton_cancelar_alteracoes->setText("Cancelar\nAlterações");
    ui->pushButton_sair->setText("Sair");

    ui->dateEdit_data_nascimento->setDate(QDate::currentDate());

    ui->pushButton_pesquisar_alvo->setEnabled(true);
    ui->pushButton_criar_alvo->setEnabled(true);
    ui->pushButton_abrir_alvo->setEnabled(false);
    ui->pushButton_gerar_relatorio->setEnabled(false);
    ui->pushButton_salvar_alteracoes->setEnabled(false);
    ui->pushButton_cancelar_alteracoes->setEnabled(false);
    ui->pushButton_sair->setEnabled(true);

    ui->lineEdit_nome->setEnabled(false);
    ui->lineEdit_mae->setEnabled(false);
    ui->dateEdit_data_nascimento->setEnabled(false);
    ui->lineEdit_pai->setEnabled(false);
    ui->lineEdit_naturalidade->setEnabled(false);
    ui->lineEdit_idade_atual->setEnabled(false);

    ui->tabWidget_alvo->setEnabled(true);

    ui->tableWidget_historico->setColumnCount(6);
    // id
    // data_time
    // id_usuario
    // acao [create, read, update, delete]
    // entidade
    // histórico
    ui->tableWidget_historico->setColumnWidth(0, 50);
    ui->tableWidget_historico->setColumnWidth(1, 150);
    ui->tableWidget_historico->setColumnWidth(2, 100);
    ui->tableWidget_historico->setColumnWidth(3, 150);
    ui->tableWidget_historico->setColumnWidth(4, 150);
    ui->tableWidget_historico->setColumnWidth(5, 250);
    QStringList cabecalho_tableWidget_historico = {"ID", "DATA/HORÁRIO", "USUÁRIO", "AÇÃO", "ENTIDADE", "REGISTRO"};
    ui->tableWidget_historico->setHorizontalHeaderLabels(cabecalho_tableWidget_historico);
    //ui->tableWidget_historico->setStyleSheet("QTableView {selection-background-color:blue}");
    ui->tableWidget_historico->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_historico->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_historico->verticalHeader()->setVisible(false);


    if (global_variaveis::id_alvo_selecionado != "")
    {
        local_variaveis_alvo::id_alvo_temp = global_variaveis::id_alvo_selecionado;

        ui->pushButton_pesquisar_alvo->setEnabled(true);
        ui->pushButton_criar_alvo->setEnabled(true);
        ui->pushButton_abrir_alvo->setEnabled(false);
        ui->pushButton_gerar_relatorio->setEnabled(true);
        ui->pushButton_salvar_alteracoes->setEnabled(true);
        ui->pushButton_cancelar_alteracoes->setEnabled(true);
        ui->pushButton_sair->setEnabled(true);

        ui->lineEdit_nome->setEnabled(true);
        ui->lineEdit_mae->setEnabled(true);
        ui->dateEdit_data_nascimento->setEnabled(true);
        ui->lineEdit_pai->setEnabled(true);
        ui->lineEdit_naturalidade->setEnabled(true);
        ui->lineEdit_idade_atual->setEnabled(false);

        global_database::abrir_conexao();

        QSqlQuery query;

        query.prepare("SELECT * FROM u566464314_hrcls.nuc_alvo WHERE id = " + global_variaveis::id_alvo_selecionado);

        if (query.exec())
        {
            query.first();

            // query.value(0).toString() = id
            ui->lineEdit_nome->setText(query.value(1).toString());
            ui->dateEdit_data_nascimento->setDate(query.value(2).toDate());
            ui->lineEdit_mae->setText(query.value(3).toString());
            ui->lineEdit_pai->setText(query.value(4).toString());
            ui->lineEdit_naturalidade->setText(query.value(5).toString());
        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executa a query ALVO SELECIONADO para preenchimento dos lineEdit");
            qDebug() << query.lastError();
            qDebug() << "ID ALVO SELECIONADO (global): " + global_variaveis::id_alvo_selecionado;
        }

        global_database::fechar_conexao();
    }










}

fm_nuc_alvo::~fm_nuc_alvo()
{
    local_variaveis_alvo::id_alvo_temp = "";

    global_database::fechar_conexao();

    delete ui;
}

void fm_nuc_alvo::on_pushButton_pesquisar_alvo_clicked()
{
    fm_nuc_alvo_pesquisar janela_fm_nuc_alvo_pesquisar;
    janela_fm_nuc_alvo_pesquisar.exec();

    if (local_variaveis_alvo::id_alvo_temp != "")
    {

        ui->pushButton_pesquisar_alvo->setEnabled(true);
        ui->pushButton_criar_alvo->setEnabled(true);
        ui->pushButton_abrir_alvo->setEnabled(true);
        ui->pushButton_gerar_relatorio->setEnabled(true);
        ui->pushButton_salvar_alteracoes->setEnabled(true);
        ui->pushButton_cancelar_alteracoes->setEnabled(true);
        ui->pushButton_sair->setEnabled(true);

        ui->lineEdit_nome->setEnabled(true);
        ui->lineEdit_mae->setEnabled(true);
        ui->dateEdit_data_nascimento->setEnabled(true);
        ui->lineEdit_pai->setEnabled(true);
        ui->lineEdit_naturalidade->setEnabled(true);
        ui->lineEdit_idade_atual->setEnabled(false);


        global_database::abrir_conexao();

        QSqlQuery query;

        query.prepare("SELECT * FROM u566464314_hrcls.nuc_alvo WHERE id = " + local_variaveis_alvo::id_alvo_temp);

        if (query.exec())
        {
            query.first();

            // query.value(0).toString() = id
            ui->lineEdit_nome->setText(query.value(1).toString());
            ui->dateEdit_data_nascimento->setDate(query.value(2).toDate());
            ui->lineEdit_mae->setText(query.value(3).toString());
            ui->lineEdit_pai->setText(query.value(4).toString());
            ui->lineEdit_naturalidade->setText(query.value(5).toString());

        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executa a query ALVO SELECIONADO para preenchimento dos lineEdit");
            qDebug() << query.lastError();
        }

    }

    global_database::fechar_conexao();


}

void fm_nuc_alvo::on_pushButton_abrir_alvo_clicked()
{
    global_variaveis::id_alvo_selecionado = local_variaveis_alvo::id_alvo_temp;
    local_variaveis_alvo::id_alvo_temp = "";

    qDebug() << "ALVO ABERTO: " + global_variaveis::id_alvo_selecionado;

    this->close();
}

void fm_nuc_alvo::on_pushButton_criar_alvo_clicked()
{

    ui->pushButton_pesquisar_alvo->setEnabled(false);
    ui->pushButton_criar_alvo->setEnabled(false);
    ui->pushButton_abrir_alvo->setEnabled(false);
    ui->pushButton_gerar_relatorio->setEnabled(false);
    ui->pushButton_salvar_alteracoes->setEnabled(true);
    ui->pushButton_cancelar_alteracoes->setEnabled(true);
    ui->pushButton_sair->setEnabled(true);

    ui->lineEdit_nome->setEnabled(true);
    ui->lineEdit_mae->setEnabled(true);
    ui->dateEdit_data_nascimento->setEnabled(true);
    ui->lineEdit_pai->setEnabled(true);
    ui->lineEdit_naturalidade->setEnabled(true);
    ui->lineEdit_idade_atual->setEnabled(false);

    ui->lineEdit_nome->setText("");
    ui->lineEdit_mae->setText("");
    ui->dateEdit_data_nascimento->setDate(QDate::currentDate());
    ui->lineEdit_pai->setText("");
    ui->lineEdit_naturalidade->setText("");
    ui->lineEdit_idade_atual->setText("");

    ui->tabWidget_alvo->setEnabled(false);

    int ultima_linha = ui->tableWidget_historico->rowCount();
    while (ultima_linha > -1)
    {
        ui->tableWidget_historico->removeRow(ultima_linha);
        ultima_linha--;
    }

}

void fm_nuc_alvo::on_pushButton_cancelar_alteracoes_clicked()
{

    ui->pushButton_pesquisar_alvo->setEnabled(true);
    ui->pushButton_criar_alvo->setEnabled(true);
    //
    //
    ui->pushButton_salvar_alteracoes->setEnabled(false);
    ui->pushButton_cancelar_alteracoes->setEnabled(false);
    ui->pushButton_sair->setEnabled(true);

    ui->tabWidget_alvo->setEnabled(true);

    if (local_variaveis_alvo::id_alvo_temp == "")
    {
        ui->pushButton_abrir_alvo->setEnabled(false);
        ui->pushButton_gerar_relatorio->setEnabled(false);

        ui->lineEdit_nome->setEnabled(false);
        ui->lineEdit_mae->setEnabled(false);
        ui->dateEdit_data_nascimento->setEnabled(false);
        ui->lineEdit_pai->setEnabled(false);
        ui->lineEdit_naturalidade->setEnabled(false);
        //

        ui->lineEdit_nome->setText("");
        ui->lineEdit_mae->setText("");
        ui->dateEdit_data_nascimento->setDate(QDate::currentDate());
        ui->lineEdit_pai->setText("");
        ui->lineEdit_naturalidade->setText("");
        ui->lineEdit_idade_atual->setText("");
    }
    else
    {
        ui->pushButton_abrir_alvo->setEnabled(true);
        ui->pushButton_gerar_relatorio->setEnabled(true);

        ui->lineEdit_nome->setEnabled(true);
        ui->lineEdit_mae->setEnabled(true);
        ui->dateEdit_data_nascimento->setEnabled(true);
        ui->lineEdit_pai->setEnabled(true);
        ui->lineEdit_naturalidade->setEnabled(true);
        //

        global_database::abrir_conexao();

        QSqlQuery query;

        query.prepare("SELECT * FROM u566464314_hrcls.nuc_alvo WHERE id = " + local_variaveis_alvo::id_alvo_temp);

        if (query.exec())
        {
            query.first();

            // query.value(0).toString() = id
            ui->lineEdit_nome->setText(query.value(1).toString());
            ui->dateEdit_data_nascimento->setDate(query.value(2).toDate());
            ui->lineEdit_mae->setText(query.value(3).toString());
            ui->lineEdit_pai->setText(query.value(4).toString());
            ui->lineEdit_naturalidade->setText(query.value(5).toString());

        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executa a query ALVO SELECIONADO para preenchimento dos lineEdit");
            qDebug() << query.lastError();
        }

        global_database::fechar_conexao();
    }

}

void fm_nuc_alvo::on_pushButton_sair_clicked()
{
    this->close();
}

void fm_nuc_alvo::on_pushButton_salvar_alteracoes_clicked()
{
    if (local_variaveis_alvo::id_alvo_temp == "")
    {
        QString nome_alvo_criar = ui->lineEdit_nome->text();
        QDate nascimento_alvo_criar = ui->dateEdit_data_nascimento->date();
        QString mae_alvo_criar = ui->lineEdit_mae->text();
        QString pai_alvo_criar = ui->lineEdit_pai->text();
        QString naturalidade_alvo_criar = ui->lineEdit_naturalidade->text();

        global_database::abrir_conexao();
        QSqlQuery query;

        if (nome_alvo_criar == "")
        {
            query.prepare("SELECT id FROM u566464314_hrcls.nuc_alvo");

            if (query.exec())
            {
                query.last();
                int qts_alvos = query.value(0).toInt();
                qts_alvos++;
                nome_alvo_criar = "NÃO IDENTIFICADO " + QString::number(qts_alvos);

                QString inserir = "INSERT INTO u566464314_hrcls.nuc_alvo (nome, nascimento, mae, pai, naturalidade) VALUES ('"+ nome_alvo_criar +"', '"+ nascimento_alvo_criar.toString("yyyy-MM-dd") +"', '"+ mae_alvo_criar +"', '"+ pai_alvo_criar +"', '"+ naturalidade_alvo_criar +"')";

                query.prepare(inserir);
                query.exec();
                qDebug() << inserir;

                QMessageBox::information(this, "Alvo " + nome_alvo_criar, "O alvo " + nome_alvo_criar + " foi criado com sucesso.");

                ui->lineEdit_nome->setText(nome_alvo_criar);
            }
            else
            {
                QMessageBox::warning(this, "Erro", "Erro ao inserir alvo não-identificado");
                qDebug() << query.lastError();
            }
        }
        else  // SE O NOME NÃO EXTIVER VAZIO
        {
            query.exec("INSERT INTO u566464314_hrcls.nuc_alvo (nome, nascimento, mae, pai, naturalidade) VALUES ('"+ nome_alvo_criar +"', '"+ nascimento_alvo_criar.toString("yyyy-MM-dd") +"', '"+ mae_alvo_criar +"', '"+ pai_alvo_criar +"', '"+ naturalidade_alvo_criar +"')");

            QMessageBox::information(this, "Alvo " + nome_alvo_criar, "O alvo " + nome_alvo_criar + " foi criado com sucesso.");
        }

        ui->tabWidget_alvo->setEnabled(true);

        global_database::fechar_conexao();
    }

    else // se tiver algum id carregado local_variaveis_alvo::id_alvo_temp != ""
    {
        QString nome_alvo_atualizar = ui->lineEdit_nome->text();
        QDate nascimento_alvo_atualizar = ui->dateEdit_data_nascimento->date();
        QString mae_alvo_atualizar = ui->lineEdit_mae->text();
        QString pai_alvo_atualizar = ui->lineEdit_pai->text();
        QString naturalidade_alvo_atualizar = ui->lineEdit_naturalidade->text();

        global_database::abrir_conexao();
        QSqlQuery query;

        query.prepare("UPDATE u566464314_hrcls.nuc_alvo SET nome = '"+ nome_alvo_atualizar +"', nascimento = '"+ nascimento_alvo_atualizar.toString("yyyy-MM-dd") +"', mae = '"+ mae_alvo_atualizar +"', pai = '"+ pai_alvo_atualizar +"', naturalidade = '"+ naturalidade_alvo_atualizar +"' WHERE ID = " + local_variaveis_alvo::id_alvo_temp);

        if (query.exec())
        {
            QMessageBox::information(this, "UPDATE", "O alvo "+ nome_alvo_atualizar +" foi atualizado com sucesso!");
        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao editar alvo");
            qDebug() << query.lastError();
        }

        global_database::fechar_conexao();
    }
}











