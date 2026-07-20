/**
 * @file zipcode.cpp
 * @brief This file contains the implementation of the Zipcode class. It is used for project 3.
 */
 
#include "zipcode.h" ///< Include the Zipcode2 class
#include <sstream>   ///< Include the stringstream library for string manipulation

/**
 * @brief Construct a new Zipcode object.
 * 
 * Initializes all fields to their default values.
 */
Zipcode::Zipcode() {
    zipcode = 0;
    placeName = "";
    state = "";
    county = "";
    lat = 0.0;
    lon = 0.0;
}

/**
 * @brief Packs the Zipcode object into a string.
 * @return The packed string.
 */
std::string Zipcode::Pack() {
    std::stringstream ss;
    ss << zipcode << "," << placeName << "," << state << "," << county << "," << lat << "," << lon;
    return ss.str();
}

/**
 * @brief Unpacks a string into a Zipcode object.
 * @param str The string to unpack.
 */
void Zipcode::Unpack(std::string str) {
    std::stringstream ss(str);
    ss >> zipcode;
    ss.ignore(1, ',');
    std::getline(ss, placeName, ',');
    std::getline(ss, state, ',');
    std::getline(ss, county, ',');
    ss >> lat;
    ss.ignore(1, ',');
    ss >> lon;
}

/**
 * @brief Sets the zipcode.
 * @param newZipcode The new zipcode.
 */
void Zipcode::setZipcode(int newZipcode) {
    zipcode = newZipcode;
}

/**
 * @brief Sets the place name.
 * @param newPlaceName The new place name.
 */
void Zipcode::setPlaceName(std::string newPlaceName) {
    placeName = newPlaceName;
}

/**
 * @brief Sets the state.
 * @param newState The new state.
 */
void Zipcode::setState(std::string newState) {
    state = newState;
}

/**
 * @brief Sets the county.
 * @param newCounty The new county.
 */
void Zipcode::setCounty(std::string newCounty) {
    county = newCounty;
}

/**
 * @brief Sets the latitude.
 * @param new_lat The new latitude.
 */
void Zipcode::setLat(double newLat) {
    lat = newLat;
}

/**
 * @brief Sets the longitude.
 * @param newLon The new longitude.
 */
void Zipcode::setLon(double newLon) {
    lon = newLon;
}