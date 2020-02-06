#pragma once
#include "string"
#include <cstring>

namespace ProtocolBuilder{
    enum ProtocolMode{
        Binary = 0,
        Json = 1
    };

	extern std::string Encode_FromByte(char value);
    extern std::string Encode_FromShort(short value);
	extern std::string Encode_FromInt(int value);
	extern std::string Encode_FromDouble(double value);
	extern std::string Encode_FromString(std::string value);
	
    extern std::string Encode_FromInt64(int64_t value);
    extern std::string Encode_FromUInt64(uint64_t value);

    extern char Decode_ToByte(std::string& buffer);
    extern short Decode_ToShort(std::string& buffer);
    extern int Decode_ToInt(std::string& buffer);
    extern double Decode_ToDouble(std::string& buffer);
    extern std::string Decode_ToString(std::string& buffer);

    extern int64_t Decode_ToInt64(std::string& buffer);
    extern uint64_t Decode_ToUInt64(std::string& buffer);
};
