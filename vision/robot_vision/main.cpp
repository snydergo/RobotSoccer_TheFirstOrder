#include <stdio.h>
#include "hsvcolorsubspace.h"
#include "utils.h"
#include "objectdetection.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/filereadstream.h>

using namespace rapidjson;
using namespace std;

int main( int argc, char** argv )
{

    if (argc != 2) { cout << "no param file. Usage: program [param.json]\n"; return 1; }
    FILE* pFile = fopen(argv[1], "r");
    char buffer[65536];
    FileReadStream is(pFile, buffer, sizeof(buffer));
    Document document;
    document.ParseStream<0, UTF8<>, FileReadStream>(is);

    Value& root = document["param"];

}
