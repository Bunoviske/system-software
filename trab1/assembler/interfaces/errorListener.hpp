#ifndef ERROR_LISTENER
#define ERROR_LISTENER

class ErrorListener {

public:

    ErrorListener(){};//construtor
    ~ErrorListener(){};

    //metodos virtuais puros -> necessario implementar na classe derivada
    virtual void onError(int line) = 0;

};

#endif