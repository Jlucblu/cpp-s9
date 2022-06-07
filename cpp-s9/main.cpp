//#include "T1L2.h"
//#include "T1L3.h"
//#include "T1L4.h"
//#include "T1L5.1.h"
//#include "T1L5.2.h"
//#include "T1L6.h"
//#include "T1L7.1.h"
//#include "T1L7.2.h"
//#include "T1L8.h"
//#include "T1L9.h"
//#include "T2L2.h"

#include <cassert>
#include <sstream>

// в этой задаче ситуация обратная:
// меняйте только файлы ini.cpp, ini.h
// а main.cpp трогать не нужно
#include "ini.h"

using namespace std;

int main() {
    std::istringstream input{
        "[vegetables]\n"
        "potatoes=10\n"
        "onions=1 \n"
        "\n"
        "cucumbers=12\n"
        "\n"
        "[guests] \n"
        "guest1_name = Ivan Durak\n"
        "guest2_name =  Vasilisa Premudraya\n"
        "[guest black list]" };
    ini::Document doc = ini::Load(input);

    assert(doc.GetSectionCount() == 3);
    assert((doc.GetSection("vegetables"s)
        == ini::Section{
            {"potatoes"s, "10"s},
            {"onions"s, "1"s},
            {"cucumbers"s, "12"s},
        }));
    assert((doc.GetSection("guests"s)
        == ini::Section{ {"guest1_name"s, "Ivan Durak"s}, {"guest2_name"s, "Vasilisa Premudraya"s} }));
    assert((doc.GetSection("dragons"s) == ini::Section{}));
    assert((doc.GetSection("guest black list"s) == ini::Section{}));

    doc.AddSection("pets"s) = ini::Section{ {"nasty"s, "rat"s} };
    assert(doc.GetSectionCount() == 4);
}