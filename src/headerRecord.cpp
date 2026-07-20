/**
 * @file headerRecord.cpp
 * @brief This file contains the implementation of the HeaderRecord class.
 */

#include "headerRecord.h" ///< Include the header file for the HeaderRecord class
#include <sstream>        ///< Include the sstream library for string stream operations
#include <iostream>       ///< Include the iostream library for input/output stream operations
#include <iomanip>        ///< Include the iomanip library for input/output manipulations

/**
 * @brief Constructor for HeaderRecord.
 * @param numFields The number of fields in the record. Default is 6.
 */
HeaderRecord::HeaderRecord(int numFields) {
    fileStructureType = "ZCR";
    version = "1.0";
    headerRecordSize = 450; // This will be the size of the entire header in bytes
    recordSizeInteger = -1; // This will be the number of bytes for each record size integer
    sizeFormatType = "ASCII";
    indexFileName = "primaryKey.idx"; // This will be the name of the index file
    indexFileSchemaInfo = "Key, Offset"; // This will be the information on how to read the index file format
    recordCount = 0; // This will be the total number of records in the file
    fieldsPerRecord = numFields;

    // Define all possible fields
    std::vector<std::pair<std::string, std::string>> allFields = {
        {"Zip Code", "Int"},
        {"Place Name", "String"},
        {"State", "String"},
        {"County", "String"},
        {"Lat", "Double"},
        {"Long", "Double"}
    };

	if(numFields == 6){
		// Add fields up to numFields
		for (int i = 0; i < numFields; ++i) 
		{
			fields.push_back(allFields[i]);		
		}
	}
	else{
		// Add fields up to 
		for (int i = 0; i < numFields; ++i) {
			// Add a blank pair
			fields.push_back(std::make_pair("", ""));
		}
    }
	primaryKey = "Zip Code";
}

/**
 * @brief Set the file structure type.
 * @param value The file structure type as a string.
 */
void HeaderRecord::setFileStructureType(const std::string& value) {
    fileStructureType = value;
}

/**
 * @brief Set the version of the file.
 * @param value The version as a string.
 */
void HeaderRecord::setVersion(const std::string& value) {
    version = value;
}

/**
 * @brief Set the size of the header record.
 * @param value The size of the header record as an integer.
 */
void HeaderRecord::setHeaderRecordSize(int value) {
    headerRecordSize = value;
}

/**
 * @brief Set the size of the record as an integer.
 * @param value The size of the record as an integer.
 */
void HeaderRecord::setRecordSizeInteger(int value) {
    recordSizeInteger = value;
}

/**
 * @brief Set the format type of the size.
 * @param value The format type of the size as a string.
 */
void HeaderRecord::setSizeFormatType(const std::string& value) {
    sizeFormatType = value;
}

/**
 * @brief Set the name of the index file.
 * @param value The name of the index file as a string.
 */
void HeaderRecord::setIndexFileName(const std::string& value) {
    indexFileName = value;
}

/**
 * @brief Set the schema information of the index file.
 * @param value The schema information of the index file as a string.
 */
void HeaderRecord::setIndexFileSchemaInfo(const std::string& value) {
    indexFileSchemaInfo = value;
}

/**
 * @brief Set the count of the records.
 * @param value The count of the records as an integer.
 */
void HeaderRecord::setRecordCount(int value) {
    recordCount = value;
}

/**
 * @brief Set the number of fields per record.
 * @param value The number of fields per record as an integer.
 */
void HeaderRecord::setFieldsPerRecord(int value) {
    fieldsPerRecord = value;
}

/**
 * @brief Set a field in the record.
 * @param index The index of the field.
 * @param fieldName The name of the field.
 * @param fieldType The type of the field.
 */
void HeaderRecord::setField(int index, const std::string& fieldName, const std::string& fieldType) {
    // Check if the index is valid
    if (index >= 0 && index < fields.size()) {
        // Set the field name and type
        fields[index] = std::make_pair(fieldName, fieldType);
    } else {
        std::cout << "Invalid index. Please provide an index between 0 and " << fields.size()-1 << ".\n";
    }
}

/**
 * @brief Set the primary key of the record.
 * @param value The primary key of the record as a string.
 */
void HeaderRecord::setPrimaryKey(const std::string& value) {
    primaryKey = value;
}

/**
 * @brief Get the type of a specific field.
 * @param fieldName The name of the field.
 * @return The type of the field as a string.
 */
std::string HeaderRecord::getFieldType(const std::string& fieldName) const {
    for (const auto& field : fields) {
        if (field.first == fieldName) {
            return field.second;
        }
    }
    return "";  // Return an empty string if the field is not found
}

/**
 * @brief Pack the header record into a string.
 * @return The packed header record as a string.
 */
std::string HeaderRecord::Pack() {
    std::ostringstream oss;
    oss << "File Structure Type: " << fileStructureType << ";\n";
    oss << "Version: " << version << ";\n";
    oss << "Header Record Size: " << headerRecordSize << ";\n";
    oss << "Record Size Integer Bytes: " << recordSizeInteger << ";\n";
    oss << "Size Format Type: " << sizeFormatType << ";\n";
    oss << "Index File Name: " << indexFileName << ";\n";
    oss << "Index File Schema: " << indexFileSchemaInfo << ";\n";
    oss << "Record Count: " << recordCount << ";\n";
    oss << "Fields Per Record: " << fieldsPerRecord << ";\n";
    for (int i = 0; i < fields.size(); i++) {
        oss << "Field " << i+1 << ": " << fields[i].first << ", " << fields[i].second << ";\n";
    }
    oss << "Primary Key: " << primaryKey << ";";

    return oss.str();
}

/**
 * @brief Unpack a string into a header record.
 * @param str The string to unpack.
 */
void HeaderRecord::Unpack(std::string str) {
	
    std::istringstream iss(str);
    std::string token;
	std::string tmp;

    std::getline(iss, token, ':'); // Ignore "File Structure Type:"
	iss.ignore(); // Ignore the space after the colon
    std::getline(iss, fileStructureType, ';'); // Read the file structure type

    std::getline(iss, token, ':'); // Ignore "Version:"
    iss.ignore();
    std::getline(iss, version, ';'); // Read the version

    std::getline(iss, token, ':'); // Ignore "Header Record Size:"
    iss.ignore();
    std::getline(iss, tmp, ';'); // Read the header record size
	headerRecordSize = std::stoi(tmp); // Convert string to int

	std::getline(iss, token, ':'); // Ignore "Record Size Integer Bytes:"
	iss.ignore();
    std::getline(iss, tmp, ';'); // Read the record size integer
	recordSizeInteger = std::stoi(tmp); // Convert string to int

	std::getline(iss, token, ':'); // Ignore "Size Format Type:"
	iss.ignore();
    std::getline(iss, sizeFormatType, ';'); // Read the size format type

	std::getline(iss, token, ':'); // Ignore "Index File Name:"
	iss.ignore();
    std::getline(iss, indexFileName, ';'); // Read the index file name

	std::getline(iss, token, ':'); // Ignore "Index File Schema:"
	iss.ignore();
    std::getline(iss, indexFileSchemaInfo, ';'); // Read the index file schema

	std::getline(iss, token, ':'); // Ignore "Record Count:"
	iss.ignore();
    std::getline(iss, tmp, ';'); // Read the record count
	recordCount = std::stoi(tmp); // Convert string to int

	std::getline(iss, token, ':'); // Ignore "Fields Per Record:"
	iss.ignore();
    std::getline(iss, tmp, ';'); // Read the fields per record
	fieldsPerRecord = std::stoi(tmp); // Convert string to int

    for (int i = 0; i < fieldsPerRecord; i++) {
        std::getline(iss, token, ':'); // Ignore "Field i:"
        iss.ignore();
        std::string fieldName, fieldType;
        std::getline(iss, fieldName, ','); // Read the field name
        iss.ignore();
        std::getline(iss, fieldType, ';'); // Read the field type
        // Bounds check: fieldsPerRecord is read from file content and may
        // exceed the number of slots allocated in 'fields'. Skip storing
        // (but still consume the stream tokens above) instead of writing
        // out of bounds.
        if (i < static_cast<int>(fields.size())) {
            fields[i].first = fieldName;
            fields[i].second = fieldType;
        } else {
            std::cerr << "Warning: Fields Per Record (" << fieldsPerRecord
                       << ") exceeds allocated field slots (" << fields.size()
                       << "). Ignoring extra field '" << fieldName << "'.\n";
        }
    }

	std::getline(iss, token, ':'); // Ignore "Primary Key:"
	iss.ignore();
    std::getline(iss, primaryKey, ';'); // Read the primary key
}

/**
 * @brief Print the header record.
 */
void HeaderRecord::Print() {
    std::cout << "File Structure Type: " << fileStructureType << "\n";
    std::cout << "Version: " << version << "\n";
    std::cout << "Header Record Size: " << headerRecordSize << "\n";
    std::cout << "Record Size Integer Bytes: " << recordSizeInteger << "\n";
    std::cout << "Size Format Type: " << sizeFormatType << "\n";
    std::cout << "Index File Name: " << indexFileName << "\n";
    std::cout << "Index File Schema: " << indexFileSchemaInfo << "\n";
    std::cout << "Record Count: " << recordCount << "\n";
    std::cout << "Fields Per Record: " << fieldsPerRecord << "\n";
    for (int i = 0; i < fields.size(); i++) {
        std::cout << "Field " << i+1 << ": " << fields[i].first << ", " << fields[i].second << "\n";
    }
    std::cout << "Primary Key: " << primaryKey << "\n";
}

/**
 * @brief Get the order of the fields in the record.
 * @return A vector of strings representing the field order.
 */
std::vector<std::string> HeaderRecord::getFieldOrder() const {
    std::vector<std::string> fieldOrder;
    for (const auto& field : fields) {
        fieldOrder.push_back(field.first);
    }
    return fieldOrder;
}