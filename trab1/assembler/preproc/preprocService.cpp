#include "preprocService.hpp"

void PreProcessingService::runService(string readFile, string writeFile)
{
        passage->run(passage->getFileReader(readFile),
                           passage->getFileWriter(writeFile));
}