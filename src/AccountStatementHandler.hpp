#ifndef ACCOUNTSTATEMENTHANDLER_HPP
#define ACCOUNTSTATEMENTHANDLER_HPP
#include "drogon/HttpController.h"

using namespace drogon;

class AccountStatementHandler {
public:
    static Task<> execute(int id,
                          const std::function<void(const HttpResponsePtr&)>& callback);
};


#endif //ACCOUNTSTATEMENTHANDLER_HPP
