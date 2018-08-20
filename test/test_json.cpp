#include "json.hpp"

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

int main(int argc, char *argv[]) {
    json::Value v;
    if (json::Parse(json_str, v)) {
        std::cout << v << std::endl;
    } else {
        std::cout << "failed!" << std::endl;
    }
}