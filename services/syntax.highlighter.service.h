#ifndef SYNTAX_HIGHLIGHT_SERVICE_H
#define SYNTAX_HIGHLIGHT_SERVICE_H

#define CODE_SEGMENT_FLAG ".code"
#define TEXT_SEGMENT_FLAG ".text"
#define DATA_SEGMENT_FLAG ".data"
#define LABELS_PATTERN "^[\\S][^:]+:(.*)"
#define STRING_PATTERN "^'[\\w|\\W]+'$"
#define DIGIT_PATTERN "^\\d*\\.?\\d*$"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "../bases/lexical/commands.h"
#include "../bases/lexical/conditional.crossings.h"
#include "../bases/lexical/data.types.h"
#include "../bases/lexical/interruptions.h"
#include "../bases/lexical/registers.h"
#include "../bases/lexical/signs.h"



class SyntaxHighlightService {
    public:
    SyntaxHighlightService(std::string raw_code);
    ~SyntaxHighlightService();

    std::string InitLexAnalyze();

    private:
    std::vector<std::string> code_lines;
    std::vector<std::string> names_list;

    std::string Trim(std::string str);
    std::string TokenHandling(std::string token);
    void ColorWrapper(std::string* token, std::string color);
    void CheckCodeValNames();
    void CheckVarsNames();
    void CheckCodeLex();
};

#endif