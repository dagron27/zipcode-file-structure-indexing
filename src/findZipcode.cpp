/** 
 * @file findZipcode.cpp
 * @brief A program to find and display zip codes from a ZCR file.
 * 
 * This program reads zipcode record data from a custom ZCR file that includes a header and the records are length indicated.
 * An index file (IDX) is generated and utilized to quickly locate specific records by zipocde.
 * The record is located based on user input from the command line with the -Z flag. The record is displayed if found, or a 
 * message is displayed stating the record is not present. Multiple records can be accessed at one execution of the program.
 * 
 * @author Daniel Leone
 * @date 04/08/2024
 */

#include "headerBuffer.h"    ///< Include the HeaderBuffer class for handling the header buffer
#include "headerRecord.h"    ///< Include the HeaderRecord class for handling the header record
#include "buffer2.h"         ///< Include the Buffer2 class for handling the data buffer
#include "zipcode2.h"        ///< Include the Zipcode2 class for handling zipcode data
#include "primaryKeyIndex.h" ///< Include the PrimaryKeyIndex class for handling the primary key index
#include <iostream>          ///< Include the iostream library for input/output operations
#include <fstream>           ///< Include the fstream library for file handling operations
#include <cstring>           ///< Include the cstring library for string handling functions
#include <cerrno>            ///< Include the cerrno library for error handling

/**
 * @brief Initialize the program by reading the header, updating the field order, calculating the record count, writing back the header, and creating the index file.
 * @param dataFile The data file to be read and written.
 * @param hb The HeaderBuffer object.
 * @param buffer The Buffer2 object.
 * @param index The PrimaryKeyIndex object.
 * @param header The HeaderRecord object.
 */
void InitializeProgram(std::fstream& dataFile, HeaderBuffer& hb, Buffer2& buffer, PrimaryKeyIndex& index, HeaderRecord& header);

/**
 * @brief Process the command line arguments or user input to search for a record.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @param dataFile The data file to be read.
 * @param index The PrimaryKeyIndex object.
 * @param buffer The Buffer2 object.
 * @return void
 */
void processArguments(int argc, char* argv[], std::fstream& dataFile, PrimaryKeyIndex& index, Buffer2& buffer);

/**
 * @brief The main function of the program.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return 0 if the program executed successfully, and 1 otherwise.
 */
int main(int argc, char* argv[]) {
    // Open the data file
    std::fstream dataFile("data/us_postal_codes.zcr", std::ios::in | std::ios::out);
    if (!dataFile.is_open()) {
        std::cerr << "Failed to open file: data/us_postal_codes.zcr" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;  // Print the error message
        return 1;
    }

    // Initialize the buffer, header buffer, and primary key index
    HeaderBuffer hb;
    HeaderRecord header;
    Buffer2 buffer;
    PrimaryKeyIndex index;

    InitializeProgram(dataFile, hb, buffer, index, header);
    // Open the index file. header.getIndexFileName() is a bare filename
    // (e.g. "us_postal_codes.idx") persisted inside the .zcr's own header
    // record, not a path -- the "data/" prefix here matches where this
    // codebase's data files live, mirroring the fix in
    // PrimaryKeyIndex::createIndex (primaryKeyIndex.cpp).
    std::string indexFilePath = "data/" + header.getIndexFileName();
    std::fstream indexFile(indexFilePath, std::ios::in | std::ios::out);
    if (!indexFile.is_open()) {
        std::cerr << "Failed to open index file: " << indexFilePath << std::endl;
        return 1;
    }
    index.readIndexFromFile(indexFile);

    processArguments(argc, argv, dataFile, index, buffer);

    return 0;
}

void InitializeProgram(std::fstream& dataFile, HeaderBuffer& hb, Buffer2& buffer, PrimaryKeyIndex& index, HeaderRecord& header) {
    // Step 1: Read the file header
    if (!hb.ReadHeader(header, dataFile)) {
        std::cerr << "Invalid header in ZRC file" << std::endl;
        return;
    }
    // Step 2: Provide field order to the buffer class
    buffer.UpdateFieldOrder(header.getFieldOrder());

    // Step 3: Calculate the record count
    int recordCount = buffer.CalculateRecordCount(dataFile);
    header.setRecordCount(recordCount);

    // Step 4: Write back the header to the file
    dataFile.clear();
    dataFile.seekg(0, std::ios::beg);
    if (hb.WriteHeader(header, dataFile) == -1) {
        std::cerr << "Failed to write header back to ZRC file" << std::endl;
        return;
    }

    // Step 5: Create the index file
    dataFile.clear();
    dataFile.seekg(0, std::ios::beg);
    if (!index.createIndex(dataFile)) {
        std::cerr << "Failed to create index file" << std::endl;
        return;
    }
}

int checkRecord(std::fstream& dataFile, PrimaryKeyIndex& index, Buffer2& buffer, int zipcode) {
	// Search for the offset of the record with the given zipcode
	int offset = index.searchIndex(zipcode);
	if (offset == -1) {
		return 2;
	}

	// Read the record at the found offset
	Zipcode2 zc = buffer.DRead(dataFile, zipcode, offset);
	if (zc.getZipcode() != zipcode) {
		return 0;
	}

	// If the function hasn't returned yet, the search was successful
	zc.Print();
	return 1;
}

void processArguments(int argc, char* argv[], std::fstream& dataFile, PrimaryKeyIndex& index, Buffer2& buffer) {
    if (argc > 1) {
        // Loop through the command line arguments
        for (int i = 1; i < argc; ++i) {
            // Check if the argument starts with '-Z'
            if (strncmp(argv[i], "-Z", 2) == 0) {
                // Convert the rest of the argument to an integer
                int zipcode = std::stoi(argv[i] + 2);
                // Search for the record
                int result = checkRecord(dataFile, index, buffer, zipcode);
                if (result == 1) {
                    std::cout << std::endl;
                } else if (result == 2) {
                    std::cout << "No record found for zipcode: " << zipcode << std::endl << std::endl;
                } else {
                    std::cout << "Error occurred while searching for zipcode: " << zipcode << std::endl << std::endl;
                }
            }
        }
    } else {
        // No command line arguments provided, take user input
        std::cout << "Please enter a zipcode: ";
        int zipcode;
        std::cin >> zipcode;
        // Search for the record
        int result = checkRecord(dataFile, index, buffer, zipcode);
        if (result == 1) {
            std::cout << std::endl;
        } else if (result == 2) {
            std::cout << "No record found for zipcode: " << zipcode << std::endl << std::endl;
        } else {
            std::cout << "Error occurred while searching for zipcode: " << zipcode << std::endl << std::endl;
        }
    }
}
