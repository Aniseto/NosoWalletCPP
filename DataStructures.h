#pragma once
#include <iostream>
#include <string>
#include <botan/bigint.h>


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
// Summary Struct to process Data from sumary.zip

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
    class OrderData {

    private:
        int Block;
        std::string OrderID;
        int OrderLines;
        char OrderType[6];
        int64_t TimeStamp;
        char Reference[64];
        int TrxLine;
        char Sender[120]; //Public KJey from Sender
        char Address[40]; //Sender Noso Address Hash
        char Receiver[40]; //Receiver Noso Address Hash
        int64_t AmmountFee;
        int64_t AmmountTrf;
        char Signature[120];
        char TrfrID[64];
    public:
        //Getters
        int64_t GetTimeStamp() {

            return TimeStamp;
        }  
        
        //Setters & Getters
        void SetSenderPublicKey(const std::string& SenderPublicKeyAddress) //Save Public Key from Sender Noso Address
        {
            {

                size_t copySize = std::min(SenderPublicKeyAddress.size(), sizeof(Sender) - 1);
                //TextBox->AppendText("SenderPublicKeyAddress: " + SenderPublicKeyAddress + "\n");
                // Copiar los datos manualmente
                for (size_t i = 0; i < copySize; ++i) {
                    Sender[i] = SenderPublicKeyAddress[i];
                }
                Sender[copySize] = '\0';
            }
        }
        std::string GetSenderPublicKey()
        {
            return std::string(Sender);
        }
        void SetSenderHashAddress(const std::string& SenderHashAddress) //Save Sender NOSO address Hash propertie
        {
            {

                size_t copySize = std::min(SenderHashAddress.size(), sizeof(Address) - 1);

                // Copy data manually
                for (size_t i = 0; i < copySize; ++i) {
                    Address[i] = SenderHashAddress[i];
                }
                Address[copySize] = '\0';
            }
        }
        std::string GetSenderHashAddress()
        {
            return std::string(Address);
        }
        void SetReceiverHashAddress(const std::string& ReceiverHashAddress) //Save Sender NOSO address Hash propertie
        {
            {

                size_t copySize = std::min(ReceiverHashAddress.size(), sizeof(Receiver) - 1);

                // Copiar los datos manualmente
                for (size_t i = 0; i < copySize; ++i) {
                    Receiver[i] = ReceiverHashAddress[i];
                }
                Receiver[copySize] = '\0';
            }
        }
        std::string GetDestinationHashAddress()
        {
            return std::string(Receiver);
        }
        void SetOrderType(const std::string& OrderTypeSet) //Save Sender NOSO address Hash propertie
        {
             {

                 size_t copySize = std::min(OrderTypeSet.size(), sizeof(OrderType) - 1);

            // Copiar los datos manualmente
                 for (size_t i = 0; i < copySize; ++i) {
                        OrderType[i] = OrderTypeSet[i];
                   }
                 OrderType[copySize] = '\0';
            }
        }
        void SetOrderReference(const std::string& OrderReference) 
        {
            {

                size_t copySize = std::min(OrderReference.size(), sizeof(Reference) - 1);

                // Copiar los datos manualmente
                for (size_t i = 0; i < copySize; ++i) {
                    Reference[i] = OrderReference[i];
                }
                Reference[copySize] = '\0';
            }
        }
        void SetBlock(const int block) 
        {
            Block = block;
        }
        void SetOrderID(const std::string& SetOrderID)
        {
                size_t copySize = std::min(SetOrderID.size(), sizeof(OrderID) - 1);

                // Copiar los datos manualmente
                for (size_t i = 0; i < copySize; ++i) {
                    OrderID[i] = SetOrderID[i];
                }
                OrderID[copySize] = '\0';

           // OrderID = OrderID;
        }
        void SetOrderLines(const int Orderlines)
        {
            OrderLines = Orderlines;
        }
        void SetTimeStamp(const int Timestamp)
        {
            TimeStamp = Timestamp;
        }
        void SetTrxLine(const int Trxline)
        {
            TrxLine = Trxline;
        }
       
        void SetAmmountFee(const int64_t Ammountfee)
        {
            AmmountFee = Ammountfee;
        }
        void SetAmmountTrfe(const int64_t Ammounttrf)
        {
            AmmountTrf = Ammounttrf;
        }
        int64_t GetAmountTrfe()
        {
            return AmmountTrf;
        }
        int64_t GetAmountFee()
        {
            return AmmountFee;
        }
        int GetTrxLine()
        {
			return TrxLine;
		}
        int GetBlock()
        {
			return Block;
		}
        std::string GetOrderType()
        {
            return std::string(OrderType);
        }
        std::string GetOrderReference()
        {
			return std::string(Reference);
		}
        int GetOrderLines()
        {
            return OrderLines;
        }
        std::string GetOrderID()
        {
			return OrderID;
		}
        std::string GetSignature()
        {
			return std::string(Signature);
		}
        std::string GetTrfID()
        {
            return std::string(TrfrID);
        }

        void SetSignature(const std::string& signature)
        {
            {

                size_t copySize = std::min(signature.size(), sizeof(Signature) - 1);

                // Copiar los datos manualmente
                for (size_t i = 0; i < copySize; ++i) {
                    Signature[i] = signature[i];
                }
                Signature[copySize] = '\0';

            }
        }
    
        void SetTrfID(const std::string& trfid)
        {
            {

                size_t copySize = std::min(trfid.size(), sizeof(TrfrID) - 1);

                // Copiar los datos manualmente
                for (size_t i = 0; i < copySize; ++i) {
                    TrfrID[i] = trfid[i];
                }
                TrfrID[copySize] = '\0';
            }
        }
    };
#pragma pack(pop)
 
    
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
            PrivateKey[copySize] = '\0';
        }
        void SetPublicKey(const std::string& publicKey) {

            size_t copySize = std::min(publicKey.size(), sizeof(PublicKey) - 1);

            // Copiar los datos manualmente
            for (size_t i = 0; i < copySize; ++i) {
                PublicKey[i] = publicKey[i];
            }
            PublicKey[copySize] = '\0';
        }
        void SetHash(const std::string& hash) {

            size_t copySize = std::min(hash.size(), sizeof(Hash) - 1);

            // Copiar los datos manualmente
            for (size_t i = 0; i < copySize; ++i) {
                Hash[i] = hash[i];
            }
            Hash[copySize] = '\0';
        }
        void SetCustom(const std::string& custom) {

            size_t copySize = std::min(custom.size(), sizeof(Custom) - 1);

            // Copiar los datos manualmente
            for (size_t i = 0; i < copySize; ++i) {
                Custom[i] = custom[i];
            }
            Custom[copySize] = '\0';
        }
        void SetBalance(int64_t balance) {

            Balance = balance;
        }
        void SetPending(int64_t pending) {

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


#pragma pack(push, 1)
    class PendingOrders {
    public:
        
        std::string OrderType;
        std::string SourceAddress;
        std::string DestinationAddress;
        int64_t Amount;
        int64_t Fee;
    };
#pragma pack(pop)

#pragma pack(push, 1)   
    class DivResult {
    public:
        Botan::BigInt coefficient = Botan::BigInt(0);   //= Botan::BigInt.zero;
        Botan::BigInt remainder = Botan::BigInt(0);    // = BigInt.zero;


    };
#pragma pack(pop)