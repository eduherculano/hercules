#include "fm_main.h"
#include <QApplication>

#include "global_variaveis.h"
#include "fm_login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    fm_login janela_login;
    janela_login.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    janela_login.setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    //janela_login.setWindowFlag(Qt::WindowTitleHint, false);
    //janela_login.setWindowFlag(Qt::WindowSystemMenuHint, false);
    janela_login.show();


    /*
    if (global_variaveis::id_usuario_ativo != "")
    {
        fm_main janela_main;
        janela_main.show();
    }
    */


    return a.exec();
}
