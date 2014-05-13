#include <iostream>

#include "VMCodeWriter.h"
#include "VMParser.h"

using namespace std;

int main()
{
    char fileNameInput[200], fileNameOutput[200];

    cout << "Input a file name for input...";
    cin >> fileNameInput;

    cout << "Input a file name for output...";
    cin >> fileNameOutput;

    VMParser parser(fileNameInput);

    VMCodeWriter writer(fileNameOutput);
    writer.writeInit();

    //while the parser has more commands
    while(parser.hasMoreCommands())
    {
        parser.advance(); //get new line of data that is not a comment

        char cmd = parser.commandType();

        //if the command is not return
        if (cmd != 'r')
        {
            parser.arg1();
        }

        //if the command is push, pop, function, or call
        if (cmd == 'u' || cmd == 'o' || cmd == 'f' || cmd == 'c')
        {
            parser.arg2();
        }

        //write arithmetic
        if (cmd == 'a')
        {
            writer.writeArithmetic(parser.getName());
        }

        //write push and pop
        if (cmd == 'u' || cmd == 'o')
        {
            writer.writePushPop(cmd, parser.getSegment(), parser.getIndex());
        }

        if (cmd == 'l')
        {
            writer.writeLabel(parser.getName());
        }

        if (cmd == 'g')
        {
            writer.writeGoto(parser.getName());
        }

        if (cmd == 'i')
        {
            writer.writeIf(parser.getName());
        }

        if (cmd == 'c')
        {
            writer.writeCall(parser.getName(), parser.getIndex());
        }

        if (cmd == 'f')
        {
            writer.writeFun(parser.getName(), parser.getIndex());
        }

        if (cmd == 'r')
        {
            writer.writeRet();
        }
    }
    cout << "hello world";
    writer.writeEnd();
}
