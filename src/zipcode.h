/**
 * @file zipcode.h
 * @brief This file contains the declaration of the Zipcode class. It is used for project 4.
 * @class Zipcode
 * @brief A class for handling zipcode records with associated geographic information.
 * @details The Zipcode class provides methods to pack and unpack the data into a string for storage, 
 *          as well as getter and setter methods for each data field. This class is used in project 3.
 */

 #ifndef ZIPCODE_H
#define ZIPCODE_H

#include <string>

class Zipcode {
public:
    /**
     * @brief Construct a new Zipcode object.
     */
    Zipcode();
		
    /**
     * @brief Pack the Zipcode object into a string for storage.
     * @return The packed string.
     */
    std::string Pack();

    /**
     * @brief Unpack a Zipcode object from a stored string.
     * @param str The string to unpack.
     */
    void Unpack(std::string str);

    /**
     * @brief Get the Zipcode.
     * @return int The zipcode.
     */
    int getZipcode() { return zipcode; };

    /**
     * @brief Get the Place Name.
     * @return std::string The place name.
     */
    std::string getPlaceName() { return placeName; };

    /**
     * @brief Get the State.
     * @return std::string The state.
     */
    std::string getState() { return state; };

    /**
     * @brief Get the County.
     * @return std::string The county.
     */
    std::string getCounty() { return county; };

    /**
     * @brief Get the Latitude.
     * @return double The latitude.
     */
    double getLat() { return lat; };

    /**
     * @brief Get the Longitude.
     * @return double The longitude.
     */
    double getLon() { return lon; };


    /**
     * @brief Set the Zipcode.
     * @param zipcode The new zipcode.
     */
    void setZipcode(int zipcode);

    /**
     * @brief Set the Place Name.
     * @param placeName The new place name.
     */
    void setPlaceName(std::string placeName);

    /**
     * @brief Set the State.
     * @param state The new state.
     */
    void setState(std::string state);

    /**
     * @brief Set the County.
     * @param county The new county.
     */
    void setCounty(std::string county);

    /**
     * @brief Set the Latitude.
     * @param lat The new latitude.
     */
    void setLat(double lat);

    /**
     * @brief Set the Longitude.
     * @param lon The new longitude.
     */
    void setLon(double lon);
	
private:
    int zipcode; ///< The zipcode.
    std::string placeName; ///< The place name associated with the zipcode.
    std::string state; ///< The state associated with the zipcode.
    std::string county; ///< The county associated with the zipcode.
    double lat; ///< The latitude of the zipcode.
    double lon; ///< The longitude of the zipcode.
};

#endif
