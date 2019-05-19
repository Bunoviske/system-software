#include "sintaticAnalysis.hpp"

//TODA VEZ QUE CHAMAR LEXICO, TEM QUE CHAMAR A FUNCAO lexical.setCurrentLine();

//IMPORTANTE: toda funcao do analisador sintatico so deve ser chamada depois que souber que o primeiro token é valido
// isso quer dizer que nao se analisa lexicamente tokens[0] em nenhuma funcao!

bool SintaticAnalyser::checkInstructionSintax(vector<string> tokens)
{
    lexical.setLineNumber(this->currentLine);
    //JMP
    if((tokens[0] == "JMP") || (tokens[0] == "JMPN") || (tokens[0] == "JMPP") || (tokens[0] == "JMPZ")){
        if(tokens.size() > 2){
            throwError("Instrucoes de jump aceitam apenas 1 argumento");
            return false;
        }
        else{
            if(tokens.size() == 1){
                throwError("Instrucoes de jump precisam de 1 argumento");
                return false;
            }
            else{ //numero de argumentos correto
                return true;
            }
        }
    }

    //TODO - ADD L1 + 1
    return true;
}

//quando essa funcao é chamada, sabe-se q tem pelo menos uma posicao no vetor igual ao LABEL
bool SintaticAnalyser::checkMacroCallSintax(vector<string> &tokens)
{
    lexical.setLineNumber(this->currentLine);

    //sintaxe de uma chamada de macro deve ser LABEL ARG1, ARG2, ARG3

    //TODO //ARG1 + 2, //ARG1 + 2. Essa primeira condicao ja daria problema, pois teria mais que 4 tokens
    if (tokens.size() > 4)
    {
        throwError("Chamadas de macro podem ter no maximo 3 argumentos");
        return false;
    }
    else
    {
        if (lexical.getTokenType(tokens[0]) == LABEL)
        {
            for (size_t i = 1; i < tokens.size(); i++) //percorre todos argumentos
            {
                //pode ser nas seguintes formas:
                //ARG1,
                //ARG1
                int tokenType = lexical.getTokenType(tokens[i]);
                if (tokenType == COPY_ARGUMENT)
                {
                    if (i == tokens.size() - 1)
                    {
                        throwError("Ultimo argumento nao pode ter virgula");
                        return false;
                    }
                }
                else if (tokenType == LABEL)
                {
                    if (i < tokens.size() - 1)
                    {
                        throwError("Somente ultimo argumento nao tem virgula");
                        return false;
                    }
                }
                else
                {
                    throwError("Os unicos argumentos que a macro aceita sao labels");
                    return false;
                }
            }
            return true; //se passar pelo loop sem erros, esta tudo certo
        }
        else
        {
            throwError("Chamadas de macro devem ser feitas com um label no primeiro token da linha");
            return false;
        }
    }
}

bool SintaticAnalyser::isValidLabel(string label)
{
    label.pop_back();                   //tira ':' do final
    return !lexical.isOperation(label); //se o label for uma operacao, nao detecta como sendo label valido
}

bool SintaticAnalyser::checkLabelDefinitionSintax(vector<string> &tokens)
{
    lexical.setLineNumber(this->currentLine);

    vector<string> tokensAux = tokens;

    //toda definicao de label deve ter um rotulo valido e uma operacao (diretiva ou instrucao) como argumentos

    if (tokensAux.size() >= 2)
    {
        if (lexical.getTokenType(tokensAux[0]) == DEF_LABEL)
        { //ja se sabe que o primeiro token é valido (redundante)
            if (isValidLabel(tokensAux[0]))
            { // porem nao se sabe se o label é valido (pode ser uma inst ou diretiva)
                int tokenType = lexical.getTokenType(tokensAux[1]);
                if (tokenType == INSTRUCTION)
                    return true; //nao verifica sintaxe da instrucao na definicao de label!!!

                else if (tokenType == DIRECTIVE)
                {
                    tokensAux.erase(tokensAux.begin()); //tira definicao de rotulo para analisar a diretiva
                    return checkDirectiveSintax(tokensAux);
                }
                else
                {
                    throwError("Definicao de label deve ocorrer com alguma operacao (instrucao ou diretiva)");
                    return false;
                }
            }
            else
            {
                throwError("Label nao pode ter o mesmo nome que uma operacao");
                return false;
            }
        }
        else
        {
            throwError("Definicao de label no formato incorreto. Deve ter ':' no final e ser um label valido");
            return false;
        }
    }
    else
    {
        throwError("Definicao de rotulo deve ter no minimo 2 argumentos");
        return false;
    }
}

//sempre que chega nessa funcao, ja foi analisado lexicamente o token da diretiva. esse token tb esta na posicao 0
bool SintaticAnalyser::checkDirectiveSintax(vector<string> &tokens)
{
    lexical.setLineNumber(this->currentLine);

    //quando chega aqui, ja se sabe que o token da diretiva é valido e esta na tabela
    if (tokens[0] == "SPACE")
        return checkSpaceSintax(tokens);

    else if (tokens[0] == "CONST")
        return checkConstSintax(tokens);

    else if (tokens[0] == "SECTION")
        return checkSectionSintax(tokens);

    else if (tokens[0] == "EQU")
        return checkEquSintax(tokens);

    else if (tokens[0] == "IF")
        return checkIFSintax(tokens);

    else if (tokens[0] == "MACRO")
        return checkMacroDefSintax(tokens);

    else
    { //redundante, nunca entra aqui
        cout << "DIRETIVA NAO ENCONTRADA NA ANALISE SINTATICA. REDUNDANTE" << endl;
        return false;
    }
}

bool SintaticAnalyser::checkSymbolOffsetSintax(vector<string> tokens, int labelIndex){
    lexical.setLineNumber(this->currentLine);
    if(tokens[labelIndex+1] == "+"){
        if(tokens[labelIndex+1] != tokens.back()){
            if(lexical.getTokenType(tokens[labelIndex+2]) == NUMBER){
                return true;
            }
            else{
                throwError("Erro na sintaxe de vetores");
                return false;
            }
        }
        else{
            throwError("Erro na sintaxe de vetores");
            return false;
        }
    }
}
/********************************* METODOS PRIVADOS QUE ANALISAM DIRETIVAS
 *
 *
 *  sao necessarios varios metodos pois nao existe arvore de derivacao para fazer algo generico.
 *  Cada funcao deve saber se os operandos sao do tipo valido. Para instrucoes, isso nao é necessario
 *  pois so se aceita rotulo como argumento
 *
 *
 * *********************************/

bool SintaticAnalyser::checkDirectiveNumOperands(vector<string> &tokens)
{
    int numOperands = tables.getDirectiveOperands(tokens[0]);
    if (numOperands == -1)
    { //so pode ser uma dessas duas diretivas
        if (tokens[0] == "SPACE")
        {
            if (tokens.size() <= 2) //maximo 1 argumento
                return true;
        }
        else if (tokens[0] == "MACRO")
        {
            if (tokens.size() <= 4) //maximo 3 argumentos
                return true;
        }
    }
    else if (tokens.size() - 1 == numOperands) //faz tokens.size - 1 para tirar o nome da diretiva da comparacao
        return true;

    throwError("Numero de argumentos errado"); //se chegar aqui é pq numero de argumento é invalido
    return false;
}

bool SintaticAnalyser::checkSectionSintax(vector<string> &tokens)
{
    if (checkDirectiveNumOperands(tokens))
    { //quer dizer que tem um operando apenas, entao pode acessar proxima posicao do vetor

        if (lexical.getTokenType(tokens[1]) == LABEL) //cada diretiva tem um tipo de argumento diferente
            return true;
        else
        {
            throwError("Argumento deve ser uma string valida. Nao pode ser operacao ou diretiva");
            return false;
        }
    }
    else
        return false;
}

bool SintaticAnalyser::checkIFSintax(vector<string> &tokens)
{
    if (checkDirectiveNumOperands(tokens))
    { //quer dizer que tem um operando apenas, entao pode acessar proxima posicao do vetor

        if (lexical.getTokenType(tokens[1]) == NUMBER)
            //deve ser um numero pois o preproc ja substituiu pelos equs definidos
            return true;
        else
        {
            throwError("Argumento deve ser um numero ja definido por uma EQU");
            return false;
        }
    }
    else
        return false;
}

bool SintaticAnalyser::checkEquSintax(vector<string> &tokens)
{
    if (checkDirectiveNumOperands(tokens)) //quer dizer que tem um operando apenas
    {
        if (lexical.getTokenType(tokens[1]) == NUMBER)
            //deve ser um numero pois o preproc ja substituiu pelos equs definidos
            return true;
        else
        {
            throwError("Argumento deve ser um numero");
            return false;
        }
    }
    else
        return false;
}

bool SintaticAnalyser::checkSpaceSintax(vector<string> &tokens)
{
    if (checkDirectiveNumOperands(tokens))
    {
        if (tokens.size() == 2) //so analisa quando tem argumentos.
                                //checkDirectiveNumOperands() garante que numero de argumento está correto
            if (lexical.getTokenType(tokens[1]) == NUMBER)
                return true;
            else
            {
                throwError("Argumento deve ser um numero");
                return false;
            }
        else
            return true; //apenas a diretiva SPACE sem argumentos
    }
    else
        return false;
}

bool SintaticAnalyser::checkConstSintax(vector<string> &tokens)
{
    if (checkDirectiveNumOperands(tokens)) //quer dizer que tem um operando apenas
    {
        if (lexical.getTokenType(tokens[1]) == NUMBER)
            //deve ser um numero pois o preproc ja substituiu pelos equs definidos
            return true;
        else
        {
            throwError("Argumento deve ser um numero");
            return false;
        }
    }
    else
        return false;
}

bool SintaticAnalyser::checkMacroDefSintax(vector<string> &tokens)
{
    //macro def LABEL: MACRO &A, &B, &C

    if (checkDirectiveNumOperands(tokens))
    { //checkDirectiveNumOperands garante que numero de argumento está correto

        for (size_t i = 1; i < tokens.size(); i++)
        {
            if (lexical.getTokenType(tokens[i]) != MACRO_ARGUMENT)
            {
                throwError("Argumentos da definicao de macro invalidos");
                return false;
            }
            else if ((i == tokens.size() - 1 &&
                      tokens[i][tokens[i].size() - 1] == ',') ||
                     (i < tokens.size() - 1 &&
                      tokens[i][tokens[i].size() - 1] != ','))
            { //checa se argumentos que nao sao o ultimo sao separados por virgula
                throwError("Argumentos da macro devem ser separados por virgula (ultimo argumento nao precisa)");
                return false;
            }
        }
        return true; //se passar pelo loop sem erros, tudo esta certo
    }
    else
        return false;
}
