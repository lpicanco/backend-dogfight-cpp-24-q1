#include <iostream>
#include <string>
#include "rapidjson/prettywriter.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpController.h>
#include "AccountStatementHandler.hpp"
#include "TransactionHandler.hpp"
#include <cmath>

using namespace rapidjson;
using namespace drogon;

int main() {
    const auto databaseHost = getenv("DATABASE_HOST");
    const auto databasePort = std::stoi(getenv("DATABASE_PORT"));
    const auto databaseName = getenv("DATABASE_NAME");
    const auto databaseUser = getenv("DATABASE_USER");
    const auto databasePassword = getenv("DATABASE_PASSWORD");
    const auto serverPort = std::stoi(getenv("PORT") ? getenv("PORT") : "9999");

    std::cout << "🚀 Server running at http://localhost:" << serverPort << std::endl;

    app()
        .setThreadNum(10)
        .createDbClient("postgresql", databaseHost, databasePort, databaseName, databaseUser, databasePassword, 1, "", "default", true)
        .registerHandler("/clientes/{1}/extrato", [](
                         HttpRequestPtr req,
                         std::function<void(const HttpResponsePtr&)> callback,
                         const float& id
                     ) -> Task<> {
                             if (ceil(id) != id)
                             {
                                 const auto resp = HttpResponse::newHttpResponse();
                                 resp->setStatusCode(k422UnprocessableEntity);
                                 callback(resp);
                                 co_return;
                             }
                             co_await AccountStatementHandler::execute(ceil(id), callback);
                             co_return;
                         },
                         {Get})
        .registerHandler("/clientes/{1}/transacoes", [](
                         const HttpRequestPtr req,
                         std::function<void(const HttpResponsePtr&)> callback,
                         const float& id
                     ) -> Task<> {
                             if (ceil(id) != id)
                             {
                                 const auto resp = HttpResponse::newHttpResponse();
                                 resp->setStatusCode(k422UnprocessableEntity);
                                 callback(resp);
                                 co_return;
                             }
                             co_await TransactionHandler::execute(req, ceil(id), callback);
                             co_return;
                         },
                         {Post})
        .addListener("0.0.0.0", serverPort)
        .run();
    return 0;
}
