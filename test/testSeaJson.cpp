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
#include <float.h>
#include <sys/time.h>

#include <iostream>
#include <stdexcept>
#include <string>

#include "../include/public/SeaJson.hpp"

typedef long long llong;

#define REPEAT_COUNT    10000

using namespace SEAJSON;
using namespace std;

enum ValType {
    JSON_null = 0,
    JSON_char,
    JSON_llong,
    JSON_double,
    JSON_bool,
    JSON_object,
    JSON_array
};

unsigned long usecs = 0;
unsigned long usecs2 = 0;
long usecsOffset = 0;
struct timeval tvStart, tvEnd;

long iterationNumber = REPEAT_COUNT;
long nTestCase = 0;
long nSuccess = 0;
long nFailure = 0;

char longJsStr[] = "{\r\n    \"web-app\": {\r\n        \"servlet\": [\r\n            {\r\n                \"servlet-name\": \"cofaxCDS\",\r\n                \"servlet-class\": \"org.cofax.cds.CDSServlet\",\r\n                \"init-param\": {\r\n                    \"configGlossary:installationAt\": \"Philadelphia, PA\",\r\n                    \"configGlossary:adminEmail\": \"ksm@pobox.com\",\r\n                    \"configGlossary:poweredBy\": \"Cofax\",\r\n                    \"configGlossary:poweredByIcon\": \"/images/cofax.gif\",\r\n                    \"configGlossary:staticPath\": \"/content/static\",\r\n                    \"templateProcessorClass\": \"org.cofax.WysiwygTemplate\",\r\n                    \"templateLoaderClass\": \"org.cofax.FilesTemplateLoader\",\r\n                    \"templatePath\": \"templates\",\r\n                    \"templateOverridePath\": \"\",\r\n                    \"defaultListTemplate\": \"listTemplate.htm\",\r\n                    \"defaultFileTemplate\": \"articleTemplate.htm\",\r\n                    \"useJSP\": false,\r\n                    \"jspListTemplate\": \"listTemplate.jsp\",\r\n                    \"jspFileTemplate\": \"articleTemplate.jsp\",\r\n                    \"cachePackageTagsTrack\": 200,\r\n                    \"cachePackageTagsStore\": 200,\r\n                    \"cachePackageTagsRefresh\": 60,\r\n                    \"cacheTemplatesTrack\": 100,\r\n                    \"cacheTemplatesStore\": 50,\r\n                    \"cacheTemplatesRefresh\": 15,\r\n                    \"cachePagesTrack\": 200,\r\n                    \"cachePagesStore\": 100,\r\n                    \"cachePagesRefresh\": 10,\r\n                    \"cachePagesDirtyRead\": 10,\r\n                    \"searchEngineListTemplate\": \"forSearchEnginesList.htm\",\r\n                    \"searchEngineFileTemplate\": \"forSearchEngines.htm\",\r\n                    \"searchEngineRobotsDb\": \"WEB-INF/robots.db\",\r\n                    \"useDataStore\": true,\r\n                    \"dataStoreClass\": \"org.cofax.SqlDataStore\",\r\n                    \"redirectionClass\": \"org.cofax.SqlRedirection\",\r\n                    \"dataStoreName\": \"cofax\",\r\n                    \"dataStoreDriver\": \"com.microsoft.jdbc.sqlserver.SQLServerDriver\",\r\n                    \"dataStoreUrl\": \"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",\r\n                    \"dataStoreUser\": \"sa\",\r\n                    \"dataStorePassword\": \"dataStoreTestQuery\",\r\n                    \"dataStoreTestQuery\": \"SET NOCOUNT ON;select test='test';\",\r\n                    \"dataStoreLogFile\": \"/usr/local/tomcat/logs/datastore.log\",\r\n                    \"dataStoreInitConns\": 10,\r\n                    \"dataStoreMaxConns\": 100,\r\n                    \"dataStoreConnUsageLimit\": 100,\r\n                    \"dataStoreLogLevel\": \"debug\",\r\n                    \"maxUrlLength\": 500\r\n                }\r\n            },\r\n            {\r\n                \"servlet-name\": \"cofaxEmail\",\r\n                \"servlet-class\": \"org.cofax.cds.EmailServlet\",\r\n                \"init-param\": {\r\n                    \"mailHost\": \"mail1\",\r\n                    \"mailHostOverride\": \"mail2\"\r\n                }\r\n            },\r\n            {\r\n                \"servlet-name\": \"cofaxAdmin\",\r\n                \"servlet-class\": \"org.cofax.cds.AdminServlet\"\r\n            },\r\n            {\r\n                \"servlet-name\": \"fileServlet\",\r\n                \"servlet-class\": \"org.cofax.cds.FileServlet\"\r\n            },\r\n            {\r\n                \"servlet-name\": \"cofaxTools\",\r\n                \"servlet-class\": \"org.cofax.cms.CofaxToolsServlet\",\r\n                \"init-param\": {\r\n                    \"templatePath\": \"toolstemplates/\",\r\n                    \"log\": 1,\r\n                    \"logLocation\": \"/usr/local/tomcat/logs/CofaxTools.log\",\r\n                    \"logMaxSize\": \"\",\r\n                    \"dataLog\": 1,\r\n                    \"dataLogLocation\": \"/usr/local/tomcat/logs/dataLog.log\",\r\n                    \"dataLogMaxSize\": \"\",\r\n                    \"removePageCache\": \"/content/admin/remove?cache=pages&id=\",\r\n                    \"removeTemplateCache\": \"/content/admin/remove?cache=templates&id=\",\r\n                    \"fileTransferFolder\": \"/usr/local/tomcat/webapps/content/fileTransferFolder\",\r\n                    \"lookInContext\": 1,\r\n                    \"adminGroupID\": 4,\r\n                    \"betaServer\": true\r\n                }\r\n            }\r\n        ],\r\n        \"servlet-mapping\": {\r\n            \"cofaxCDS\": \"/\",\r\n            \"cofaxEmail\": \"/cofaxutil/aemail/*\",\r\n            \"cofaxAdmin\": \"/admin/*\",\r\n            \"fileServlet\": \"/static/*\",\r\n            \"cofaxTools\": \"/tools/*\"\r\n        },\r\n        \"taglib\": {\r\n            \"taglib-uri\": \"cofax.tld\",\r\n            \"taglib-location\": \"/WEB-INF/tlds/cofax.tld\"\r\n        }\r\n    }\r\n}";
char longJsStrOrig[] = "{\"web-app\":{\"servlet\":[{\"servlet-name\":\"cofaxCDS\",\"servlet-class\":\"org.cofax.cds.CDSServlet\",\"init-param\":{\"configGlossary:installationAt\":\"Philadelphia, PA\",\"configGlossary:adminEmail\":\"ksm@pobox.com\",\"configGlossary:poweredBy\":\"Cofax\",\"configGlossary:poweredByIcon\":\"/images/cofax.gif\",\"configGlossary:staticPath\":\"/content/static\",\"templateProcessorClass\":\"org.cofax.WysiwygTemplate\",\"templateLoaderClass\":\"org.cofax.FilesTemplateLoader\",\"templatePath\":\"templates\",\"templateOverridePath\":\"\",\"defaultListTemplate\":\"listTemplate.htm\",\"defaultFileTemplate\":\"articleTemplate.htm\",\"useJSP\":false,\"jspListTemplate\":\"listTemplate.jsp\",\"jspFileTemplate\":\"articleTemplate.jsp\",\"cachePackageTagsTrack\":200,\"cachePackageTagsStore\":200,\"cachePackageTagsRefresh\":60,\"cacheTemplatesTrack\":100,\"cacheTemplatesStore\":50,\"cacheTemplatesRefresh\":15,\"cachePagesTrack\":200,\"cachePagesStore\":100,\"cachePagesRefresh\":10,\"cachePagesDirtyRead\":10,\"searchEngineListTemplate\":\"forSearchEnginesList.htm\",\"searchEngineFileTemplate\":\"forSearchEngines.htm\",\"searchEngineRobotsDb\":\"WEB-INF/robots.db\",\"useDataStore\":true,\"dataStoreClass\":\"org.cofax.SqlDataStore\",\"redirectionClass\":\"org.cofax.SqlRedirection\",\"dataStoreName\":\"cofax\",\"dataStoreDriver\":\"com.microsoft.jdbc.sqlserver.SQLServerDriver\",\"dataStoreUrl\":\"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",\"dataStoreUser\":\"sa\",\"dataStorePassword\":\"dataStoreTestQuery\",\"dataStoreTestQuery\":\"SET NOCOUNT ON;select test='test';\",\"dataStoreLogFile\":\"/usr/local/tomcat/logs/datastore.log\",\"dataStoreInitConns\":10,\"dataStoreMaxConns\":100,\"dataStoreConnUsageLimit\":100,\"dataStoreLogLevel\":\"debug\",\"maxUrlLength\":500}},{\"servlet-name\":\"cofaxEmail\",\"servlet-class\":\"org.cofax.cds.EmailServlet\",\"init-param\":{\"mailHost\":\"mail1\",\"mailHostOverride\":\"mail2\"}},{\"servlet-name\":\"cofaxAdmin\",\"servlet-class\":\"org.cofax.cds.AdminServlet\"},{\"servlet-name\":\"fileServlet\",\"servlet-class\":\"org.cofax.cds.FileServlet\"},{\"servlet-name\":\"cofaxTools\",\"servlet-class\":\"org.cofax.cms.CofaxToolsServlet\",\"init-param\":{\"templatePath\":\"toolstemplates/\",\"log\":1,\"logLocation\":\"/usr/local/tomcat/logs/CofaxTools.log\",\"logMaxSize\":\"\",\"dataLog\":1,\"dataLogLocation\":\"/usr/local/tomcat/logs/dataLog.log\",\"dataLogMaxSize\":\"\",\"removePageCache\":\"/content/admin/remove?cache=pages&id=\",\"removeTemplateCache\":\"/content/admin/remove?cache=templates&id=\",\"fileTransferFolder\":\"/usr/local/tomcat/webapps/content/fileTransferFolder\",\"lookInContext\":1,\"adminGroupID\":4,\"betaServer\":true}}],\"servlet-mapping\":{\"cofaxCDS\":\"/\",\"cofaxEmail\":\"/cofaxutil/aemail/*\",\"cofaxAdmin\":\"/admin/*\",\"fileServlet\":\"/static/*\",\"cofaxTools\":\"/tools/*\"},\"taglib\":{\"taglib-uri\":\"cofax.tld\",\"taglib-location\":\"/WEB-INF/tlds/cofax.tld\"}}}";

char shortJsStr[] = "{\r\n    \"menu\": {\r\n        \"id\": \"file\",\r\n        \"id\":\"file\",\r\n       \"value\": \"File\",\r\n        \"size\": 1234,\r\n        \"popup\": {\r\n            \"menuitem\": [\r\n                {\r\n                    \"value\": \"New\",\r\n                    \"onclick\": \"CreateNewDoc()\"\r\n                },\r\n                {\r\n                    \"value\": \"Open\",\r\n                    \"onclick\": \"OpenDoc()\"\r\n                },\r\n                {\r\n                    \"value\": \"Close\",\r\n                    \"onclick\": \"CloseDoc()\"\r\n                }\r\n            ]\r\n        }\r\n    }\r\n}";
char shortJsStrOrig[] = "{\"menu\":{\"id\":\"file\",\"value\":\"File\",\"size\":1234,\"popup\":{\"menuitem\":[{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"},{\"value\":\"Open\",\"onclick\":\"OpenDoc()\"},{\"value\":\"Close\",\"onclick\":\"CloseDoc()\"}]}}}";

/*
void measureTimeToCalDiffTime()
{
    gettimeofday(&tvStart, NULL);

    for (int i=1; i<100; i++)
        gettimeofday(&tvEnd, NULL);

    usecsOffset = ((tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec)) / 100;

    cout << "Average of Offset Time : " << usecsOffset << endl;
}
*/

void compareResult(const char* origStr, const char* cmpStr)
{
    cout << "Compare JSON string" << endl;
    cout << "\tOriginal\n\t\t" << origStr << endl;
    cout << "\tComposite\n\t\t" << cmpStr << endl;

    long cmpResult = strcmp(origStr, cmpStr);
    cout << "\tComposition Result : " << ((cmpResult == 0)? "OK" : "Fail") << endl;

    if (cmpResult != 0) {
        int index;
        for (index=0; (origStr[index] != '\0') && (cmpStr[index] != '\0'); index++) {
            if (origStr[index] != cmpStr[index]) break;
        }
        cout << "\tDifferent character is " << index << "th. original : " << origStr[index] << ", composed : " << cmpStr[index] << endl;
    }

    cout << endl;
}

void checkUnitTestResult(const char* unitTest, const char* expectedStr, const char* resultStr)
{
    nTestCase++;

    cout << "\tTest Case " << nTestCase << " : "<< unitTest << endl;
    cout << "\t\tExpected : " << expectedStr << endl;
    cout << "\t\tResult   : " << resultStr << endl;

    long cmpResult = strcmp(expectedStr, resultStr);
    cout << "\t\t" << unitTest << " : " << ((cmpResult == 0)? "OK" : "Fail") << endl;

    if (cmpResult == 0) {
        nSuccess++;
    } else {
        int index;
        for (index=0; (expectedStr[index] != '\0') && (resultStr[index] != '\0'); index++) {
            if (expectedStr[index] != resultStr[index]) break;
        }
        cout << "\t\tDifferent character is " << index << "th. expected string : " << expectedStr[index] << ", result string : " << resultStr[index] << endl;
        nFailure++;
    }

    cout << endl;
}

void checkUnitTestResult(const char* unitTest, bool expectedVal, bool resultVal)
{
    nTestCase++;

    cout << "\tTest Case " << nTestCase << " : "<< unitTest << endl;
    cout << "\t\tExpected : " << expectedVal << endl;
    cout << "\t\tResult   : " << resultVal << endl;

    bool cmpResult = expectedVal == resultVal;
    cout << "\t\t" << unitTest << " : " << (cmpResult? "OK" : "Fail") << endl;

    if (cmpResult != 0) {
        nSuccess++;
    } else {
        cout << "\t\tExpected value is " << (expectedVal? "true" : "false") << ", result value : " << (resultVal? "true" : "false") << endl;
        nFailure++;
    }

    cout << endl;
}

template <typename T>
void checkUnitTestResult(const char* unitTest, T expectedVal, T resultVal)
{
    nTestCase++;

    cout << "\tTest Case " << nTestCase << " : "<< unitTest << endl;
    cout << "\t\tExpected : " << expectedVal << endl;
    cout << "\t\tResult   : " << resultVal << endl;

    bool cmpResult = expectedVal == resultVal;
    cout << "\t\t" << unitTest << " : " << (cmpResult? "OK" : "Fail") << endl;

    if (cmpResult) {
        nSuccess++;
    } else {
        cout << "\t\tExpected value is " << expectedVal << ", result value : " << resultVal << endl;
        nFailure++;
    }

    cout << endl;
}

void parseLongJson()
{
    usecs = 0;
    usecs2 = 0;
    for (int i=0; i<iterationNumber; i++) {
        SeaJson sjObj;
        gettimeofday(&tvStart, NULL);

        sjObj.parse(longJsStr);

        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;

        // stringify after parsing
        gettimeofday(&tvStart, NULL);

        sjObj.stringify();

        gettimeofday(&tvEnd, NULL);
        usecs2 += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to parse long JSON string (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;
    cout << "Total Time to stringify after parsing long JSON string (Repeat Count : " << iterationNumber << ") = " << usecs2 << " usecs" << endl;

    SeaJson sjObj;
    sjObj.parse(longJsStr);
    const char* cmpStr = sjObj.stringify();
    compareResult(longJsStrOrig, cmpStr);
}

void parseShortJson()
{
    usecs = 0;
    usecs2 = 0;
    for (int i=0; i<iterationNumber; i++) {
        SeaJson sjObj;
        gettimeofday(&tvStart, NULL);

        sjObj.parse(shortJsStr);

        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;

        // stringify after parsing
        gettimeofday(&tvStart, NULL);

        sjObj.stringify();

        gettimeofday(&tvEnd, NULL);
        usecs2 += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to parse short JSON string (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;
    cout << "Total Time to stringify after parsing short JSON string (Repeat Count : " << iterationNumber << ") = " << usecs2 << " usecs" << endl;

    SeaJson sjObj;
    sjObj.parse(shortJsStr);
    const char* cmpStr = sjObj.stringify();
    compareResult(shortJsStrOrig, cmpStr);
}

void stringifyLongJson()
{
    usecs = 0;
    for (int i=0; i<iterationNumber; i++) {
        SeaJson sjObj(longJsStr);
        gettimeofday(&tvStart, NULL);

        sjObj.stringify();

        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to stringify long JSON (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;

    SeaJson sjObj(longJsStr);
    const char* cmpStr = sjObj.stringify();
    compareResult(longJsStrOrig, cmpStr);
}

void stringifyShortJson()
{
    usecs = 0;
    for (int i=0; i<iterationNumber; i++) {
        SeaJson sjObj(shortJsStr);
        gettimeofday(&tvStart, NULL);

        sjObj.stringify();

        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to stringify short JSON (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;

    SeaJson sjObj(shortJsStr);
    const char* cmpStr = sjObj.stringify();
    compareResult(shortJsStrOrig, cmpStr);
}

void composeLongJson()
{
    usecs = 0;
    usecs2 = 0;
    for (int i=0; i<iterationNumber; i++) {
        gettimeofday(&tvStart, NULL);

        SeaJson root;
        SeaJson webApp;
        root["web-app"] += webApp;

        SeaJson servlet;
        webApp["servlet"] += servlet;

        servlet[0]["servlet-name"] = "cofaxCDS";
        servlet[0]["servlet-class"] = "org.cofax.cds.CDSServlet";

        SeaJson initParam0;
        servlet[0]["init-param"] += initParam0; // this can be appended after all sub data are set.

        initParam0["configGlossary:installationAt"] = "Philadelphia, PA";
        initParam0["configGlossary:adminEmail"] = "ksm@pobox.com";
        initParam0["configGlossary:poweredBy"] = "Cofax";
        initParam0["configGlossary:poweredByIcon"] = "/images/cofax.gif";
        initParam0["configGlossary:staticPath"] = "/content/static";
        initParam0["templateProcessorClass"] = "org.cofax.WysiwygTemplate";
        initParam0["templateLoaderClass"] = "org.cofax.FilesTemplateLoader";
        initParam0["templatePath"] = "templates";
        initParam0["templateOverridePath"] = "";
        initParam0["defaultListTemplate"] = "listTemplate.htm";
        initParam0["defaultFileTemplate"] = "articleTemplate.htm";
        initParam0["useJSP"] = false;
        initParam0["jspListTemplate"] = "listTemplate.jsp";
        initParam0["jspFileTemplate"] = "articleTemplate.jsp";
        initParam0["cachePackageTagsTrack"] = 200;
        initParam0["cachePackageTagsStore"] = 200;
        initParam0["cachePackageTagsRefresh"] = 60;
        initParam0["cacheTemplatesTrack"] = 100;
        initParam0["cacheTemplatesStore"] = 50;
        initParam0["cacheTemplatesRefresh"] = 15;
        initParam0["cachePagesTrack"] = 200;
        initParam0["cachePagesStore"] = 100;
        initParam0["cachePagesRefresh"] = 10;
        initParam0["cachePagesDirtyRead"] = 10;
        initParam0["searchEngineListTemplate"] = "forSearchEnginesList.htm";
        initParam0["searchEngineFileTemplate"] = "forSearchEngines.htm";
        initParam0["searchEngineRobotsDb"] = "WEB-INF/robots.db";
        initParam0["useDataStore"] = true;
        initParam0["dataStoreClass"] = "org.cofax.SqlDataStore";
        initParam0["redirectionClass"] = "org.cofax.SqlRedirection";
        initParam0["dataStoreName"] = "cofax";
        initParam0["dataStoreDriver"] = "com.microsoft.jdbc.sqlserver.SQLServerDriver";
        initParam0["dataStoreUrl"] = "jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon";
        initParam0["dataStoreUser"] = "sa";
        initParam0["dataStorePassword"] = "dataStoreTestQuery";
        initParam0["dataStoreTestQuery"] = "SET NOCOUNT ON;select test='test';";
        initParam0["dataStoreLogFile"] = "/usr/local/tomcat/logs/datastore.log";
        initParam0["dataStoreInitConns"] = 10;
        initParam0["dataStoreMaxConns"] = 100;
        initParam0["dataStoreConnUsageLimit"] = 100;
        initParam0["dataStoreLogLevel"] = "debug";
        initParam0["maxUrlLength"] = 500;

        SeaJson servlet1;
        servlet[1] += servlet1;
        servlet1["servlet-name"] = "cofaxEmail";
        servlet1["servlet-class"] = "org.cofax.cds.EmailServlet";

        SeaJson initParam1;
        servlet1["init-param"] += initParam1;
        initParam1["mailHost"] = "mail1";
        initParam1["mailHostOverride"] = "mail2";

        servlet[2]["servlet-name"] = "cofaxAdmin";
        servlet[2]["servlet-class"] = "org.cofax.cds.AdminServlet";

        servlet[3]["servlet-name"] = "fileServlet";
        servlet[3]["servlet-class"] = "org.cofax.cds.FileServlet";

        SeaJson servlet4;
        servlet[4] += servlet4;
        servlet4["servlet-name"] = "cofaxTools";
        servlet4["servlet-class"] = "org.cofax.cms.CofaxToolsServlet";

        SeaJson initParam4;
        servlet4["init-param"] += initParam4;
        initParam4["templatePath"] = "toolstemplates/";
        initParam4["log"] = 1;
        initParam4["logLocation"] = "/usr/local/tomcat/logs/CofaxTools.log";
        initParam4["logMaxSize"] = "";
        initParam4["dataLog"] = 1;
        initParam4["dataLogLocation"] = "/usr/local/tomcat/logs/dataLog.log";
        initParam4["dataLogMaxSize"] = "";
        initParam4["removePageCache"] = "/content/admin/remove?cache=pages&id=";
        initParam4["removeTemplateCache"] = "/content/admin/remove?cache=templates&id=";
        initParam4["fileTransferFolder"] = "/usr/local/tomcat/webapps/content/fileTransferFolder";
        initParam4["lookInContext"] = 1;
        initParam4["adminGroupID"] = 4;
        initParam4["betaServer"] = true;

        SeaJson servletMapping;
        webApp["servlet-mapping"] += servletMapping;
        servletMapping["cofaxCDS"] = "/";
        servletMapping["cofaxEmail"] = "/cofaxutil/aemail/*";
        servletMapping["cofaxAdmin"] = "/admin/*";
        servletMapping["fileServlet"] = "/static/*";
        servletMapping["cofaxTools"] = "/tools/*";

        SeaJson taglib;
        webApp["taglib"] += taglib;
        taglib["taglib-uri"] = "cofax.tld";
        taglib["taglib-location"] = "/WEB-INF/tlds/cofax.tld";

        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;

        // stringify after compose
        gettimeofday(&tvStart, NULL);

        root.stringify();

        gettimeofday(&tvEnd, NULL);
        usecs2 += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to compose long JSON (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;
    cout << "Total Time to stringify after composing long JSON (Repeat Count : " << iterationNumber << ") = " << usecs2 << " usecs" << endl;

    SeaJson root;
        SeaJson webApp;
        root["web-app"] += webApp;

        SeaJson servlet;
        webApp["servlet"] += servlet;

        servlet[0]["servlet-name"] = "cofaxCDS";
        servlet[0]["servlet-class"] = "org.cofax.cds.CDSServlet";

        SeaJson initParam0;
        servlet[0]["init-param"] += initParam0; // this can be appended after all sub data are set.

        initParam0["configGlossary:installationAt"] = "Philadelphia, PA";
        initParam0["configGlossary:adminEmail"] = "ksm@pobox.com";
        initParam0["configGlossary:poweredBy"] = "Cofax";
        initParam0["configGlossary:poweredByIcon"] = "/images/cofax.gif";
        initParam0["configGlossary:staticPath"] = "/content/static";
        initParam0["templateProcessorClass"] = "org.cofax.WysiwygTemplate";
        initParam0["templateLoaderClass"] = "org.cofax.FilesTemplateLoader";
        initParam0["templatePath"] = "templates";
        initParam0["templateOverridePath"] = "";
        initParam0["defaultListTemplate"] = "listTemplate.htm";
        initParam0["defaultFileTemplate"] = "articleTemplate.htm";
        initParam0["useJSP"] = false;
        initParam0["jspListTemplate"] = "listTemplate.jsp";
        initParam0["jspFileTemplate"] = "articleTemplate.jsp";
        initParam0["cachePackageTagsTrack"] = 200;
        initParam0["cachePackageTagsStore"] = 200;
        initParam0["cachePackageTagsRefresh"] = 60;
        initParam0["cacheTemplatesTrack"] = 100;
        initParam0["cacheTemplatesStore"] = 50;
        initParam0["cacheTemplatesRefresh"] = 15;
        initParam0["cachePagesTrack"] = 200;
        initParam0["cachePagesStore"] = 100;
        initParam0["cachePagesRefresh"] = 10;
        initParam0["cachePagesDirtyRead"] = 10;
        initParam0["searchEngineListTemplate"] = "forSearchEnginesList.htm";
        initParam0["searchEngineFileTemplate"] = "forSearchEngines.htm";
        initParam0["searchEngineRobotsDb"] = "WEB-INF/robots.db";
        initParam0["useDataStore"] = true;
        initParam0["dataStoreClass"] = "org.cofax.SqlDataStore";
        initParam0["redirectionClass"] = "org.cofax.SqlRedirection";
        initParam0["dataStoreName"] = "cofax";
        initParam0["dataStoreDriver"] = "com.microsoft.jdbc.sqlserver.SQLServerDriver";
        initParam0["dataStoreUrl"] = "jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon";
        initParam0["dataStoreUser"] = "sa";
        initParam0["dataStorePassword"] = "dataStoreTestQuery";
        initParam0["dataStoreTestQuery"] = "SET NOCOUNT ON;select test='test';";
        initParam0["dataStoreLogFile"] = "/usr/local/tomcat/logs/datastore.log";
        initParam0["dataStoreInitConns"] = 10;
        initParam0["dataStoreMaxConns"] = 100;
        initParam0["dataStoreConnUsageLimit"] = 100;
        initParam0["dataStoreLogLevel"] = "debug";
        initParam0["maxUrlLength"] = 500;

        SeaJson servlet1;
        servlet[1] += servlet1;
        servlet1["servlet-name"] = "cofaxEmail";
        servlet1["servlet-class"] = "org.cofax.cds.EmailServlet";

        SeaJson initParam1;
        servlet1["init-param"] += initParam1;
        initParam1["mailHost"] = "mail1";
        initParam1["mailHostOverride"] = "mail2";

        servlet[2]["servlet-name"] = "cofaxAdmin";
        servlet[2]["servlet-class"] = "org.cofax.cds.AdminServlet";

        servlet[3]["servlet-name"] = "fileServlet";
        servlet[3]["servlet-class"] = "org.cofax.cds.FileServlet";

        SeaJson servlet4;
        servlet[4] += servlet4;
        servlet4["servlet-name"] = "cofaxTools";
        servlet4["servlet-class"] = "org.cofax.cms.CofaxToolsServlet";

        SeaJson initParam4;
        servlet4["init-param"] += initParam4;
        initParam4["templatePath"] = "toolstemplates/";
        initParam4["log"] = 1;
        initParam4["logLocation"] = "/usr/local/tomcat/logs/CofaxTools.log";
        initParam4["logMaxSize"] = "";
        initParam4["dataLog"] = 1;
        initParam4["dataLogLocation"] = "/usr/local/tomcat/logs/dataLog.log";
        initParam4["dataLogMaxSize"] = "";
        initParam4["removePageCache"] = "/content/admin/remove?cache=pages&id=";
        initParam4["removeTemplateCache"] = "/content/admin/remove?cache=templates&id=";
        initParam4["fileTransferFolder"] = "/usr/local/tomcat/webapps/content/fileTransferFolder";
        initParam4["lookInContext"] = 1;
        initParam4["adminGroupID"] = 4;
        initParam4["betaServer"] = true;

        SeaJson servletMapping;
        webApp["servlet-mapping"] += servletMapping;
        servletMapping["cofaxCDS"] = "/";
        servletMapping["cofaxEmail"] = "/cofaxutil/aemail/*";
        servletMapping["cofaxAdmin"] = "/admin/*";
        servletMapping["fileServlet"] = "/static/*";
        servletMapping["cofaxTools"] = "/tools/*";

        SeaJson taglib;
        webApp["taglib"] += taglib;
        taglib["taglib-uri"] = "cofax.tld";
        taglib["taglib-location"] = "/WEB-INF/tlds/cofax.tld";

    const char* cmpStr = root.stringify();
    compareResult(longJsStrOrig, cmpStr);
}

void composeShortJson()
{
    usecs = 0;
    usecs2 = 0;
    for (int i=0; i<iterationNumber; i++) {
        gettimeofday(&tvStart, NULL);

        SeaJson root;
        SeaJson menu;
        root["menu"] += menu;

        menu["id"] = "file";
        menu["value"] = "File";
        menu["size"] = 1234;

        SeaJson popup;
        menu["popup"] += popup;

        SeaJson menuitem;
        popup["menuitem"] += menuitem;

        SeaJson buf;

        menuitem[-1] += buf;
        buf["value"] = "New";
        buf["onclick"] = "CreateNewDoc()";

        buf.clear();
        menuitem[TAIL] += buf;
        buf["value"] = "Open";
        buf["onclick"] = "OpenDoc()";

        buf.clear();
        menuitem[TAIL] += buf;
        buf["value"] = "Close";
        buf["onclick"] = "CloseDoc()";

        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;

        // stringify after compose
        gettimeofday(&tvStart, NULL);

        root.stringify();

        gettimeofday(&tvEnd, NULL);
        usecs2 += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to compose short JSON (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;
    cout << "Total Time to stringify after composing short JSON (Repeat Count : " << iterationNumber << ") = " << usecs2 << " usecs" << endl;

    SeaJson root;
        SeaJson menu;
        root["menu"] += menu;

        menu["id"] = "file";
        menu["value"] = "File";
        menu["size"] = 1234;

        SeaJson popup;
        menu["popup"] += popup;

        SeaJson menuitem;
        popup["menuitem"] += menuitem;

        SeaJson buf;

        menuitem[-1] += buf;
        buf["value"] = "New";
        buf["onclick"] = "CreateNewDoc()";

        buf.clear();
        menuitem[TAIL] += buf;
        buf["value"] = "Open";
        buf["onclick"] = "OpenDoc()";

        buf.clear();
        menuitem[TAIL] += buf;
        buf["value"] = "Close";
        buf["onclick"] = "CloseDoc()";

    const char* cmpStr = root.stringify();
    compareResult(shortJsStrOrig, cmpStr);
}

void deleteLongJson()
{
    usecs = 0;
    for (int i=0; i<iterationNumber; i++) {
        {
            SeaJson sjObj(longJsStr);
            gettimeofday(&tvStart, NULL);
        }
        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to delete long JSON string (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;
}

void deleteShortJson()
{
    usecs = 0;
    for (int i=0; i<iterationNumber; i++) {
        {
            SeaJson sjObj(shortJsStr);
            gettimeofday(&tvStart, NULL);
        }
        gettimeofday(&tvEnd, NULL);
        usecs += (tvEnd.tv_sec  - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec) - usecsOffset;
    }

    cout << "Total Time to delete long JSON string (Repeat Count : " << iterationNumber << ") = " << usecs << " usecs" << endl;
}

void unitTestConstructors()
{
    cout << "Constructors..." << endl;
    {
        char title[] = "Default Constructor";
        SeaJson sjObj;
        checkUnitTestResult(title, "null", sjObj.stringify());
    }
    {
        char title[] = "Constructor with parsing (from string)";
        SeaJson sjObj(shortJsStrOrig);
        checkUnitTestResult(title, shortJsStrOrig, sjObj.stringify());
    }
    {
        char title[] = "Constructor with parsing (from variable)";
        SeaJson sjObj(shortJsStrOrig);
        checkUnitTestResult(title, shortJsStrOrig, sjObj.stringify());
    }
    {
        char title[] = "Copy Constructor (normal)";
        SeaJson sjObj(shortJsStrOrig);
        SeaJson sjNew(sjObj);
        checkUnitTestResult(title, shortJsStrOrig, sjNew.stringify());
    }
    {
        char title[] = "Copy Constructor (copy const object)";
        const SeaJson sjObj(shortJsStrOrig);
        SeaJson sjNew(sjObj);
        checkUnitTestResult(title, shortJsStrOrig, sjNew.stringify());
    }
    {
        char title[] = "Copy Constructor (with copy operator)";
        const SeaJson sjObj(shortJsStrOrig);
        SeaJson sjNew = SeaJson(sjObj);
        checkUnitTestResult(title, shortJsStrOrig, sjNew.stringify());
    }
}

void unitTestMemberFunctions()
{
    cout << "Member functions..." << endl;
    {
        char title[] = "parse (from string)";
        SeaJson sjObj;
        sjObj.parse(shortJsStrOrig);
        checkUnitTestResult(title, shortJsStrOrig, sjObj.stringify());
    }
    {
        char title[] = "parse (from variable)";
        SeaJson sjObj;
        sjObj.parse(shortJsStrOrig);
        checkUnitTestResult(title, shortJsStrOrig, sjObj.stringify());
    }
    {
        string title;
        SeaJson sjObj;

        title = "stringify - root";
        sjObj.parse(shortJsStrOrig);
        checkUnitTestResult(title.c_str(), shortJsStrOrig, sjObj.stringify());

        title = "stringify - object";
        checkUnitTestResult(title.c_str(), "{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"}", sjObj["menu"]["popup"]["menuitem"][0].stringify());

        title = "stringify - array";
        checkUnitTestResult(title.c_str(), "[{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"},{\"value\":\"Open\",\"onclick\":\"OpenDoc()\"},{\"value\":\"Close\",\"onclick\":\"CloseDoc()\"}]", sjObj["menu"]["popup"]["menuitem"].stringify());

        title = "stringify - string value";
        checkUnitTestResult(title.c_str(), "\"CreateNewDoc()\"", sjObj["menu"]["popup"]["menuitem"][0]["onclick"].stringify());
    }
    {
        string title;
        SeaJson sjObj("{\"null value\":null,\"string value\":\"Hello world~!\",\"long long value\":1234567,\"double value\":12345.6789,\"bool value\":true,\"object\":{\"obj1\":\"name\",\"obj2\":4321},\"array\":[1,2,3,4]}");

        title = "getType - null";
        checkUnitTestResult(title.c_str(), JSON_null, sjObj["null value"].getType());

        title = "getType - char";
        checkUnitTestResult(title.c_str(), JSON_char, sjObj["string value"].getType());

        title = "getType - long long";
        checkUnitTestResult(title.c_str(), JSON_llong, sjObj["long long value"].getType());

        title = "getType - double";
        checkUnitTestResult(title.c_str(), JSON_double, sjObj["double value"].getType());

        title = "getType - bool";
        checkUnitTestResult(title.c_str(), JSON_bool, sjObj["bool value"].getType());

        title = "getType - object";
        checkUnitTestResult(title.c_str(), JSON_object, sjObj["object"].getType());

        title = "getType - array";
        checkUnitTestResult(title.c_str(), JSON_array, sjObj["array"].getType());
    }
    {
        string title;
        SeaJson sjObj("{\"null value\":null,\"string value\":\"Hello world~!\",\"long long value\":1234567,\"double value\":12345.6789,\"bool value\":true,\"object\":{\"obj1\":\"name\",\"obj2\":4321},\"array\":[1,2,3,4]}");

        title = "isValid - true";
        checkUnitTestResult(title.c_str(), true, sjObj["null value"].isValid());

        title = "isValid - false";
        checkUnitTestResult(title.c_str(), false, sjObj["float value"].isValid());
    }
    {
        char title[] = "clear";
        SeaJson sjObj("{\"null value\":null,\"string value\":\"Hello world~!\",\"long long value\":1234567,\"double value\":12345.6789,\"bool value\":true,\"object\":{\"obj1\":\"name\",\"obj2\":4321},\"array\":[1,2,3,4]}");
        sjObj.clear();
        checkUnitTestResult(title, "null", sjObj.stringify());
    }
    {
        char title[] = "remove";
        SeaJson sjObj("{\"null value\":null,\"string value\":\"Hello world~!\",\"long long value\":1234567,\"double value\":12345.6789,\"bool value\":true,\"object\":{\"obj1\":\"name\",\"obj2\":4321},\"array\":[1,2,3,4]}");
        //delete sjObj["web-app"]["servlet"][0];
        sjObj["object"]["obj2"].remove();
        char strCmp[] = "{\"null value\":null,\"string value\":\"Hello world~!\",\"long long value\":1234567,\"double value\":12345.6789,\"bool value\":true,\"object\":{\"obj1\":\"name\"},\"array\":[1,2,3,4]}";
        checkUnitTestResult(title, strCmp, sjObj.stringify());
    }
}

void unitTestArrayOperators()
{
    cout << "Array Operators..." << endl;
    {
        char objStr[] = "{\"widget\":{\"debug\":\"on\",\"window\":{\"title\":\"Sample Konfabulator Widget\",\"name\":\"main_window\",\"width\":500,\"height\":500}}}";

        string title = "\tparse & stringify object";
        SeaJson sjObj(objStr);
        checkUnitTestResult(title.c_str(), objStr, sjObj.stringify());

        title = "\tadd element to object";
        sjObj["widget"]["added string"] = "Hellow world~!";
        checkUnitTestResult(title.c_str(), "{\"widget\":{\"debug\":\"on\",\"window\":{\"title\":\"Sample Konfabulator Widget\",\"name\":\"main_window\",\"width\":500,\"height\":500},\"added string\":\"Hellow world~!\"}}", sjObj.stringify());

        title = "\tupdate element of object";
        sjObj["widget"]["added string"] = "How are you~?";
        checkUnitTestResult(title.c_str(), "{\"widget\":{\"debug\":\"on\",\"window\":{\"title\":\"Sample Konfabulator Widget\",\"name\":\"main_window\",\"width\":500,\"height\":500},\"added string\":\"How are you~?\"}}", sjObj.stringify());

        title = "\tdelete element from object";
        //delete sjObj["widget"]["window"]["name"];
        sjObj["widget"]["window"]["name"].remove();
        checkUnitTestResult(title.c_str(), "{\"widget\":{\"debug\":\"on\",\"window\":{\"title\":\"Sample Konfabulator Widget\",\"width\":500,\"height\":500},\"added string\":\"How are you~?\"}}", sjObj.stringify());
    }
    {
        char arrayStr[] = "[{\"name\":\"John Doe\",\"age\":29},{\"name\":\"Anna Smith\",\"age\":24},{\"name\":\"Peter Jones\",\"age\":39}]";

        string title = "\tparse & stringify array";
        SeaJson sjObj(arrayStr);
        checkUnitTestResult(title.c_str(), arrayStr, sjObj.stringify());

        title = "\tadd element to tail of array";
        sjObj[TAIL] = "Hellow world~!";
        checkUnitTestResult(title.c_str(), "[{\"name\":\"John Doe\",\"age\":29},{\"name\":\"Anna Smith\",\"age\":24},{\"name\":\"Peter Jones\",\"age\":39},\"Hellow world~!\"]", sjObj.stringify());

        title = "\tupdate element of array";
        sjObj[3] = "How are you~?";
        checkUnitTestResult(title.c_str(), "[{\"name\":\"John Doe\",\"age\":29},{\"name\":\"Anna Smith\",\"age\":24},{\"name\":\"Peter Jones\",\"age\":39},\"How are you~?\"]", sjObj.stringify());

        title = "\tdelete element from array";
        //delete sjObj[1];
        sjObj[1].remove();
        checkUnitTestResult(title.c_str(), "[{\"name\":\"John Doe\",\"age\":29},{\"name\":\"Peter Jones\",\"age\":39},\"How are you~?\"]", sjObj.stringify());
    }
}

void unitTestAssignmentOperators()
{
    cout << "Assignment Operators..." << endl;
    {
        SeaJson sjObj;
        string title = "assign const char* from const";
        sjObj["const char value"] = "This is const chars";
        checkUnitTestResult(title.c_str(), "{\"const char value\":\"This is const chars\"}", sjObj.stringify());

        title = "assign const char* from char variable";
        char charVal[] = "This is char variable";
        sjObj["char variable"] = charVal;
        checkUnitTestResult(title.c_str(), "{\"const char value\":\"This is const chars\",\"char variable\":\"This is char variable\"}", sjObj.stringify());

        title = "update const char* from const";
        sjObj["const char value"] = "This is updated const chars";
        checkUnitTestResult(title.c_str(), "{\"const char value\":\"This is updated const chars\",\"char variable\":\"This is char variable\"}", sjObj.stringify());

        title = "update const char* from variable";
        char updatedCharVal[] = "This is updated char variable";
        sjObj["char variable"] = updatedCharVal;
        checkUnitTestResult(title.c_str(), "{\"const char value\":\"This is updated const chars\",\"char variable\":\"This is updated char variable\"}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        string title = "assign const string from std::string variable";
        string strVal = "This is string variable";
        sjObj["string variable"] = strVal;
        checkUnitTestResult(title.c_str(), "{\"string variable\":\"This is string variable\"}", sjObj.stringify());

        title = "update const string from std::string variable";
        string updatedStrVal = "This is updated string variable";
        sjObj["string variable"] = updatedStrVal;
        checkUnitTestResult(title.c_str(), "{\"string variable\":\"This is updated string variable\"}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        string title;

        title = "assign int from const";
        sjObj["int value"] = 123456789;
        checkUnitTestResult(title.c_str(), "{\"int value\":123456789}", sjObj.stringify());

        title = "assign int from variable";
        int nVal = 987654321;
        sjObj["int variable"] = nVal;
        checkUnitTestResult(title.c_str(), "{\"int value\":123456789,\"int variable\":987654321}", sjObj.stringify());

        title = "update int from const";
        sjObj["int value"] = 842657139;
        checkUnitTestResult(title.c_str(), "{\"int value\":842657139,\"int variable\":987654321}", sjObj.stringify());

        title = "update int from variable";
        nVal = 713958426;
        sjObj["int variable"] = nVal;
        checkUnitTestResult(title.c_str(), "{\"int value\":842657139,\"int variable\":713958426}", sjObj.stringify());
    }
    {
        SeaJson sjObj("{\"int value\":123456789}");
        string title;

        title = "assign unsigned int from variable";
        unsigned int uVal = 987654321;
        sjObj["unsigned int variable"] = uVal;
        checkUnitTestResult(title.c_str(), "{\"int value\":123456789,\"unsigned int variable\":987654321}", sjObj.stringify());

        title = "update unsigned int from variable";
        uVal = 713958426;
        sjObj["unsigned int variable"] = uVal;
        checkUnitTestResult(title.c_str(), "{\"int value\":123456789,\"unsigned int variable\":713958426}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        string title = "assign long from const";
        sjObj["long value"] = 123456789l;
        checkUnitTestResult(title.c_str(), "{\"long value\":123456789}", sjObj.stringify());

        title = "assign long from variable";
        long lVal = 987654321;
        sjObj["long variable"] = lVal;
        checkUnitTestResult(title.c_str(), "{\"long value\":123456789,\"long variable\":987654321}", sjObj.stringify());

        title = "update long from const";
        sjObj["long value"] = 842657139l;
        checkUnitTestResult(title.c_str(), "{\"long value\":842657139,\"long variable\":987654321}", sjObj.stringify());

        title = "update long from variable";
        lVal = 713958426;
        sjObj["long variable"] = lVal;
        checkUnitTestResult(title.c_str(), "{\"long value\":842657139,\"long variable\":713958426}", sjObj.stringify());
    }
    {
        SeaJson sjObj("{\"long value\":123456789}");
        string title;

        title = "assign unsigned long from variable";
        unsigned long ulVal = 987654321;
        sjObj["unsigned long variable"] = ulVal;
        checkUnitTestResult(title.c_str(), "{\"long value\":123456789,\"unsigned long variable\":987654321}", sjObj.stringify());

        title = "update unsigned long from variable";
        ulVal = 713958426;
        sjObj["unsigned long variable"] = ulVal;
        checkUnitTestResult(title.c_str(), "{\"long value\":123456789,\"unsigned long variable\":713958426}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        string title = "assign long long from const";
        sjObj["long long value"] = 123456789012345;
        checkUnitTestResult(title.c_str(), "{\"long long value\":123456789012345}", sjObj.stringify());

        title = "assign long long from variable";
        long long llVal = 987654321098765;
        sjObj["long long variable"] = llVal;
        checkUnitTestResult(title.c_str(), "{\"long long value\":123456789012345,\"long long variable\":987654321098765}", sjObj.stringify());

        title = "update long long from const";
        sjObj["long long value"] = 842657139084265;
        checkUnitTestResult(title.c_str(), "{\"long long value\":842657139084265,\"long long variable\":987654321098765}", sjObj.stringify());

        title = "update long long from variable";
        llVal = 71395842671395;
        sjObj["long long variable"] = llVal;
        checkUnitTestResult(title.c_str(), "{\"long long value\":842657139084265,\"long long variable\":71395842671395}", sjObj.stringify());
    }
    {
        SeaJson sjObj("{\"long value\":123456789}");
        string title;

        title = "assign unsigned long long from variable";
        long long unsigned int ullVal = 987654321;
        sjObj["unsigned long long variable"] = ullVal;
        checkUnitTestResult(title.c_str(), "{\"long value\":123456789,\"unsigned long long variable\":987654321}", sjObj.stringify());

        title = "update unsigned long long from variable";
        ullVal = 713958426;
        sjObj["unsigned long long variable"] = ullVal;
        checkUnitTestResult(title.c_str(), "{\"long value\":123456789,\"unsigned long long variable\":713958426}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        char org[100];
        string title = "assign double from const";
        sjObj["double value"] = 12345.6789;
        checkUnitTestResult(title.c_str(), "{\"double value\":12345.6789}", sjObj.stringify());

        title = "assign double from variable";
        double dbVal = 98765.4321;
        sjObj["double variable"] = dbVal;
        sprintf(org, "{\"double value\":12345.6789,\"double variable\":%.06lf}", dbVal);
        checkUnitTestResult(title.c_str(), "{\"double value\":12345.6789,\"double variable\":98765.4321}", sjObj.stringify());

        title = "update double from const";
        sjObj["double value"] = 84265.7139;
        checkUnitTestResult(title.c_str(), "{\"double value\":84265.7139,\"double variable\":98765.4321}", sjObj.stringify());

        title = "update double from variable";
        dbVal = 71395.8426;
        sjObj["double variable"] = dbVal;
        sprintf(org, "{\"double value\":12345.6789,\"double variable\":%.06lf}", dbVal);
        checkUnitTestResult(title.c_str(), "{\"double value\":84265.7139,\"double variable\":71395.8426}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        char org[100];
        string title = "assign float from variable";
        float fVal = 98765.4321;
        sjObj["float variable"] = fVal;
        sprintf(org, "{\"float variable\":%.15G}", fVal);
        checkUnitTestResult(title.c_str(), org, sjObj.stringify());

        title = "update float from variable";
        fVal = 84265.7139;
        sjObj["float variable"] = fVal;
        sprintf(org, "{\"float variable\":%.15G}", fVal);
        checkUnitTestResult(title.c_str(), org, sjObj.stringify());
    }
    {
        SeaJson sjObj;
        string title = "assign bool from const";
        sjObj["bool value"] = true;
        checkUnitTestResult(title.c_str(), "{\"bool value\":true}", sjObj.stringify());

        title = "assign bool from variable";
        bool bVal = false;
        sjObj["bool variable"] = bVal;
        checkUnitTestResult(title.c_str(), "{\"bool value\":true,\"bool variable\":false}", sjObj.stringify());

        title = "update bool from const";
        sjObj["bool value"] = false;
        checkUnitTestResult(title.c_str(), "{\"bool value\":false,\"bool variable\":false}", sjObj.stringify());

        title = "update bool from variable";
        bVal = true;
        sjObj["bool variable"] = bVal;
        checkUnitTestResult(title.c_str(), "{\"bool value\":false,\"bool variable\":true}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        string title = "assign 0";
        sjObj["zero"] = 0;
        checkUnitTestResult(title.c_str(), "{\"zero\":0}", sjObj.stringify());

        title = "update 0";
        sjObj["zero"] = 1234;
        sjObj["zero"] = 0;
        checkUnitTestResult(title.c_str(), "{\"zero\":0}", sjObj.stringify());
    }
    {
        SeaJson sjObj;
        string title = "assign by copy";
        SeaJson sjOrg("{\"null value\":null, \"string value\":\"Hello world~!\", \"long long value\":1234567, \"double value\":12345.6789, \"bool value\":true, \"object\":{\"obj1\":\"name\", \"obj2\":4321}, \"array\":[1, 2, 3, 4]}");
        sjObj = sjOrg;
        checkUnitTestResult(title.c_str(), sjOrg.stringify(), sjObj.stringify());
    }
    {
        SeaJson sjObj("{\"string value\":\"Hello world~!\", \"long long value\":1234567, \"double value\":12345.6789, \"bool value\":true, \"object\":{\"obj1\":\"name\", \"obj2\":4321}, \"array\":[1, 2, 3, 4]}");
        string title = "assign null";
        sjObj["null value"] = JsonNull;
        checkUnitTestResult(title.c_str(), "{\"string value\":\"Hello world~!\",\"long long value\":1234567,\"double value\":12345.6789,\"bool value\":true,\"object\":{\"obj1\":\"name\",\"obj2\":4321},\"array\":[1,2,3,4],\"null value\":null}", sjObj.stringify());
    }
}

void unitTestCompositeAssignmentOperators()
{
    cout << "Composite Assignment Operators..." << endl;
    {
        string title = "compose assign - object";
        SeaJson sjObj("{\"widget\":{\"debug\":\"on\",\"window\":{\"title\":\"Sample Konfabulator Widget\",\"name\":\"main_window\",\"width\":500,\"height\":500}}}");
        SeaJson sjAppend("{\"src\":\"Images/Sun.png\",\"name\":\"sun1\",\"hOffset\":250,\"vOffset\":250,\"alignment\":\"center\"}");
        sjObj["widget"]["image"] += sjAppend;
        checkUnitTestResult(title.c_str(), "{\"widget\":{\"debug\":\"on\",\"window\":{\"title\":\"Sample Konfabulator Widget\",\"name\":\"main_window\",\"width\":500,\"height\":500},\"image\":{\"src\":\"Images/Sun.png\",\"name\":\"sun1\",\"hOffset\":250,\"vOffset\":250,\"alignment\":\"center\"}}}", sjObj.stringify());
    }
    {
        string title = "compose assign - array";
        SeaJson sjObj("[{\"name\":\"John Doe\",\"age\":29},{\"name\":\"Anna Smith\",\"age\":24}]");
        SeaJson sjAppend("[1,2,3,4,5]");
        sjObj[TAIL]["new array"][2] += sjAppend;
        checkUnitTestResult(title.c_str(), "[{\"name\":\"John Doe\",\"age\":29},{\"name\":\"Anna Smith\",\"age\":24},{\"new array\":[null,null,[1,2,3,4,5]]}]", sjObj.stringify());
    }
}

void unitTestCastingOperators()
{
    cout << "Casting Operators..." << endl;
    {
        SeaJson sjObj("{\"web-app\":{\"servlet\":[{\"servlet-name\":\"cofaxCDS\",\"servlet-class\":\"org.cofax.cds.CDSServlet\",\"init-param\":{\"configGlossary:installationAt\":\"Philadelphia, PA\",\"configGlossary:adminEmail\":\"ksm@pobox.com\",\"configGlossary:poweredBy\":\"Cofax\",\"configGlossary:poweredByIcon\":\"/images/cofax.gif\",\"configGlossary:staticPath\":\"/content/static\",\"templateProcessorClass\":\"org.cofax.WysiwygTemplate\",\"templateLoaderClass\":\"org.cofax.FilesTemplateLoader\",\"templatePath\":\"templates\",\"templateOverridePath\":\"\",\"defaultListTemplate\":\"listTemplate.htm\",\"defaultFileTemplate\":\"articleTemplate.htm\",\"useJSP\":false,\"jspListTemplate\":\"listTemplate.jsp\",\"jspFileTemplate\":\"articleTemplate.jsp\",\"cachePackageTagsTrack\":200,\"cachePackageTagsStore\":200,\"cachePackageTagsRefresh\":60,\"cacheTemplatesTrack\":100,\"cacheTemplatesStore\":50,\"cacheTemplatesRefresh\":15,\"cachePagesTrack\":200,\"cachePagesStore\":100,\"cachePagesRefresh\":10,\"cachePagesDirtyRead\":10,\"searchEngineListTemplate\":\"forSearchEnginesList.htm\",\"searchEngineFileTemplate\":\"forSearchEngines.htm\",\"searchEngineRobotsDb\":\"WEB-INF/robots.db\",\"useDataStore\":true,\"dataStoreClass\":\"org.cofax.SqlDataStore\",\"redirectionClass\":\"org.cofax.SqlRedirection\",\"dataStoreName\":\"cofax\",\"dataStoreDriver\":\"com.microsoft.jdbc.sqlserver.SQLServerDriver\",\"dataStoreUrl\":\"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",\"dataStoreUser\":\"sa\",\"dataStorePassword\":\"dataStoreTestQuery\",\"dataStoreTestQuery\":\"SET NOCOUNT ON;select test='test';\",\"dataStoreLogFile\":\"/usr/local/tomcat/logs/datastore.log\",\"dataStoreInitConns\":10,\"dataStoreMaxConns\":100,\"dataStoreConnUsageLimit\":100,\"dataStoreLogLevel\":\"debug\",\"maxUrlLength\":500}},{\"servlet-name\":\"cofaxEmail\",\"servlet-class\":\"org.cofax.cds.EmailServlet\",\"init-param\":{\"mailHost\":\"mail1\",\"mailHostOverride\":\"mail2\"}},{\"servlet-name\":\"cofaxAdmin\",\"servlet-class\":\"org.cofax.cds.AdminServlet\"},{\"servlet-name\":\"fileServlet\",\"servlet-class\":\"org.cofax.cds.FileServlet\"},{\"servlet-name\":\"cofaxTools\",\"servlet-class\":\"org.cofax.cms.CofaxToolsServlet\",\"init-param\":{\"templatePath\":\"toolstemplates/\",\"log\":1,\"logLocation\":\"/usr/local/tomcat/logs/CofaxTools.log\",\"logMaxSize\":\"\",\"dataLog\":1,\"dataLogLocation\":\"/usr/local/tomcat/logs/dataLog.log\",\"dataLogMaxSize\":\"\",\"removePageCache\":\"/content/admin/remove?cache=pages&id=\",\"removeTemplateCache\":\"/content/admin/remove?cache=templates&id=\",\"fileTransferFolder\":\"/usr/local/tomcat/webapps/content/fileTransferFolder\",\"lookInContext\":1,\"adminGroupID\":4,\"betaServer\":true}}],\"servlet-mapping\":{\"cofaxCDS\":\"/\",\"cofaxEmail\":\"/cofaxutil/aemail/*\",\"cofaxAdmin\":\"/admin/*\",\"fileServlet\":\"/static/*\",\"cofaxTools\":\"/tools/*\"},\"taglib\":{\"taglib-uri\":\"cofax.tld\",\"taglib-location\":\"/WEB-INF/tlds/cofax.tld\",\"double value\":12345.6789}}}");
        string title;
        {
            title = "get char* value";
            char* charVal = sjObj["web-app"]["servlet"][0]["init-param"]["dataStoreLogLevel"];
            checkUnitTestResult(title.c_str(), "debug", charVal);
        }
        {
            title = "get const char* value";
            const char* constCharVal = sjObj["web-app"]["servlet"][0]["init-param"]["dataStoreLogLevel"];
            checkUnitTestResult(title.c_str(), "debug", constCharVal);
        }
        {
            title = "get std::string value";
            string strVal = sjObj["web-app"]["servlet"][0]["init-param"]["dataStoreLogLevel"];
            checkUnitTestResult(title.c_str(), "debug", strVal.c_str());
        }
        {
            title = "get int value";
            int nVal = sjObj["web-app"]["servlet"][0]["init-param"]["maxUrlLength"];
            checkUnitTestResult(title.c_str(), int(500), nVal);
        }
        {
            title = "get unsigned int value";
            unsigned int uVal = sjObj["web-app"]["servlet"][0]["init-param"]["maxUrlLength"];
            checkUnitTestResult(title.c_str(), (unsigned int)(500), uVal);
        }
        {
            title = "get long value";
            long lVal = sjObj["web-app"]["servlet"][0]["init-param"]["maxUrlLength"];
            checkUnitTestResult(title.c_str(), long(500), lVal);
        }
        {
            title = "get unsigned long value";
            unsigned long ulVal = sjObj["web-app"]["servlet"][0]["init-param"]["maxUrlLength"];
            checkUnitTestResult(title.c_str(), (unsigned long)(500), ulVal);
        }
        {
            title = "get long long value";
            long long llVal = sjObj["web-app"]["servlet"][0]["init-param"]["maxUrlLength"];
            checkUnitTestResult(title.c_str(), (long long)(500), llVal);
        }
        {
            title = "get long long value";
            long long unsigned int ullVal = sjObj["web-app"]["servlet"][0]["init-param"]["maxUrlLength"];
            checkUnitTestResult(title.c_str(), (long long unsigned int)(500), ullVal);
        }
        {
            title = "get double value";
            double dbVal = sjObj["web-app"]["taglib"]["double value"];
            checkUnitTestResult(title.c_str(), double(12345.6789), dbVal);
        }
        {
            title = "get float value";
            float fVal = sjObj["web-app"]["taglib"]["double value"];
            checkUnitTestResult(title.c_str(), float(12345.6789), fVal);
        }
        {
            title = "get bool value";
            bool bVal = sjObj["web-app"]["servlet"][0]["init-param"]["useDataStore"];
            checkUnitTestResult(title.c_str(), true, bVal);
        }
        {
            title = "get short int value";
            short int snVal = sjObj["web-app"]["servlet"][0]["init-param"]["maxUrlLength"];
            checkUnitTestResult(title.c_str(), (short int)(500), snVal);
        }
    }
}

void unitTestComparisonOperations()
{
    cout << "Comparison Operators..." << endl;
    {
        SeaJson sjObj("{\"value\":0}");
        bool isEqual;
        bool isDiff;
        int rightVal = 0;
        int wrongVal = 123;
        string title;

        title = "compare int value - equal true";
        isEqual = (sjObj["value"] == 0);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare int value - equal false";
        isEqual = (sjObj["value"] == 123);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare int value - different true";
        isDiff = (sjObj["value"] != 123);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare int value - different false";
        isDiff = (sjObj["value"] != 0);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare int variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare int variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare int variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare int variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":0}");
        bool isEqual;
        bool isDiff;
        unsigned int rightVal = 0;
        unsigned int wrongVal = 123;
        string title;

        title = "compare unsigned int value - equal true";
        isEqual = (sjObj["value"] == 0u);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare unsigned int value - equal false";
        isEqual = (sjObj["value"] == 123u);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare unsigned int value - different true";
        isDiff = (sjObj["value"] != 123u);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare unsigned int value - different false";
        isDiff = (sjObj["value"] != 0u);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare unsigned int variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare unsigned int variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare unsigned int variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare unsigned int variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":12345}");
        bool isEqual;
        bool isDiff;
        long rightVal = 12345;
        long wrongVal = 54321;
        string title;

        title = "compare long value - equal true";
        isEqual = (sjObj["value"] == 12345);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare long value - equal false";
        isEqual = (sjObj["value"] == 54321);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare long value - different true";
        isDiff = (sjObj["value"] != 54321);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare long value - different false";
        isDiff = (sjObj["value"] != 12345);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare long variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare long variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare long variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare long variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":12345}");
        bool isEqual;
        bool isDiff;
        unsigned long rightVal = 12345;
        unsigned long wrongVal = 54321;
        string title;

        title = "compare unsigned long value - equal true";
        isEqual = (sjObj["value"] == 12345);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare unsigned long value - equal false";
        isEqual = (sjObj["value"] == 54321);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare unsigned long value - different true";
        isDiff = (sjObj["value"] != 54321);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare unsigned long value - different false";
        isDiff = (sjObj["value"] != 12345);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare unsigned long variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare unsigned long variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare unsigned long variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare unsigned long variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":1234567890123}");
        bool isEqual;
        bool isDiff;
        llong rightVal = 1234567890123;
        llong wrongVal = 3210987654321;
        string title;

        title = "compare long long value - equal true";
        isEqual = (sjObj["value"] == 1234567890123);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare long long value - equal false";
        isEqual = (sjObj["value"] == 3210987654321);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare long long value - different true";
        isDiff = (sjObj["value"] != 3210987654321);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare long long value - different false";
        isDiff = (sjObj["value"] != 1234567890123);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare long long variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare long long variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare long long variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare long long variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":1234567890123}");
        bool isEqual;
        bool isDiff;
        long long unsigned int rightVal = 1234567890123;
        long long unsigned int wrongVal = 3210987654321;
        string title;

        title = "compare long long unsigned int value - equal true";
        isEqual = (sjObj["value"] == 1234567890123);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare long long unsigned int value - equal false";
        isEqual = (sjObj["value"] == 3210987654321);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare long long unsigned int value - different true";
        isDiff = (sjObj["value"] != 3210987654321);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare long long unsigned int value - different false";
        isDiff = (sjObj["value"] != 1234567890123);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare long long unsigned int variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare long long unsigned int variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare long long unsigned int variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare long long unsigned int variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":true}");
        bool isEqual;
        bool isDiff;
        bool rightVal = true;
        bool wrongVal = false;
        string title;

        title = "compare bool value - equal true";
        isEqual = (sjObj["value"] == true);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare bool value - equal false";
        isEqual = (sjObj["value"] == false);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare bool value - different true";
        isDiff = (sjObj["value"] != false);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare bool value - different false";
        isDiff = (sjObj["value"] != true);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare bool variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare bool variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare bool variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare bool variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":123456789.12345}");
        bool isEqual;
        bool isDiff;
        double rightVal = 123456789.12345;
        double wrongVal = 987654321.54321;
        string title;

        title = "compare double value - equal true";
        isEqual = (sjObj["value"] == 123456789.12345);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare double value - equal false";
        isEqual = (sjObj["value"] == 987654321.54321);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare double value - different true";
        isDiff = (sjObj["value"] != 987654321.54321);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare double value - different false";
        isDiff = (sjObj["value"] != 123456789.12345);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare double variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare double variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare double variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare double variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":1234.56789}");
        bool isEqual;
        bool isDiff;
        float rightVal = 1234.56789;
        float wrongVal = 9876.54321;
        string title;

        title = "compare float value - equal true";
        isEqual = (sjObj["value"] == 1234.56789);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare float value - equal false";
        isEqual = (sjObj["value"] == 9876.54321);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare float value - different true";
        isDiff = (sjObj["value"] != 9876.54321);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare float value - different false";
        isDiff = (sjObj["value"] != 1234.56789);
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare float variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare float variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare float variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare float variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":\"Hello world~!\"}");
        bool isEqual;
        bool isDiff;
        char rightVal[] = "Hello world~!";
        char wrongVal[] = "How are you?";
        string title;

        title = "compare char* value - equal true";
        isEqual = (sjObj["value"] == "Hello world~!");
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare char* value - equal false";
        isEqual = (sjObj["value"] == "How are you?");
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare char* value - different true";
        isDiff = (sjObj["value"] != "How are you?");
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare char* value - different false";
        isDiff = (sjObj["value"] != "Hello world~!");
        checkUnitTestResult(title.c_str(), false, isDiff);

        title = "compare char* variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare char* variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare char* variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare char* variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
    {
        SeaJson sjObj("{\"value\":\"Hello world~!\"}");
        bool isEqual;
        bool isDiff;
        string rightVal = "Hello world~!";
        string wrongVal = "How are you?";
        string title;

        title = "compare string variable - equal true";
        isEqual = (sjObj["value"] == rightVal);
        checkUnitTestResult(title.c_str(), true, isEqual);

        title = "compare string variable - equal false";
        isEqual = (sjObj["value"] == wrongVal);
        checkUnitTestResult(title.c_str(), false, isEqual);

        title = "compare string variable - different true";
        isDiff = (sjObj["value"] != wrongVal);
        checkUnitTestResult(title.c_str(), true, isDiff);

        title = "compare string variable - different false";
        isDiff = (sjObj["value"] != rightVal);
        checkUnitTestResult(title.c_str(), false, isDiff);
    }
}

void unitTestNavigation()
{
    SeaJson sjObj(longJsStrOrig);

    char cmpStr[2][20] = {"cofaxCDS", "cofaxEmail"};

    const char* charVal;
    string title;

    {
        title = "navigate with const 0";
        charVal = sjObj["web-app"]["servlet"][0]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[0], charVal);

        title = "navigate with const 1";
        charVal = sjObj["web-app"]["servlet"][1]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[1], charVal);
    }
    {
        int nIndex;
        title = "navigate with int index = 0";
        nIndex = 0;
        charVal = sjObj["web-app"]["servlet"][nIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[nIndex], charVal);

        title = "navigate with int index = 1";
        nIndex = 1;
        charVal = sjObj["web-app"]["servlet"][nIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[nIndex], charVal);
    }
    {
        unsigned int uIndex;
        title = "navigate with unsigned int index = 0";
        uIndex = 0;
        charVal = sjObj["web-app"]["servlet"][uIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[uIndex], charVal);

        title = "navigate with unsigned int index = 1";
        uIndex = 1;
        charVal = sjObj["web-app"]["servlet"][uIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[uIndex], charVal);
    }
    {
        const int nConstIndex0 = 0;
        const int nConstIndex1 = 0;
        title = "navigate with const int index = 0";
        charVal = sjObj["web-app"]["servlet"][nConstIndex0]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[nConstIndex0], charVal);

        const int nConstIndex = 1;
        title = "navigate with const int index = 1";
        charVal = sjObj["web-app"]["servlet"][nConstIndex1]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[nConstIndex1], charVal);
    }
    {
        long lIndex;
        title = "navigate with long index = 0";
        lIndex = 0;
        charVal = sjObj["web-app"]["servlet"][lIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[lIndex], charVal);

        title = "navigate with long index = 1";
        lIndex = 1;
        charVal = sjObj["web-app"]["servlet"][lIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[lIndex], charVal);
    }
    {
        unsigned long ulIndex;
        title = "navigate with unsigned long index = 0";
        ulIndex = 0;
        charVal = sjObj["web-app"]["servlet"][ulIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[ulIndex], charVal);

        title = "navigate with unsigned long index = 1";
        ulIndex = 1;
        charVal = sjObj["web-app"]["servlet"][ulIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[ulIndex], charVal);
    }
    {

        long long llIndex;
        title = "navigate with long long index = 0";
        llIndex = 0;
        charVal = sjObj["web-app"]["servlet"][llIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[llIndex], charVal);

        title = "navigate with long long index = 1";
        llIndex = 1;
        charVal = sjObj["web-app"]["servlet"][llIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[llIndex], charVal);
    }
    {
        long long unsigned int ullIndex;
        title = "navigate with long long unsigned int index = 0";
        ullIndex = 0;
        charVal = sjObj["web-app"]["servlet"][ullIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[ullIndex], charVal);

        title = "navigate with long long unsigned int index = 1";
        ullIndex = 1;
        charVal = sjObj["web-app"]["servlet"][ullIndex]["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[ullIndex], charVal);
    }
    {
        long long unsigned int ullIndex;
        title = "navigate with long long unsigned int index = 0 after appending";
        ullIndex = 0;
        SeaJson& sjTemp1 = sjObj["web-app"]["servlet"][ullIndex];
        charVal = sjTemp1["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[ullIndex], charVal);

        title = "navigate with long long unsigned int index = 1 after appending";
        ullIndex = 1;
        SeaJson sjTemp2;
        sjTemp2 += sjObj["web-app"]["servlet"][ullIndex];
        charVal = sjTemp2["servlet-name"];
        checkUnitTestResult(title.c_str(), cmpStr[ullIndex], charVal);
    }
    {
        title = "get long value after navigating";
        SeaJson sjObj(shortJsStr);
        long fileSize = sjObj["menu"]["size"];
        checkUnitTestResult(title.c_str(), fileSize, 1234);

        title = "get string value after navigating with append and reference";
        SeaJson& sjPopup = sjObj["menu"]["popup"];
        SeaJson sjMenuItem;
        sjMenuItem += sjPopup["menuitem"];
        string strCloseOnclick = sjMenuItem[2]["onclick"];
        checkUnitTestResult(title.c_str(), strCloseOnclick, string("CloseDoc()"));

        title = "update json object after navigating with append and reference";
        sjObj["menu"]["size"] = 4321;
        sjObj["menu"]["popup"]["menuitem"][2].parse("{\"value\":\"newClose\",\"onclick\":\"newCloseDoc()\"}");

        char shortJsStrOrig[] = "{\"menu\":{\"id\":\"file\",\"value\":\"File\",\"size\":1234,\"popup\":{\"menuitem\":[{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"},{\"value\":\"Open\",\"onclick\":\"OpenDoc()\"},{\"value\":\"newClose\",\"onclick\":\"newCloseDoc()\"}]}}}";
        charVal = sjObj.stringify();
        checkUnitTestResult(title.c_str(), strCloseOnclick, string("CloseDoc()"));
    }
}

void unitTest()
{
    nTestCase = 0;
    nSuccess = 0;
    nFailure = 0;

    cout << "\n\n/**************************************** Unit Test Start ****************************************/" << endl;
    try {
        unitTestConstructors();
        unitTestMemberFunctions();
        unitTestArrayOperators();
        unitTestAssignmentOperators();
        unitTestCompositeAssignmentOperators();
        unitTestCastingOperators();
        unitTestComparisonOperations();
        unitTestNavigation();
    } catch (std::exception& e) {
        cout << "exception caught: " << e.what() << endl;
    }

    cout << "Unit Test Result" << endl;
    float successRatio = float(nSuccess) / nTestCase * 100;
    cout << "Total Test Case : " << nTestCase << ", Success : " << nSuccess << "(" << successRatio << "%), Failure : " << nFailure << endl;

    cout << "/**************************************** Unit Test End ****************************************/" << endl;
}

void sampleTest()
{
    {
        // Constructor
        SeaJson sjObj(shortJsStr);
    }
    {
        // parse member function
        SeaJson sjObj;
        sjObj.parse(shortJsStr);
    }
    {
        // stringify json object
        SeaJson sjObj(shortJsStr);

        const char* newJstr = sjObj.stringify();
    }
    {
        // compose json object
        SeaJson root;
        SeaJson menu;
        root["menu"] += menu;

        menu["id"] = "file";
        menu["value"] = "File";
        root["menu"]["size"] = 1234;

        SeaJson popup;
        menu["popup"] += popup;

        SeaJson menuitem;
        popup["menuitem"] += menuitem;

        SeaJson buf;

        menuitem[-1] += buf;
        buf["value"] = "New";
        buf["onclick"] = "CreateNewDoc()";

        buf.clear();
        menuitem[TAIL] += buf;
        buf["value"] = "Open";
        buf["onclick"] = "OpenDoc()";

        buf.clear();
        menuitem[TAIL] += buf;
        buf["value"] = "Close";
        buf["onclick"] = "CloseDoc()";
    }
    {
        // get json value
        SeaJson sjObj(shortJsStr);
        long fileSize = sjObj["menu"]["size"];

        SeaJson& sjPopup = sjObj["menu"]["popup"];
        SeaJson sjMenuItem;
        sjMenuItem += sjPopup["menuitem"];
        std::string strCloseOnclick = sjMenuItem[2]["onclick"];
    }
    {
        // update
        SeaJson sjObj(shortJsStr);
        sjObj["menu"]["size"] = 4321;
        sjObj["menu"]["popup"]["menuitem"][2].parse("{\"value\":\"newClose\",\"onclick\":\"newCloseDoc()\"}");
    }
}

int main(int argc, char* argv[])
{
    if (argc == 2) {
        iterationNumber = atoi(argv[1]);
    }

//    measureTimeToCalDiffTime();

    parseLongJson();
    parseShortJson();
//    stringifyLongJson();
//    stringifyShortJson();
    composeLongJson();
    composeShortJson();
    deleteLongJson();
    deleteShortJson();
    unitTest();
    sampleTest();

    return 0;
}
