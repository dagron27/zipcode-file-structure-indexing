/**
 * @file headerRecord.h
 * @brief This file contains the declaration of the HeaderRecord class.
 * @class HeaderRecord
 * @brief A class to contain and process header information for a ZRC and IDX file.
 * @details The HeaderRecord class represents a header record in a file. It provides methods to pack and unpack the data into a string for storage, as well as getter and setter methods for each data field.
 */

#ifndef HEADER_RECORD_H
#define HEADER_RECORD_H

#include <string>    ///< Include the string library for string manipulation
#include <vector>    ///< Include the vector library for dynamic array operations

class HeaderRecord {
	
public:

   /**
     * @brief Default constructor for HeaderRecord.
     * @param numFields The number of fields in the record. Default is 6.
     */
    HeaderRecord(int numFields = 6);

    // Getter methods
	/**
     * @brief Get the file structure type.
     * @return The file structure type as a string.
     */
    std::string getFileStructureType() const { return fileStructureType; };
    /**
     * @brief Get the version of the file.
     * @return The version as a string.
     */
    std::string getVersion() const { return version; };

    /**
     * @brief Get the size of the header record.
     * @return The size of the header record as an integer.
     */
    int getHeaderRecordSize() const { return headerRecordSize; };

    /**
     * @brief Get the size of the record as an integer.
     * @return The size of the record as an integer.
     */
    int getRecordSizeInteger() const { return recordSizeInteger; };

    /**
     * @brief Get the format type of the size.
     * @return The format type of the size as a string.
     */
    std::string getSizeFormatType() const { return sizeFormatType; };

    /**
     * @brief Get the name of the index file.
     * @return The name of the index file as a string.
     */
    std::string getIndexFileName() const { return indexFileName; };

    /**
     * @brief Get the schema information of the index file.
     * @return The schema information of the index file as a string.
     */
    std::string getIndexFileSchemaInfo() const { return indexFileSchemaInfo; };

    /**
     * @brief Get the count of the records.
     * @return The count of the records as an integer.
     */
    int getRecordCount() const { return recordCount; };

    /**
     * @brief Get the number of fields per record.
     * @return The number of fields per record as an integer.
     */
    int getFieldsPerRecord() const { return fieldsPerRecord; };

    /**
     * @brief Get the type of a specific field.
     * @param fieldName The name of the field.
     * @return The type of the field as a string.
     */
    std::string getFieldType(const std::string& fieldName) const;

    /**
     * @brief Get the primary key of the record.
     * @return The primary key of the record as a string.
     */
    std::string getPrimaryKey() const { return primaryKey; };


	// Setter methods for each private member
	/**
     * @brief Set the file structure type.
     * @param value The file structure type as a string.
     */
	void setFileStructureType(const std::string& value);
    /**
     * @brief Set the version of the file.
     * @param value The version as a string.
     */
    void setVersion(const std::string& value);

    /**
     * @brief Set the size of the header record.
     * @param value The size of the header record as an integer.
     */
    void setHeaderRecordSize(int value);

    /**
     * @brief Set the size of the record as an integer.
     * @param value The size of the record as an integer.
     */
    void setRecordSizeInteger(int value);

    /**
     * @brief Set the format type of the size.
     * @param value The format type of the size as a string.
     */
    void setSizeFormatType(const std::string& value);

    /**
     * @brief Set the name of the index file.
     * @param value The name of the index file as a string.
     */
    void setIndexFileName(const std::string& value);

    /**
     * @brief Set the schema information of the index file.
     * @param value The schema information of the index file as a string.
     */
    void setIndexFileSchemaInfo(const std::string& value);

    /**
     * @brief Set the count of the records.
     * @param value The count of the records as an integer.
     */
    void setRecordCount(int value);

    /**
     * @brief Set the number of fields per record.
     * @param value The number of fields per record as an integer.
     */
    void setFieldsPerRecord(int value);

    /**
     * @brief Set a field in the record.
     * @param index The index of the field.
     * @param fieldName The name of the field.
     * @param fieldType The type of the field.
     */
    void setField(int index, const std::string& fieldName, const std::string& fieldType);

    /**
     * @brief Set the primary key of the record.
     * @param value The primary key of the record as a string.
     */
    void setPrimaryKey(const std::string& value);

	/**
     * @brief Pack the header record into a string.
     * @return The packed header record as a string.
     */
	std::string Pack();

    /**
     * @brief Unpack a string into a header record.
     * @param str The string to unpack.
     */
    void Unpack(std::string str);

    /**
     * @brief Print the header record.
     */
	void Print();
	
	/**
     * @brief Get the order of the fields in the record.
     * @return A vector of strings representing the field order.
     */
	std::vector<std::string> getFieldOrder() const;
	
private:
    std::string fileStructureType; ///< The file structure type.
    std::string version; ///< The version.
    int headerRecordSize; ///< The header record size.
    int recordSizeInteger; ///< The record size as an integer.
    std::string sizeFormatType; ///< The size format type.
    std::string indexFileName; ///< The index file name.
    std::string indexFileSchemaInfo; ///< The index file schema information.
    int recordCount; ///< The record count.
    int fieldsPerRecord; ///< The number of fields per record.
	std::vector<std::pair<std::string, std::string>> fields; ///< The fields.
	std::string primaryKey; ///< The primary key.
};

#endif // HEADER_RECORD_H