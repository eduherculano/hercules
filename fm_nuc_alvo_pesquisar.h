#ifndef FM_NUC_ALVO_PESQUISAR_H
#define FM_NUC_ALVO_PESQUISAR_H

#include <QDialog>

namespace Ui {
class fm_nuc_alvo_pesquisar;
}

class fm_nuc_alvo_pesquisar : public QDialog
{
    Q_OBJECT

public:
    explicit fm_nuc_alvo_pesquisar(QWidget *parent = nullptr);
    ~fm_nuc_alvo_pesquisar();

private slots:
    void on_pushButton_pesquisar_clicked();

    void on_tableWidget_alvo_cellDoubleClicked(int row, int column);

private:
    Ui::fm_nuc_alvo_pesquisar *ui;
};

#endif // FM_NUC_ALVO_PESQUISAR_H
