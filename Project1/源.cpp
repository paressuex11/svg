#include <iostream>
#include <string>
#include <vector>

using namespace std;

void replace(string& str, const string& str1,const string& str2) {
	size_t left = str.find(str1);
	if (left == string::npos) {
		cout << "not found";
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
	node* nodes[10];
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
		this->whether_leaf = false;
		if (!this->whether_dad) {
			this->word = get_word(value);
			this->whether_leaf = true;
		}
		else {
			this->word = "";
		}
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
		string rect = "<rect x = \"%x\" y = \"%y\" height = \"17\" width = \"%z\" style = \"fill:url(#grlue)\" />";
		replace(rect, "%x", to_string(this->x * 100) + "%");
		replace(rect, "%y", to_string(this->y * 100) + "%");
		replace(rect, "%z", to_string(this->label.length() * 9));
		string text = "<text font-size=\"12\" x=\"%x\" y=\"%y\" fill=\"#000000\">of</text>";
		replace(text, "%x", to_string(this->x * 100) + "%");
		cout << rect;
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
	double offset = 0.00001;
	double off = 1.0 / node::leaves.size();

	while (off - offset  > 0.005) {
		offset += 0.001;
	}
	for (int i = 0; i < node::leaves.size(); ++i) {
		node::leaves[i]->x = 1.0  * (i + 1) / node::leaves.size() - offset;
		if (node::leaves[i]->rank > max_rank) max_rank = node::leaves[i]->rank;
	}
	for (int i = 0; i < node::leaves.size(); ++i) {
		node::leaves[i]->y = 1.0  * node::leaves[i]->rank / max_rank - 0.1;
	}
	while (!node::no_leaves.empty())
	{
		node* no_leaf = node::no_leaves[node::no_leaves.size() - 1];
		no_leaf->y = 1.0 / (max_rank) * no_leaf->rank - 0.1;
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

int main() {

	string str = "(SBARQ (WHADJP (WRB How) (JJ many)) (S (NNS customers) (VP (VBP have) (NP (DT an) (NN account)))) (. ?))";

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

	build_html(&root);
	cout << R"(</svg></html>)";
	system("pause");
	return 0;
}