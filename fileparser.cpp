#include "fileparser.h"

FileParser::FileParser()
{
}

namespace
{
bool isIdentifier(const verilog_token &token)
{
    return (token.getType() == identifier);
}

void addDeclaredNames(
    verilog_module &module,
    const QVector<verilog_token> &tokens,
    qsizetype &position,
    void (verilog_module::*adder)(const QString &),
    qsizetype limit)
{
    qsizetype squareDepth = 0;

    for (; position < limit; ++position)
    {
        const verilog_token &token = tokens.at(position);

        if (token.getType() == closed_bracket)
            break;

        if (token.getType() == open_square)
        {
            ++squareDepth;
            continue;
        }
        if (token.getType() == close_square)
        {
            if (squareDepth > 0)
                --squareDepth;
            continue;
        }
        if (squareDepth > 0)
            continue;

        if (token.getType() == comma)
            continue;

        if (token.getType() == semicolon)
            break;

        if (token.getType() == reserved_keyword)
        {
            const TokenType keyword = token.getReservedKeyword();
            if (keyword == TokenType::KW_INPUT ||
                keyword == TokenType::KW_OUTPUT ||
                keyword == TokenType::KW_INOUT ||
                keyword == TokenType::KW_ENDMODULE)
            {
                if (keyword == TokenType::KW_ENDMODULE)
                    break;
                break;
            }
            if (keyword == TokenType::KW_WIRE ||
                keyword == TokenType::KW_REG)
            {
                break;
            }
        }

        if (isIdentifier(token))
            (module.*adder)(token.getID());
    }

    if (position < limit &&
        (tokens.at(position).getType() == semicolon ||
         tokens.at(position).getType() == comma))
    {
        ++position;
    }
}

void collectOldStylePorts(
    verilog_module &module,
    const QVector<verilog_token> &tokens,
    qsizetype start,
    qsizetype end)
{
    for (qsizetype i = start; i < end; ++i)
    {
        if (isIdentifier(tokens.at(i)))
            module.add_input(tokens.at(i).getID());
    }
}
}

QVector<verilog_module> FileParser::ModProducer(
    const QVector<verilog_token> &tokens)
{
    QVector<verilog_module> modules;

    for (qsizetype i = 0; i < tokens.size(); ++i)
    {
        if (tokens.at(i).getType() != reserved_keyword ||
            tokens.at(i).getReservedKeyword() != TokenType::KW_MODULE)
        {
            continue;
        }

        qsizetype namePosition = i + 1;
        while (namePosition < tokens.size() &&
               tokens.at(namePosition).getType() != identifier)
        {
            ++namePosition;
        }
        if (namePosition >= tokens.size())
            continue;

        verilog_module module(tokens.at(namePosition).getID());
        qsizetype endmodulePosition = namePosition + 1;

        bool foundHeaderOpen = false;
        qsizetype headerOpen = -1;
        qsizetype headerClose = -1;
        for (qsizetype j = namePosition + 1; j < tokens.size(); ++j)
        {
            if (tokens.at(j).getType() == reserved_keyword &&
                tokens.at(j).getReservedKeyword() == TokenType::KW_ENDMODULE)
            {
                endmodulePosition = j;
                break;
            }
            if (!foundHeaderOpen &&
                tokens.at(j).getType() == open_bracket)
            {
                foundHeaderOpen = true;
                headerOpen = j;
            }
            else if (foundHeaderOpen &&
                     headerClose < 0 &&
                     tokens.at(j).getType() == closed_bracket)
            {
                headerClose = j;
            }
        }

        if (headerOpen >= 0 && headerClose > headerOpen)
        {
            bool containsPortDirection = false;
            for (qsizetype j = headerOpen + 1; j < headerClose; ++j)
            {
                if (tokens.at(j).getType() == reserved_keyword &&
                    (tokens.at(j).getReservedKeyword() == TokenType::KW_INPUT ||
                     tokens.at(j).getReservedKeyword() == TokenType::KW_OUTPUT ||
                     tokens.at(j).getReservedKeyword() == TokenType::KW_INOUT))
                {
                    containsPortDirection = true;
                    break;
                }
            }

            if (containsPortDirection)
            {
                for (qsizetype j = headerOpen + 1; j < headerClose;)
                {
                    if (tokens.at(j).getType() != reserved_keyword)
                    {
                        ++j;
                        continue;
                    }

                    const TokenType keyword = tokens.at(j).getReservedKeyword();
                    if (keyword == TokenType::KW_INPUT)
                    {
                        ++j;
                        addDeclaredNames(module, tokens, j,
                                         &verilog_module::add_input,
                                         headerClose);
                    }
                    else if (keyword == TokenType::KW_OUTPUT)
                    {
                        ++j;
                        addDeclaredNames(module, tokens, j,
                                         &verilog_module::add_output,
                                         headerClose);
                    }
                    else if (keyword == TokenType::KW_INOUT)
                    {
                        ++j;
                        addDeclaredNames(module, tokens, j,
                                         &verilog_module::add_inout,
                                         headerClose);
                    }
                    else
                    {
                        ++j;
                    }
                }
            }
            else
            {
                collectOldStylePorts(module, tokens,
                                     headerOpen + 1, headerClose);
            }
        }

        for (qsizetype j = namePosition + 1; j < endmodulePosition; ++j)
        {
            if (tokens.at(j).getType() != reserved_keyword)
                continue;

            const TokenType keyword = tokens.at(j).getReservedKeyword();
            if (keyword == TokenType::KW_INPUT)
            {
                ++j;
                addDeclaredNames(module, tokens, j,
                                 &verilog_module::add_input,
                                 endmodulePosition);
            }
            else if (keyword == TokenType::KW_OUTPUT)
            {
                ++j;
                addDeclaredNames(module, tokens, j,
                                 &verilog_module::add_output,
                                 endmodulePosition);
            }
            else if (keyword == TokenType::KW_INOUT)
            {
                ++j;
                addDeclaredNames(module, tokens, j,
                                 &verilog_module::add_inout,
                                 endmodulePosition);
            }
            else if (keyword == TokenType::KW_WIRE)
            {
                ++j;
                addDeclaredNames(module, tokens, j,
                                 &verilog_module::add_wire,
                                 endmodulePosition);
            }
            else if (keyword == TokenType::KW_REG)
            {
                ++j;
                addDeclaredNames(module, tokens, j,
                                 &verilog_module::add_reg,
                                 endmodulePosition);
            }
        }

        modules.append(module);
        i = endmodulePosition;
    }
    return modules;
}