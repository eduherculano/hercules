#ifndef FM_NUC_ALVO_H
#define FM_NUC_ALVO_H

#include <QDialog>

namespace Ui {
class fm_nuc_alvo;
}

class fm_nuc_alvo : public QDialog
{
    Q_OBJECT

public:
    explicit fm_nuc_alvo(QWidget *parent = nullptr);
    ~fm_nuc_alvo();

private slots:
    void on_pushButton_pesquisar_alvo_clicked();

    void on_pushButton_abrir_alvo_clicked();

    void on_pushButton_criar_alvo_clicked();

    void on_pushButton_cancelar_alteracoes_clicked();

    void on_pushButton_sair_clicked();

    void on_pushButton_salvar_alteracoes_clicked();

private:
    Ui::fm_nuc_alvo *ui;
};

#endif // FM_NUC_ALVO_H
