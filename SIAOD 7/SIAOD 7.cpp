#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>
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
				ref = buffer.find(data.substr(0, border - 1));
				dataIter = 0;
			}
			else
				ref = buffer.find(data.length() > 1 ? data.substr(0, data.length() - 1) : data.substr(0, data.length()));
			if (ref == string::npos) {
				encoded.push_back(Code{ 0,0,text[i] });

			}
			else if (buffer.length() < 5) {
				encoded.push_back(Code{ i - ((int)data.length() - 1) - ref,(int)data.length() - 1,text[i] });
			}
			else if (i + 1 < text.length()) {
				encoded.push_back(Code{ (i - ((int)data.length())) - ((i - 5) - ref),(int)data.length() - 1,text[i] });
			}
			else {
				encoded.push_back(Code{ (i - ((int)data.length()) - 1) - ((i - 5) - ref),(int)data.length(),text[i] });

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
namespace ShenonFano {
	// declare structure node
	struct node {

		// for storing symbol
		string sym;
		// for storing probability or frequency
		float pro;
		int arr[256];
		int top;
	} p[256];

	typedef struct node node;

	// function to find shannon code
	void shannon(int l, int h, node p[])
	{
		float pack1 = 0, pack2 = 0, diff1 = 0, diff2 = 0;
		int i, d, k, j;
		if ((l + 1) == h || l == h || l > h) {
			if (l == h || l > h)
				return;
			p[h].arr[++(p[h].top)] = 0;
			p[l].arr[++(p[l].top)] = 1;
			return;
		}
		else {
			for (i = l; i <= h - 1; i++)
				pack1 = pack1 + p[i].pro;
			pack2 = pack2 + p[h].pro;
			diff1 = pack1 - pack2;
			if (diff1 < 0)
				diff1 = diff1 * -1;
			j = 2;
			while (j != h - l + 1) {
				k = h - j;
				pack1 = pack2 = 0;
				for (i = l; i <= k; i++)
					pack1 = pack1 + p[i].pro;
				for (i = h; i > k; i--)
					pack2 = pack2 + p[i].pro;
				diff2 = pack1 - pack2;
				if (diff2 < 0)
					diff2 = diff2 * -1;
				if (diff2 >= diff1)
					break;
				diff1 = diff2;
				j++;
			}
			k++;
			for (i = l; i <= k; i++)
				p[i].arr[++(p[i].top)] = 1;
			for (i = k + 1; i <= h; i++)
				p[i].arr[++(p[i].top)] = 0;

			// Invoke shannon function
			shannon(l, k, p);
			shannon(k + 1, h, p);
		}
	}

	// Function to sort the symbols
	// based on their probability or frequency
	void sortByProbability(int n, node p[])
	{
		int i, j;
		node temp;
		for (j = 1; j <= n - 1; j++) {
			for (i = 0; i < n - 1; i++) {
				if ((p[i].pro) > (p[i + 1].pro)) {
					temp.pro = p[i].pro;
					temp.sym = p[i].sym;

					p[i].pro = p[i + 1].pro;
					p[i].sym = p[i + 1].sym;

					p[i + 1].pro = temp.pro;
					p[i + 1].sym = temp.sym;
				}
			}
		}
	}

	// function to display shannon codes
	void display(int n, node p[])
	{
		int i, j;
		cout << "\n\n\n\tSymbol\t\tProbability\tCode";
		for (i = n - 1; i >= 0; i--) {
			if (p[i].sym == " ")
				cout << "\n\t" << "\" \"" << "\t\t" << p[i].pro << "\t\t";
			else if (p[i].sym == "\n")
				cout << "\n\t" << "\\n" << "\t\t" << p[i].pro << "\t";
			else
				cout << "\n\t" << p[i].sym << "\t\t" << p[i].pro << "\t";
			for (j = 0; j <= p[i].top; j++)
				cout << p[i].arr[j];
		}
	}

	void calcProbs(string text, node p[]) {
		int symCount = 0;
		int counter = 0;
		float prob = 0;
		for (int i = 0; i < text.length(); i++) {

			symCount = 1;
			for (int j = i + 1; j < text.length(); j++)
			{
				if (text[i] == text[j])
					symCount++;
			}
			if (text.find(text[i]) == i) {
				p[counter].sym = text[i];
				prob = (float)symCount / (float)text.length();
				p[counter].pro = prob;
				counter++;
			}
		}
	}
	/*
Эни бэни рики паки Турбаурбасентибряки.
Может – выйдет, может – нет,
В общем – полный
Интернет
*/
	int calcTops(string text) {
		int count = 0;
		for (int i = 0; i < text.length(); i++)
		{
			if (text.find(text[i]) == i) {
				count++;
			}
		}
		return count;
	}
	string encode(string test) {
		int n = calcTops(test);
		calcProbs(test, ShenonFano::p);
		sortByProbability(n, ShenonFano::p);
		for (int i = 0; i < n; i++)
		{
			ShenonFano::p[i].top = -1;
		}
		shannon(0, n - 1, ShenonFano::p);
		display(n, ShenonFano::p);
		string res = "";
		for (char c : test)
		{
			for (auto e : p) {
				if (e.sym[0] == c) {
					for (int i = 0; i <= e.top; i++) {
						res += to_string(e.arr[i]);
					}
				}

			}
		}
		return res;
	}
}
namespace Hofman {
	// A Tree node
	struct Node
	{
		char ch;
		int freq;
		Node* left, * right;
	};

	// Function to allocate a new tree node
	Node* getNode(char ch, int freq, Node* left, Node* right)
	{
		Node* node = new Node();

		node->ch = ch;
		node->freq = freq;
		node->left = left;
		node->right = right;

		return node;
	}

	// Comparison object to be used to order the heap
	struct comp
	{
		bool operator()(Node* l, Node* r)
		{
			// highest priority item has lowest frequency
			return l->freq > r->freq;
		}
	};

	// traverse the Huffman Tree and store Huffman Codes
	// in a map.
	void encode(Node* root, string str,
		unordered_map<char, string>& huffmanCode)
	{
		if (root == nullptr)
			return;

		// found a leaf node
		if (!root->left && !root->right) {
			huffmanCode[root->ch] = str;
		}

		encode(root->left, str + "0", huffmanCode);
		encode(root->right, str + "1", huffmanCode);
	}

	//// traverse the Huffman Tree and decode the encoded string
	//void decode(Node* root, int& index, string str)
	//{
	//	if (root == nullptr) {
	//		return;
	//	}
	//	// found a leaf node
	//	if (!root->left && !root->right)
	//	{
	//		cout << root->ch;
	//		return;
	//	}
	//	index++;
	//	if (str[index] == '0')
	//		decode(root->left, index, str);
	//	else
	//		decode(root->right, index, str);
	//}

	// Builds Huffman Tree and decode given input text
	string buildHuffmanTree(string text)
	{
		// count frequency of appearance of each character
		// and store it in a map
		unordered_map<char, int> freq;
		for (char ch : text) {
			freq[ch]++;
		}

		// Create a priority queue to store live nodes of
		// Huffman tree;
		priority_queue<Node*, vector<Node*>, comp> pq;

		// Create a leaf node for each character and add it
		// to the priority queue.
		for (auto pair : freq) {
			pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
		}

		// do till there is more than one node in the queue
		while (pq.size() != 1)
		{
			// Remove the two nodes of highest priority
			// (lowest frequency) from the queue
			Node* left = pq.top(); pq.pop();
			Node* right = pq.top();	pq.pop();

			// Create a new internal node with these two nodes
			// as children and with frequency equal to the sum
			// of the two nodes' frequencies. Add the new node
			// to the priority queue.
			int sum = left->freq + right->freq;
			pq.push(getNode('\0', sum, left, right));
		}

		// root stores pointer to root of Huffman Tree
		Node* root = pq.top();

		// traverse the Huffman Tree and store Huffman Codes
		// in a map. Also prints them
		unordered_map<char, string> huffmanCode;
		encode(root, "", huffmanCode);

		cout << "Huffman Codes are :\n" << '\n';
		for (auto pair : huffmanCode) {
			cout << pair.first << " " << pair.second << '\n';
		}


		// print encoded string
		string str = "";
		for (char ch : text) {
			str += huffmanCode[ch];
		}
		return str;

		// traverse the Huffman Tree again and this time
		// decode the encoded string
		//int index = -1;
		//cout << "\nDecoded string is: \n";
		//while (index < (int)str.size() - 2) {
		//	decode(root, index, str);
		//}
	}

	/*
Эни бэни рики паки Турбаурбасентибряки.
Может – выйдет, может – нет,
В общем – полный
Интернет
*/

}

int main()
{
	setlocale(LC_ALL, "ru");
	ifstream fin("test.txt");
	string test = "Эни бэни рики паки Турбаурбасентибряки.\nМожет – выйдет, может – нет,\nВ общем – полный\nИнтернет";
	string tmp;
	//while (!fin.eof()) {
	//	getline(fin, tmp);
	//	if (!fin.eof())
	//		test += tmp;
	//}
	//fin.close();
	string str = Hofman::buildHuffmanTree(test);
	cout << "\nOriginal string size: " << test.length() * 8 << endl;
	cout << "\nEncoded size: " << str.length();




	//string encodedTest = ShenonFano::encode(test);
	//cout << endl << "test size: " << test.length() * 8 << endl;
	//cout << "encodedTest size: " << encodedTest.length() << endl;



	//string encodedTest = lz78::encode(test);
	//for (size_t i = 0; i < encodedTest.size(); i++)
	//{
	//	//cout << "<" << encodedTest[i].diff << "," << encodedTest[i].len << ",";
	//	//cout<< encodedTest[i].sym << ">, ";
	//	cout << encodedTest<<endl;
	//}
}
