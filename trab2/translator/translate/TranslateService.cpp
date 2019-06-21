#include "TranslateService.hpp"

void TranslateService::runService(string readFile, string writeFile)
{
        passage->run(passage->getFileReader(readFile),
                           passage->getFileWriter(writeFile));
}