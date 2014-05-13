#ifndef VMCODEWRITER_H
#define VMCODEWRITER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class VMCodeWriter
{
public:
    VMCodeWriter(char* fileName); //constuctor
    ~VMCodeWriter(); //deconstructor

    void writeArithmetic(string commandName);

    void writePushPop(char commandType, string segment, int index);

    void writeInit();

    void writeLabel(string label);

    void writeGoto(string label);

    void writeIf(string label);

    void writeCall(string name, int numArgs);

    void writeRet();

    void writeFun(string name, int numLocals);

    void writeEnd();

protected:
    //convert segment
    string segmentConvert(string segment, int index);

    //arithmetic functions
    void writeAdd();
    void writeSub();
    void writeNeg();
    void writeEq();
    void writeGt();
    void writeLt();
    void writeAnd();
    void writeOr();
    void writeNot();

    //push and pop
    void writePush(string segment, int index);
    void writePop(string segment, int index);

private:
    ofstream file;
    string fileName, retString;
    int cnt; //counter to make multiple lables unique

};

VMCodeWriter::VMCodeWriter(char* fileName)
{
    file.open(fileName);
    this -> fileName = fileName;
    cnt = 0;
}

VMCodeWriter::~VMCodeWriter()
{
    file.close();
}



string VMCodeWriter::segmentConvert(string segment, int index)
{
    string retSeg;
    char intToStr[200];

    if (segment == "local")
    {
        retSeg = "LCL";
    }
    else if (segment == "argument")
    {
        retSeg = "ARG";
    }
    else if (segment == "this")
    {
        retSeg = "THIS";
    }
    else if (segment == "that")
    {
        retSeg = "THAT";
    }
    else if (segment == "pointer")
    {
        retSeg = "R3";
    }
    else if (segment == "temp")
    {
        retSeg = "R5";
    }
    else //it is static
    {
        //return index in base 10
        itoa(index,intToStr,10);

        retSeg = fileName + "." + intToStr;
    }

    return retSeg;
}

void VMCodeWriter::writeArithmetic(string commandName)
{
    if (commandName == "add")
    {
        writeAdd();
    }
    else if (commandName == "sub")
    {
        writeSub();
    }
    else if (commandName == "neg")
    {
        writeNeg();
    }
    else if (commandName == "eq")
    {
        writeEq();
    }
    else if (commandName == "gt")
    {
        writeGt();
    }
    else if (commandName == "lt")
    {
        writeLt();
    }
    else if (commandName == "and")
    {
        writeAnd();
    }
    else if (commandName == "or")
    {
        writeOr();
    }
    else
    {
        writeNot();
    }
}

void VMCodeWriter::writeAdd()
{
    file << "//add" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "@R13" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "@R13" << endl;
    file << "M=M+D" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;
}

void VMCodeWriter::writeSub()
{
    file << "//sub" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "@R13" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "@R13" << endl;
    file << "M=D-M" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;
}

void VMCodeWriter::writeNeg()
{
    file << "//neg" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "A=A-1" << endl;
    file << "M=-M" << endl;
}

void VMCodeWriter::writeEq()
{
    //use this for labels
    string label = "EQUALS";

    file << "//eq" << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M-D" << endl;
    file << "@" + label << cnt << endl;
    file << "D;JEQ" << endl;
    file << "D=1" << endl;
    file << "(" + label << cnt << ")" << endl;
    file << "D=D-1" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;
    cnt++;
}

void VMCodeWriter::writeGt()
{
    //use this for labels
    string label1 = "GREATER";
    string label2 = "NOTGREATER";

    file << "//gt" << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M-D" << endl;
    file << "@" + label1 << cnt << endl;
    file << "D;JGT" << endl;
    file << "D=0" << endl;
    file << "@" + label2 << cnt << endl;
    file << "0;JMP" << endl;
    file << "(" + label1 << cnt << ")" << endl;
    file << "D=-1" << endl;
    file << "(" + label2 << cnt << ")" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;
    cnt++;
}

void VMCodeWriter::writeLt()
{
    //use this for labels
    string label1 = "LESS";
    string label2 = "NOTLESS";

    file << "//lt" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M-D" << endl;
    file << "@" + label1 << cnt << endl;
    file << "D;JLT" << endl;
    file << "D=0" << endl;
    file << "@" + label2 << cnt <<endl;
    file << "0;JMP" << endl;
    file << "(" + label1 << cnt << ")" << endl;
    file << "D=-1" << endl;
    file << "(" + label2 << cnt << ")" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;
    cnt++;
}

void VMCodeWriter::writeAnd()
{
    file << "//and" << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M" << endl;
    file << "A=A-1" << endl;
    file << "M=D&M" << endl;
}

void VMCodeWriter::writeOr()
{
    file << "//or" << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M" << endl;
    file << "A=A-1" << endl;
    file << "M=D|M" << endl;
}

void VMCodeWriter::writeNot()
{
    file << "//not" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "D=!D" << endl;
}

void VMCodeWriter::writePushPop(char commandType, string segment, int index)
{
    if (commandType == 'u') //push
    {
        writePush(segment, index);
    }
    else //pop
    {
        writePop(segment, index);
    }
}

void VMCodeWriter::writePush(string segment, int index)
{
    string segStrConvert = segmentConvert(segment, index);

    file << "//push" << endl;

    if (segment == "constant")
    {
        file << "@" << index << endl;
        file << "D=A" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        file << "M=D" << endl;
        file << "@SP" << endl;
        file << "M=M+1" << endl;
    }
    else if (segment == "static" || segment == "temp" || segment == "pointer")
    {
        file << "@" + segStrConvert << endl;
        file << "D=M" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        file << "M=D" << endl;
        file << "@SP" << endl;
        file << "M=M+1" << endl;
    }
    else
    {
        file << "@" + segStrConvert << endl;
        file << "D=M" << endl;
        file << "@" << index << endl;
        file << "A=D+A" << endl;
        file << "D=M" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        file << "M=D" << endl;
        file << "@SP" << endl;
        file << "M=M+1" << endl;
    }
}

void VMCodeWriter::writePop(string segment, int index)
{
    string segStrConvert = segmentConvert(segment, index);

    if (segment == "static" || segment == "temp" || segment == "pointer")
    {
        file << "@" + segStrConvert << endl;
        file << "D=A" << endl;
        file << "@R13" << endl;
        file << "M=D" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        file << "D=M" << endl;
        file << "@SP" << endl;
        file << "M=M-1" << endl;
        file << "@R13" << endl;
        file << "A=M" << endl;
        file << "M=D" << endl;
    }
    else
    {
        file << "@" + segStrConvert << endl;
        file << "D=M" << endl;
        file << "@" << index << endl;
        file << "D=D+A" << endl;
        file << "@R13" << endl;
        file << "M=D" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        file << "D=M" << endl;
        file << "@SP" << endl;
        file << "M=M-1" << endl;
        file << "@R13" << endl;
        file << "A=M" << endl;
        file << "M=D" << endl;
    }
}

void VMCodeWriter::writeInit()
{
    file << "//init" << endl;
    file << "@256" << endl;
    file << "D=A" << endl;
    file << "@SP" << endl;
    file << "M=D" << endl;
}

void VMCodeWriter::writeLabel(string label)
{
    file << "//label" << endl;
    file << "("+label+")" << endl;
}

void VMCodeWriter::writeGoto(string label)
{
    file << "//goto" << endl;
    file << "@" + label << endl;
    file << "0;JMP" << endl;
}

void VMCodeWriter::writeIf(string label)
{
    file << "//if-goto" << endl;
    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "@" + label << endl;
    file << "D;JNE" << endl;
}

void VMCodeWriter::writeCall(string name, int numArgs)
{
    int offset = numArgs + 5;

    file << "//call" << endl;
    file << "@" + retString << endl;
    file << "D=A" << endl;

    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    file << "@LCL" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    file << "@ARG" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    file << "@THIS" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    file << "@THAT" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    file << "@" << numArgs << endl;
    file << "D=M" << endl;
    file << "@" << offset << endl;
    file << "D=D+M" << endl;
    file << "@SP" << endl;
    file << "D=M-D" << endl;
    file << "@ARG" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "D=M" << endl;
    file << "@LCL" << endl;
    file << "M=D" << endl;

    file << "@" + name << endl;
    file << "0;JMP" << endl;

    file << "(" + retString + ")" << endl;
}

void VMCodeWriter::writeRet()
{
    /**reference "sheet" pg163
        Frame = LCL
        Ret = *(Frame-5)
        *Arg = pop()
        SP = arg+1
        that = *(frame-1)
        this = *(frame-2)
        arg = *(frame-3)
        lcl = *(frame-4)
        goto ret
    */

    file << "//ret" << endl;
    file << "@LCL" << endl;
    file << "D=M" << endl;
    file << "@R13" << endl;
    file << "M=D" << endl;
    file << "@5" << endl;
    file << "D=D-A" << endl;
    file << "@R13" << endl;
    file << "A=D" << endl;
    file << "D=M" << endl;
    file << "@R14" << endl;
    file << "M=D" << endl;

    file << "@SP" << endl;
    file << "M=M-1" << endl;
    file << "A=M" << endl;
    file << "D=M" << endl;
    file << "@ARG" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "D=M" << endl;
    file << "@SP" << endl;
    file << "M=D" << endl;
    file << "M=M+1" << endl;

    file << "@R13" << endl;
    file << "A=M-D" << endl;
    file << "D=M" << endl;
    file << "@THAT" << endl;
    file << "M=D" << endl;

    file << "@R13" << endl;
    file << "D=M" << endl;
    file << "@2" << endl;
    file << "A=D-A" << endl;
    file << "D=M" << endl;
    file << "@THIS" << endl;
    file << "M=D" << endl;

    file << "@R13" << endl;
    file << "D=M" << endl;
    file << "@3" << endl;
    file << "A=D-A" << endl;
    file << "D=M" << endl;
    file << "@ARG" << endl;
    file << "M=D" << endl;

    file << "@R13" << endl;
    file << "D=M" << endl;
    file << "@4" << endl;
    file << "A=D-A" << endl;
    file << "D=M" << endl;
    file << "@LCL" << endl;
    file << "M=D" << endl;

    file << "@R14" << endl;
    file << "D=M" << endl;
    file << "A=D" << endl;
    file << "0;JMP" << endl;

    writeEnd();
}

void VMCodeWriter::writeFun(string name, int numLocals)
{
    /**reference "sheet" pg163
        (f)
            repeat k times
            push 0

        pretty straight forward
    */

    retString = "RET_" + name;

    file << "//function" << endl;
    file << "("+name+")" << endl;

    for (int i = 0; i < numLocals; i++)
    {
        writePush("constant",0);
    }
}

void VMCodeWriter::writeEnd()
{
    file << "(END)" << endl;
    file << "@END" << endl;
    file << "0;JMP" << endl;
}

#endif
