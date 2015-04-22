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

#include "SeaJsonData.hpp"
#include "../include/public/SeaJson.hpp"

#define _VECTOR_INIT_SIZE   64

namespace SEAJSON {

using namespace std;

SeaJsonData::SeaJsonData()
    : type_(JSON_null)
    , refCount_(1)
{
}

SeaJsonData::~SeaJsonData()
{
    removeData();
}

void SeaJsonData::removeData()
{
    if ((type_ == JSON_char) && val_.charVal_) {
        delete [] val_.charVal_;
    } else if ((type_ >= JSON_object) && val_.vtChildren_) {
        for (size_t i = 0; i < val_.vtChildren_->size(); ++i) {
            (*val_.vtChildren_)[i]->deletedByParent();
        }
        delete val_.vtChildren_;
    }

    type_ = JSON_null;
    val_.llongVal_ = 0;
}

vector<SeaJson*>* SeaJsonData::getChildren() const
{
    return val_.vtChildren_;
}

void SeaJsonData::removeChildLink(SeaJson* child)
{
    for (size_t i = 0; i < val_.vtChildren_->size(); ++i) {
        if ((*val_.vtChildren_)[i] == child) {
            val_.vtChildren_->erase(val_.vtChildren_->begin() + i);
            break;
        }
    }
}

void SeaJsonData::rollback(long index)
{
    for (size_t i = val_.vtChildren_->size() - 1; i >= index; --i) {
        val_.vtChildren_->back()->deletedByParent();
        val_.vtChildren_->pop_back();
    }
}

SeaJsonData& SeaJsonData::operator=(const SeaJsonData& sjData)
{
    if (this != &sjData) {
        copyData(sjData);
    }
    return *this;
}

long SeaJsonData::getRefCount() const
{
    return refCount_;
}

void SeaJsonData::increaseRefCount()
{
    refCount_++;
}

void SeaJsonData::decreaseRefCount()
{
    refCount_--;
}

long SeaJsonData::getType() const
{
    return type_;
}

bool SeaJsonData::setType(long type)
{
    if (type_ <= JSON_null) {
        type_ = type;
        if (type_ >= JSON_object) {
            val_.vtChildren_ = new vector<SeaJson*>;
            val_.vtChildren_->reserve(_VECTOR_INIT_SIZE);
        }
    }

    return (type_ == type);
}

bool SeaJsonData::getLLongValue(llong& llVal) const
{
    if (type_ == JSON_llong) {
        llVal = val_.llongVal_;
    } else if (type_ == JSON_double) {
        llVal = llong(val_.doubleVal_);
    } else {
        return false;
    }

    return true;
}

bool SeaJsonData::getBoolValue(bool& boolVal) const
{
    if (type_ == JSON_bool) {
        boolVal = val_.boolVal_;
        return true;
    }

    return false;
}

bool SeaJsonData::getDoubleValue(double& dbVal) const
{
    if (type_ == JSON_double) {
        dbVal = val_.doubleVal_;
    } else if (type_ == JSON_llong) {
        dbVal = double(val_.llongVal_);
    } else {
        return false;
    }

    return true;
}

bool SeaJsonData::getCharValue(char*& charVal) const
{
    if (type_ == JSON_char) {
        charVal = val_.charVal_;
        return true;
    }

    return false;
}

SeaJson* SeaJsonData::getObject(SeaJson* sjObj, const char* key, long length, bool pushBack)
{
    SeaJson* sjChildObj = NULL;

    if (length == -1) {
        length = strlen(key);
    }

    if (!pushBack) {
        for (size_t i = 0; i < val_.vtChildren_->size(); ++i) {
            sjChildObj = val_.vtChildren_->at(i);
            if (sjChildObj->compareKey(key, length) == 0) {
                return sjChildObj;
            }
        }
    }

    sjChildObj = new SeaJson(this);
    if (sjObj != NULL) {
        sjObj->tossRollbackInfo(sjChildObj, val_.vtChildren_->size());
    }
    sjChildObj->setKey(key, length);
    val_.vtChildren_->push_back(sjChildObj);
    return sjChildObj;
}

SeaJson* SeaJsonData::getArray(SeaJson* sjObj, long index)
{
    SeaJson* sjChildObj = NULL;
    long rbIndex = val_.vtChildren_->size();
    if (index == -1) {
        sjChildObj = new SeaJson(this);
        val_.vtChildren_->push_back(sjChildObj);
    } else if (index < val_.vtChildren_->size()) {
        sjChildObj = val_.vtChildren_->at(index);
        return sjChildObj;  //  do not call "tossRollbackInfo" because this is not created
    } else {
        for (long i = val_.vtChildren_->size(); i <= index; i++) {
            sjChildObj = new SeaJson(this);
            val_.vtChildren_->push_back(sjChildObj);
        }
    }

    if (sjObj != NULL) {
        sjObj->tossRollbackInfo(sjChildObj, rbIndex);
    }

    return sjChildObj;
}

bool SeaJsonData::setLLongValue(const llong& llongVal)
{
    if (setType(JSON_llong)) {
        val_.llongVal_ = llongVal;
        return true;
    }

    return false;
}

bool SeaJsonData::setBoolValue(const bool& boolVal)
{
    if (setType(JSON_bool)) {
        val_.boolVal_ = boolVal;
        return true;
    }

    return false;
}

bool SeaJsonData::setDoubleValue(const double& doubleVal)
{
    if (setType(JSON_double)) {
        val_.doubleVal_ = doubleVal;
        return true;
    }

    return false;
}

bool SeaJsonData::setCharValue(const char* charVal, long length)
{
    if (!setType(JSON_char)) {
        return false;
    }

    if (val_.charVal_) {
        delete [] val_.charVal_;
    }

    if (length == -1) {
        length = strlen(charVal);
    }
    val_.charVal_ = new char[length + 1];
    strncpy(val_.charVal_, charVal, length);
    val_.charVal_[length] = '\0';

    return true;
}

void SeaJsonData::copyData(const SeaJsonData& sjData)
{
    removeData();

    type_ = sjData.type_;
    refCount_ = 1;

    if (type_ == JSON_char) {
        char* srcCharValue = NULL;
        sjData.getCharValue(srcCharValue);
        val_.charVal_ = new char[strlen(srcCharValue) + 1];
        strcpy(val_.charVal_, srcCharValue);
    } else if (type_ >= JSON_object) {
        SeaJson* newSjObj;
        val_.vtChildren_ = new vector<SeaJson*>;
        vector<SeaJson*>* srcChildren = sjData.getChildren();
        for (size_t i = 0; i < srcChildren->size(); ++i) {
            newSjObj = new SeaJson(this);
            *newSjObj = *(srcChildren->at(i));
            val_.vtChildren_->push_back(newSjObj);
        }
    } else {
        val_ = sjData.val_;
    }
}

}   // namespace SEAJSON