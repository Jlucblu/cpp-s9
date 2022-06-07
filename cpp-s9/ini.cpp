#include "ini.h"

using namespace std;

namespace ini {
	Section& Document::AddSection(string name) {
		sections_[name];
		return sections_.at(name);
	}
	
	const Section& Document::GetSection(const string& name) const {
		auto it = sections_.find(name);
		Section *sec = new Section();

		if (it == sections_.end()) {
			return *sec;
		}
		return sections_.at(name);
	}

	size_t Document::GetSectionCount() const {
		return sections_.size();
	}

	Document Load(istream& input) {
		Document result;
		string str;
		string name;
		Section *last_section = nullptr;

		while (!input.eof()) {
			getline(input, str);
			str.erase(0, str.find_first_not_of(' '));
			str.erase(str.find_last_not_of(' ') + 1);

			if (str.size() == 0) {
				continue;
			}
			else if (str[0] == '[') {
				str.erase(0, str.find_first_not_of('['));
				str.erase(str.find_last_not_of(']') + 1);
				last_section = &result.AddSection(str);
			}
			else {
				int equal = str.find('=');
				string left(str.begin(), str.begin() + equal);
				string right(str.begin() + equal + 1, str.end());
				left.erase(left.find_last_not_of(' ') + 1);
				right.erase(0, right.find_first_not_of(' '));

				last_section->emplace(left, right);
			}
		}

		return result;
	}
}