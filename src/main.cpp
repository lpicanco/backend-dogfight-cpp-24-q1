#include <iostream>
#include <string>
#include "rapidjson/prettywriter.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>
#include "AccountStatementHandler.hpp"
#include "TransactionHandler.hpp"

using namespace rapidjson;
using namespace drogon;

int main() {
    const auto databaseHost = getenv("DATABASE_HOST");
    const auto databasePort = std::stoi(getenv("DATABASE_PORT"));
    const auto databaseName = getenv("DATABASE_NAME");
    const auto databaseUser = getenv("DATABASE_USER");
    const auto databasePassword = getenv("DATABASE_PASSWORD");

    app()
        .setThreadNum(10)
        .createDbClient("postgresql", databaseHost, databasePort, databaseName, databaseUser, databasePassword)
        .registerHandler("/clientes/{1}/extrato", [](
                         HttpRequestPtr req,
                         std::function<void(const HttpResponsePtr&)> callback,
                         const int& id
                     ) -> Task<> {
                             co_await AccountStatementHandler::execute(req, id, callback);
                             co_return;
                         },
                         {Get})
        .registerHandler("/clientes/{1}/transacoes", [](
                         HttpRequestPtr req,
                         std::function<void(const HttpResponsePtr&)> callback,
                         const int& id
                     ) -> Task<> {
                             co_await TransactionHandler::execute(req, id, callback);
                             co_return;
                         },
                         {Post})
        .addListener("0.0.0.0", 9999)
        .run();
    return 0;
}
