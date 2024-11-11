#include "Fileblock.h"

//Fileblock constructor initializes it's member variables, and fills the payload with the null character
Fileblock::Fileblock() : id(0), storedChecksum(0) {
    // Fill payload with zeros
    for (int i = 0; i < 501; ++i) {
        payload[i] = '\0';
    }
}

Fileblock::~Fileblock() {
    //no dynamic allocation, so nothing for destructore to tke care of
}

//this return the id of the fileblock instance. Used in other function. eg used in store to check if the id already exists in the table
unsigned int Fileblock::getID() const {
    return id;
}

//set payload  first clears all the data by filling up the array with null character, then fills in the wanted value
void Fileblock::set_payload(const char* Payload) {
    for (int i = 0; i < 501; ++i) {
        payload[i] = '\0'; 
    }
    
    for (int i = 0; i < 500 && Payload[i] != '\0'; ++i) {
        payload[i] = Payload[i];
    }
}

//calculate the checksum based off the ASCII values of the elements in the payload
int Fileblock::compute_checksum() const {
    int checksum = 0;
    for (int i = 0; i < 500; ++i) {
        checksum += static_cast<unsigned char>(payload[i]); 
    }
    return checksum % 256; 
}

//Returns true if the computed checksum is equal to the checksum that is stored
bool Fileblock::validate_checksum() const {
    //calls compute checksum before comparing
    return compute_checksum() == storedChecksum; 
}

//created so ctoredChecksum can be accessed when it needs to be updated
void Fileblock::setChecksum(int checksum) {
    storedChecksum = checksum; // Set stored checksum
}

// this is to retrieve the value of storedchecksum
int Fileblock::getStoredChecksum() const {
    return storedChecksum; // Return stored checksum
}