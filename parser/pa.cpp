// Parser.h - Base interface for all parsers
#pragma once
#include <string>
#include <memory>

// Abstract base class for all parsers
class IParser {
public:
    virtual ~IParser() = default;
    virtual std::string parse(const std::string& content) = 0;
    virtual std::string getFileType() const = 0;
};

// =====================================================

// JsonParser.h
#pragma once
#include "Parser.h"
#include <nlohmann/json.hpp>

class JsonParser : public IParser {
public:
    std::string parse(const std::string& content) override;
    std::string getFileType() const override { return "JSON"; }
private:
    std::string formatJsonOutput(const nlohmann::json& j, int indent = 0);
};

// =====================================================

// CsvParser.h  
#pragma once
#include "Parser.h"
#include <rapidcsv.h>
#include <sstream>

class CsvParser : public IParser {
public:
    std::string parse(const std::string& content) override;
    std::string getFileType() const override { return "CSV"; }
private:
    std::string formatCsvOutput(const rapidcsv::Document& doc);
};

// =====================================================

// XmlParser.h
#pragma once
#include "Parser.h"
#include <tinyxml2.h>

class XmlParser : public IParser {
public:
    std::string parse(const std::string& content) override;
    std::string getFileType() const override { return "XML"; }
private:
    std::string formatXmlNode(tinyxml2::XMLNode* node, int indent = 0);
};

// =====================================================

// ParserHandler.h
#pragma once
#include "Parser.h"
#include <memory>
#include <fstream>
#include <iostream>

class ParserHandler {
public:
    ParserHandler();
    ~ParserHandler() = default;
    
    bool parseFile(const std::string& filePath);
    void printParsedData(const std::string& data, const std::string& fileType);
    
private:
    std::unique_ptr<IParser> createParser(const std::string& fileExtension);
    std::string getFileExtension(const std::string& filePath);
    std::string readFile(const std::string& filePath);
    bool fileExists(const std::string& filePath);
};

// =====================================================
// IMPLEMENTATION FILES
// =====================================================

// JsonParser.cpp
#include "JsonParser.h"
#include <iostream>
#include <iomanip>

std::string JsonParser::parse(const std::string& content) {
    try {
        nlohmann::json j = nlohmann::json::parse(content);
        return formatJsonOutput(j);
    } catch (const nlohmann::json::parse_error& e) {
        return "JSON Parse Error: " + std::string(e.what());
    } catch (const std::exception& e) {
        return "Error parsing JSON: " + std::string(e.what());
    }
}

std::string JsonParser::formatJsonOutput(const nlohmann::json& j, int indent) {
    std::ostringstream oss;
    std::string indentStr(indent * 2, ' ');
    
    if (j.is_object()) {
        oss << "{\n";
        bool first = true;
        for (auto& [key, value] : j.items()) {
            if (!first) oss << ",\n";
            oss << indentStr << "  \"" << key << "\": ";
            if (value.is_string()) {
                oss << "\"" << value.get<std::string>() << "\"";
            } else if (value.is_number()) {
                oss << value;
            } else if (value.is_boolean()) {
                oss << (value.get<bool>() ? "true" : "false");
            } else if (value.is_null()) {
                oss << "null";
            } else {
                oss << formatJsonOutput(value, indent + 1);
            }
            first = false;
        }
        oss << "\n" << indentStr << "}";
    } else if (j.is_array()) {
        oss << "[\n";
        bool first = true;
        for (const auto& item : j) {
            if (!first) oss << ",\n";
            oss << indentStr << "  " << formatJsonOutput(item, indent + 1);
            first = false;
        }
        oss << "\n" << indentStr << "]";
    } else if (j.is_string()) {
        oss << "\"" << j.get<std::string>() << "\"";
    } else {
        oss << j;
    }
    
    return oss.str();
}

// =====================================================

// CsvParser.cpp
#include "CsvParser.h"
#include <sstream>
#include <iomanip>

std::string CsvParser::parse(const std::string& content) {
    try {
        std::istringstream iss(content);
        rapidcsv::Document doc(iss);
        return formatCsvOutput(doc);
    } catch (const std::exception& e) {
        return "CSV Parse Error: " + std::string(e.what());
    }
}

std::string CsvParser::formatCsvOutput(const rapidcsv::Document& doc) {
    std::ostringstream oss;
    oss << "CSV Data Summary:\n";
    oss << "================\n";
    
    size_t rowCount = doc.GetRowCount();
    size_t colCount = doc.GetColumnCount();
    
    oss << "Rows: " << rowCount << ", Columns: " << colCount << "\n\n";
    
    // Print column headers if available
    std::vector<std::string> columnNames = doc.GetColumnNames();
    if (!columnNames.empty()) {
        oss << "Columns: ";
        for (size_t i = 0; i < columnNames.size(); ++i) {
            oss << columnNames[i];
            if (i < columnNames.size() - 1) oss << ", ";
        }
        oss << "\n\n";
    }
    
    // Print first few rows as sample
    size_t maxRowsToShow = std::min(rowCount, static_cast<size_t>(10));
    oss << "Sample Data (first " << maxRowsToShow << " rows):\n";
    oss << std::string(50, '-') << "\n";
    
    for (size_t row = 0; row < maxRowsToShow; ++row) {
        oss << "Row " << std::setw(3) << row + 1 << ": ";
        for (size_t col = 0; col < colCount && col < 5; ++col) { // Limit to 5 columns
            try {
                std::string cellValue = doc.GetCell<std::string>(col, row);
                oss << "[" << cellValue << "]";
                if (col < colCount - 1 && col < 4) oss << " | ";
            } catch (...) {
                oss << "[N/A]";
            }
        }
        if (colCount > 5) oss << " ... (+" << (colCount - 5) << " more cols)";
        oss << "\n";
    }
    
    if (rowCount > maxRowsToShow) {
        oss << "... (+" << (rowCount - maxRowsToShow) << " more rows)\n";
    }
    
    return oss.str();
}

// =====================================================

// XmlParser.cpp
#include "XmlParser.h"
#include <sstream>

std::string XmlParser::parse(const std::string& content) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.Parse(content.c_str());
    
    if (result != tinyxml2::XML_SUCCESS) {
        return "XML Parse Error: " + std::string(doc.ErrorStr());
    }
    
    std::ostringstream oss;
    oss << "XML Document Structure:\n";
    oss << "=====================\n";
    
    tinyxml2::XMLNode* root = doc.FirstChild();
    if (root) {
        oss << formatXmlNode(root);
    } else {
        oss << "No root element found.\n";
    }
    
    return oss.str();
}

std::string XmlParser::formatXmlNode(tinyxml2::XMLNode* node, int indent) {
    std::ostringstream oss;
    std::string indentStr(indent * 2, ' ');
    
    if (!node) return "";
    
    if (node->ToElement()) {
        tinyxml2::XMLElement* element = node->ToElement();
        oss << indentStr << "<" << element->Name();
        
        // Print attributes
        const tinyxml2::XMLAttribute* attr = element->FirstAttribute();
        while (attr) {
            oss << " " << attr->Name() << "=\"" << attr->Value() << "\"";
            attr = attr->Next();
        }
        oss << ">";
        
        // Check if element has text content
        const char* text = element->GetText();
        if (text && strlen(text) > 0) {
            oss << text;
        } else {
            oss << "\n";
        }
        
        // Process child nodes
        tinyxml2::XMLNode* child = element->FirstChild();
        bool hasChildElements = false;
        while (child) {
            if (child->ToElement()) {
                hasChildElements = true;
                oss << formatXmlNode(child, indent + 1);
            }
            child = child->NextSibling();
        }
        
        if (hasChildElements) {
            oss << indentStr;
        }
        oss << "</" << element->Name() << ">\n";
        
    } else if (node->ToText()) {
        std::string textContent = node->ToText()->Value();
        if (!textContent.empty()) {
            oss << indentStr << "TEXT: " << textContent << "\n";
        }
    } else if (node->ToComment()) {
        oss << indentStr << "<!-- " << node->ToComment()->Value() << " -->\n";
    }
    
    return oss.str();
}

// =====================================================

// ParserHandler.cpp
#include "ParserHandler.h"
#include "JsonParser.h"
#include "CsvParser.h"
#include "XmlParser.h"
#include <algorithm>
#include <filesystem>

ParserHandler::ParserHandler() {
    std::cout << "Parser Handler initialized.\n";
}

bool ParserHandler::parseFile(const std::string& filePath) {
    std::cout << "Processing file: " << filePath << std::endl;
    
    // Check if file exists
    if (!fileExists(filePath)) {
        std::cerr << "Error: File does not exist: " << filePath << std::endl;
        return false;
    }
    
    // Get file extension
    std::string extension = getFileExtension(filePath);
    if (extension.empty()) {
        std::cerr << "Error: Unable to determine file type from extension." << std::endl;
        return false;
    }
    
    // Create appropriate parser
    auto parser = createParser(extension);
    if (!parser) {
        std::cerr << "Error: Unsupported file type: " << extension << std::endl;
        return false;
    }
    
    // Read file content
    std::string content = readFile(filePath);
    if (content.empty()) {
        std::cerr << "Error: Unable to read file or file is empty." << std::endl;
        return false;
    }
    
    std::cout << "File read successfully. Size: " << content.length() << " bytes\n";
    std::cout << "Parser type: " << parser->getFileType() << "\n\n";
    
    // Parse content
    std::string parsedData = parser->parse(content);
    
    // Print parsed data
    printParsedData(parsedData, parser->getFileType());
    
    return true;
}

void ParserHandler::printParsedData(const std::string& data, const std::string& fileType) {
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "PARSED " << fileType << " DATA" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << data << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

std::unique_ptr<IParser> ParserHandler::createParser(const std::string& fileExtension) {
    std::string ext = fileExtension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == "json") {
        return std::make_unique<JsonParser>();
    } else if (ext == "csv") {
        return std::make_unique<CsvParser>();
    } else if (ext == "xml") {
        return std::make_unique<XmlParser>();
    }
    
    return nullptr;
}

std::string ParserHandler::getFileExtension(const std::string& filePath) {
    size_t lastDot = filePath.find_last_of('.');
    if (lastDot == std::string::npos) {
        return "";
    }
    return filePath.substr(lastDot + 1);
}

std::string ParserHandler::readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "";
    }
    
    std::ostringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

bool ParserHandler::fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

// =====================================================

// main.cpp - Example usage
#include "ParserHandler.h"
#include <iostream>
#include <vector>

int main() {
    ParserHandler handler;
    
    std::cout << "=== Multi-Format File Parser ===\n";
    std::cout << "Supported formats: JSON, CSV, XML\n\n";
    
    // Example usage - you can replace these with actual file paths
    std::vector<std::string> testFiles = {
        "example.json",
        "data.csv", 
        "config.xml"
    };
    
    std::cout << "Enter file path to parse (or 'quit' to exit): ";
    std::string filePath;
    
    while (std::getline(std::cin, filePath)) {
        if (filePath == "quit" || filePath == "q") {
            break;
        }
        
        if (!filePath.empty()) {
            std::cout << "\n";
            bool success = handler.parseFile(filePath);
            
            if (success) {
                std::cout << "\nFile parsed successfully!\n";
            } else {
                std::cout << "\nFailed to parse file.\n";
            }
        }
        
        std::cout << "\nEnter another file path (or 'quit' to exit): ";
    }
    
    std::cout << "Parser application terminated.\n";
    return 0;
}

// =====================================================

// CMakeLists.txt - Build configuration
/*
cmake_minimum_required(VERSION 3.12)
project(MultiFormatParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)

# Add executable
add_executable(parser 
    main.cpp
    ParserHandler.cpp
    JsonParser.cpp
    CsvParser.cpp
    XmlParser.cpp
)

# Link libraries
target_link_libraries(parser 
    nlohmann_json::nlohmann_json
    rapidcsv
    tinyxml2
)

# Include directories
target_include_directories(parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
*/