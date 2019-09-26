#include "fm_nuc_alvo_pesquisar.h"
#include "ui_fm_nuc_alvo_pesquisar.h"

#include "global_database.h"
#include "global_variaveis.h"
#include "local_variaveis_alvo.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDate>


#include <QPixmap>
#include <QStatusBar>
#include <QVariant>


static QString query_com_operacao_aberta = "";

fm_nuc_alvo_pesquisar::fm_nuc_alvo_pesquisar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_nuc_alvo_pesquisar)
{
    ui->setupUi(this);

    this->setFixedSize(710, 565);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);  // não exibe a seta de redimencionamento

    ui->dateEdit_data_inicial->setDate(QDate::fromString("19010101", "yyyyMMdd"));
    ui->dateEdit_data_final->setDate(QDate::currentDate());

    QStringList mais_ou_menos_valores = {"idade exata", "5 anos", "10 anos", "15 anos"};
    // 0 = idade exata
    // 1 = 5 anos
    // 2 = 10 anos
    // 3 = 15 anos
    ui->comboBox_mais_ou_menos->addItems(mais_ou_menos_valores);


    ui->tableWidget_alvo->setColumnCount(6);
    // id
    // nome
    // nascimento
    // mae
    // pai
    // naturalidade
    ui->tableWidget_alvo->setColumnWidth(0, 50);
    ui->tableWidget_alvo->setColumnWidth(1, 150);
    ui->tableWidget_alvo->setColumnWidth(2, 100);
    ui->tableWidget_alvo->setColumnWidth(3, 150);
    ui->tableWidget_alvo->setColumnWidth(4, 150);
    ui->tableWidget_alvo->setColumnWidth(5, 250);
    QStringList cabecalho_tableWidget_historico = {"ID", "NOME", "NASCIMENTO", "MÃE", "PAI", "NATURALIDADE"};
    ui->tableWidget_alvo->setHorizontalHeaderLabels(cabecalho_tableWidget_historico);
    ui->tableWidget_alvo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_alvo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_alvo->verticalHeader()->setVisible(false);


    global_database::abrir_conexao();

    QSqlQuery query;

    if (global_variaveis::id_operacao_aberta == "")
    {
        query.prepare("SELECT * FROM u566464314_hrcls.nuc_alvo");
    }
    else
    {
        query.prepare("SELECT nuc_alvo.id, nuc_alvo.nome, nuc_alvo.nascimento, nuc_alvo.mae, nuc_alvo.pai, nuc_alvo.naturalidade FROM u566464314_hrcls.nuc_alvo INNER JOIN u566464314_hrcls.nuc_operacao_alvo ON u566464314_hrcls.nuc_alvo.id = u566464314_hrcls.nuc_operacao_alvo.id_alvo INNER JOIN u566464314_hrcls.nuc_operacao ON u566464314_hrcls.nuc_operacao.id = u566464314_hrcls.nuc_operacao_alvo.id_operacao WHERE u566464314_hrcls.nuc_operacao.id = " + global_variaveis::id_operacao_aberta);
    }


    if (query.exec())
    {
        int linha = 0;
        while (query.next())
        {
            ui->tableWidget_alvo->insertRow(linha);
            ui->tableWidget_alvo->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));  // id
            ui->tableWidget_alvo->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));  // nome
            QDate nasc = query.value(2).toDate();
            ui->tableWidget_alvo->setItem(linha, 2, new QTableWidgetItem(nasc.toString("dd-MM-yyyy")));  // nascimento
            ui->tableWidget_alvo->setItem(linha, 3, new QTableWidgetItem(query.value(3).toString()));  // mãe
            ui->tableWidget_alvo->setItem(linha, 4, new QTableWidgetItem(query.value(4).toString()));  // pai
            ui->tableWidget_alvo->setItem(linha, 5, new QTableWidgetItem(query.value(5).toString()));  // naturalidade

            ui->tableWidget_alvo->setRowHeight(linha, 20);
            linha++;
        }
    }
    else
    {
        QMessageBox::warning(this, "Erro", "Erro ao executar a query Select All");
    }

    qDebug() << query.lastError();

    global_database::fechar_conexao();



}

fm_nuc_alvo_pesquisar::~fm_nuc_alvo_pesquisar()
{
    delete ui;
}

void fm_nuc_alvo_pesquisar::on_pushButton_pesquisar_clicked()
{

    QString id_nome = ui->lineEdit_id_nome->text();

    QDate inic = QDate::fromString("19010101", "yyyyMMdd");
    QString data_inicial = ui->dateEdit_data_inicial->date().toString("yyyy-MM-dd");
    QDate fim = QDate::currentDate();
    QString data_final = ui->dateEdit_data_final->date().toString("yyyy-MM-dd");

    QString idade_atual = ui->lineEdit_idade->text();
    int variacao_idade_index = ui->comboBox_mais_ou_menos->currentIndex();

    int mais_ou_menos = 0;
    if (variacao_idade_index == 1)
    {
        mais_ou_menos = 5;
    }
    else if (variacao_idade_index == 2)
    {
        mais_ou_menos = 10;
    }
    else if (variacao_idade_index == 3)
    {
        mais_ou_menos = 15;
    }


     QString criar_query = "";

    if (global_variaveis::id_operacao_aberta == "")
    {
        criar_query = "SELECT * FROM u566464314_hrcls.nuc_alvo";  // 1.0 Padrão de pesquisa
    }
    else
    {
        criar_query = "SELECT nuc_alvo.id, nuc_alvo.nome, nuc_alvo.nascimento, nuc_alvo.mae, nuc_alvo.pai, nuc_alvo.naturalidade FROM u566464314_hrcls.nuc_alvo INNER JOIN u566464314_hrcls.nuc_operacao_alvo ON u566464314_hrcls.nuc_alvo.id = u566464314_hrcls.nuc_operacao_alvo.id_alvo INNER JOIN u566464314_hrcls.nuc_operacao ON u566464314_hrcls.nuc_operacao.id = u566464314_hrcls.nuc_operacao_alvo.id_operacao WHERE u566464314_hrcls.nuc_operacao.id = " + global_variaveis::id_operacao_aberta;  // 1.0 Padrão de pesquisa
    }

    if (id_nome != "" || data_inicial != inic.toString("yyyy-MM-dd") || data_final != fim.toString("yyyy-MM-dd") || idade_atual != "")  // 1.1 Se tiver qualquer alteração (exceção quando somente +ou-), adiciona WHERE
    {
        if (global_variaveis::id_operacao_aberta == "")
        {
            criar_query += " WHERE ";
        }
        else
        {
            criar_query += " AND ";
        }


        if (id_nome != "")  // 1.2 se a alteração for no id_nome
        {
            criar_query += "(nuc_alvo.id = '"+ id_nome +"' OR nuc_alvo.nome LIKE '%"+ id_nome +"%')";  // 1.3 cria a query de pesquisa nas colunas id e nome
        }


        if (data_inicial != inic.toString("yyyy-MM-dd") || data_final != fim.toString("yyyy-MM-dd"))  // 1.4 se a alteração for nos campos data_inicial ou data_final
        {
            criar_query += "nuc_alvo.nascimento BETWEEN '"+ data_inicial +"' AND '"+ data_final +"'";  // 1.6 cria a query de pesquisa entre datas
        }


        if (idade_atual != "") // 1.7 se houver alteração no campo idade
        {
            if (id_nome != "" || data_inicial != inic.toString("yyyy-MM-dd") || data_final != fim.toString("yyyy-MM-dd")) // 1.7 se houver alteração em qualquer outro campo
            {
                criar_query += " AND ";
            }

            QDate idade_base = QDate::currentDate();

            int valor_de_variacao = -1 * (idade_atual.toInt() + 1); // +1 porque existe a idade 0 anos

            idade_base = idade_base.addYears(valor_de_variacao);

            QString nascidos_de = idade_base.addDays(1).addYears(-1 * mais_ou_menos).toString("yyyy-MM-dd");
            QString nascidos_ate = idade_base.addYears(1).addYears(mais_ou_menos).toString("yyyy-MM-dd");

            criar_query += "nuc_alvo.nascimento BETWEEN '"+ nascidos_de +"' AND '"+ nascidos_ate +"'";  // 1.8 cria a query de pesquisa a partir da idade e variação mais ou menos
        }

    }

    qDebug() << criar_query;


    int ultima_linha = ui->tableWidget_alvo->rowCount();
    while (ultima_linha > -1)
    {
        ui->tableWidget_alvo->removeRow(ultima_linha);
        ultima_linha--;
    }

    global_database::abrir_conexao();

    QSqlQuery query;

    query.prepare(criar_query);

    if (query.exec())
    {
        int linha = 0;
        while (query.next())
        {
            ui->tableWidget_alvo->insertRow(linha);
            ui->tableWidget_alvo->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));  // id
            ui->tableWidget_alvo->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));  // nome
            QDate nasc = query.value(2).toDate();
            ui->tableWidget_alvo->setItem(linha, 2, new QTableWidgetItem(nasc.toString("dd-MM-yyyy")));  // nascimento
            ui->tableWidget_alvo->setItem(linha, 3, new QTableWidgetItem(query.value(3).toString()));  // mãe
            ui->tableWidget_alvo->setItem(linha, 4, new QTableWidgetItem(query.value(4).toString()));  // pai
            ui->tableWidget_alvo->setItem(linha, 5, new QTableWidgetItem(query.value(5).toString()));  // naturalidade

            ui->tableWidget_alvo->setRowHeight(linha, 20);
            linha++;
        }
    }
    else
    {
        QMessageBox::warning(this, "Erro", "Erro ao executar a query *");
    }

    global_database::fechar_conexao();


}

void fm_nuc_alvo_pesquisar::on_tableWidget_alvo_cellDoubleClicked(int row, int column)
{
    // duplo click no tableWidget

    QString id_alvo = ui->tableWidget_alvo->item(row, 0)->text();
    QString nome_alvo = ui->tableWidget_alvo->item(row, 1)->text();
    QString nascimento_alvo = ui->tableWidget_alvo->item(row, 2)->text();
    QString mae_alvo = ui->tableWidget_alvo->item(row, 3)->text();
    QString pai_alvo = ui->tableWidget_alvo->item(row, 4)->text();
    QString naturalidade_alvo = ui->tableWidget_alvo->item(row, 5)->text();

    local_variaveis_alvo::id_alvo_temp = id_alvo;
    local_variaveis_alvo::nome_alvo_temp = nome_alvo;
    local_variaveis_alvo::nascimento_alvo_temp = nascimento_alvo;
    local_variaveis_alvo::mae_alvo_temp = mae_alvo;
    local_variaveis_alvo::pai_alvo_temp = pai_alvo;
    local_variaveis_alvo::naturalidade_alvo_temp = naturalidade_alvo;

    qDebug() << "[Linha: " << row << " - Coluna: " << column << "]";
    qDebug() << "id: " << id_alvo;
    qDebug() << "nome: " << nome_alvo;
    qDebug() << "nascimento: " << nascimento_alvo;
    qDebug() << "mae: " << mae_alvo;
    qDebug() << "pai: " << pai_alvo;
    qDebug() << "naturalidade: " << naturalidade_alvo;

    global_database::fechar_conexao();

    this->close();
}
