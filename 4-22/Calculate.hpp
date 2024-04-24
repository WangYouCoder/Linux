#include <iostream>
#include "Protocol.hpp"
#include <memory>
enum ErrCode
{
    Success = 0,
    DivZeroErr,
    ModZeroErr,
    UnknowOper
};

class Calculate
{
public:
    Calculate()
    {}
    std::shared_ptr<Response> Cal(std::shared_ptr<Request> req)
    {
        std::shared_ptr<Response> resp = factory.BuildResponse();
        resp->SetCode(Success);
        switch(req->GetOper())
        {
            case '+':
                resp->SetResult(req->GetX() + req->GetY());
                break;
            case '-':
                resp->SetResult(req->GetX() - req->GetY());
                break;
            case '*':
                resp->SetResult(req->GetX() * req->GetY());
                break;
            case '/':
            {
                if(req->GetY() == 0) 
                {
                    resp->SetCode(DivZeroErr);
                }
                else
                {
                    resp->SetResult(req->GetX() / req->GetY());
                }
            }
            break;
            case '%':
            {
                if(req->GetY() == 0)
                {
                    resp->SetCode(ModZeroErr);
                }
                else
                {
                    resp->SetResult(req->GetX() % req->GetY());
                }
            }
            break;
            default:
                resp->SetCode(UnknowOper);
                break;
        }

        return resp;
    }
    ~Calculate()
    {}
private:
    Factory factory;
};