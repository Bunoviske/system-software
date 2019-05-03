#include "passage2Service.hpp"

void Passage2Service::runService(string readFile, string writeFile)
{
        passage->run(passage->getFileReader(readFile),
                           passage->getFileWriter(writeFile));
}