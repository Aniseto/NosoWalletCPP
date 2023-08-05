#pragma once
#include <iostream>
#include <string>

//NodeStatus Struct to process data from NODESTATUS command.

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
// Sumary Struct to process Data from sumary.zip

#pragma pack(push, 1)
    struct TSummaryData {
        char empty[1];     // Added because sumary.psk fine is created by a Pascal app that adds an initial Char specifying Numer of Chars
        char Hash[40];     // Public hash (including null terminator)
        char empty2[1];    // Added because sumary.psk fine is created by a Pascal app that adds an initial Char specifying Numer of Chars
        char Custom[40];   // Custom alias (including null terminator)
        int64_t Balance;   // Noso balance
        int64_t Score;     // Token balance
        int64_t LastOP;    // Last operation block
    };
#pragma pack(pop)

    // Wallet Struct Data

#pragma pack(push, 1)
    struct WalletData {

        char Empty[1]; //Due pascal original file
        char Hash[40]; // Public address HashEl hash publico o direccion
        char Empty2[1]; //Due Pascal original file
        char Custom[40]; // Custom Name
        char Empty3[1];  //Due Pascal original file
        char PublicKey[255]; // Public Key
        char Empty4[1]; // Due pascal original file
        char PrivateKey[255]; // Private Key
        int64_t Balance; // Last balance from this address
        int64_t Pending; // Last pending payment
        int64_t Score; // Address status.
        int64_t LastOP;// Time from the last operation in UnixTime Format

    };
#pragma pack(pop)

