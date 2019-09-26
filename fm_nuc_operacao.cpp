#include "fm_nuc_operacao.h"
#include "ui_fm_nuc_operacao.h"

#include "global_database.h"
#include "global_variaveis.h"
#include "local_variaveis_operacao.h"
#include "fm_nuc_operacao_pesquisar.h"
#include "fm_main.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QPixmap>
#include <QStatusBar>
#include <QMessageBox>
#include <QDate>
#include <QVariant>

static QString duracao_em_dias; // variável local, global, mas não manipulada em outros locais (por isso não está no .h)


fm_nuc_operacao::fm_nuc_operacao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fm_nuc_operacao)
{
    ui->setupUi(this);

    this->setFixedSize(670, 670);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);  // não exibe a seta de redimencionamento

    this->setWindowTitle("MÓDULO OPERAÇÃO");

    ui->pushButton_criar_operacao->setText("Nova\nOperação");
    ui->pushButton_pesquisar_operacao->setText("Pesquisar\nOperação");
    ui->pushButton_abrir_operacao->setText("Abrir\nOperação");
    ui->pushButton_sair_operacao->setText("Sair da\nOperação");
    ui->pushButton_salvar_alteracoes->setText("Salvar\nAlterações");
    ui->pushButton_cancelar_alteracoes->setText("Cancelar\nAlterações");
    ui->pushButton_sair->setText("Sair");    

    ui->pushButton_abrir_operacao->setEnabled(false);
    ui->pushButton_sair_operacao->setEnabled(false);
    ui->pushButton_salvar_alteracoes->setEnabled(false);
    ui->pushButton_cancelar_alteracoes->setEnabled(false);

    ui->lineEdit_nome->setEnabled(false);
    ui->lineEdit_inicio->setEnabled(false);
    ui->lineEdit_duracao->setEnabled(false);
    ui->lineEdit_fim->setEnabled(false);

    ui->radioButton_operacao_ativa->setEnabled(false);
    ui->radioButton_arquivar_operacao->setEnabled(false);


    if (global_variaveis::id_operacao_aberta != "")
    {
        ui->lineEdit_nome->setEnabled(true);
        ui->pushButton_sair_operacao->setEnabled(true);
    }



    ui->tabWidget_operacao->setCurrentIndex(0);


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



    global_database::abrir_conexao();  // abertura da conexão com o banco de dados
    QSqlQuery query;


    if (global_variaveis::id_operacao_aberta == "")  // --- *** Se a janela for aberta SEM uma operação aberta
    {

        ui->lineEdit_nome->setFocus();
        ui->lineEdit_nome->clear();
        ui->lineEdit_inicio->clear();
        ui->lineEdit_duracao->clear();
        ui->lineEdit_fim->clear();
        ui->radioButton_arquivar_operacao->setChecked(true);


        if (global_variaveis::nivel_usuario_ativo == 1)
        {
            query.prepare("SELECT * FROM u566464314_hrcls.nuc_historico ORDER BY nuc_historico.id DESC");

            if (query.exec())
            {
                int linha = 0;
                while (query.next())
                {
                    ui->tableWidget_historico->insertRow(linha);
                    ui->tableWidget_historico->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));
                    ui->tableWidget_historico->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));
                    ui->tableWidget_historico->setItem(linha, 2, new QTableWidgetItem(query.value(2).toString()));
                    ui->tableWidget_historico->setItem(linha, 3, new QTableWidgetItem(query.value(3).toString()));
                    ui->tableWidget_historico->setItem(linha, 4, new QTableWidgetItem(query.value(4).toString()));
                    ui->tableWidget_historico->setItem(linha, 5, new QTableWidgetItem(query.value(5).toString()));

                    ui->tableWidget_historico->setRowHeight(linha, 20);
                    linha++;
                }
            }
            else
            {
                QMessageBox::warning(this, "Erro", "Erro ao executa a query HISTÓRICO");
                qDebug() << query.lastError();
            }
        }
    }
    else  // --- *** Se a janela for aberta COM uma opração aberta
    {
        ui->pushButton_salvar_alteracoes->setEnabled(true);
        ui->lineEdit_nome->setEnabled(true);


        query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE id = " + global_variaveis::id_operacao_aberta);

        if (query.exec())
        {
            query.first();
            // query.value(0).toString()  // id

            QString nome_da_operacao_aberta = query.value(1).toString();
            ui->lineEdit_nome->setText(nome_da_operacao_aberta);
            this->setWindowTitle("MÓDULO OPERAÇÃO - Operação Aberta: " + nome_da_operacao_aberta);
            ui->tabWidget_operacao->setTabText(0, "Registros - Operação: " + nome_da_operacao_aberta);


            ui->radioButton_operacao_ativa->setEnabled(true);
            ui->radioButton_arquivar_operacao->setEnabled(true);

            if (query.value(2) == true)
            {
                ui->radioButton_operacao_ativa->setChecked(true);
            }
            else
            {
                ui->radioButton_arquivar_operacao->setChecked(true);
            }


            QString data_inicio = query.value(3).toString(); // data_inicio
            ui->lineEdit_inicio->setText(data_inicio);

            QString data_fim = query.value(4).toString();
            ui->lineEdit_fim->setText(data_fim);


            // CALCULA DIFERENÇA DE DIAS ENTRE O INÍCIO DA OPERAÇÃO E O TÉRMINO (OU O DIA DE HOJE)

            QDate inicio = QDate::fromString(data_inicio, "yyyy-MM-dd");

            // Se tiver a data final for vazio, computa até hoje, se tiver alguma data, computa até essa data
            if (query.value(4).toString() == "") // data_fim
            {
                QDate hoje = QDate::currentDate();
                QString duracao_em_dias = (QString::number(inicio.daysTo(hoje)));
                ui->lineEdit_duracao->setText(duracao_em_dias + " dias");
            }
            else
            {
               QDate fim = QDate::fromString(data_fim, "yyyy-MM-dd");
               QString duracao_em_dias = (QString::number(inicio.daysTo(fim)));
               ui->lineEdit_duracao->setText(duracao_em_dias + " dias");
            }

        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executa a query OPERAÇÃO ABERTA para preenchimento dos lineEdit");
        }



        // PREENCHE O TABLEWIDGET COM O HISTÓRICO FILTRADO
        QSqlQuery query;
        query.prepare("SELECT nuc_historico.id, nuc_historico.data_time,  nuc_historico.id_usuario, nuc_historico.acao, nuc_historico.entidade, nuc_historico.registro FROM u566464314_hrcls.nuc_historico INNER JOIN u566464314_hrcls.nuc_operacao_historico ON u566464314_hrcls.nuc_historico.id = u566464314_hrcls.nuc_operacao_historico.id_historico INNER JOIN u566464314_hrcls.nuc_operacao ON u566464314_hrcls.nuc_operacao.id = u566464314_hrcls.nuc_operacao_historico.id_operacao WHERE u566464314_hrcls.nuc_operacao.id = " + global_variaveis::id_operacao_aberta);
        //query.prepare("CALL procedure_historicos_por_operacao("+ global_variaveis::id_operacao_aberta +")");


        if (query.exec())
        {
            int linha = 0;
            while (query.next())
            {
                ui->tableWidget_historico->insertRow(linha);
                ui->tableWidget_historico->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));
                ui->tableWidget_historico->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));
                ui->tableWidget_historico->setItem(linha, 2, new QTableWidgetItem(query.value(2).toString()));
                ui->tableWidget_historico->setItem(linha, 3, new QTableWidgetItem(query.value(3).toString()));
                ui->tableWidget_historico->setItem(linha, 4, new QTableWidgetItem(query.value(4).toString()));
                ui->tableWidget_historico->setItem(linha, 5, new QTableWidgetItem(query.value(5).toString()));

                ui->tableWidget_historico->setRowHeight(linha, 20);
                linha++;
            }
        }



    }

    global_database::fechar_conexao();  // abertura da conexão com o banco de dados

}



fm_nuc_operacao::~fm_nuc_operacao()
{            
    local_variaveis_operacao::id_operacao_temp = "";

    global_database::fechar_conexao();

    delete ui;
}

void fm_nuc_operacao::on_pushButton_sair_clicked()
{
    global_database::fechar_conexao();  // abertura da conexão com o banco de dados

    this->close();
}

void fm_nuc_operacao::on_tabWidget_operacao_tabBarClicked(int index)
{
    global_database::abrir_conexao();  // abertura da conexão com o banco de dados
    switch (index)
    {
        case 0: // histórico
            if (global_variaveis::id_operacao_aberta == "")
            {
                // EXIBE A PESQUISA DE TODOS OS REGISTROS DE TODAS OPERAÇÕES

            }
            else
            {

                int ultima_linha = ui->tableWidget_historico->rowCount();
                while (ultima_linha > -1)
                {
                    ui->tableWidget_historico->removeRow(ultima_linha);
                    ultima_linha--;
                }


                QSqlQuery query;
                query.prepare("SELECT nuc_historico.id, nuc_historico.data_time,  nuc_historico.id_usuario, nuc_historico.acao, nuc_historico.entidade, nuc_historico.registro FROM u566464314_hrcls.nuc_historico INNER JOIN u566464314_hrcls.nuc_operacao_historico ON u566464314_hrcls.nuc_historico.id = u566464314_hrcls.nuc_operacao_historico.id_historico INNER JOIN u566464314_hrcls.nuc_operacao ON u566464314_hrcls.nuc_operacao.id = u566464314_hrcls.nuc_operacao_historico.id_operacao WHERE u566464314_hrcls.nuc_operacao.id = " + global_variaveis::id_operacao_aberta);

                if (query.exec())
                {
                    int linha = 0;
                    while (query.next())
                    {
                        ui->tableWidget_historico->insertRow(linha);
                        ui->tableWidget_historico->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));
                        ui->tableWidget_historico->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));
                        ui->tableWidget_historico->setItem(linha, 2, new QTableWidgetItem(query.value(2).toString()));
                        ui->tableWidget_historico->setItem(linha, 3, new QTableWidgetItem(query.value(3).toString()));
                        ui->tableWidget_historico->setItem(linha, 4, new QTableWidgetItem(query.value(4).toString()));
                        ui->tableWidget_historico->setItem(linha, 5, new QTableWidgetItem(query.value(5).toString()));

                        ui->tableWidget_historico->setRowHeight(linha, 20);
                        linha++;
                    }
                }
                else
                {
                    QMessageBox::warning(this, "Erro", "Erro ao executa a query HISTÓRICO");
                }
            }
        break;


    }

    global_database::fechar_conexao();  // abertura da conexão com o banco de dados
}

void fm_nuc_operacao::on_pushButton_pesquisar_operacao_clicked()
{
    fm_nuc_operacao_pesquisar janela_fm_nuc_operacoes_pesquisar;
    janela_fm_nuc_operacoes_pesquisar.exec();




    global_database::abrir_conexao();  // abertura da conexão com o banco de dados



    if (local_variaveis_operacao::id_operacao_temp != "")
    {

        ui->pushButton_abrir_operacao->setEnabled(true);
        ui->pushButton_salvar_alteracoes->setEnabled(true);
        ui->pushButton_cancelar_alteracoes->setEnabled(true);

        ui->lineEdit_nome->setEnabled(true);


        QSqlQuery query;

        query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE id = " + local_variaveis_operacao::id_operacao_temp);

        if (query.exec())
        {
            query.first();
            // query.value(0).toString()  // id
            ui->lineEdit_nome->setText(query.value(1).toString());
            ui->tabWidget_operacao->setTabText(0, "Registros - Operação: " + query.value(1).toString());

            ui->radioButton_operacao_ativa->setEnabled(true);
            ui->radioButton_arquivar_operacao->setEnabled(true);

            if (query.value(2) == true)
            {
                ui->radioButton_operacao_ativa->setChecked(true);
            }
            else
            {
                ui->radioButton_arquivar_operacao->setChecked(true);
            }

            QString data_inicio = query.value(3).toString(); // data_inicio
            ui->lineEdit_inicio->setText(data_inicio);

            QString data_fim = query.value(4).toString();
            ui->lineEdit_fim->setText(data_fim);


            // CALCULA DIFERENÇA DE DIAS ENTRE O INÍCIO DA OPERAÇÃO E O TÉRMINO (OU O DIA DE HOJE)

            QDate inicio = QDate::fromString(data_inicio, "yyyy-MM-dd");

            // Se tiver a data final for vazio, computa até hoje, se tiver alguma data, computa até essa data
            if (query.value(4).toString() == "") // data_fim
            {
                QDate hoje = QDate::currentDate();
                duracao_em_dias = (QString::number(inicio.daysTo(hoje)));
                ui->lineEdit_duracao->setText(duracao_em_dias + " dias");
            }
            else
            {
               QDate fim = QDate::fromString(data_fim, "yyyy-MM-dd");
               duracao_em_dias = (QString::number(inicio.daysTo(fim)));
               ui->lineEdit_duracao->setText(duracao_em_dias + " dias");
            }

        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executa a query OPERAÇÃO ABERTA para preenchimento dos lineEdit");
        }



        int ultima_linha = ui->tableWidget_historico->rowCount();
        while (ultima_linha > -1)
        {
            ui->tableWidget_historico->removeRow(ultima_linha);
            ultima_linha--;
        }


        // PREENCHE O TABLEWIDGET COM O HISTÓRICO FILTRADO

        query.prepare("SELECT nuc_historico.id, nuc_historico.data_time,  nuc_historico.id_usuario, nuc_historico.acao, nuc_historico.entidade, nuc_historico.registro FROM u566464314_hrcls.nuc_historico INNER JOIN u566464314_hrcls.nuc_operacao_historico ON u566464314_hrcls.nuc_historico.id = u566464314_hrcls.nuc_operacao_historico.id_historico INNER JOIN u566464314_hrcls.nuc_operacao ON u566464314_hrcls.nuc_operacao.id = u566464314_hrcls.nuc_operacao_historico.id_operacao WHERE u566464314_hrcls.nuc_operacao.id = " + local_variaveis_operacao::id_operacao_temp);

        if (query.exec())
        {
            int linha = 0;
            while (query.next())
            {
                ui->tableWidget_historico->insertRow(linha);
                ui->tableWidget_historico->setItem(linha, 0, new QTableWidgetItem(query.value(0).toString()));
                ui->tableWidget_historico->setItem(linha, 1, new QTableWidgetItem(query.value(1).toString()));
                ui->tableWidget_historico->setItem(linha, 2, new QTableWidgetItem(query.value(2).toString()));
                ui->tableWidget_historico->setItem(linha, 3, new QTableWidgetItem(query.value(3).toString()));
                ui->tableWidget_historico->setItem(linha, 4, new QTableWidgetItem(query.value(4).toString()));
                ui->tableWidget_historico->setItem(linha, 5, new QTableWidgetItem(query.value(5).toString()));

                ui->tableWidget_historico->setRowHeight(linha, 20);
                linha++;
            }
        }



    }

    global_database::fechar_conexao();  // abertura da conexão com o banco de dados


}

void fm_nuc_operacao::on_pushButton_abrir_operacao_clicked()
{
    global_variaveis::id_operacao_aberta = local_variaveis_operacao::id_operacao_temp;
    local_variaveis_operacao::id_operacao_temp = "";

    global_variaveis::id_alvo_selecionado = "";



    this->close();
}

void fm_nuc_operacao::on_pushButton_sair_operacao_clicked()
{
    global_variaveis::id_operacao_aberta = "";
    global_variaveis::id_alvo_selecionado = "";

    local_variaveis_operacao::id_operacao_temp = "";

    this->close();
}

void fm_nuc_operacao::on_pushButton_criar_operacao_clicked()
{
    local_variaveis_operacao::id_operacao_temp = "";
    local_variaveis_operacao::nome_operacao_temp = "";
    local_variaveis_operacao::ativa_operacao_temp = "";
    local_variaveis_operacao::data_inicio_operacao_temp = "";
    local_variaveis_operacao::data_fim_operacao_temp = "";

    ui->pushButton_pesquisar_operacao->setEnabled(false);
    ui->pushButton_criar_operacao->setEnabled(false);
    ui->pushButton_abrir_operacao->setEnabled(false);
    ui->pushButton_sair_operacao->setEnabled(false);
    ui->pushButton_salvar_alteracoes->setEnabled(true);
    ui->pushButton_cancelar_alteracoes->setEnabled(true);

    ui->radioButton_operacao_ativa->setEnabled(true);
    ui->radioButton_arquivar_operacao->setEnabled(false);

    ui->lineEdit_nome->setEnabled(true);
    ui->lineEdit_nome->setFocus();
    ui->lineEdit_nome->clear();
    ui->lineEdit_inicio->clear();
    ui->lineEdit_duracao->clear();
    ui->lineEdit_fim->clear();

    QDate hoje;
    ui->lineEdit_inicio->setText(hoje.currentDate().toString("dd-MM-yyyy"));
    ui->radioButton_operacao_ativa->setChecked(true);

    int ultima_linha = ui->tableWidget_historico->rowCount();
    while (ultima_linha > -1)
    {
        ui->tableWidget_historico->removeRow(ultima_linha);
        ultima_linha--;
    }

    ui->tabWidget_operacao->setTabText(0, "Registros");


}

void fm_nuc_operacao::on_pushButton_cancelar_alteracoes_clicked()
{

    if (local_variaveis_operacao::id_operacao_temp == "")
    {
        ui->pushButton_pesquisar_operacao->setEnabled(true);
        ui->pushButton_criar_operacao->setEnabled(true);
        ui->pushButton_abrir_operacao->setEnabled(false);
        ui->pushButton_sair_operacao->setEnabled(false);
        ui->pushButton_salvar_alteracoes->setEnabled(false);
        ui->pushButton_cancelar_alteracoes->setEnabled(false);

        ui->radioButton_operacao_ativa->setEnabled(false);
        ui->radioButton_arquivar_operacao->setEnabled(false);
        ui->radioButton_arquivar_operacao->setChecked(true);

        local_variaveis_operacao::nome_operacao_temp = "";
        local_variaveis_operacao::ativa_operacao_temp = "";
        local_variaveis_operacao::data_inicio_operacao_temp = "";
        local_variaveis_operacao::data_fim_operacao_temp = "";


        ui->lineEdit_nome->setEnabled(false);
        ui->lineEdit_nome->clear();
        ui->lineEdit_inicio->clear();
        ui->lineEdit_duracao->clear();
        ui->lineEdit_fim->clear();
    }
    else
    {
        ui->lineEdit_nome->setText(local_variaveis_operacao::nome_operacao_temp);
        ui->lineEdit_inicio->setText(local_variaveis_operacao::data_inicio_operacao_temp);
        ui->lineEdit_duracao->setText(duracao_em_dias + " dias");
        ui->lineEdit_fim->setText(local_variaveis_operacao::data_fim_operacao_temp);

        bool operacao_ativa = QVariant(local_variaveis_operacao::ativa_operacao_temp).toBool();
        if (operacao_ativa)
        {
            ui->radioButton_operacao_ativa->setChecked(true);
        }
        else
        {
            ui->radioButton_arquivar_operacao->setChecked(true);
        }
    }


}

void fm_nuc_operacao::on_pushButton_salvar_alteracoes_clicked()
{
    if (local_variaveis_operacao::id_operacao_temp == "")  //
    {
        QString nome_operacao_criar = ui->lineEdit_nome->text();
        QString operacao_ativa_criar = "true";

        global_database::abrir_conexao();
        QSqlQuery query;

        if (nome_operacao_criar == "")
        {
            QMessageBox::warning(this, "Erro", "Digite o nome da nova operação!");
        }
        else
        {
            query.prepare("INSERT INTO u566464314_hrcls.nuc_operacao (nome, ativa) VALUES ('"+ nome_operacao_criar +"', "+ operacao_ativa_criar +")");

            if (query.exec())
            {
                QMessageBox::information(this, "Operação " + nome_operacao_criar, "A Operação \"" + nome_operacao_criar + "\" foi criada com sucesso!");
                this->close();
            }
            else
            {
                QMessageBox::warning(this, "Erro", "Erro ao executar a query de criação de nova operação!");
            }

        }
        query.prepare("SELECT * FROM u566464314_hrcls.nuc_operacao WHERE nome = '"+ nome_operacao_criar +"'");
        if (query.exec())
        {
            query.last();

            global_variaveis::id_operacao_aberta = query.value(0).toString();
        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executar a query de abertura da operação criada!");
        }


        global_database::fechar_conexao();


    }
    else
    {
        // (local_variaveis_operacao::id_operacao_temp != "")
        // ATUALIZAR O BANCO DE DADOS

        QString nome_operacao_atualizar = ui->lineEdit_nome->text();
        QString operacao_ativa_atualizar;

        global_database::abrir_conexao();

        QSqlQuery query;

        if (ui->radioButton_operacao_ativa->isChecked())
        {
            operacao_ativa_atualizar = "true";
            //qDebug() << "UPDATE u566464314_hrcls.nuc_operacao SET nome = '"+ nome_operacao_atualizar +"', ativa = "+ operacao_ativa_atualizar +" WHERE id = "+ local_variaveis_operacao::id_operacao_temp;
            query.prepare("UPDATE u566464314_hrcls.nuc_operacao SET nome = '"+ nome_operacao_atualizar +"', ativa = "+ operacao_ativa_atualizar +" WHERE id = "+ local_variaveis_operacao::id_operacao_temp);
        }
        else
        {
            operacao_ativa_atualizar = "false";
            QString data_fim_atualizar = QDate::currentDate().toString("yyyy-MM-dd");
            //qDebug() << "UPDATE u566464314_hrcls.nuc_operacao SET nome = '"+ nome_operacao_atualizar +"', ativa = "+ operacao_ativa_atualizar +", data_fim = "+ data_fim_atualizar +" WHERE id = "+ local_variaveis_operacao::id_operacao_temp;
            query.prepare("UPDATE u566464314_hrcls.nuc_operacao SET nome = '"+ nome_operacao_atualizar +"', ativa = "+ operacao_ativa_atualizar +", data_fim = '"+ data_fim_atualizar +"' WHERE id = "+ local_variaveis_operacao::id_operacao_temp);
        }

        if (query.exec())
        {
            QMessageBox::information(this, "Ok", "A operação foi atualizada com sucesso!");
        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executar a query de atualização");
        }

        global_database::fechar_conexao();
    }


}
