#include <iostream>
#include <string>
#include "Engine.h"

using namespace std;

int main(int argc, const char* argv[])
{
    Engine engine(argc, argv);
    return engine.run();
}