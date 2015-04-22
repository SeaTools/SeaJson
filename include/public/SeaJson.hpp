// @@@LICENSE
//
// Copyright (C) 2015, SeaTools, All Right Reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// author Sungsik Kim
//
// LICENSE@@@

#ifndef SEAJSON_CORE_H_
#define SEAJSON_CORE_H_

#include <string>

namespace SEAJSON {

#define TAIL  -1

class SeaJsonData;

struct RollbackInfo {
    SeaJsonData* sjData_;
    long index_;

    RollbackInfo(SeaJsonData* sjData, long index)
        : sjData_(sjData)
        , index_(index)
    {}
};

class SeaJson
{
    friend class SeaJsonData;

private:
    SeaJsonData* parent_;
    char* key_;
    char* jstr_;
    long jstrLength_;
    RollbackInfo* rbInfo_;
    SeaJsonData* seaJsonData_;

public:
    SeaJson();
    explicit SeaJson(SeaJsonData* parent);   //child creators
    explicit SeaJson(const char* jstr);
    SeaJson(const SeaJson& sjObj);

    virtual ~SeaJson();

    void parse(const char* jstr);
    const char* stringify();
    long getType();
    bool isValid();
    void remove();
    void clear();

    // Casting Operators
    operator int();
    operator unsigned int();
    operator short();
    operator unsigned short();
    operator long();
    operator unsigned long();
    operator long long();
    operator long long unsigned int();
    operator bool();
    operator double();
    operator float();
    operator char*();
    operator std::string();

/*
    // functor
    SeaJson& operator()();
    SeaJson& operator()(const char* key);
    SeaJson& operator()(const std::string& key);
*/

    // Array Operators
    SeaJson& operator[](int index);
    SeaJson& operator[](unsigned int index);
    SeaJson& operator[](short index);
    SeaJson& operator[](unsigned short index);
    SeaJson& operator[](long index);
    SeaJson& operator[](unsigned long index);
    SeaJson& operator[](long long index);
    SeaJson& operator[](long long unsigned int index);
    SeaJson& operator[](const std::string& key);
    SeaJson& operator[](const char* key);
    SeaJson& operator[](char* key);

    // Assignment Operators
    SeaJson& operator=(int const& nVal);
    SeaJson& operator=(unsigned int const& nVal);
    SeaJson& operator=(const short& nVal);
    SeaJson& operator=(const unsigned short& nVal);
    SeaJson& operator=(const long& nVal);
    SeaJson& operator=(const unsigned long& nVal);
    SeaJson& operator=(const long long &nVal);
    SeaJson& operator=(const long long unsigned int &nVal);
    SeaJson& operator=(const bool& boolVal);
    SeaJson& operator=(const double& doubleVal);
    SeaJson& operator=(const float& floatVal);
    SeaJson& operator=(const std::string& strVal);
    SeaJson& operator=(const SeaJson& sjObj);
    SeaJson& operator=(const char* val);
    SeaJson& operator=(char* val);

    // Composite Assignment Operators
    SeaJson& operator+=(const SeaJson& sjObj);

    // Comparison Operations
    bool operator==(int const& nVal);
    bool operator==(unsigned int const& nVal);
    bool operator==(const long& nVal);
    bool operator==(const short& nVal);
    bool operator==(const unsigned short& nVal);
    bool operator==(const unsigned long& nVal);
    bool operator==(const long long &nVal);
    bool operator==(const long long unsigned int& nVal);
    bool operator==(const bool& boolVal);
    bool operator==(const double& doubleVal);
    bool operator==(const float& floatVal);
    bool operator==(const char* charVal);
    bool operator==(char* charVal);
    bool operator==(const std::string& strVal);

    bool operator!=(int const& nVal);
    bool operator!=(unsigned int const& nVal);
    bool operator!=(const short& nVal);
    bool operator!=(const unsigned short& nVal);
    bool operator!=(const long& nVal);
    bool operator!=(const unsigned long& nVal);
    bool operator!=(const long long& nVal);
    bool operator!=(const long long unsigned int& nVal);
    bool operator!=(const bool& boolVal);
    bool operator!=(const double& doubleVal);
    bool operator!=(const float& floatVal);
    bool operator!=(const char* charVal);
    bool operator!=(char* charVal);
    bool operator!=(const std::string& strVal);

private:
    void tossRollbackInfo(SeaJson* sjObj, long index = -1);
    void setRollbackInfo(RollbackInfo* rbInfo);
    void rollback();
    void commit();

    void parseJson(const char*& jstr);
    void parseObject(const char*& jstr);
    void parseArray(const char*& jstr);

    void stringifyJson(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyKey(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyObject(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyArray(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyNull(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyChar(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyLLong(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyDouble(char*& jstr, long& jstrBufferLength, long& index);
    void stringifyBool(char*& jstr, long& jstrBufferLength, long& index);

    long getKeyFromJsonString(const char*& jstr, const char*& jsKey);
    void getStringFromJsonString(const char*& jstr);
    bool getNumberFromJsonString(const char*& jstr);

    SeaJsonData* getParent() const;
    void deletedByParent();

    char* getKey() const;
    void setKey(const char* key, long length = -1);
    long compareKey(const char* key, long length = -1) const;

    void setType(long type);

    bool createDataObject();
    void deleteDataObject();

    static void trimLeft(const char*& jstr);
    static void increaseJsonStringBuffer(char*& jstr, long& jstrBufferLength, long index, long appendStrLength);
    static long getStrLength(char*& charVale);
};

#define JsonNull    SeaJson()

}   // namespace SEAJSON

#endif  // SEAJSON_CORE_H_