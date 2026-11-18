#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QVector>
#include "verilog_token.h"
#include "verilog_module.h"
class FileParser
{
public:
    FileParser();
    QVector <verilog_module> ModProducer(QVector<verilog_token> vtokens);// function used for 
    
};

#endif // FILEPARSER_H
