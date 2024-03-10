#include "AccountStatementHandler.hpp"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "Model.h"

Task<> AccountStatementHandler::execute(int id,
                                        const std::function<void(const HttpResponsePtr&)>& callback) {
    if (id <= 0 || id > 5)
    {
        const auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k404NotFound);
        callback(resp);
        co_return;
    }

    const auto dbClient = app().getFastDbClient();

    const auto balanceResult = co_await dbClient->execSqlCoro(
        R"(SELECT balance, TO_CHAR(now(), 'YYYY-MM-DD"T"HH24:MI:SS.US"Z"') as now, account_limit from clients WHERE id = $1)",
        id);

    const auto balance = Balance(balanceResult[0]["balance"].as<int>(),
                                 balanceResult[0]["now"].as<std::string>(),
                                 balanceResult[0]["account_limit"].as<int>());

    const auto transactionsResult = co_await dbClient->execSqlCoro(
        R"(SELECT value, operation, description, TO_CHAR(created_at, 'YYYY-MM-DD"T"HH24:MI:SS.US"Z"') as created_at FROM transactions WHERE client_id = $1 ORDER BY id DESC LIMIT 10)",
        id);

    std::vector<TransactionDetail> lastTransactions;
    for (const auto& row : transactionsResult)
    {
        lastTransactions.emplace_back(row["value"].as<int>(), row["operation"].as<std::string>(),
                                      row["description"].as<std::string>(), row["created_at"].as<std::string>());
    }

    const auto statement = AccountStatement(balance, lastTransactions);
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter writer(sb);
    statement.Serialize(writer);

    const auto resp = HttpResponse::newHttpResponse();
    resp->setBody(sb.GetString());
    resp->setContentTypeCode(CT_APPLICATION_JSON);

    callback(resp);

    co_return;
}
