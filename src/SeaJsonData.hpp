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

#ifndef SEAJSON_DATA_H_
#define SEAJSON_DATA_H_

#include <vector>
#include <string>

namespace SEAJSON {

typedef long long llong;

class SeaJson;

enum ValType {
    JSON_null = 0,
    JSON_char,
    JSON_llong,
    JSON_double,
    JSON_bool,
    JSON_object,
    JSON_array
};

union   UNION_JSON
{
    llong       llongVal_;
    bool        boolVal_;
    double      doubleVal_;
    char*       charVal_;
    std::vector<SeaJson*>* vtChildren_;

    UNION_JSON() : llongVal_(0) {}
};

class SeaJsonData {
private:
    UNION_JSON  val_;
    long        type_;
    long        refCount_;

public:
    SeaJsonData();
    virtual ~SeaJsonData();

    void removeData();

    std::vector<SeaJson*>* getChildren() const;
    void removeChildLink(SeaJson* child);
    void rollback(long index);

    // 대입 연산자
    SeaJsonData& operator=(const SeaJsonData& sjData);

    long getRefCount() const;
    void increaseRefCount();
    void decreaseRefCount();

    long getType() const;
    bool setType(long type);

    bool getLLongValue(llong& llVal) const;
    bool getBoolValue(bool& boolVal) const;
    bool getDoubleValue(double& dbVal) const;
    bool getCharValue(char*& charVal) const;
    SeaJson* getObject(SeaJson* sjObj, const char* key, long length = -1, bool pushBack = false);
    SeaJson* getArray(SeaJson* sjObj, long index = -1);

    bool setLLongValue(const llong& llongVal);
    bool setBoolValue(const bool& boolVal);
    bool setDoubleValue(const double& doubleVal);
    bool setCharValue(const char* charVal, long length = -1);

private:
    void copyData(const SeaJsonData& sjData);
};

}   // namespace SEAJSON

#endif  // SEAJSON_DATA_H_