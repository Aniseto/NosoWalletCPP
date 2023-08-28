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

 //OrderData Struct

#pragma pack(push, 1)
    struct OrderData {

        int Block;
        std::string OrderID;
        int OrderLines;
        char OrderType[6];
        int64_t TimeStamp;
        char Reference[64];
        int TrxLine;
        char Sender[120];
        char Address[40];
        char Receiver[40];
        int64_t AmmountFee;
        int64_t AmmountTrf;
        char Signature[120];
        char TrfrID[64];
    };
#pragma pack(pop)
  /*OrderData = Packed Record
   Block : integer;
   OrderID : String[64];
   OrderLines : Integer;
   OrderType : String[6];
   TimeStamp : Int64;
   Reference : String[64];
     TrxLine : integer;
     Sender : String[120];    // La clave publica de quien envia
     Address : String[40];
     Receiver : String[40];
     AmmountFee : Int64;
     AmmountTrf : Int64;
     Signature : String[120];
     TrfrID : String[64];
   end;
    

        
        char empty[1];     // Added because sumary.psk fine is created by a Pascal app that adds an initial Char specifying Numer of Chars
        char Hash[40];     // Public hash (including null terminator)
        char empty2[1];    // Added because sumary.psk fine is created by a Pascal app that adds an initial Char specifying Numer of Chars
        char Custom[40];   // Custom alias (including null terminator)
        int64_t Balance;   // Noso balance
        int64_t Score;     // Token balance
        int64_t LastOP;    // Last operation block
    };
#pragma pack(pop)
 
  */  
    
    // Wallet Struct Data

#pragma pack(push, 1)
    class WalletData {

    private:
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
    public:
        // Getter para obtener el valor de privateKey
        std::string GetPrivateKey() {

            return PrivateKey;
        }
        std::string GetPublicKey() {
            return PublicKey;
        }
        std::string GetHash() {
            return Hash;
        }
        std::string GetLabel() {
            return Custom;
        }
        std::int64_t GetPending() {
            return Pending;
        }
        std::int64_t GetBalance() {
            return Balance;
        }
        //Setters
        void SetPrivateKey(const std::string& privateKey) {

            size_t copySize = std::min(privateKey.size(), sizeof(PrivateKey) - 1);

            // Copiar los datos manualmente
            for (size_t i = 0; i < copySize; ++i) {
                PrivateKey[i] = privateKey[i];
            }
        }
        void SetPublicKey(const std::string& publicKey) {

            size_t copySize = std::min(publicKey.size(), sizeof(PublicKey) - 1);

            // Copiar los datos manualmente
            for (size_t i = 0; i < copySize; ++i) {
                PublicKey[i] = publicKey[i];
            }
        }
        void SetHash(const std::string& hash) {

            size_t copySize = std::min(hash.size(), sizeof(PublicKey) - 1);

            // Copiar los datos manualmente
            for (size_t i = 0; i < copySize; ++i) {
                Hash[i] = hash[i];
            }
        }
        void SetCustom(const std::string& custom) {

            size_t copySize = std::min(custom.size(), sizeof(custom) - 1);

            // Copiar los datos manualmente
            for (size_t i = 0; i < copySize; ++i) {
                Custom[i] = custom[i];
            }
        }
        void SetBalance(int64_t balance) {

            Balance = balance;
        }
        void SetPendig(int64_t pending) {

            Pending = pending;
        }
        void SetScore(int64_t score) {

            Score = score;
        }
        void SetLastOp(int64_t lastop) {

            LastOP = lastop;
        }


    };
#pragma pack(pop)

