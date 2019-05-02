#include "preprocService.hpp"

void PreProcessingService::runService(string readFile, string writeFile)
{
        preProcessing->run(preProcessing->getFileReader(readFile),
                           preProcessing->getFileWriter(writeFile));
}