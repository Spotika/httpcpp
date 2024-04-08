#pragma once

#include<exception>

enum HTTP_STATUS_CODE {
    OK = 200,
    INTERNAL_SERVER_ERROR = 500,
    NOT_FOUND = 404,
    BAD_REQUEST = 400,
    UNPROCESSABLE_ENTITY = 422
};

class HTTPException : public std::exception {
public:
    HTTPException(const char * message) : message_(message) {}

    HTTPException(
        const char* message, 
        HTTP_STATUS_CODE status_code
    ) : HTTPException(message) {
        status_code_ = status_code;
    }

    const char* what() {
        return message_;
    }

    const HTTP_STATUS_CODE status() {
        return status_code_;
    }

private:
    const char* message_;
    HTTP_STATUS_CODE status_code_ = HTTP_STATUS_CODE::BAD_REQUEST;
};
