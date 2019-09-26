#ifndef GLOBAL_VARIAVEIS_H
#define GLOBAL_VARIAVEIS_H

#include <QString>

class global_variaveis
{
public:

    global_variaveis();
    ~global_variaveis();

    static bool database_status;  // true == ativo; false == conexão não estabelecida

    static QString id_usuario_ativo;
    static QString nome_usuario_ativo;
    static int nivel_usuario_ativo;

    static QString id_operacao_aberta;
    static QString id_alvo_selecionado;

};

#endif // GLOBAL_VARIAVEIS_H
