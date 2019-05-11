#ifndef ERROR_LISTENER
#define ERROR_LISTENER

#include <string>
using namespace std;

class ErrorListener {

public:

    ErrorListener(){};//construtor
    ~ErrorListener(){};

    //metodos virtuais puros -> necessario implementar na classe derivada
    virtual void onError(int line, string error) = 0;

};

#endif