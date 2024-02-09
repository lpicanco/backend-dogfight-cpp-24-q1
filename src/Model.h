#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <rapidjson/rapidjson.h>


class Balance {
public:
    Balance(const int total, std::string data_extrato, const int limite) : total(total),
                                                                           data_extrato(std::move(data_extrato)),
                                                                           limite(limite) {}

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.String("total");
        writer.Int(total);

        writer.String("data_extrato");
        writer.String(data_extrato.c_str(), static_cast<rapidjson::SizeType>(data_extrato.length()));

        writer.String("limite");
        writer.Int(limite);

        writer.EndObject();
    }

private:
    int total;
    std::string data_extrato;
    int limite;
};

class TransactionDetail {
public:
    TransactionDetail(int valor, std::string tipo, std::string descricao, std::string realizada_em) : valor(valor),
        tipo(std::move(tipo)),
        descricao(std::move(descricao)),
        realizada_em(std::move(realizada_em)) {}

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.String("valor");
        writer.Int(valor);

        writer.String("tipo");
        writer.String(tipo.c_str(), static_cast<rapidjson::SizeType>(tipo.length()));

        writer.String("descricao");
        writer.String(descricao.c_str(), static_cast<rapidjson::SizeType>(descricao.length()));

        writer.String("realizada_em");
        writer.String(realizada_em.c_str(), static_cast<rapidjson::SizeType>(realizada_em.length()));

        writer.EndObject();
    }

private:
    int valor;
    std::string tipo;
    std::string descricao;
    std::string realizada_em;
};


class AccountStatement {
public:
    explicit AccountStatement(Balance balance, std::vector<TransactionDetail> lastTransactions) :
        balance(std::move(balance)), lastTransactions(std::move(lastTransactions)) {}

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.String("saldo");
        balance.Serialize(writer);

        writer.String("ultimas_transacoes");
        writer.StartArray();

        for (const auto& transaction : lastTransactions)
        {
            transaction.Serialize(writer);
        }
        writer.EndArray();

        writer.EndObject();
    }

private:
    const Balance balance;
    std::vector<TransactionDetail> lastTransactions;
};


#endif //MODEL_H
