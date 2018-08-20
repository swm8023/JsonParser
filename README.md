A Simple Json Library support both construct and parse json.
Need C++17. 
Both Boost.Spirit X3 parser and text parser avaiable.

### 1. Construct Json
```c++
    using namespace json;
    Value v = array_t{
        1, 
        3.5, 
        "abc", 
        array_t{4, 5}, 
        object_t{
            {"a", 1}, 
            {"b", false},
            {"c", array_t{4, 5, "ccc"}}
        }
    };

    v[6] = object_t{
        {"hello", array_t{"world", "!"}},
        {"ni hao", "shi jie!"}
    };

    v[4]["a"] = array_t{2, 3, 4 ,5};
    v[4].Remove("b");
    v.Remove(1);

    std::cout << v << std::endl;
```
OutPut
```text
[
    1,
    "abc",
    [
        4,
        5
    ],
    {
        "a" : [
            2,
            3,
            4,
            5
        ],
        "c" : [
            4,
            5,
            "ccc"
        ]
    },
    null,
    {
        "hello" : [
            "world",
            "!"
        ],
        "ni hao" : "shi jie!"
    }
]
```

### 2. Parse Json
```c++
std::string json_str = R"(
{
	"title": "example glossary",
	"GlossList": {
		"ID": 123,
		"SortAs": ["aa", "bb", true, 12.5, {
			"t0": true,
			"t1": "666"
		}],
		"Abbrev": "ISO 8879:1986",
		"GlossDef": {
			"para": "A meta-markup language, used to create markup languages such as DocBook.",
			"GlossSeeAlso": ["GML", "XML"]
		},
		"GlossSee": "markup"
	}
}
)";

json::Value v;
if (json::Parse(json_str, v)) {
    std::cout << v << std::endl;
} else {
    std::cout << "failed!" << std::endl;
}
```
Output
```text
{
    "GlossList" : {
        "Abbrev" : "ISO 8879:1986",
        "GlossDef" : {
            "GlossSeeAlso" : [
                "GML",
                "XML"
            ],
            "para" : "A meta-markup language, used to create markup languages such as DocBook."
        },
        "GlossSee" : "markup",
        "ID" : 123,
        "SortAs" : [
            "aa",
            "bb",
            true,
            12.5,
            {
                "t0" : true,
                "t1" : "666"
            }
        ]
    },
    "title" : "example glossary"
}
```