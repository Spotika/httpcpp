#include "../cpp-httplib/httplib.h"
#include <iostream>



int main() {
    
    httplib::Server svr;

    svr.Get("/ping", [](const httplib::Request &, httplib::Response &res) {
    res.set_content("OK", "text/plain");
    });

    svr.listen("0.0.0.0", 8080);

    return 0;
}
