#ifndef FM_NUC_OPERACAO_H
#define FM_NUC_OPERACAO_H

#include <QDialog>

namespace Ui {
class fm_nuc_operacao;
}

class fm_nuc_operacao : public QDialog
{
    Q_OBJECT

public:
    explicit fm_nuc_operacao(QWidget *parent = nullptr);
    ~fm_nuc_operacao();

private slots:
    void on_pushButton_sair_clicked();

    void on_tabWidget_operacao_tabBarClicked(int index);

    void on_pushButton_pesquisar_operacao_clicked();

    void on_pushButton_abrir_operacao_clicked();

    void on_pushButton_sair_operacao_clicked();

    void on_pushButton_criar_operacao_clicked();

    void on_pushButton_cancelar_alteracoes_clicked();

    void on_pushButton_salvar_alteracoes_clicked();

private:
    Ui::fm_nuc_operacao *ui;
};

#endif // FM_NUC_OPERACAO_H
