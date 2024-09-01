#include "syntax.highlighter.service.h"

SyntaxHighlightService::SyntaxHighlightService(std::string raw_code) {
    std::stringstream code_stream(raw_code);
    std::string code_line;
    while (getline(code_stream, code_line, '\n'))
        this->code_lines.push_back(code_line);
}

SyntaxHighlightService::~SyntaxHighlightService() {
}

void SyntaxHighlightService::ColorWrapper(std::string* token, std::string color) {
    *token = std::format("<font color = {}>{}</font>", color, *token);
};

std::string SyntaxHighlightService::Trim(std::string str) {
    size_t comment = str.find(";");
    if (comment != std::string::npos)
        str.erase(comment, str.size());
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

std::string SyntaxHighlightService::InitLexAnalyze() {
    this->CheckCodeValNames();
    this->CheckVarsNames();
    this->CheckCodeLex();

    std::string out_str;
    for (std::string code_line : this->code_lines) {
        std::string line = code_line;
        out_str.append(line).append("<br>");
    }
    return out_str;
}

void SyntaxHighlightService::CheckCodeValNames() {
    bool code_segment = false;
    for (auto code_line : this->code_lines) {
        if (code_segment && code_line.find(DATA_SEGMENT_FLAG) == std::string::npos) {
            std::string trimmed_str = this->Trim(code_line);
            if (std::regex_match(trimmed_str, std::regex(LABELS_PATTERN))) {
                std::string point_name;
                for (auto ch : trimmed_str) {
                    if (ch == ':')
                        this->names_list.push_back(point_name);
                    point_name.push_back(ch);
                }
            }
        } else
            code_segment = (code_line.find(TEXT_SEGMENT_FLAG) != std::string::npos) ||
                (code_line.find(CODE_SEGMENT_FLAG) != std::string::npos);
    }
}

void SyntaxHighlightService::CheckVarsNames() {
    bool data_segment = false;
    for (auto code_line : this->code_lines) {
        if (data_segment &&
            ((code_line.find(TEXT_SEGMENT_FLAG) == std::string::npos) &&
                (code_line.find(CODE_SEGMENT_FLAG) == std::string::npos))) {
            std::string val_name;
            std::string trimmed_str = this->Trim(code_line);
            for (auto ch : trimmed_str) {
                if (ch == ' ')
                    break;
                val_name.push_back(ch);
            }
            this->names_list.push_back(val_name);
        } else
            data_segment = code_line.find(DATA_SEGMENT_FLAG) != std::string::npos;
    }
}

std::string SyntaxHighlightService::TokenHandling(std::string token) {
    std::string upper_case = token;
    if (std::find(this->names_list.begin(), this->names_list.end(), upper_case) !=
        this->names_list.end()) {
        this->ColorWrapper(&token, "#E6DB00");
        return token;
    }
    std::transform(upper_case.begin(), upper_case.end(), upper_case.begin(), ::toupper);
    if (std::regex_match(upper_case, std::regex(STRING_PATTERN)) ||
        std::regex_match(upper_case, std::regex(DIGIT_PATTERN)))
        this->ColorWrapper(&token, "darkOrange");
    else if (std::find(commands_list.begin(), commands_list.end(), upper_case) !=
        commands_list.end())
        this->ColorWrapper(&token, "darkRed");
    else if (std::find(conditional_crossings_list.begin(),
                 conditional_crossings_list.end(),
                 upper_case) != conditional_crossings_list.end())
        this->ColorWrapper(&token, "darkRed");
    else if (std::find(registers_list.begin(), registers_list.end(), upper_case) !=
        registers_list.end())
        this->ColorWrapper(&token, "darkGreen");
    else if (std::find(registers_list.begin(), registers_list.end(), upper_case) !=
        registers_list.end())
        this->ColorWrapper(&token, "darkMagenta");
    else if (std::find(signs_list.begin(), signs_list.end(), upper_case) !=
        signs_list.end())
        this->ColorWrapper(&token, "darkBlue");
    else if (std::find(data_types_list.begin(), data_types_list.end(), upper_case) !=
        data_types_list.end())
        this->ColorWrapper(&token, "blue");
    else
        this->ColorWrapper(&token, "darkCyan");

    return token;
};

void SyntaxHighlightService::CheckCodeLex() {
    bool is_comment, comma_separated, dub_dots = false;
    std::string buffer;
    for (int i = 0; i < this->code_lines.size(); i++) {
        std::stringstream code_stream(this->code_lines[i]);
        std::string token, handled_string = "";
        is_comment = false;
        while (getline(code_stream, token, ' ')) {
            if (token.empty()) {
                handled_string.push_back(' ');
                continue;
            }

            size_t comment_index = token.find(';');
            if (comment_index != std::string::npos) {
                std::string token_part   = token.substr(0, comment_index);
                std::string comment_part = token.substr(comment_index);
                handled_string.append(this->TokenHandling(token_part));
                handled_string.append("<font color = gray>")
                    .append(comment_part)
                    .append("</font>");
                if (comment_index == token.length() - 1)
                    handled_string.push_back(' ');

                while (getline(code_stream, token, ' '))
                    buffer.append(token).append(" ");
                this->ColorWrapper(&buffer, "gray");
                handled_string.append(buffer);
                buffer.clear();
                continue;
            }

            std::string term = " ";
            if ((token.back() == ':') || (token.back() == ',')) {
                dub_dots = true;
                term     = std::format("{} ", token.back());
                token.pop_back();
            }
            token = this->TokenHandling(token);
            handled_string.append(token).append(term);
        }
        this->code_lines[i] = handled_string;
    }
}