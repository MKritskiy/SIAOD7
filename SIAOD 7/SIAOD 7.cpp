#include <iostream>
#include <string>
#include <vector>
using namespace std;
//0100101010010000101
//mantopmentopomantomen
/*
Эни бэни рики паки Турбаурбасентибряки. 
Может – выйдет, может – нет, 
В общем – полный 
Интернет
*/
namespace rle {
	string encode(string text) {
		string encodedText;
		string appendedString;
		int repeatedCount = 0;
		for (int i = 0; i < text.length(); i++) {
			if (text[i] == text[i + 1]) {
				repeatedCount++;
			}
			else {
				repeatedCount++;
				appendedString = text[i] + to_string(repeatedCount);
				encodedText.append(appendedString);
				repeatedCount = 0;
			}
		}
		return encodedText;
	}
}
namespace lz77 {
	struct Code {
		int diff;
		int len;
		char sym;
	};
	vector<Code> encode(string text, int buffer_size) {
		string encodedText;
		string buffer;
		vector<Code> encoded;
		string data;
		int dataIter = 0;
		int border = 0;
		bool isIterated = false;
		data += text[0];
		buffer.reserve(buffer_size);
		int i = 0;
		int ref;
		while (i < text.length()) {
			while ((buffer.find(data) != string::npos || isIterated) && i + 1 < text.length()) {
				if (i + 1 < text.length()) {
					data += text[i + 1];
					i++;
				}
				if (buffer.find(data) == string::npos && data.find(text[i], dataIter) == dataIter) {
					
					dataIter++;
					isIterated = true;
				}
				else {
					isIterated = false;
				}
			}
			border = data.length() - dataIter;
			if (dataIter != 0) {
				ref = buffer.find(data.substr(0, border-1));
				dataIter = 0;
			}
			else
				ref = buffer.find(data.length()>1 ? data.substr(0, data.length()-1) : data.substr(0, data.length()));
			if (ref == string::npos) {
				encoded.push_back(Code{ 0,0,text[i] });

			}
			else if (buffer.length()<5) {
				encoded.push_back(Code{ i - ((int)data.length() - 1) - ref,(int)data.length() - 1,text[i]});
			}
			else if (i + 1 < text.length()) {
				encoded.push_back(Code{ (i - ((int)data.length())) - ((i - 5) - ref),(int)data.length() - 1,text[i] });
			}
			else {
				encoded.push_back(Code{ (i - ((int)data.length())-1) - ((i - 5) - ref),(int)data.length(),text[i] });

			}
			if (i + 1 < text.length()) {
				buffer = text.substr(i - 5 + 1 < 0 ? 0 : i - 5 + 1, i + 1 < 5 ? i + 1 : buffer_size);
				data = text[i + 1];
			}

			i++;
		}
		return encoded;
	}

	string decode(string encodedText) {
		string text;

		return text;
	}
}
namespace lz78 {
	string encode(string text) {
		string encodedText = text;
		string subString;
		string appendedString;
		int currPos = 2;
		int subStringSize = 2;
		int refPos = 0;
		int replacedSize = 0;
		for (int i = 2; i < text.length(); i++) {
			subString = text.substr(currPos, subStringSize);
			refPos = text.substr(0, i).find(subString);
			if (refPos == -1) {
				subString = text.substr(currPos, subStringSize - 1);
				if (subString.length() > 2 && (refPos = text.substr(0, i).find(subString)) != -1) {
					appendedString = to_string(currPos - refPos) + to_string(subStringSize - 1);
					encodedText.replace(currPos - replacedSize, subString.length(), appendedString);
					currPos += subString.length() - 1;
					replacedSize += subString.length() - appendedString.length();
				}
				currPos++;
				subStringSize = 2;
			}
			else {
				subStringSize++;
			}

		}
		return encodedText;
	}
}
int main()
{
	string test = "mantopmentopomantomen";

	string encodedTest = lz78::encode(test);
	cout << test << "\tsize: " << test.length()<<endl;

	cout << encodedTest << "\tEncoded size: "<< encodedTest.length()<<endl;
	//vector<lz77::Code> encodedTest = lz77::encode(test, 5);
	//for (size_t i = 0; i < encodedTest.size(); i++)
	//{
	//	cout << "<" << encodedTest[i].diff << "," << encodedTest[i].len << ",";
	//	cout<< encodedTest[i].sym << ">, ";
	//}
}
