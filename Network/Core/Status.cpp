//
// Created by pozni on 21.12.2024.
//

#include <string>
#include <list>
#include "Status.h"

void Status::OK(const std::string& massage,const std::list<std::string>& headers) const
{
    // std::string response =  "HTTP/1.1 200 OK\r\n"
    //                         "Content-Type: Application/json\r\n"
    //                         "Content-Length:5\r\n"
    //
    // ;
}

void Status::Created(const std::string& massage) const
{
}

void Status::Accepted(const std::string& massage) const
{
}

void Status::NoContent(const std::string& massage) const
{
}

void Status::BadRequest(const std::string& massage) const
{
}

void Status::NotFound(const std::string& massage) const
{
}

void Status::InternalServerError(const std::string& massage) const
{
}
