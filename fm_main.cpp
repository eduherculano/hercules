#include "fm_main.h"
#include "ui_fm_main.h"

#include "global_database.h"
#include "global_variaveis.h"
#include "fm_nuc_operacao.h"
#include "fm_nuc_alvo.h"

#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>

fm_main::fm_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fm_main)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: white");

    ui->mainToolBar->setMinimumSize(0, 85);  // largura, altura
    ui->mainToolBar->addWidget(ui->tabWidget_menu);

    ui->tabWidget_menu->setCurrentIndex(0); // núcleo

    ui->tabWidget_menu->setTabText(0, "NÚCLEO DE INTELIGÊNCIA");
    ui->tabWidget_menu->setTabText(1, "DEPARTAMENTO PESSOAL");
    ui->tabWidget_menu->setTabText(2, "CONTROLE DE MATERIAL");
    ui->tabWidget_menu->setTabText(3, "...");

    ui->tabWidget_menu->setTabEnabled(0, true);
    ui->tabWidget_menu->setTabEnabled(1, false);
    ui->tabWidget_menu->setTabEnabled(2, false);
    ui->tabWidget_menu->setTabEnabled(3, false);

    if (global_variaveis::nivel_usuario_ativo == 1)
    {
        ui->tabWidget_menu->setTabText(3, "SISTEMA");
        ui->tabWidget_menu->setTabEnabled(3, true);
    }



    ui->pushButton_nuc_operacao->setEnabled(true);
    ui->pushButton_nuc_agente->setEnabled(false);
    //ui->pushButton_nuc_alvo->setEnabled(false);
    //ui->pushButton_nuc_vitima->setEnabled(false);
    ui->pushButton_nuc_degravacao->setEnabled(false);
    ui->pushButton_nuc_inqt_proc->setEnabled(false);


    ui->label_usuario_logado->setText("Usuario logado: " + global_variaveis::nome_usuario_ativo);
    ui->label_operacao_aberta->setText("Operação aberta: ");
    ui->label_footer->setText("CONECTADO AO BANCO DE DADOS HÉRCULES");

    ui->statusBar->setSizeGripEnabled(false); // desabilita o sizeGrip
    ui->statusBar->addWidget(ui->label_footer, 0);

}

fm_main::~fm_main()
{

    global_database::fechar_conexao();

    delete ui;

}

void fm_main::on_pushButton_nuc_operacao_clicked()
{
    fm_nuc_operacao janela_nuc_operacao;
    janela_nuc_operacao.exec();


    if (global_variaveis::id_operacao_aberta == "")
    {
        ui->label_operacao_aberta->setText("Operação aberta: ");

        ui->pushButton_nuc_agente->setEnabled(false);
        ui->pushButton_nuc_alvo->setEnabled(true);
        ui->pushButton_nuc_vitima->setEnabled(true);
        ui->pushButton_nuc_degravacao->setEnabled(false);
        ui->pushButton_nuc_inqt_proc->setEnabled(false);
    }
    else
    {
        global_database::abrir_conexao();

        QSqlQuery query;
        query.prepare("SELECT nome FROM u566464314_hrcls.nuc_operacao WHERE id = " + global_variaveis::id_operacao_aberta);

        if (query.exec())
        {
            query.first();

            QString nome_operacao = query.value(0).toString();

            ui->label_operacao_aberta->setText("Operação aberta: " + nome_operacao);
            ui->label_alvo_selecionado->setText("Alvo selecionado: ");

            ui->pushButton_nuc_agente->setEnabled(true);
            ui->pushButton_nuc_alvo->setEnabled(true);
            ui->pushButton_nuc_degravacao->setEnabled(true);
            ui->pushButton_nuc_vitima->setEnabled(true);
            ui->pushButton_nuc_inqt_proc->setEnabled(true);

        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executar a query para recuperar o nome da operação.");
        }
    }

}

void fm_main::on_pushButton_nuc_alvo_clicked()
{
    fm_nuc_alvo janela_nuc_alvo;
    janela_nuc_alvo.exec();

    if (global_variaveis::id_alvo_selecionado != "")
    {
        global_database::abrir_conexao();

        QSqlQuery query;
        query.prepare("SELECT nome FROM u566464314_hrcls.nuc_alvo WHERE id = " + global_variaveis::id_alvo_selecionado);

        if (query.exec())
        {
            query.first();

            QString nome_alvo = query.value(0).toString();

            ui->label_alvo_selecionado->setText("Alvo selecionado: " + nome_alvo);

        }
        else
        {
            QMessageBox::warning(this, "Erro", "Erro ao executar a query para recuperar o nome do alvo aberto");
            qDebug() << query.lastError();
        }

        global_database::fechar_conexao();
    }
}







