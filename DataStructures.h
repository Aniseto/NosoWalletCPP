#pragma once
#include <iostream>
#include <string>

    struct NodeStatusData {

        std::string NodeStatus = "Empty";             // 0- Saves NODESTATUS Message, just to have control
        unsigned int Peers{ 0 };                       // 1- Saves Number of Peers connected to this Node.
        unsigned int BlockNumber{ 0 };                 // 2- Saves Current Block Number
        unsigned int Pending{ 0 };                     // 3- Number of Pending Operations
        unsigned int Delta{ 0 };                       // 4- Number of Pending Operations
        std::string Headers = "Empty";             // Number of Pending Operations
        std::string Version = "Empty";        // Current Node Version Software
        unsigned int UTCTIme{ 0 };
        std::string MNsHash = "Empty";
        unsigned int MNsCount{ 0 };
        //int Updated {0};
        std::string LastBlockHash = "Empty";
        std::string BestHashDiff = "Empty";
        int LastBlockTimeEnd{ 0 };
        std::string LastBLockMiner = "Empty";
        std::string ChecksCount = "Empty";
        std::string LastBlockPoW = "Empty";
        std::string LastBlockDiff = "Empty";
        std::string Summary = "Empty";
        std::string GVTHash = "Empty";
        std::string NosoCFG = "Empty";
        std::string PSOHash = "Empty";

 
};

