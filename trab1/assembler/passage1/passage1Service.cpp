#include "passage1Service.hpp"

void Passage1Service::runService(string readFile)
{
        passage->run(passage->getFileReader(readFile));
}