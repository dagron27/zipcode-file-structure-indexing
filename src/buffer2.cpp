/**
 * @file buffer2.cpp
 * @brief This file contains the implementation of the Buffer2 class. It is used for project 4.
 */

#include "buffer2.h" ///< Include the Buffer2 class
#include <iostream> ///< Include the iostream library for input/output operations
#include <vector> ///< Include the vector library for dynamic array operations
#include <sstream>  ///< Include the sstream library for string stream operations

/**
 * @brief Construct a new Buffer2 object.
 * 
 * Initializes the indices for the input and output buffers to 0.
 */
Buffer2::Buffer2() : ibufferIndex(0), obufferIndex(0) {
    fieldOrder = {"Zip Code", "Place Name", "State", "County", "Lat", "Long"};
}

/**
 * @brief Set the Record Count.
 * 
 * Sets the record count to the specified value.
 * @param count The new record count.
 */
void Buffer2::setRecordCount(int count){
	recordCount = count;
}

/**
 * @brief Write a Zipcode2 object to a file.
 * 
 * Writes the given Zipcode2 object to the specified output file.
 * @param zipcode The Zipcode2 object to write.
 * @param outFile The output file stream.
 * @return int The number of characters written. Returns -1 if the file is not open or the record format is invalid.
 */
int Buffer2::Write(Zipcode2& zipcode, std::fstream& outFile) {
    // If the file is not open, print an error message and return -1
    if (!outFile.is_open()) {
        std::cerr << "File is not open. Cannot write to file." << std::endl;
        return -1;
    }
	
    // Pack the Zipcode2 object into a string
    std::string packed = Pack(zipcode);
	
    // If the packed string does not start with a digit, skip it and return -1
    if (!std::isdigit(packed[0])) {
        std::cerr << "Invalid record format. Skipping record." << std::endl;
        return -1;
    }
	
    // Add the packed string to the output buffer
    obuffer.push_back(packed);
    // If the output buffer is full, write it to the file
    if (obuffer.size() >= BUFFER_SIZE) {
        for (const auto& record : obuffer) {
            outFile << record << std::endl;
        }
        // Clear the output buffer
        obuffer.clear();
    }
    // Flush the file stream to ensure all buffered data is written
    outFile.flush();
	
    // If we've reached this point, the write operation was successful, so return 0
    return 0;
}

/**
 * @brief Read a Zipcode2 object from a file.
 * 
 * Reads a Zipcode2 object from the specified input file. If the buffer is empty or has been fully read, it refills the buffer.
 * @param inFile The input file stream.
 * @param offset The offset in the file to start reading from. Default is -1, which means read from the current position.
 * @return Zipcode2 The Zipcode2 object read from the file. Returns an empty Zipcode2 object if the file is not open or an invalid argument is encountered.
 */
Zipcode2 Buffer2::Read(std::fstream& inFile, int offset) {
    // If the file is not open, print an error message and return
    if (!inFile.is_open()) {
        std::cerr << "File is not open. Cannot read from file." << std::endl;
        return Zipcode2();
    }

    // If the buffer is empty or has been fully read, refill it
    if (ibufferIndex >= ibuffer.size()) {
		ibuffer.clear();
        ibufferIndex = 0;
        std::string line;

        // Read lines from the file until the buffer is full or the end of the file is reached
        while (ibuffer.size() < BUFFER_SIZE && std::getline(inFile, line)) {
            // Skip lines that do not start with a digit
            if (!std::isdigit(line[0])) {
                continue;
            }
			ibuffer.push_back(line);
            line.clear();
        }
    }

    // If the buffer is still empty after trying to refill it, we've reached the end of the file
    if (ibuffer.empty()) {
        return Zipcode2();
    }

    // Process the next line in the buffer
    std::string process_line = ibuffer[ibufferIndex];
    size_t space_pos = process_line.find(' ');

    try {
        // Extract the record length from the beginning of the line to the space
        lastRecordLength = std::stoi(process_line.substr(0, space_pos));

        // Ensure the line is the correct length
        if (process_line.length() >= lastRecordLength) {
            process_line = process_line.substr(space_pos + 1, lastRecordLength);  // Skip the space
        }

        Zipcode2 tmp;
        tmp = Unpack(process_line);
        ibufferIndex++;

        return tmp;
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << '\n';
        // Handle the exception
        return Zipcode2();
    }
}

/**
 * @brief Read a Zipcode2 object from a file using a key.
 * 
 * Reads a Zipcode2 object from the specified input file using a key. If the key does not match the zipcode read from the file, it returns an empty Zipcode2 object.
 * @param inFile The input file stream.
 * @param key The key to use for reading.
 * @param offset The offset in the file to start reading from.
 * @return Zipcode2 The Zipcode2 object read from the file. Returns an empty Zipcode2 object if the file is not open, an invalid argument is encountered, or the key does not match.
 */
Zipcode2 Buffer2::DRead(std::fstream& inFile, const int key, int offset) {
    inFile.clear();
    inFile.seekg((offset), std::ios::beg);

    // If the file is not open, print an error message and return
    if (!inFile.is_open()) {
        std::cerr << "File is not open. Cannot read from file." << std::endl;
        return Zipcode2();
    }
    
    std::string line;
    std::getline(inFile, line);

    try {
		// Process the next line in the buffer
		std::string process_line = line;
		size_t space_pos = process_line.find(' ');

		// Extract the record length from the beginning of the line to the space
        lastRecordLength = std::stoi(process_line.substr(0, space_pos));

        // Ensure the line is the correct length
        if (process_line.length() >= lastRecordLength) {
            process_line = process_line.substr(space_pos + 1, lastRecordLength);  // Skip the space
        }

        Zipcode2 tmp;
        tmp = Unpack(process_line);

        // Compare the key with the zipcode read from the file
        if (tmp.getZipcode() == key) {
            return tmp;
        } else {
            std::cerr << "Key does not match the record read from the file." << std::endl;
            return Zipcode2();
        }
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << '\n';
        // Handle the exception
        return Zipcode2();
    }
}

/**
 * @brief Pack a Zipcode2 object into a string.
 * 
 * Packs the given Zipcode2 object into a string using the field order.
 * @param zipcode The Zipcode2 object to pack.
 * @return std::string The packed Zipcode2 object as a string.
 */
std::string Buffer2::Pack(Zipcode2& zipcode) {
    return zipcode.Pack(fieldOrder);
}

/**
 * @brief Unpack a string into a Zipcode2 object.
 * 
 * Unpacks the given string into a Zipcode2 object using the field order.
 * @param str The string to unpack.
 * @return Zipcode2 The unpacked Zipcode2 object.
 */
Zipcode2 Buffer2::Unpack(std::string str) {
    Zipcode2 zipcode;
    std::istringstream iss(str);
    std::string field;

    // Use fieldOrder to unpack str into zipcode
    for (int i = 0; i < fieldOrder.size(); i++) {
        std::getline(iss, field, ','); // assuming fields are comma-separated
        if (fieldOrder[i] == "Zip Code") {
            zipcode.setZipcode(std::stoi(field));
        } else if (fieldOrder[i] == "Place Name") {
            zipcode.setPlaceName(field);
        } else if (fieldOrder[i] == "State") {
            zipcode.setState(field);
        } else if (fieldOrder[i] == "County") {
            zipcode.setCounty(field);
        } else if (fieldOrder[i] == "Lat") {
            zipcode.setLat(std::stod(field));
        } else if (fieldOrder[i] == "Long") {
            zipcode.setLon(std::stod(field));
        }
    }
    return zipcode;
}

/**
 * @brief Print a Zipcode2 object.
 * 
 * Prints the given Zipcode2 object. If the Zipcode2 object is empty, it prints a message indicating that the Zipcode2 is empty.
 * @param zipcode The Zipcode2 object to print.
 */
void Buffer2::Print(Zipcode2& zipcode) {
    try {
        if (zipcode.isEmpty()) {
            std::cout << "The Zipcode2 is empty." << std::endl;
        } else {
            zipcode.Print();
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to print Zipcode2: " << e.what() << std::endl;
    }
}

/**
 * @brief Flush the output buffer to a file.
 * 
 * Writes all remaining records in the output buffer to the specified output file and then clears the output buffer.
 * @param outFile The output file stream.
 */
void Buffer2::Flush(std::fstream& outFile) {
    // If the file is not open, print an error message and return
    if (!outFile.is_open()) {
        std::cerr << "File is not open. Cannot write to file." << std::endl;
        return;
    }

    // Write all remaining records in the obuffer to the file
    for (const auto& record : obuffer) {
        outFile << record << std::endl;
    }

    // Clear the obuffer
    obuffer.clear();

    // Flush the file stream to ensure all buffered data is written
    outFile.flush();
}

/**
 * @brief Calculate the record count in a file.
 * 
 * Calculates the number of records in the specified input file.
 * @param inFile The input file stream.
 * @return int The record count.
 */
int Buffer2::CalculateRecordCount(std::fstream& inFile) {

	inFile.clear();
    // Save the current position in the file
    std::streampos currentPos = inFile.tellg();
    // Reset the position to the beginning of the file
    inFile.seekg(0, std::ios::beg);

    // Count the records
    std::string line;
    recordCount = 0;
    while (std::getline(inFile, line)) {
		// Skip lines that do not start with a digit
		if (!std::isdigit(line[0])) {
			continue;
		}
        ++recordCount;
    }

    // Return to the saved position
    inFile.seekg(currentPos);
	
	return recordCount;
}

/**
 * @brief Update the field order for packing/unpacking.
 * 
 * Updates the field order for packing and unpacking Zipcode2 objects.
 * @param newOrder The new field order.
 */
void Buffer2::UpdateFieldOrder(std::vector<std::string> newOrder) {
	fieldOrder = newOrder;
}