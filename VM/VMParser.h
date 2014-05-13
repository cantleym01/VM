#ifndef VMPARSER_H
#define VMPARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class VMParser
{
public:
    VMParser(char* fileName); //constuctor
    ~VMParser(); //deconstructor
    bool hasMoreCommands(); //is there more?
    void advance(); //getline with fancy name
    char commandType(); //what type of command?
    void arg1();
    void arg2();

    //get functions
    string getSegment();
    string getName();
    int getIndex();

private:
    ifstream file;
    string commandLine;
    char command;
    string name;
    string segment;
    int index;

    vector<string> code;
};

VMParser::VMParser(char* fileName)
{
    file.open(fileName);

    segment = ""; //initialize this since it can be empty
    name = "";
    index = 0;
}

VMParser::~VMParser()
{
    file.close();
}

bool VMParser::hasMoreCommands()
{
    bool isThereMore = true;
    //if it is at the end of the file
    cout << file.eof() << "file"<< endl;
    if (file.eof())
    {
        cout << "herro" << endl;
        isThereMore = false;
    }
cout << isThereMore << endl;
    return isThereMore;
}

void VMParser::advance()
{
    //now get what that line is
    getline (file, commandLine);
    string compStr =  commandLine.substr(0,1);

    //while the command line is a comment, keep getting new lines
    while(compStr == "/" || compStr == "")
    {
        getline (file, commandLine);
        compStr =  commandLine.substr(0,1);
    }

    //with the correct getline, push that onto a vector
    istringstream codePieces(commandLine);
    string temp;

    //separate everything with a ' '
    while (getline(codePieces, temp, ' '))
    {
        code.push_back(temp);
    }
}

char VMParser::commandType()
{
    if (commandLine.substr(0,6) == "return") //it is an arithmatic command, so return
    {
        command =  'r'; //return 'r' for return
    }
    else if (commandLine.substr(0,4) == "call")
    {
        command = 'c'; //return 'c' for call
    }
    else if (commandLine.substr(0,4) == "push")
    {
        command = 'u'; //return 'u' for push
    }
    else if (commandLine.substr(0,3) == "pop")
    {
        command = 'o'; //return 'o' for pop
    }
    else if (commandLine.substr(0,5) == "label")
    {
        command = 'l'; //return 'l' for label
    }
    else if (commandLine.substr(0,4) == "goto")
    {
        command = 'g'; //return 'g' for goto
    }
    else if (commandLine.substr(0,2) == "if")
    {
        command = 'i'; //return 'i' for if
    }
    else if (commandLine.substr(0,8) == "function")
    {
        command = 'f'; //return 'f' for function
    }
    else if (commandLine.substr(0,2) == "if")
    {
        command = 'i'; //return 'i' for if
    }
    else
    {
        command =  'a'; //return 'a' for arithmatic
    }

    return command;
}

void VMParser::arg1()
{
    if(command == 'a')
    {
        name = commandLine;
    }
    else
    {
        name = code.at(1);
    }
}

void VMParser::arg2()
{
    //boolean parts to shorten arguements
    bool part1 = (code.at(1) == "argument" || code.at(1) == "local");
    bool part2 = (code.at(1) == "static" || code.at(1) == "constant");
    bool part3 = (code.at(1) == "this" || code.at(1) == "that");
    bool part4 = (code.at(1) == "pointer" || code.at(1) == "temp");
    bool part5 = (code.at(0) == "function" || code.at(0) == "push" || code.at(0) == "pop");

    if (part1 || part2 || part3 || part4 || part5)
    {
        segment = code.at(1); //make the segment
        index = atoi(code.at(2).c_str()); //make the index
    }
    else
    {
        index = atoi(code.at(1).c_str()); //make the index
    }

    //resize vector to use again
    code.resize(0);
}

string VMParser::getSegment()
{
    return segment;
}

string VMParser::getName()
{
    return name;
}

int VMParser::getIndex()
{
    return index;
}

#endif
