#ifndef TRANSLATE
#define TRANSLATE

#include "../interfaces/ServiceInterface.hpp"
#include "../baseClass/baseClass.hpp"
#include <sstream>

class TranslateProcessing : public ServiceInterface, private BaseClass
{

public:
    TranslateProcessing()
    {
        INPUT_FUNCTION = "BLABLA";
        OUTPUT_FUNCTION = "BLABLA";
        C_INPUT_FUNCTION = "BLABLA";
        C_OUTPUT_FUNCTION = "BLABLA";
        H_INPUT_FUNCTION = "BLABLA";
        H_OUTPUT_FUNCTION = "BLABLA";
        S_INPUT_FUNCTION = "BLABLA";
        S_OUTPUT_FUNCTION = "BLABLA";
    };                        //construtor
    ~TranslateProcessing(){}; //destrutor

    void run(FileReader *rawFile, FileWriter *preprocFile);
    FileReader *getFileReader(string filename);
    FileWriter *getFileWriter(string filename);

private:
    string INPUT_FUNCTION;
    string OUTPUT_FUNCTION;
    string C_INPUT_FUNCTION;
    string C_OUTPUT_FUNCTION;
    string H_INPUT_FUNCTION;
    string H_OUTPUT_FUNCTION;
    string S_INPUT_FUNCTION;
    string S_OUTPUT_FUNCTION;

    string translatedLine;
    int lineNumber;

    void analyseDefLabel(vector<string> &tokens, FileReader *rawFile);
    void analyseDirective(vector<string> &tokens, FileReader *rawFile);
    void analyseInstruction(vector<string> &tokens, FileReader *rawFile, int variableBegin);

    string getLabel(vector<string> &tokens, int begin);
    void parseCodeLine(string line, FileReader *rawFile, FileWriter *preprocFile);
    void translateTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile);
    bool preprocTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile);
    void assembletranslatedLine(vector<string> &tokens);
    void changeEquValues(vector<string> *tokens);
    void checkDefLabelInNextLine(vector<string> *tokens, FileReader *rawFile);
    void getLabelDefInNextLine(vector<string> *tokens, FileReader *rawFile);
};

#endif