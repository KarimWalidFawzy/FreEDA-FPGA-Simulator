#include "verilog_token.h"


namespace
{
struct KeywordEntry
{
    const char *name;
    TokenType type;
};

const KeywordEntry keywordEntries[] = {
    {"always", TokenType::KW_ALWAYS},
    {"always_comb", TokenType::KW_ALWAYS_COMB},
    {"always_ff", TokenType::KW_ALWAYS_FF},
    {"always_latch", TokenType::KW_ALWAYS_LATCH},
    {"and", TokenType::KW_AND},
    {"assert", TokenType::KW_ASSERT},
    {"assign", TokenType::KW_ASSIGN},
    {"automatic", TokenType::KW_AUTOMATIC},
    {"begin", TokenType::KW_BEGIN},
    {"case", TokenType::KW_CASE},
    {"casex", TokenType::KW_CASEX},
    {"casez", TokenType::KW_CASEZ},
    {"class", TokenType::KW_CLASS},
    {"default", TokenType::KW_DEFAULT},
    {"defparam", TokenType::KW_DEFPARAM},
    {"disable", TokenType::KW_DISABLE},
    {"edge", TokenType::KW_EDGE},
    {"else", TokenType::KW_ELSE},
    {"end", TokenType::KW_END},
    {"endcase", TokenType::KW_ENDCASE},
    {"endclass", TokenType::KW_ENDCLASS},
    {"endfunction", TokenType::KW_ENDFUNCTION},
    {"endgenerate", TokenType::KW_ENDGENERATE},
    {"endmodule", TokenType::KW_ENDMODULE},
    {"endspecify", TokenType::KW_ENDSPECIFY},
    {"endtable", TokenType::KW_ENDTABLE},
    {"endtask", TokenType::KW_ENDTASK},
    {"event", TokenType::KW_EVENT},
    {"for", TokenType::KW_FOR},
    {"force", TokenType::KW_FORCE},
    {"forever", TokenType::KW_FOREVER},
    {"fork", TokenType::KW_FORK},
    {"function", TokenType::KW_FUNCTION},
    {"generate", TokenType::KW_GENERATE},
    {"genvar", TokenType::KW_GENVAR},
    {"if", TokenType::KW_IF},
    {"ifnone", TokenType::KW_IFNONE},
    {"import", TokenType::KW_IMPORT},
    {"initial", TokenType::KW_INITIAL},
    {"inout", TokenType::KW_INOUT},
    {"input", TokenType::KW_INPUT},
    {"instance", TokenType::KW_INSTANCE},
    {"integer", TokenType::KW_INTEGER},
    {"join", TokenType::KW_JOIN},
    {"localparam", TokenType::KW_LOCALPARAM},
    {"macromodule", TokenType::KW_MACROMODULE},
    {"module", TokenType::KW_MODULE},
    {"nand", TokenType::KW_NAND},
    {"negedge", TokenType::KW_NEGEDGE},
    {"nor", TokenType::KW_NOR},
    {"not", TokenType::KW_NOT},
    {"or", TokenType::KW_OR},
    {"output", TokenType::KW_OUTPUT},
    {"parameter", TokenType::KW_PARAMETER},
    {"posedge", TokenType::KW_POSEDGE},
    {"reg", TokenType::KW_REG},
    {"release", TokenType::KW_RELEASE},
    {"repeat", TokenType::KW_REPEAT},
    {"specify", TokenType::KW_SPECIFY},
    {"specparam", TokenType::KW_SPECPARAM},
    {"supply0", TokenType::KW_SUPPLY0},
    {"supply1", TokenType::KW_SUPPLY1},
    {"table", TokenType::KW_TABLE},
    {"task", TokenType::KW_TASK},
    {"time", TokenType::KW_TIME},
    {"tran", TokenType::KW_TRAN},
    {"tri", TokenType::KW_TRI},
    {"tri0", TokenType::KW_TRI0},
    {"tri1", TokenType::KW_TRI1},
    {"triand", TokenType::KW_TRIAND},
    {"trior", TokenType::KW_TRIOR},
    {"trireg", TokenType::KW_TRIREG},
    {"wait", TokenType::KW_WAIT},
    {"while", TokenType::KW_WHILE},
    {"wire", TokenType::KW_WIRE},
    {"xnor", TokenType::KW_XNOR},
    {"xor", TokenType::KW_XOR},
};

const QRegularExpression numberPrefixRegex(
    R"(^(?:(?:[0-9][0-9_]*)?'[sS]?[bB][01xXzZ_]+|(?:[0-9][0-9_]*)?'[sS]?[oO][0-7xXzZ_]+|(?:[0-9][0-9_]*)?'[sS]?[dD][0-9_]+|(?:[0-9][0-9_]*)?'[sS]?[hH][0-9a-fA-FxXzZ_]+|'[01xXzZ]|[0-9][0-9_]*))");

struct OperatorEntry
{
    const char *text;
    vtoken_t type;
};

const OperatorEntry operators[] = {
    {"...", ellipsis},
    {"<<=", shifteql},
    {">>=", shifteqr},
    {"**=", pow_equal},
    {"===", equality},
    {"!==", notequalto},
    {"&&", and_operator_symbol},
    {"||", bitwise_or},
    {"<=", lessthanorequalto},
    {">=", greaterthanorequalto},
    {"==", equality},
    {"!=", notequalto},
    {"<<", bitwise_shift_left},
    {">>", bitwise_shift_right},
    {"**", power},
    {"+=", plusequal},
    {"-=", minusequal},
    {"*=", mulequal},
    {"/=", divequal},
    {"%=", modequal},
    {"&=", andequal},
    {"^=", xorequal},
    {"|=", orequal},
    {"->", rarrow},
    {":=", colequal},
    {"~^", bitwise_xor},
    {"^~", bitwise_xor},
    {"+", Plus},
    {"-", Minus},
    {"*", multiply},
    {"/", divide},
    {"%", mod_operator},
    {"&", bitwise_and},
    {"|", bitwise_or},
    {"^", bitwise_xor},
    {"~", bitwise_not},
    {"!", not_operator_symbol},
    {"<", lessthan},
    {">", greaterthan},
    {"=", assignment},
    {",", comma},
    {".", dot},
    {":", colon},
    {";", semicolon},
    {"?", conditional_operator},
    {"@", at_operator},
    {"[", open_square},
    {"]", close_square},
    {"{", open_curly},
    {"}", close_curly},
    {"(", open_bracket},
    {")", closed_bracket},
    {"'", single_quotation},
    {"#", delay},
};

bool isIdentifierStart(QChar ch)
{
    const ushort value = ch.unicode();
    return ch == QLatin1Char('_') ||
           (value >= 'a' && value <= 'z') ||
           (value >= 'A' && value <= 'Z');
}

bool isIdentifierPart(QChar ch)
{
    const ushort value = ch.unicode();
    return isIdentifierStart(ch) || (value >= '0' && value <= '9');
}
}

const QString Reserved_functions[] = {
    "$display", "$write", "$strobe", "$monitor", "$fdisplay", "$fwrite",
    "$fstrobe", "$fmonitor", "$monitoron", "$monitoroff", "$timeformat",
    "$finish", "$stop", "$exit", "$fatal", "$error", "$warning", "$info",
    "$dumpfile", "$dumpvars", "$dumpon", "$dumpoff", "$dumpall", "$dumpflush",
    "$dumpports", "$dumpportsall", "$dumpportson", "$dumpportsoff",
    "$dumpportsflush", "$readmemh", "$readmemb", "$printtimescale",
    "$showscopes", "$showvars", "$assertcontrol", "$assertoff", "$asserton",
    "$assertkill", "$clog2", "$bits", "$countbits", "$countones", "$onehot",
    "$onehot0", "$isunknown", "$typename", "$cast", "$signed", "$unsigned",
    "$dimensions", "$unpacked_dimensions", "$left", "$right", "$low", "$high",
    "$increment", "$size", "$time", "$stime", "$realtime", "$timeunit",
    "$timeprecision", "$random", "$urandom", "$urandom_range", "$test_plusargs",
    "$value_plusargs", "$ln", "$log10", "$exp", "$sqrt", "$floor", "$ceil",
    "$sin", "$cos", "$tan", "$asin", "$acos", "$atan", "$atan2", "$hypot",
    "$sinh", "$cosh", "$tanh", "$asinh", "$acosh", "$atanh", "$bitstoreal",
    "$realtobits", "$shortrealtobits", "$bitstoshortreal", "$itor", "$rtoi",
    "$sformat", "$sformatf", "$swrite", "$sscanf", "$fopen", "$fclose",
    "$fflush", "$feof", "$ferror", "$fgetc", "$ungetc", "$fgets", "$fread",
    "$fseek", "$ftell", "$rewind", "$fscanf", "$displayb", "$displayh",
    "$displayo", "$writeb", "$writeh", "$writeo", "$strobeb", "$strobeh",
    "$strobeo", "$monitorb", "$monitorh", "$monitoro", "$fdisplayb",
    "$fdisplayh", "$fdisplayo", "$fwriteb", "$fwriteh", "$fwritero",
    "$fstrobeb", "$fstrobeh", "$fstrobeo", "$fmonitorb", "$fmonitorh",
    "$fmonitoro"
};

reserved_functions get_reserved_function(QString function_name)
{
    const int count = static_cast<int>(sizeof(Reserved_functions) /
                                       sizeof(Reserved_functions[0]));
    for (int i = 0; i < count; ++i)
    {
        if (Reserved_functions[i] == function_name)
        {
            return static_cast<reserved_functions>(i);
        }
    }
    return reserved_functions::SYS_NONE;
}

TokenType get_reserved_keyword(QString keyword_name)
{
    for (const KeywordEntry &entry : keywordEntries)
    {
        if (keyword_name == QLatin1String(entry.name))
        {
            return entry.type;
        }
    }
    return TokenType::KW_NONE;
}

verilog_token::verilog_token()
    : id(), res_key(TokenType::KW_NONE), Type(error),
      function(reserved_functions::SYS_NONE)
{
}

verilog_token::verilog_token(QString tokenText)
    : id(tokenText), res_key(TokenType::KW_NONE), Type(error),
      function(reserved_functions::SYS_NONE)
{
    if (identifier_regex.match(tokenText).hasMatch())
    {
        const TokenType keyword = get_reserved_keyword(tokenText);
        if (keyword != TokenType::KW_NONE)
        {
            Type = reserved_keyword;
            res_key = keyword;
        }
        else
        {
            Type = identifier;
        }
        return;
    }

    if (num_regex.match(tokenText).hasMatch())
    {
        Type = number;
        return;
    }

    function = get_reserved_function(tokenText);
    if (function != reserved_functions::SYS_NONE)
    {
        Type = reserve_function;
    }
}

verilog_token::verilog_token(TokenType keyword)
    : id(), res_key(keyword), Type(reserved_keyword),
      function(reserved_functions::SYS_NONE)
{
}

verilog_token::verilog_token(float num)
    : id(QString::number(num)), res_key(TokenType::KW_NONE), Type(number),
      function(reserved_functions::SYS_NONE)
{
}

verilog_token::verilog_token(vtoken_t tokenType)
    : id(), res_key(TokenType::KW_NONE), Type(tokenType),
      function(reserved_functions::SYS_NONE)
{
}

verilog_token::verilog_token(reserved_functions reservedFunction)
    : id(), res_key(TokenType::KW_NONE), Type(reserve_function),
      function(reservedFunction)
{
}

void verilog_token::setstring(QString value)
{
    id = value;
    Type = string_tkn;
}

QString verilog_token::getID(void)
{
    return id;
}

vtoken_t verilog_token::getType(void)
{
    return Type;
}

TokenType verilog_token::getReservedKeyword(void)
{
    return res_key;
}

reserved_functions verilog_token::getReservedFunction(void)
{
    return function;
}

QVector<verilog_token> verilog_token::fileparser(QString source)
{
    QVector<verilog_token> tokens;
    int position = 0;

    while (position < source.size())
    {
        const QChar ch = source.at(position);

        if (ch.isSpace())
        {
            ++position;
            continue;
        }

        if (ch == QLatin1Char('/') && position + 1 < source.size())
        {
            const QChar next = source.at(position + 1);
            if (next == QLatin1Char('/'))
            {
                position += 2;
                while (position < source.size() &&
                       source.at(position) != QLatin1Char('\n'))
                {
                    ++position;
                }
                continue;
            }
            if (next == QLatin1Char('*'))
            {
                position += 2;
                while (position + 1 < source.size() &&
                       !(source.at(position) == QLatin1Char('*') &&
                         source.at(position + 1) == QLatin1Char('/')))
                {
                    ++position;
                }
                if (position + 1 < source.size())
                {
                    position += 2;
                }
                else
                {
                    position = source.size();
                }
                continue;
            }
        }

        if (ch == QLatin1Char('"'))
        {
            const int start = position++;
            bool escaped = false;
            bool closed = false;
            while (position < source.size())
            {
                const QChar current = source.at(position++);
                if (escaped)
                {
                    escaped = false;
                }
                else if (current == QLatin1Char('\\'))
                {
                    escaped = true;
                }
                else if (current == QLatin1Char('"'))
                {
                    closed = true;
                    break;
                }
            }

            if (closed)
            {
                verilog_token token(string_tkn);
                token.setstring(source.mid(start, position - start));
                tokens.append(token);
            }
            else
            {
                tokens.append(verilog_token(error));
            }
            continue;
        }

        if (ch == QLatin1Char('$'))
        {
            const int start = position++;
            while (position < source.size() &&
                   isIdentifierPart(source.at(position)))
            {
                ++position;
            }
            tokens.append(verilog_token(source.mid(start, position - start)));
            continue;
        }

        if (isIdentifierStart(ch))
        {
            const int start = position++;
            while (position < source.size() &&
                   isIdentifierPart(source.at(position)))
            {
                ++position;
            }
            tokens.append(verilog_token(source.mid(start, position - start)));
            continue;
        }

        if (ch.isDigit() || ch == QLatin1Char('\''))
        {
            const QRegularExpressionMatch match =
                numberPrefixRegex.match(source.mid(position));
            if (match.hasMatch() && !match.captured().isEmpty())
            {
                tokens.append(verilog_token(match.captured()));
                position += match.capturedLength();
            }
            else
            {
                tokens.append(verilog_token(error));
                ++position;
            }
            continue;
        }

        bool matchedOperator = false;
        for (const OperatorEntry &entry : operators)
        {
            const QString spelling = QLatin1String(entry.text);
            if (source.mid(position).startsWith(spelling))
            {
                tokens.append(verilog_token(entry.type));
                position += spelling.size();
                matchedOperator = true;
                break;
            }
        }

        if (!matchedOperator)
        {
            tokens.append(verilog_token(error));
            ++position;
        }
    }

    return tokens;
}
