# SeaJson

Summary
-------
Simple and Easily Accessible JSON class

Description
-----------
A light JSON class for C++, implemented with reference to the UX of the json javascript.
The basic usage of SeaJson is similar to javascript, so you can easily learn it from the guide document and the test codes which are included in the test directory.
A difference from other JSON libraries and classes, there is no restriction on JSON string if it satisfies the JSON format described in the json.org
It may be a string, it may be a number too, and this feature is same with the json javascript.

Brief Guide
-----------
- **example json string**
```json
    {
        "menu": {
            "id": "file",
            "value": "File",
            "size": 1234,
            "popup": {
                "menuitem": [
                    {
                        "value": "New",
                        "onclick": "CreateNewDoc()"
                    },
                    {
                        "value": "Open",
                        "onclick": "OpenDoc()"
                    },
                    {
                        "value": "Close",
                        "onclick": "CloseDoc()"
                    }
                ]
            }
        }
    }
```

**1. parse json string**
```cpp
    char jstr[] = "{\r\n    \"menu\": {\r\n        \"id\": \"file\",\r\n        \"value\": \"File\",\r\n        \"size\": 1234,\r\n        \"popup\": {\r\n            \"menuitem\": [\r\n                {\r\n                    \"value\": \"New\",\r\n                    \"onclick\": \"CreateNewDoc()\"\r\n                },\r\n                {\r\n                    \"value\": \"Open\",\r\n                    \"onclick\": \"OpenDoc()\"\r\n                },\r\n                {\r\n                    \"value\": \"Close\",\r\n                    \"onclick\": \"CloseDoc()\"\r\n                }\r\n            ]\r\n        }\r\n    }\r\n}";
    A. Constructor
        SeaJson sjObj(jstr);
    B. parse member function
        SeaJson sjObj;
        sjObj.parse(jstr);
```

**2. stringify json object**
```cpp
    SeaJson sjObj(jstr);
    const char* newJstr = sjObj.stringify();
```

**3. compose json object**
```cpp
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
```

**4. get json value**
```cpp
    SeaJson sjObj(jstr);
    long fileSize = sjObj["menu"]["size"];

    SeaJson sjMenuItem;
    sjMenuItem += sjPopup["menu"]["popup"]["menuitem"];
    std::string strCloseOnclick = sjMenuItem[2]["onclick"];
```

**5. update**
```cpp
    SeaJson sjObj(jstr);
    sjObj["menu"]["size"] = 4321;
    sjObj["menu"]["popup"]["menuitem"][2].parse("{\"value\":\"newClose\",\"onclick\":\"newCloseDoc()\"}");
```

Build
-----
```bash
g++ -std=c++0x ./src/SeaJsonData.cpp ./src/SeaJson.cpp ./test/testSeaJson.cpp -o ./test/testSeaJson;
```

Copyright and License Information
=================================
The class is licensed under the MIT License:

Copyright © 2015 SeaTools (Sungsik Kim)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
