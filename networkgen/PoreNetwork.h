//
//  PoreNetwork.h
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef networkgen_PoreNetwork_h
#define networkgen_PoreNetwork_h
#include <cstdlib>
#include <iostream>

struct NetworkSpecs {
    std::string name;
	std::string pbSizeFile;
	float meanPBsize;
	float stdDev;
	float maxPbSize;
	float minPbSize;

    unsigned int Ni, Nj, Nk;
    float C[26];
    unsigned int coordNr;
    double searchDistance;
    float pbDist;
    bool periodicBounndaries = false;
    bool flowDirs[3]; // flowDirs[0] -> x-boundaries, flowDirs[1] -> y-boundaries, flowDirts[2] -> z-boundaries
	bool keepDeadEnd = false;
    bool constantPBSize = false;

};

class PoreNetwork{
    
    
private:

    int checkInput();
    template <typename T> T** paddedList(size_t amount, T **List, size_t nrOfCols ,size_t currentSize, bool headPadding);
	template <typename T> T*  paddedList(size_t amount, T *List, size_t currentSize, bool headPadding);
    void cleanPeriodic(size_t flowDir);
    
public:

    NetworkSpecs *ns;
	float *pb_sizeList; //pB_SIZES read from file
    int ***arr; //contains the pb number at the lattic coordinates, (Known as a Lookup Table)
    int **throatCounter; //number of prore throats per pb nr (1 based!) and number of throats preceding.
    int **throatList; // Connection map from pb nr -> pb nr. Is the half map!
    int **throatList_full; //Full Connection map
    size_t nrOfConnections;
    size_t nrOfActivePBs;
    size_t nrOfInlets, nrOfOutlets;
    size_t periodicListLength;

    float **locationList; // Location of a pb using its nr as index
    int *periodicThroats; // position in the throaList which has a periodic connection
    
    
    PoreNetwork(NetworkSpecs *ns);
    
    PoreNetwork(const char * networkSpecsFile);
    
    PoreNetwork(const PoreNetwork& other, std::string newName);
    
    // Loads a PoreNetwork from Files
    PoreNetwork(const std::string networkSpecs_file);
    
    PoreNetwork();
    
    virtual ~PoreNetwork();
    
    void generateBoundary(size_t dir, float inletSize, float outletSize, float inletThroatLength, float outletThroatLength);
    
    void removeFlaggedThroats(const int Flag = 0);
	
		int rebuildThroatCounters();

    int rebuildThroatCounters();

    int removeFlaggedPBs(char *pb_flag_list, char minFlag);

    void generateLocation();
    
    void generateConnectivity();
    
    size_t generateFullConnectivity();
    
    void generate_naive_array();
	
	void generatePbSizes();
	
	char* killDeadEndPores();
    
    size_t delelteThroat(size_t i, size_t deleted, int flag);
    
    size_t delelteThroat(int i, int deleted, int flag);

};

#endif
