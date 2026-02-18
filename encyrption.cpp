#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include "scheduling.h"

using namespace std;

vector<vector<string>> string_to_hex(const string& block){
    // block must be 16 bytes (padded by caller)
    vector<vector<string>> matrixMapping(4, vector<string>(4, "00"));
    for (int i = 0; i < 16; ++i) {
        unsigned char c = static_cast<unsigned char>(block[i]);
        stringstream ss;
        ss << uppercase << hex << setfill('0') << setw(2) << static_cast<int>(c);
        string hexStr = ss.str();
        matrixMapping[i % 4][i / 4] = hexStr;
    }
    return matrixMapping;
}

vector<vector<int>> hex_to_int(vector<vector<string>>& matrixMapping){
    vector<vector<int>> intMatrix(4,vector<int>(4,0));
    for (int i=0; i<matrixMapping.size(); i++){
        for (int j=0; j<matrixMapping[i].size(); j++){
            int x;
            std::stringstream ss;
            ss << std::hex << matrixMapping[i][j];
            ss >> x;
            intMatrix[i][j] = x;
        }
    }
    return intMatrix;
}

vector<vector<int>> xorCyprt(vector<vector<int>>& keyIntHex, vector<vector<int>>& TextIntHex){
    vector<vector<int>> xorCryptMatrix(4,vector<int>(4,0));
    for (int i=0; i<keyIntHex.size(); i++){
        for (int j=0; j<keyIntHex[i].size(); j++){
            xorCryptMatrix[i][j] = keyIntHex[i][j] ^ TextIntHex[i][j];
        }
    }
    return xorCryptMatrix;
}

vector<vector<string>> int_to_hex(vector<vector<int>>& intVector){
    vector<vector<string>> hexVector(4,vector<string>(4," "));
    for (int i=0; i<intVector.size(); i++){
        for (int j=0; j<intVector[i].size(); j++){
            stringstream ss;
            ss << uppercase << setfill('0') << setw(2) << hex << intVector[i][j];
            hexVector[i][j] = ss.str();
        }
    }
    return hexVector;
}

vector<vector<string>> substitutionBytes(vector<vector<string>> hexVector){
    unordered_map<string, string> sbox = {
    {"00", "63"}, {"01", "7C"}, {"02", "77"}, {"03", "7B"}, {"04", "F2"}, {"05", "6B"}, {"06", "6F"}, {"07", "C5"},
    {"08", "30"}, {"09", "01"}, {"0A", "67"}, {"0B", "2B"}, {"0C", "FE"}, {"0D", "D7"}, {"0E", "AB"}, {"0F", "76"},
    {"10", "CA"}, {"11", "82"}, {"12", "C9"}, {"13", "7D"}, {"14", "FA"}, {"15", "59"}, {"16", "47"}, {"17", "F0"},
    {"18", "AD"}, {"19", "D4"}, {"1A", "A2"}, {"1B", "AF"}, {"1C", "9C"}, {"1D", "A4"}, {"1E", "72"}, {"1F", "C0"},
    {"20", "B7"}, {"21", "FD"}, {"22", "93"}, {"23", "26"}, {"24", "36"}, {"25", "3F"}, {"26", "F7"}, {"27", "CC"},
    {"28", "34"}, {"29", "A5"}, {"2A", "E5"}, {"2B", "F1"}, {"2C", "71"}, {"2D", "D8"}, {"2E", "31"}, {"2F", "15"},
    {"30", "04"}, {"31", "C7"}, {"32", "23"}, {"33", "C3"}, {"34", "18"}, {"35", "96"}, {"36", "05"}, {"37", "9A"},
    {"38", "07"}, {"39", "12"}, {"3A", "80"}, {"3B", "E2"}, {"3C", "EB"}, {"3D", "27"}, {"3E", "B2"}, {"3F", "75"},
    {"40", "09"}, {"41", "83"}, {"42", "2C"}, {"43", "1A"}, {"44", "1B"}, {"45", "6E"}, {"46", "5A"}, {"47", "A0"},
    {"48", "52"}, {"49", "3B"}, {"4A", "D6"}, {"4B", "B3"}, {"4C", "29"}, {"4D", "E3"}, {"4E", "2F"}, {"4F", "84"},
    {"50", "53"}, {"51", "D1"}, {"52", "00"}, {"53", "ED"}, {"54", "20"}, {"55", "FC"}, {"56", "B1"}, {"57", "5B"},
    {"58", "6A"}, {"59", "CB"}, {"5A", "BE"}, {"5B", "39"}, {"5C", "4A"}, {"5D", "4C"}, {"5E", "58"}, {"5F", "CF"},
    {"60", "D0"}, {"61", "EF"}, {"62", "AA"}, {"63", "FB"}, {"64", "43"}, {"65", "4D"}, {"66", "33"}, {"67", "85"},
    {"68", "45"}, {"69", "F9"}, {"6A", "02"}, {"6B", "7F"}, {"6C", "50"}, {"6D", "3C"}, {"6E", "9F"}, {"6F", "A8"},
    {"70", "51"}, {"71", "A3"}, {"72", "40"}, {"73", "8F"}, {"74", "92"}, {"75", "9D"}, {"76", "38"}, {"77", "F5"},
    {"78", "BC"}, {"79", "B6"}, {"7A", "DA"}, {"7B", "21"}, {"7C", "10"}, {"7D", "FF"}, {"7E", "F3"}, {"7F", "D2"},
    {"80", "CD"}, {"81", "0C"}, {"82", "13"}, {"83", "EC"}, {"84", "5F"}, {"85", "97"}, {"86", "44"}, {"87", "17"},
    {"88", "C4"}, {"89", "A7"}, {"8A", "7E"}, {"8B", "3D"}, {"8C", "64"}, {"8D", "5D"}, {"8E", "19"}, {"8F", "73"},
    {"90", "60"}, {"91", "81"}, {"92", "4F"}, {"93", "DC"}, {"94", "22"}, {"95", "2A"}, {"96", "90"}, {"97", "88"},
    {"98", "46"}, {"99", "EE"}, {"9A", "B8"}, {"9B", "14"}, {"9C", "DE"}, {"9D", "5E"}, {"9E", "0B"}, {"9F", "DB"},
    {"A0", "E0"}, {"A1", "32"}, {"A2", "3A"}, {"A3", "0A"}, {"A4", "49"}, {"A5", "06"}, {"A6", "24"}, {"A7", "5C"},
    {"A8", "C2"}, {"A9", "D3"}, {"AA", "AC"}, {"AB", "62"}, {"AC", "91"}, {"AD", "95"}, {"AE", "E4"}, {"AF", "79"},
    {"B0", "E7"}, {"B1", "C8"}, {"B2", "37"}, {"B3", "6D"}, {"B4", "8D"}, {"B5", "D5"}, {"B6", "4E"}, {"B7", "A9"},
    {"B8", "6C"}, {"B9", "56"}, {"BA", "F4"}, {"BB", "EA"}, {"BC", "65"}, {"BD", "7A"}, {"BE", "AE"}, {"BF", "08"},
    {"C0", "BA"}, {"C1", "78"}, {"C2", "25"}, {"C3", "2E"}, {"C4", "1C"}, {"C5", "A6"}, {"C6", "B4"}, {"C7", "C6"},
    {"C8", "E8"}, {"C9", "DD"}, {"CA", "74"}, {"CB", "1F"}, {"CC", "4B"}, {"CD", "BD"}, {"CE", "8B"}, {"CF", "8A"},
    {"D0", "70"}, {"D1", "3E"}, {"D2", "B5"}, {"D3", "66"}, {"D4", "48"}, {"D5", "03"}, {"D6", "F6"}, {"D7", "0E"},
    {"D8", "61"}, {"D9", "35"}, {"DA", "57"}, {"DB", "B9"}, {"DC", "86"}, {"DD", "C1"}, {"DE", "1D"}, {"DF", "9E"},
    {"E0", "E1"}, {"E1", "F8"}, {"E2", "98"}, {"E3", "11"}, {"E4", "69"}, {"E5", "D9"}, {"E6", "8E"}, {"E7", "94"},
    {"E8", "9B"}, {"E9", "1E"}, {"EA", "87"}, {"EB", "E9"}, {"EC", "CE"}, {"ED", "55"}, {"EE", "28"}, {"EF", "DF"},
    {"F0", "8C"}, {"F1", "A1"}, {"F2", "89"}, {"F3", "0D"}, {"F4", "BF"}, {"F5", "E6"}, {"F6", "42"}, {"F7", "68"},
    {"F8", "41"}, {"F9", "99"}, {"FA", "2D"}, {"FB", "0F"}, {"FC", "B0"}, {"FD", "54"}, {"FE", "BB"}, {"FF", "16"}
    };
    vector<vector<string>> substitutionMatrix(4,vector<string>(4,""));
    for (int i=0; i<hexVector.size(); i++){
        for (int j=0; j<hexVector[i].size(); j++){
            substitutionMatrix[i][j] = sbox[hexVector[i][j]];
        }
    }
    return substitutionMatrix;
}

void shifRows(vector<string>& row, int offset){
    rotate(row.begin(), row.begin() + offset, row.end());
}

vector<vector<string>> shiftRowMatrix(vector<vector<string>>& substitutionMatrix){
    for (int i=0; i<substitutionMatrix.size(); i++){
        shifRows(substitutionMatrix[i],i);
    }
    return substitutionMatrix;
}

vector<vector<string>> mixColoums(vector<vector<int>>& fixedMatrix, vector<vector<string>>& substitutionMatrix){
    auto xtime = [](int x)->int {
        return ((x << 1) ^ ((x & 0x80) ? 0x1B : 0x00)) & 0xFF;
    };
    auto gfMul = [&](int a, int b)->int {
        int res = 0;
        int aa = a & 0xFF;
        int bb = b & 0xFF;
        while (bb) {
            if (bb & 1) res ^= aa;
            aa = xtime(aa);
            bb >>= 1;
        }
        return res & 0xFF;
    };

    vector<vector<int>> state = hex_to_int(substitutionMatrix);
    vector<vector<int>> out(4, vector<int>(4, 0));
    for (int col = 0; col < 4; col++) {
        int s0 = state[0][col];
        int s1 = state[1][col];
        int s2 = state[2][col];
        int s3 = state[3][col];
        out[0][col] = gfMul(s0, fixedMatrix[0][0]) ^ gfMul(s1, fixedMatrix[0][1]) ^ gfMul(s2, fixedMatrix[0][2]) ^ gfMul(s3, fixedMatrix[0][3]);
        out[1][col] = gfMul(s0, fixedMatrix[1][0]) ^ gfMul(s1, fixedMatrix[1][1]) ^ gfMul(s2, fixedMatrix[1][2]) ^ gfMul(s3, fixedMatrix[1][3]);
        out[2][col] = gfMul(s0, fixedMatrix[2][0]) ^ gfMul(s1, fixedMatrix[2][1]) ^ gfMul(s2, fixedMatrix[2][2]) ^ gfMul(s3, fixedMatrix[2][3]);
        out[3][col] = gfMul(s0, fixedMatrix[3][0]) ^ gfMul(s1, fixedMatrix[3][1]) ^ gfMul(s2, fixedMatrix[3][2]) ^ gfMul(s3, fixedMatrix[3][3]);
    }
    return int_to_hex(out);
}

vector<vector<string>> buildRoundKeyMatrix(const vector<vector<string>>& roundKeys, int index) {
    vector<vector<string>> keyMat(4, vector<string>(4, "00"));
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            keyMat[row][col] = roundKeys[index + col][row];
        }
    }
    return keyMat;
}

vector<vector<string>> encryptMessage(vector<vector<int>>& fixedMatrix, vector<vector<string>>& roundKeys, vector<vector<string>>& text) {
    // Initial AddRoundKey (round 0)
    vector<vector<string>> state = text;
    vector<vector<string>> roundKeyMat = buildRoundKeyMatrix(roundKeys, 0);
    vector<vector<int>> stateInt = hex_to_int(state);
    vector<vector<int>> keyInt = hex_to_int(roundKeyMat);
    vector<vector<int>> xorInt = xorCyprt(stateInt, keyInt);
    state = int_to_hex(xorInt);
    // Rounds 1..9
    for (int round = 1; round <= 9; ++round) {
        state = substitutionBytes(state);
        state = shiftRowMatrix(state);
        state = mixColoums(fixedMatrix, state);
        roundKeyMat = buildRoundKeyMatrix(roundKeys, round * 4);
        stateInt = hex_to_int(state);
        keyInt = hex_to_int(roundKeyMat);
        xorInt = xorCyprt(stateInt, keyInt);
        state = int_to_hex(xorInt);
    }
    // Final round (10): SubBytes -> ShiftRows -> AddRoundKey (no MixColumns)
    state = substitutionBytes(state);
    state = shiftRowMatrix(state);
    roundKeyMat = buildRoundKeyMatrix(roundKeys, 40);
    stateInt = hex_to_int(state);
    keyInt = hex_to_int(roundKeyMat);
    xorInt = xorCyprt(stateInt, keyInt);
    state = int_to_hex(xorInt);

    return state;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    string key = "Thats my Kung Fu";
    string plaintext;
    vector<vector<int>> fixedMatrix = {{2,3,1,1},{1,2,3,1},{1,1,2,3},{3,1,1,2}};
    vector<vector<string>> keyMapping = string_to_hex(key);
    unordered_map<string,string> sbox = get_sbox();
    vector<vector<string>> roundKeys = generateRoundKeys_(keyMapping,sbox);
    {
        std::ifstream infile("input.txt", std::ios::in | std::ios::binary);
        if (infile.is_open()) {
            string filetxt((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
            infile.close();
            size_t orig_len = filetxt.size();
            size_t pad_len = 16 - (orig_len % 16);
            if (pad_len == 0) pad_len = 16;
            filetxt.append(pad_len, static_cast<char>(pad_len));
            plaintext = filetxt;
        }
    }
    ofstream outfile("ciphertext.txt", ios::out | ios::trunc);
    if (!outfile.is_open()) {
        return 1;
    }
    for (int i=0; i<plaintext.length(); i+=16){
        vector<vector<string>> textMapping = string_to_hex(plaintext.substr(i,i+16));
        vector<vector<string>> encrypt = encryptMessage(fixedMatrix,roundKeys,textMapping);
        for (int r = 0; r < encrypt.size(); ++r) {
            for (int c = 0; c < encrypt[r].size(); ++c) {
                outfile << encrypt[r][c];
            }
        }
        outfile << '\n';
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Execution time: " << durationMicro.count() << " microseconds" << endl;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;
    return 0;
}