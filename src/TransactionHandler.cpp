#include "TransactionHandler.hpp"

static HttpResponsePtr unprocessableEntity() {
    const auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k422UnprocessableEntity);
    return resp;
}

Task<> TransactionHandler::execute(const HttpRequestPtr& req, int id,
                                   const std::function<void(const HttpResponsePtr&)>& callback) {

    if (id <= 0 || id > 5)
    {
        const auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k404NotFound);
        callback(resp);
        co_return;
    }

    const auto json = req->getJsonObject();

    if (!json)
    {
        callback(unprocessableEntity());
        co_return;
    }

    const auto tipo = (*json)["tipo"].asString();
    int raw_valor = (*json)["valor"].asInt();
    if (raw_valor <= 0)
    {
        callback(unprocessableEntity());
        co_return;
    }

    int valor = raw_valor;
    if (tipo == "d")
    {
        valor = raw_valor * -1;
    }
    else if (tipo != "c")
    {
        callback(unprocessableEntity());
        co_return;
    }

    auto descricao = (*json)["descricao"].asString();
    if (descricao.empty() || descricao.size() > 10)
    {
        callback(unprocessableEntity());
        co_return;
    }

    const auto dbClient = app().getDbClient();

    const auto result = co_await dbClient->execSqlCoro(
        R"(
                    SELECT balance, account_limit FROM insertTransaction($1, $2, $3, $4, $5);)",
        id, valor, raw_valor, tipo, descricao);

    if (result.affectedRows() == 0)
    {
        callback(unprocessableEntity());
        co_return;
    }

    Json::Value ret;
    ret["limite"] = result[0]["account_limit"].as<int>();
    ret["saldo"] = result[0]["balance"].as<int>();
    const auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);

    co_return;
}
