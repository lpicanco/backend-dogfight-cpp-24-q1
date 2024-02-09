#ifndef TRANSACTIONHANDLER_HPP
#define TRANSACTIONHANDLER_HPP

#include "drogon/HttpController.h"
using namespace drogon;

class TransactionHandler {
public:
    static Task<> execute(const HttpRequestPtr& req,
                          int id,
                          const std::function<void(const HttpResponsePtr&)>& callback);
};


#endif //TRANSACTIONHANDLER_HPP
