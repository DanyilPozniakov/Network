//
// Created by pozni on 21.12.2024.
//

#ifndef STATUS_H
#define STATUS_H

class std::string;

class Status {
public:
    Status() = default;
    virtual ~Status() = default;

    void OK(const std::string& massage,const std::list<std::string>& headers) const;
    void Created(const std::string& massage) const;
    void Accepted(const std::string& massage) const;
    void NoContent(const std::string& massage) const;
    void BadRequest(const std::string& massage) const;
    void NotFound(const std::string& massage) const;
    void InternalServerError(const std::string& massage) const;

    virtual void Send(const std::string& response) = 0;

};



#endif //STATUS_H
