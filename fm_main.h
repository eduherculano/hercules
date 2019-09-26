#ifndef FM_MAIN_H
#define FM_MAIN_H

#include <QMainWindow>

namespace Ui {
class fm_main;
}

class fm_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit fm_main(QWidget *parent = nullptr);
    ~fm_main();

private slots:
    void on_pushButton_nuc_operacao_clicked();

    void on_pushButton_nuc_alvo_clicked();

private:
    Ui::fm_main *ui;
};

#endif // FM_MAIN_H
