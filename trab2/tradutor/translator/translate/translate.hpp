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
        INPUT_FUNCTION = "LERINTEIRO";
        OUTPUT_FUNCTION = "ESCREVERINTEIRO";
        C_INPUT_FUNCTION = "LERCHAR";
        C_OUTPUT_FUNCTION = "ESCREVERCHAR";
        H_INPUT_FUNCTION = "LERHEXA";
        H_OUTPUT_FUNCTION = "ESCREVERHEXA";
        S_INPUT_FUNCTION = "LERSTRING";
        S_OUTPUT_FUNCTION = "ESCREVERSTRING";
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
    bool importAssembly; //used to call the function to import the assembly functions

    void importAssemblyFunctions(FileWriter *functionsFile);

    void analyseDefLabel(vector<string> &tokens, FileReader *rawFile);
    void analyseDirective(vector<string> &tokens, FileReader *rawFile);
    void analyseInstruction(vector<string> &tokens, FileReader *rawFile, int variableBegin);

    string getLabelWithDisplacement(vector<string> &tokens, int begin);
    void parseCodeLine(string line, FileReader *rawFile, FileWriter *preprocFile);
    void translateTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile);
    bool preprocTokens(vector<string> &tokens, FileReader *rawFile, FileWriter *preprocFile);
    void assembletranslatedLine(vector<string> &tokens);
    void changeEquValues(vector<string> *tokens);
    void checkDefLabelInNextLine(vector<string> *tokens, FileReader *rawFile);
    void getLabelDefInNextLine(vector<string> *tokens, FileReader *rawFile);
};

#endif
