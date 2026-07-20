/** 
 * @file fileConverter.cpp
 * @brief A program to convert a CSV file into a custom ZCR (Zipcode Record) file.
 * 
 * This program reads zipcode record data from a CSV file and converts it into a custom ZCR file that includes a header and length indicated records.
 * 
 * @author Daniel Leone
 * @date 04/05/2024
 */

#include "headerBuffer.h"   ///< Include the HeaderBuffer class for handling the header buffer
#include "headerRecord.h"   ///< Include the HeaderRecord class for handling the header record
#include "buffer.h"         ///< Include the Buffer class for handling the data buffer
#include "zipcode.h"        ///< Include the Zipcode class for handling zipcode data
#include <fstream>          ///< Include the fstream library for file handling operations
#include <sstream>          ///< Include the sstream library for string stream operations
#include <iostream>         ///< Include the iostream library for input/output operations

/**
 * @brief Open a file with error checking.
 * @param filename The name of the file to open.
 * @param mode The mode in which to open the file.
 * @return The opened file stream.
 */
std::fstream openFile(const std::string& filename, std::ios_base::openmode mode);

/**
 * @brief Write a default header to the .zcr file.
 * @param zcrFile The .zcr file to write to.
 */
void writeDefaultHeader(std::fstream& zcrFile);

/**
 * @brief Read the .csv and write records to .zcr.
 * @param csvFileName The name of the .csv file to read from.
 * @param zcrFile The .zcr file to write to.
 * @return The number of records written.
 */
int readAndWriteRecords(const std::string& csvFileName, std::fstream& zcrFile);

/**
 * @brief Update the header of the .zcr file.
 * @param zcrFile The .zcr file to update.
 * @param recordCount The number of records to set in the header.
 */
void updateHeader(std::fstream& zcrFile, int recordCount);

/**
 * @brief The main function of the program.
 * @return 0 if the program executed successfully, and 1 otherwise.
 */
int main() {
    std::string csvFileName;
    std::cout << "Enter the name of the CSV file: ";
    std::cin >> csvFileName;

    std::cout << "Program began..." << std::endl;

    // Step 1: Open CSV from user input
    // Check if the input file has a .csv extension
    size_t dotPos = csvFileName.rfind(".");
    if (dotPos == std::string::npos || csvFileName.substr(dotPos) != ".csv") {
        std::cerr << "Error: Input file is not a CSV file." << std::endl;
        return 1;
    }
    // Open the CSV file for reading
    std::fstream csvFile = openFile(csvFileName, std::ios::in);
    std::cout << "Step 1: File < " + csvFileName + " > opened successfully" << std::endl;

    // Step 2: Open ZCR from user input, Create if file does not exist
    std::string zcrFileName = csvFileName.substr(0, csvFileName.rfind(".")) + ".zcr";
    std::fstream zcrFile = openFile(zcrFileName, std::ios::out | std::ios::out);
    std::cout << "Step 2: File < " + zcrFileName + " > opened successfully" << std::endl;

    // Step 3: Write a default header to the .zcr file
    writeDefaultHeader(zcrFile);
    std::cout << "Step 3: Default header written to new .zcr" << std::endl;

    // Step 4: Read the .csv and write records to .zcr
    int recordCount = readAndWriteRecords(csvFileName, zcrFile);
    std::cout << "Step 4: Records from .cvs written to new .zcr" << std::endl;

    // Step 5: Update header
    updateHeader(zcrFile, recordCount);
    std::cout << "Step 5: Header updated for new .zcr" << std::endl;

    csvFile.close();
    zcrFile.close();

    std::cout << "File conversion complete..." << std::endl;

    return 0;
}

// Function to open a file with error checking
std::fstream openFile(const std::string& filename, std::ios_base::openmode mode) {
    std::fstream file(filename, mode);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }
    return file;
}

// Function to write a default header to the .zcr file
void writeDefaultHeader(std::fstream& zcrFile) {
    HeaderBuffer hb;
    HeaderRecord hr;
    hr.setHeaderRecordSize(hb.getMaxHeaderSize());
    hb.WriteHeader(hr, zcrFile);
}

// Function to read the .csv and write records to .zcr
int readAndWriteRecords(const std::string& csvFileName, std::fstream& zcrFile) {
    Buffer buffer(csvFileName);
    // Read each line from the file
    Zipcode zipcode;
    int count = 0;
    while ((zipcode = buffer.Read()).getZipcode() != 0) {
        // Pack the Zipcode object into a string
        std::string record = buffer.Pack(zipcode);
        // Write the length of the record and the record itself to the output file
        zcrFile << record.size() << " " << record << std::endl;
        count++;
    }
    return count;
}

// Function to update the header
void updateHeader(std::fstream& zcrFile, int recordCount) {
    HeaderBuffer hb;
    HeaderRecord hr;
    hr.setRecordCount(recordCount);

    // Reset the ZCR file to the top
    zcrFile.clear(); // Clear any error flags
    zcrFile.seekp(0, std::ios::beg); // Move the file pointer to the beginning

    // Write the updated header to the ZCR file
    hb.WriteHeader(hr, zcrFile);
}