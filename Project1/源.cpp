#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#define max(a, b) (a >= b ? a : b)
using namespace std;
const int view_x = 1100;
const int view_y = 500;
const int char_width = 10;
const int char_lower_width = 7;

void replace(string& str, const string& str1,const string& str2) {
	size_t left = str.find(str1);
	if (left == string::npos) {
		return;
	}
	str.replace(left, str1.length(), str2);
}
string get_next_child(const string &str) {
	string result = "";
	int left = str.find('(', 1);
	int count = 0;
	unsigned int i = 0;
	for (i = left; i < str.length(); ++i) {
		result += str[i];
		if (str[i] == '(') count++;
		if (str[i] == ')') count--;
		if (!count) {
			break;
		}
	}
	return result;
}
string get_label(const string& str);
string get_word(const string & str);
class node {
public:
	static vector<node*> leaves;
	static vector<node*> no_leaves;
	double x;
	double y;
	string value;
	string label;
	string word;
	int node_length;
	node* nodes[100];
	int rank;
	int child_number;
	bool whether_dad;
	bool whether_leaf;
	node(const string& value, int rank) {
		this->x = 0;
		this->y = 0;
		this->value = value;
		this->child_number = 0;
		this->rank = rank;
		this->whether_have_child();
		this->label = get_label(value);
		replace(this->label, "<", "&lt");
		replace(this->label, ">", "&gt");
		this->whether_leaf = false;
		if (!this->whether_dad) {
			this->word = get_word(value);
			this->whether_leaf = true;
			replace(this->word, "<", "&lt");
			replace(this->word, ">", "&gt");
		}
		else {
			this->word = "";
		}
		this->node_length = max(this->label.length(), this->word.length());

	}
	void add_child(const string &child) {
		node* children = new node(child, this->rank + 1);
		this->erase(child);
		this->whether_have_child();
		this->nodes[this->child_number] = children;
		this->child_number++;
	}
	void erase(const string& out) {
		int left = this->value.find(out);
		this->value.erase(left, out.length());
	}

	void whether_have_child() {
		for (unsigned int i = 1; i < this->value.length(); ++i) {
			if (this->value[i] == ')') {
				this->whether_dad = false;
				return;
			}
			if (this->value[i] == '(') {
				this->whether_dad = true;
				return;
			}
		}
	}
	void build_svg() {
		string rect = "<rect x = \"%x\" y = \"%y\" height = \"17\" fill-opacity=\"0.9\"  stroke-width=\"3\" stroke = \"#00FF00\" width = \"%z\" style = \"fill:url(#grlue)\" />";
		
		replace(rect, "%x", to_string(this->x ));
		replace(rect, "%y", to_string(this->y ));
		replace(rect, "%z", to_string(this->node_length * char_width));
		string word = "";
		int width = char_lower_width;
		if (this->word != "") {
			word = "<rect x = \"%x\" y = \"%y\" height = \"17\" fill-opacity=\"0.9\"  stroke-width=\"3\" stroke = \"#00FF00\" width = \"%z\" style = \"fill:url(#grlue)\" />";
			replace(word, "%x", to_string(this->x ) );
			replace(word, "%y", to_string(this->y + 18));
			replace(word, "%z", to_string(this->node_length * 10));
			string text = "<text font-size=\"12\" x=\"%x\" y=\"%y\" fill=\"#000000\">" + this->word + "</text>";
			for (char c : this->word) {
				if (c <= 'Z' && c >= 'A' ) width = char_width;
				break;
			}
			replace(text, "%x", to_string(this->x + this->node_length*char_width / 2.0 - this->word.length()*width / 2.0));
			replace(text, "%y", to_string(this->y + 33));
			cout << word + text;
		}
		for (char c : this->label) {
			if (c <= 'Z' && c >= 'A') width = char_width;
			break;
		}
		string text = "<text font-size=\"12\" x=\"%x\" y=\"%y\" fill=\"#000000\">" +this->label+  "</text>";
		replace(text, "%x", to_string(this->x + this->node_length*char_width / 2.0 - this->label.length()*width / 2.0) );
		replace(text, "%y", to_string(this->y + 15));
		cout << rect +  text;
	}
	void build_lines() {
		string line = "<line stroke=\"#67C23A\" stroke-width=\"3\" stroke-opacity=\"0.6\" x1=\"%x1\" y1=\"%y1\" x2=\"%x2\" y2=\"%y2\"/>";
		replace(line, "%x1", to_string(this->x + this->node_length*char_width / 2.0) );
		replace(line, "%y1", to_string(this->y ));
		for (int i = 0; i < this->child_number; ++i) {
			string linee = line;
			replace(linee, "%x2", to_string(this->nodes[i]->x + this->nodes[i]->node_length*char_width / 2.0) );
			replace(linee, "%y2", to_string(this->nodes[i]->y) );
			cout << linee;
		}
	}
	void init() {
		node::leaves.clear();
		node::no_leaves.clear();
	}

};
vector<node*> node::no_leaves;
vector<node*> node::leaves;
string get_word(const string & str) {
	int left = str.find(' ');
	int right = str.find(')');
	return str.substr(left + 1, right - left - 1);
}
string get_label(const string& str) {
	return str.substr(1, str.find(' ', 0) - 1);
}
void build_tree(node* root) {
	while (root->whether_dad) {
		string str = get_next_child(root->value);
		root->add_child(str);
	}
	for (int i = 0; i < root->child_number; ++i) {
		build_tree(root->nodes[i]);
	}
}
void print_tree(node* root) {
	cout << root->x << '\t' << root->y << endl;
	for (int i = 0; i < root->child_number; ++i) {
		print_tree(root->nodes[i]);
	}
}
void build_vector(node* root, vector<node*>* leaves, vector<node*>* no_leaves) {
	if (root->whether_leaf) leaves->push_back(root);
	else no_leaves->push_back(root);
	for (int i = 0; i < root->child_number; ++i) {
		build_vector(root->nodes[i], leaves, no_leaves);
	}
}
void build_x()
{
	int max_rank = 0;
	int last_x = view_x - node::leaves[node::leaves.size() - 1]->node_length;
	int sum = 0;
	for (int i = 0; i < node::leaves.size(); ++i) {
		if (node::leaves[i]->rank > max_rank) max_rank = node::leaves[i]->rank;
		sum += node::leaves[i]->node_length * char_width;
	}
	double space_len = (last_x - sum)* 1.0 / (node::leaves.size()-1);


	for (int i = 0; i < node::leaves.size(); ++i) {
		if (!i) node::leaves[i]->x = 0;
		else {
			node::leaves[i]->x = node::leaves[i - 1]->x + node::leaves[i - 1]->node_length * 10 + space_len;
		}
	}
	for (int i = 0; i < node::leaves.size(); ++i) {
		node::leaves[i]->y = 1.0  * node::leaves[i]->rank / max_rank * view_y;
	}
	while (!node::no_leaves.empty())
	{
		node* no_leaf = node::no_leaves[node::no_leaves.size() - 1];
		no_leaf->y = 1.0 / (max_rank) * no_leaf->rank * view_y;
		node::no_leaves.pop_back();
		for (int i = 0; i < no_leaf->child_number; ++i) {
			no_leaf->x += no_leaf->nodes[i]->x; 
		}
		no_leaf->x = no_leaf->x * 1.0 / no_leaf->child_number;
	}
	
	//build x×ø±êÓÃ
}
void build_html(node* root) {
	root->build_svg();
	for (int i = 0; i < root->child_number; ++i) {
		build_html(root->nodes[i]);
	}
}
void build_lines(node* root) {
	root->build_lines();
	for (int i = 0; i < root->child_number; ++i) {
		build_lines(root->nodes[i]);
	}
}

int main() {
	fstream input("C:/Users/11385/Desktop/project1/input.txt");
	for (int i = 0; i < 20; ++i) {
		string str = "";
		getline(input, str);
		if (str == "") continue;
		string filename = "C:/Users/11385/Desktop/a" + to_string(i) + ".html";
		freopen(filename.c_str(), "w", stdout);
		
		/*string str = "(S (S (VP (VB Show) (NP (DT all) (NX (NN card) (NN type) (NNS codes))))) (. .))";*/
		node root(str, 1);
		build_tree(&root);
		build_vector(&root, &node::leaves, &node::no_leaves);
		build_x();


		cout << R"(<html><svg id = "svg" xmlns = "http://www.w3.org/2000/svg" height = "100%" width = "100%" version = "1.1">)";
		cout << R"(<defs>
                    <linearGradient id="grlue" x1="0%" y1="0%" x2="0%" y2="100%">
                    <stop offset="0%" style="stop-color:rgb(25,255,0);
                    stop-opacity:1"/>
                    <stop offset="100%" style="stop-color:rgb(0,255,255);
                    stop-opacity:1"/>
                    </linearGradient>
                    </defs>)";
		build_lines(&root);
		build_html(&root);
		cout << R"(</svg></html>)";
		root.init();
		
	}
	system("pause");
	return 0;
}