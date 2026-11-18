#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QVector>
#include "verilog_token.h"
#include "verilog_module.h"
class FileParser
{
public:
    FileParser();
    QVector<verilog_module> ModProducer(const QVector<verilog_token> &vtokens);
};

#endif // FILEPARSER_H
