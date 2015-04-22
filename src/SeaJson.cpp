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

#include <string.h>

#include <stdexcept>

#include "SeaJsonData.hpp"
#include "../include/public/SeaJson.hpp"

typedef long long llong;

namespace SEAJSON {

// for parse
#define _ARRAY_START    '['
#define _ARRAY_END      ']'
#define _OBJECT_START   '{'
#define _OBJECT_END     '}'
#define _STRING         '"'
#define _ESCAPE         '\\'
#define _KEY_VAL_DELIMI ':'
#define _NEXT           ','
#define _NULL_STRING    '\0'
#define _END_OF_VALUE   " ,]}\r\n\t"
#define _LONGLONG_DIGIT 18

// for stringify
#define _BASE_JSTR_LENGTH   1024
#define _KEY_SYMBOL_SIZE    4
#define _CHAR_SYMBOL_SIZE   2
#define _BOOL_SIZE          6
#define _NULL_SIZE          5
#define _MAX_NUMBER_STRING  32

using namespace std;

SeaJson::SeaJson(SeaJsonData* parent)
    : parent_(parent)
    , key_(NULL)
    , jstr_(NULL)
    , jstrLength_(0)
    , rbInfo_(NULL)
    , seaJsonData_(NULL)
{
    createDataObject();
}

SeaJson::SeaJson()
    : parent_(nullptr)
    , key_(NULL)
    , jstr_(NULL)
    , jstrLength_(0)
    , rbInfo_(NULL)
    , seaJsonData_(NULL)
{
    createDataObject();
}

SeaJson::SeaJson(const char* jstr)
    : parent_(nullptr)
    , key_(NULL)
    , jstr_(NULL)
    , jstrLength_(0)
    , rbInfo_(NULL)
    , seaJsonData_(NULL)
{
    createDataObject();

    trimLeft(jstr);

    parseJson(jstr);

    trimLeft(jstr);
    if (*jstr != _NULL_STRING) {
        throw invalid_argument("JSON string is invalid, there are garbage characters at the end");
    }
}

SeaJson::SeaJson(const SeaJson& sjObj)
    : parent_(nullptr)
    , key_(NULL)
    , jstr_(NULL)
    , jstrLength_(0)
    , rbInfo_(NULL)
    , seaJsonData_(NULL)
{
    // same with operator=(const SeaJson& sjObj) exception checking copying itself
    if (this == &sjObj) {
        throw invalid_argument("JSON cannot copy itself");
    }

    createDataObject();

    setKey(sjObj.getKey());
    *seaJsonData_ = *(sjObj.seaJsonData_);
}

SeaJson::~SeaJson()
{
    if (parent_ != NULL) {
        parent_->removeChildLink(this);
    }

    deleteDataObject();

    if (key_) {
        delete [] key_;
    }
    if (jstr_) {
        delete [] jstr_;
    }
    if (rbInfo_) {
        delete rbInfo_;
    }
}

void SeaJson::tossRollbackInfo(SeaJson* sjObj, long index)
{
    if (rbInfo_ == NULL) {
        sjObj->setRollbackInfo(new RollbackInfo(seaJsonData_, index));
    } else {
        sjObj->setRollbackInfo(rbInfo_);
        rbInfo_ = NULL;
    }
}

void SeaJson::setRollbackInfo(RollbackInfo* rbInfo)
{
    rbInfo_ = rbInfo;
}

void SeaJson::rollback()
{
    if (rbInfo_ == NULL) {
        return;
    }

    RollbackInfo* rbInfo = rbInfo_;
    rbInfo_ = NULL;
    rbInfo->sjData_->rollback(rbInfo->index_);
    delete rbInfo;
}

void SeaJson::commit()
{
    if (rbInfo_ == NULL) {
        return;
    }

    delete rbInfo_;
    rbInfo_ = NULL;
}

void SeaJson::deletedByParent()
{
    parent_ = NULL;
    delete this;
}

void SeaJson::parse(const char* jstr)
{
    trimLeft(jstr);

    seaJsonData_->removeData();

    parseJson(jstr);

    trimLeft(jstr);
    if (*jstr != _NULL_STRING) {
        throw invalid_argument("JSON string is invalid, there are garbage characters at the end");
    }
}

void SeaJson::parseJson(const char*& jstr)
{
    trimLeft(jstr);

    if (*jstr == _STRING) {
        getStringFromJsonString(jstr);
    } else if (strncmp(jstr, "true", 4) == 0) {
        seaJsonData_->setBoolValue(true);
        jstr += 4;
    } else if (strncmp(jstr, "false", 5) == 0) {
        seaJsonData_->setBoolValue(false);
        jstr += 5;
    } else if (strncmp(jstr, "null", 4) == 0) {
        seaJsonData_->setType(JSON_null);
        jstr += 4;
    } else if (*jstr == _OBJECT_START) {
        parseObject(jstr);
        if (*jstr != _OBJECT_END) {
            throw invalid_argument("JSON string is invalid, Object is not closed");
        }
        jstr++;
    } else if (*jstr == _ARRAY_START) {
        parseArray(jstr);
        if (*jstr != _ARRAY_END) {
            throw invalid_argument("JSON string is invalid, Array is not closed");
        }
        jstr++;
    } else if (!getNumberFromJsonString(jstr)) {
        throw invalid_argument("JSON string is invalid");
    }
}

void SeaJson::parseObject(const char*& jstr)
{
    seaJsonData_->setType(JSON_object);
    SeaJson* sjObj = NULL;
    do {
        jstr++;
        const char *jsKey = NULL;
        long length = getKeyFromJsonString(jstr, jsKey);
        sjObj = seaJsonData_->getObject(NULL, jsKey, length);
        sjObj->parseJson(jstr);
        trimLeft(jstr);
    } while (*jstr == _NEXT);
}

void SeaJson::parseArray(const char*& jstr)
{
    seaJsonData_->setType(JSON_array);
    SeaJson* sjObj = NULL;
    do {
        jstr++;
        sjObj = seaJsonData_->getArray(NULL);
        sjObj->parseJson(jstr);
        trimLeft(jstr);
    } while (*jstr == _NEXT);
}

long SeaJson::getKeyFromJsonString(const char*& jstr, const char*& jsKey)
{
    trimLeft(jstr);

    if (*jstr != _STRING) {
        throw invalid_argument("JSON string is invalid");
    }

    long length;
    jsKey = ++jstr;
    while (((*jstr != _STRING) || (*(jstr-1) == _ESCAPE)) && (*jstr != _NULL_STRING)) {
        jstr++;
    }

    if (*jstr == _STRING) {
        length = jstr - jsKey;
        jstr++;
    } else {
        throw invalid_argument("JSON string is invalid");
    }

    trimLeft(jstr);

    if (*jstr == _KEY_VAL_DELIMI) {
        jstr++;
    } else {
        throw invalid_argument("JSON string is invalid");
    }

    return length;
}

void SeaJson::getStringFromJsonString(const char*& jstr)
{
    long length;
    const char* jsVal = ++jstr;
    while (((*jstr != _STRING) || (*(jstr-1) == _ESCAPE)) && (*jstr != _NULL_STRING)) {
        jstr++;
    }

    if (*jstr == _STRING) {
        length = jstr - jsVal;
        jstr++;
    } else {
        throw invalid_argument("JSON string is invalid");
    }

    seaJsonData_->setCharValue(jsVal, length);
}

bool SeaJson::getNumberFromJsonString(const char*& jstr)
{
    const char* jsLastChar = strpbrk(jstr, _END_OF_VALUE);
    char* jsAfterDouble;
    double dbVal = strtod(jstr, &jsAfterDouble);

    if (jsLastChar == jsAfterDouble) {
        if ((jsAfterDouble - jstr) <= _LONGLONG_DIGIT) {
            char* jsAfterInteger;
            llong llVal = strtoll(jstr, &jsAfterInteger, 0);
            if (jsAfterDouble == jsAfterInteger) {
                //cout << "Integer : " << llVal << endl;
                seaJsonData_->setLLongValue(llVal);
                jstr = jsLastChar;
                return true;
            }
        }

        seaJsonData_->setDoubleValue(dbVal);
        jstr = jsLastChar;
        return true;
    }

    return false;
}

const char* SeaJson::stringify()
{
    if (!jstr_) {
        jstr_ = new char[_BASE_JSTR_LENGTH];
        jstrLength_ = _BASE_JSTR_LENGTH;
    }

    long index = 0;
    char *temp = key_;
    key_ = nullptr;
    stringifyJson(jstr_, jstrLength_, index);
    key_ = temp;

    return jstr_;
}

void SeaJson::stringifyJson(char*& jstr, long& jstrBufferLength, long& index)
{
    stringifyKey(jstr, jstrBufferLength, index);

    switch (seaJsonData_->getType()) {
        case JSON_char:
            stringifyChar(jstr, jstrBufferLength, index);
            break;
        case JSON_llong:
            stringifyLLong(jstr, jstrBufferLength, index);
            break;
        case JSON_double:
            stringifyDouble(jstr, jstrBufferLength, index);
            break;
        case JSON_bool:
            stringifyBool(jstr, jstrBufferLength, index);
            break;
        case JSON_object:
            stringifyObject(jstr, jstrBufferLength, index);
            break;
        case JSON_array:
            stringifyArray(jstr, jstrBufferLength, index);
            break;
        case JSON_null:
            stringifyNull(jstr, jstrBufferLength, index);
            break;
    }
}

void SeaJson::stringifyKey(char*& jstr, long& jstrBufferLength, long& index)
{
    long keyLength = getStrLength(key_);
    if (keyLength > 0) {
        keyLength += _KEY_SYMBOL_SIZE;
        increaseJsonStringBuffer(jstr, jstrBufferLength, index, keyLength);

        index += snprintf(jstr + index, keyLength, "\"%s\":", key_);
    }
}

void SeaJson::stringifyObject(char*& jstr, long& jstrBufferLength, long& index)
{
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, 1);
    index += snprintf(jstr + index, _CHAR_SYMBOL_SIZE, "%c", _OBJECT_START);

    vector<SeaJson*>*vtChildren = seaJsonData_->getChildren();
    long vtSize = vtChildren->size();
    for (size_t i = 0; i < vtSize; ++i) {
        (*vtChildren)[i]->stringifyJson(jstr, jstrBufferLength, index);

        increaseJsonStringBuffer(jstr, jstrBufferLength, index, 1);
        index += snprintf(jstr + index, _CHAR_SYMBOL_SIZE, "%c", _NEXT);
    }

    snprintf(jstr + index - 1, _CHAR_SYMBOL_SIZE, "%c", _OBJECT_END);
}

void SeaJson::stringifyArray(char*& jstr, long& jstrBufferLength, long& index)
{
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, 1);
    index += snprintf(jstr + index, _CHAR_SYMBOL_SIZE, "%c", _ARRAY_START);

    vector<SeaJson*>*vtChildren = seaJsonData_->getChildren();
    for (size_t i = 0; i < vtChildren->size(); ++i) {
        (*vtChildren)[i]->stringifyJson(jstr, jstrBufferLength, index);

        increaseJsonStringBuffer(jstr, jstrBufferLength, index, 1);
        index += snprintf(jstr + index, _CHAR_SYMBOL_SIZE, "%c", _NEXT);
    }

    snprintf(jstr + index - 1, _CHAR_SYMBOL_SIZE, "%c", _ARRAY_END);
}

void SeaJson::stringifyNull(char*& jstr, long& jstrBufferLength, long& index)
{
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, _NULL_SIZE);
    index += snprintf(jstr + index, _NULL_SIZE, "null");
}

void SeaJson::stringifyChar(char*& jstr, long& jstrBufferLength, long& index)
{
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, 1);
    index += snprintf(jstr + index, _CHAR_SYMBOL_SIZE, "%c", _STRING);

    char* charVal = NULL;
    seaJsonData_->getCharValue(charVal);
    long strLength = getStrLength(charVal) + 1;
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, strLength);
    index += snprintf(jstr + index, strLength, "%s", charVal);

    increaseJsonStringBuffer(jstr, jstrBufferLength, index, 1);
    index += snprintf(jstr + index, _CHAR_SYMBOL_SIZE, "%c", _STRING);
}

void SeaJson::stringifyLLong(char*& jstr, long& jstrBufferLength, long& index)
{
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, _MAX_NUMBER_STRING);

    llong llVal = 0;
    seaJsonData_->getLLongValue(llVal);
    index += snprintf(jstr + index, _MAX_NUMBER_STRING, "%lld", llVal);
}

void SeaJson::stringifyDouble(char*& jstr, long& jstrBufferLength, long& index)
{
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, _MAX_NUMBER_STRING);

    double dbVal = 0;
    seaJsonData_->getDoubleValue(dbVal);
    index += snprintf(jstr + index, _MAX_NUMBER_STRING, "%.15g", dbVal);
}

void SeaJson::stringifyBool(char*& jstr, long& jstrBufferLength, long& index)
{
    increaseJsonStringBuffer(jstr, jstrBufferLength, index, _BOOL_SIZE);

    bool boolVal = false;
    seaJsonData_->getBoolValue(boolVal);
    if (boolVal) {
        index += snprintf(jstr + index, _BOOL_SIZE, "true");
    } else {
        index += snprintf(jstr + index, _BOOL_SIZE, "false");
    }
}

SeaJsonData* SeaJson::getParent()  const
{
    return parent_;
}

char* SeaJson::getKey() const
{
    return key_;
}

void SeaJson::setKey(const char* key, long length)
{
    if (key == NULL) {
        return;
    }

    if (key_) {
        delete [] key_;
    }

    if (length == -1) {
        length = strlen(key);
    }

    key_ = new char[length + 1];
    strncpy(key_, key, length);
    key_[length] = '\0';
}

long SeaJson::compareKey(const char* key, long length) const
{
    if (length == -1) {
        length = strlen(key);
    }

    long keyLength = strlen(key_);

    return strncmp(key_, key, (keyLength < length)? length : keyLength);
}

bool SeaJson::isValid()
{
    if (rbInfo_ != NULL) {
        rollback();
        return false;
    }

    return true;
}

long SeaJson::getType()
{
    if (rbInfo_ != NULL) {
        rollback();
        return -1;
    }

    return seaJsonData_->getType();
}

void SeaJson::setType(long type)
{
    seaJsonData_->setType(type);
}

void SeaJson::remove()
{
    if (rbInfo_ != NULL) {
        rollback();
        return;
    }

    delete this;
}

void SeaJson::clear()
{
    if (rbInfo_ != NULL) {
        rollback();
        return;
    }

    if (jstr_) {
        delete [] jstr_;
        jstr_ = NULL;
        jstrLength_ = 0;
    }
    if (rbInfo_) {
        delete rbInfo_;
        rbInfo_ = NULL;
    }

    if (seaJsonData_->getRefCount() == 1) {
        delete seaJsonData_;
    } else {
        seaJsonData_->decreaseRefCount();
    }

    seaJsonData_ = new SeaJsonData;
}

// Casting Operators
SeaJson::operator int()
{
    return (int)(this->operator llong());
}

SeaJson::operator unsigned int()
{
    return (unsigned int)(this->operator llong());
}

SeaJson::operator short()
{
    return (short)(this->operator llong());
}

SeaJson::operator unsigned short()
{
    return (unsigned short)(this->operator llong());
}

SeaJson::operator long()
{
    return (long)(this->operator llong());
}

SeaJson::operator unsigned long()
{
    return (unsigned long)(this->operator llong());
}

SeaJson::operator llong()
{
    llong llVal = 0;
    if (seaJsonData_->getLLongValue(llVal)) {
        commit();
        return llVal;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not long long");
    }
}

SeaJson::operator long long unsigned int()
{
    return (long long unsigned int)(this->operator llong());
}

SeaJson::operator bool()
{
    bool boolVal = 0;
    if (seaJsonData_->getBoolValue(boolVal)) {
        commit();
        return boolVal;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not bool");
    }
}

SeaJson::operator double()
{
    double dbVal = 0;
    if (seaJsonData_->getDoubleValue(dbVal)) {
        commit();
        return dbVal;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not double");
    }
}

SeaJson::operator float()
{
    double dbVal = 0;
    if (seaJsonData_->getDoubleValue(dbVal)) {
        commit();
        return float(dbVal);
    } else {
        rollback();
        throw invalid_argument("JSON value type is not float");
    }
}

SeaJson::operator char*()
{
    char* charVal = NULL;
    if (seaJsonData_->getCharValue(charVal)) {
        commit();
        return charVal;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not char*");
    }
}

SeaJson::operator string()
{
    char* charVal = NULL;
    if (seaJsonData_->getCharValue(charVal)) {
        commit();
        return charVal;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not string");
    }
}

/*
// functors
SeaJson& SeaJson::operator()()
{
    return *this;
}

SeaJson& SeaJson::operator()(const char* key)
{
    return this->operator[] (key);
}

SeaJson& SeaJson::operator()(const string& key)
{
    return this->operator[] (key.c_str());
}
*/

// Array Operators
SeaJson& SeaJson::operator[](int index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](unsigned int index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](short index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](unsigned short index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](long index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](unsigned long index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](long long index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](long long unsigned int index)
{
    if (!seaJsonData_->setType(JSON_array)) {
        throw invalid_argument("JSON value type is not array");
    }

    return *(seaJsonData_->getArray(this, index));
}

SeaJson& SeaJson::operator[](const std::string& key)
{
    if (!seaJsonData_->setType(JSON_object)) {
        throw invalid_argument("JSON value type is not object");
    }

    return *(seaJsonData_->getObject(this, key.c_str()));
}

SeaJson& SeaJson::operator[](const char* key)
{
    if (!seaJsonData_->setType(JSON_object)) {
        throw invalid_argument("JSON value type is not object");
    }

    return *(seaJsonData_->getObject(this, key));
}

SeaJson& SeaJson::operator[](char* key)
{
    if (!seaJsonData_->setType(JSON_object)) {
        throw invalid_argument("JSON value type is not object");
    }

    return *(seaJsonData_->getObject(this, key));
}

// Assignment Operators
SeaJson& SeaJson::operator=(int const& nVal)
{
    this->operator=(llong(nVal));
}

SeaJson& SeaJson::operator=(unsigned int const& nVal)
{
    this->operator=(llong(nVal));
}

SeaJson& SeaJson::operator=(const short& nVal)
{
    this->operator=(llong(nVal));
}

SeaJson& SeaJson::operator=(const unsigned short& nVal)
{
    this->operator=(llong(nVal));
}

SeaJson& SeaJson::operator=(const long& nVal)
{
    this->operator=(llong(nVal));
}

SeaJson& SeaJson::operator=(const unsigned long& nVal)
{
    this->operator=(llong(nVal));
}

SeaJson& SeaJson::operator=(const llong& nVal)
{
    if (seaJsonData_->setLLongValue(nVal)) {
        commit();
        return *this;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not long long");
    }
}

SeaJson& SeaJson::operator=(const long long unsigned int& nVal)
{
    this->operator=(llong(nVal));
}

SeaJson& SeaJson::operator=(const char* charVal)
{
    if (seaJsonData_->setCharValue(charVal)) {
        commit();
        return *this;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not char*");
    }
}

SeaJson& SeaJson::operator=(char* charVal)
{
    if (seaJsonData_->setCharValue(charVal)) {
        commit();
        return *this;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not char*");
    }
}

SeaJson& SeaJson::SeaJson::operator=(const bool& boolVal)
{
    if (seaJsonData_->setBoolValue(boolVal)) {
        commit();
        return *this;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not bool");
    }
}

SeaJson& SeaJson::operator=(const double& doubleVal)
{
    if (seaJsonData_->setDoubleValue(doubleVal)) {
        commit();
        return *this;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not double");
    }
}

SeaJson& SeaJson::operator=(const float& floatVal)
{
    if (seaJsonData_->setDoubleValue((double)floatVal)) {
        commit();
        return *this;
    } else {
        rollback();
        throw invalid_argument("JSON value type is not float");
    }
}

SeaJson& SeaJson::operator=(const string& strVal)
{
    return this->operator=(strVal.c_str());
}

SeaJson& SeaJson::operator=(const SeaJson& sjObj)
{
    if (this == &sjObj) {
        throw invalid_argument("JSON cannot copy itself");
    }

    setKey(sjObj.getKey());
    *seaJsonData_ = *(sjObj.seaJsonData_);

    return *this;
}

// Composite Assignment Operators
SeaJson& SeaJson::operator+=(const SeaJson& sjObj)
{
    if (this == &sjObj) {
        throw invalid_argument("JSON cannot append itself");
    }

    deleteDataObject();

    seaJsonData_ = sjObj.seaJsonData_;
    seaJsonData_->increaseRefCount();

    return *this;
}

// Comparison Operations
bool SeaJson::operator==(int const& nVal)
{
    return this->operator==(llong(nVal));
}

bool SeaJson::operator==(unsigned int const& nVal)
{
    return this->operator==(llong(nVal));
}

bool SeaJson::operator==(const short& nVal)
{
    return this->operator==(llong(nVal));
}

bool SeaJson::operator==(const unsigned short& nVal)
{
    return this->operator==(llong(nVal));
}

bool SeaJson::operator==(const long& nVal)
{
    return this->operator==(llong(nVal));
}

bool SeaJson::operator==(const unsigned long& nVal)
{
    return this->operator==(llong(nVal));
}

bool SeaJson::operator==(const llong& nVal)
{
    llong cmpVal = 0;
    if (seaJsonData_->getLLongValue(cmpVal)) {
        commit();
        return (cmpVal == nVal);
    } else {
        rollback();
        return false;
    }
}

bool SeaJson::operator==(const long long unsigned int& nVal)
{
    return this->operator==(llong(nVal));
}

bool SeaJson::operator==(const bool& boolVal)
{
    bool cmpVal = 0;
    if (seaJsonData_->getBoolValue(cmpVal)) {
        commit();
        return (cmpVal == boolVal);
    } else {
        rollback();
        return false;
    }
}

bool SeaJson::operator==(const double& doubleVal)
{
    double cmpVal = 0;
    if (seaJsonData_->getDoubleValue(cmpVal)) {
        commit();
        return (cmpVal == doubleVal);
    } else {
        rollback();
        return false;
    }
}

bool SeaJson::operator==(const float& floatVal)
{
    double cmpVal = 0;
    if (seaJsonData_->getDoubleValue(cmpVal)) {
        commit();
        return (float(cmpVal) == floatVal);
    } else {
        rollback();
        return false;
    }
}

bool SeaJson::operator==(const char* charVal)
{
    char* cmpVal = NULL;
    if (seaJsonData_->getCharValue(cmpVal)) {
        commit();
        long strLength = strlen(charVal);
        long cmpLength = strlen(cmpVal);
        return (strncmp(cmpVal, charVal, (strLength > cmpLength)? strLength : cmpLength) == 0);
    } else {
        rollback();
        return false;
    }
}

bool SeaJson::operator==(char* charVal)
{
    char* cmpVal = NULL;
    if (seaJsonData_->getCharValue(cmpVal)) {
        commit();
        long strLength = strlen(charVal);
        long cmpLength = strlen(cmpVal);
        return (strncmp(cmpVal, charVal, (strLength > cmpLength)? strLength : cmpLength) == 0);
    } else {
        rollback();
        return false;
    }
}

bool SeaJson::operator==(const string& strVal)
{
    return this->operator==(strVal.c_str());
}



bool SeaJson::operator!=(int const& nVal)
{
    return !this->operator==(llong(nVal));
}

bool SeaJson::operator!=(unsigned int const& nVal)
{
    return !this->operator==(llong(nVal));
}

bool SeaJson::operator!=(const short& nVal)
{
    return !this->operator==(llong(nVal));
}

bool SeaJson::operator!=(const unsigned short& nVal)
{
    return !this->operator==(llong(nVal));
}

bool SeaJson::operator!=(const long& nVal)
{
    return !this->operator==(llong(nVal));
}

bool SeaJson::operator!=(const unsigned long& nVal)
{
    return !this->operator==(llong(nVal));
}

bool SeaJson::operator!=(const llong& nVal)
{
    return !this->operator==(nVal);
}

bool SeaJson::operator!=(const long long unsigned int& nVal)
{
    return !this->operator==(llong(nVal));
}

bool SeaJson::operator!=(const bool& boolVal)
{
    return !this->operator==(boolVal);
}

bool SeaJson::operator!=(const double& doubleVal)
{
    return !this->operator==(doubleVal);
}

bool SeaJson::operator!=(const float& floatVal)
{
    return !this->operator==(floatVal);
}

bool SeaJson::operator!=(const char* charVal)
{
    return !this->operator==(charVal);
}

bool SeaJson::operator!=(char* charVal)
{
    return !this->operator==(charVal);
}

bool SeaJson::operator!=(const string& strVal)
{
    return !this->operator==(strVal.c_str());
}

bool SeaJson::createDataObject()
{
    if (!seaJsonData_) {
        seaJsonData_ = new SeaJsonData;
        return true;
    }
    return false;
}

void SeaJson::deleteDataObject()
{
    if (seaJsonData_->getRefCount() == 1) {
        delete seaJsonData_;
        seaJsonData_ = NULL;
    } else {
        seaJsonData_->decreaseRefCount();
    }
}

void SeaJson::trimLeft(const char*& jstr)
{
    while ((*jstr == ' ') || (*jstr == '\r') || (*jstr == '\n') || (*jstr == '\t')) {
        jstr++;
    }
}

void SeaJson::increaseJsonStringBuffer(char*& jstr, long& jstrBufferLength, long index, long appendStrLength)
{
    if ((jstrBufferLength - index) >= appendStrLength) {
        return;
    }

    char* newJstr = new char[jstrBufferLength + _BASE_JSTR_LENGTH];
    memcpy(newJstr, jstr, jstrBufferLength);
    jstrBufferLength += _BASE_JSTR_LENGTH;
    delete [] jstr;
    jstr = newJstr;
}

long SeaJson::getStrLength(char*& charVale)
{
    if (charVale == NULL) {
        return 0;
    }
    return strlen(charVale);
}

}   // namespace SEAJSON