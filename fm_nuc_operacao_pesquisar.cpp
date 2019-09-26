#include "fm_nuc_operacao_pesquisar.h"
#include "ui_fm_nuc_operacao_pesquisar.h"

#include "global_database.h"
#include "global_variaveis.h"
#include "local_variaveis_operacao.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

fm_nuc_operacao_pesquisar::fm_nuc_operacao_pesquisar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_nuc_operacao_pesquisar)
{
    ui->setupUi(this);

    this->setWindowTitle("Módulo de Operações: Operações Ativas");

    ui->radioButton_nao->setChecked(true);

    ui->dateEdit_data_inicial->setDate(QDate::currentDate());
    ui->dateEdit_data_final->setDate(QDate::currentDate());

    ui->tableWidget_operacoes->setColumnCount(5);
    // id
    // nome
    // ativa
    // data_inicio
    // data_fim
    ui->tableWidget_operacoes->setColumnWidth(0, 50);
    ui->tableWidget_operacoes->setColumnWidth(1, 150);
    ui->tableWidget_operacoes->setColumnWidth(2, 100);
    ui->tableWidget_operacoes->setColumnWidth(3, 100);
    ui->tableWidget_operacoes->setColumnWidth(4, 100);
    QStringList cabecalho_tableWidget_operacao = {"ID", "NOME", "ATIVA", "DATA DE INÍCIO", "ARQUIVAMENTO"};
    ui->tableWidget_operacoes->setHorizontalHeaderLabels(cabecalho_tableWidget_operacao);
    ui->tableWidget_operacoes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_operacoes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_operacoes->verticalHeader()->setVisible(false);


    global_database::abrir_conexao();  // abertura da conexão com o banco de dados
    QSqlQuery query;

    query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE ativa = 1 ORDER BY data_inicio DESC");

    if (query.exec())
    {
        int linha = 0;
        while (query.next())
        {
            ui->tableWidget_operacoes->insertRow(linha);
            ui->tableWidget_operacoes->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));  // id
            ui->tableWidget_operacoes->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));  // nome
            ui->tableWidget_operacoes->setItem(linha, 2, new QTableWidgetItem(query.value(2).toString()));  // ativa
            QDate d_inic = query.value(3).toDate();
            ui->tableWidget_operacoes->setItem(linha, 3, new QTableWidgetItem(d_inic.toString("dd-MM-yyyy")));  // data_inicio
            QDate d_fim = query.value(4).toDate();
            ui->tableWidget_operacoes->setItem(linha, 4, new QTableWidgetItem(d_fim.toString("dd-MM-yyyy")));  // data_fim

            ui->tableWidget_operacoes->setRowHeight(linha, 20);
            linha++;
        }
    }
    else
    {
        QMessageBox::warning(this, "Erro", "Erro ao executar a query *");
    }


}

fm_nuc_operacao_pesquisar::~fm_nuc_operacao_pesquisar()
{
    delete ui;
}

void fm_nuc_operacao_pesquisar::on_pushButton_pesquisar_clicked()
{
    QString valor_pesquisar = ui->lineEdit_pesquisar->text();

    bool por_periodo = ui->radioButton_sim->isChecked();

    //QString data_inicio = ui->dateEdit_data_inicial->text();
    //QString data_fim = ui->dateEdit_data_final->text();

    QDate inicial = ui->dateEdit_data_inicial->date();
    QString data_inicial = inicial.toString("yyyy-MM-dd");

    QDate final = ui->dateEdit_data_final->date();
    QString data_final = final.toString("yyyy-MM-dd");


    qDebug() << "DATA INICIAL: " + data_inicial;
    qDebug() << "DATA FINAL: " + data_final;
    qDebug() << "PESQUISAR POR: " + valor_pesquisar;


    int ultima_linha = ui->tableWidget_operacoes->rowCount();
    while (ultima_linha > -1)
    {
        ui->tableWidget_operacoes->removeRow(ultima_linha);
        ultima_linha--;
    }


    global_database::abrir_conexao();  // abertura da conexão com o banco de dados
    QSqlQuery query;

    if (valor_pesquisar == "" && !por_periodo)
    {
        query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE ativa = 1 ORDER BY data_inicio DESC");
    }
    else if (valor_pesquisar != "" && !por_periodo)
    {
        query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE id LIKE '%"+ valor_pesquisar +"%' OR nome LIKE '%"+ valor_pesquisar +"%' AND ativa = 1 ORDER BY data_inicio DESC");
    }
    else if (valor_pesquisar == "" && por_periodo)
    {
        query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE data_inicio BETWEEN '"+ data_inicial +"' AND '"+ data_final +"'  AND ativa = 1 ORDER BY data_inicio DESC");
    }
    else if (valor_pesquisar != "" && por_periodo)
    {
        query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE id LIKE '%"+ valor_pesquisar +"%' OR nome LIKE '%"+ valor_pesquisar +"%' AND data_inicio BETWEEN '"+ data_inicial +"' AND '"+ data_final +"'  AND ativa = 1 ORDER BY data_inicio DESC");
    }



    if (query.exec())
    {
        int linha = 0;
        while (query.next())
        {
            ui->tableWidget_operacoes->insertRow(linha);
            ui->tableWidget_operacoes->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));  // id
            ui->tableWidget_operacoes->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));  // nome
            ui->tableWidget_operacoes->setItem(linha, 2, new QTableWidgetItem(query.value(2).toString()));  // ativa
            QDate d_inic = query.value(3).toDate();
            ui->tableWidget_operacoes->setItem(linha, 3, new QTableWidgetItem(d_inic.toString("dd-MM-yyyy")));  // data_inicio
            QDate d_fim = query.value(4).toDate();
            ui->tableWidget_operacoes->setItem(linha, 4, new QTableWidgetItem(d_fim.toString("dd-MM-yyyy")));  // data_fim

            ui->tableWidget_operacoes->setRowHeight(linha, 20);
            linha++;
        }
    }
    else
    {
        QMessageBox::warning(this, "Erro", "Erro ao executar a query *");
    }
}



void fm_nuc_operacao_pesquisar::on_tableWidget_operacoes_cellDoubleClicked(int row, int column)  // (int row, int column)
{
    QString id_operacao = ui->tableWidget_operacoes->item(row, 0)->text();
    QString nome_operacao = ui->tableWidget_operacoes->item(row, 1)->text();
    QString ativa_operacao = ui->tableWidget_operacoes->item(row, 2)->text();
    QString data_inicio_operacao = ui->tableWidget_operacoes->item(row, 3)->text();
    QString data_fim_operacao = ui->tableWidget_operacoes->item(row, 4)->text();

    local_variaveis_operacao::id_operacao_temp = id_operacao;
    local_variaveis_operacao::nome_operacao_temp = nome_operacao;
    local_variaveis_operacao::ativa_operacao_temp = ativa_operacao;
    local_variaveis_operacao::data_inicio_operacao_temp = data_inicio_operacao;
    local_variaveis_operacao::data_fim_operacao_temp = data_fim_operacao;

    global_database::fechar_conexao();

    this->close();
}
