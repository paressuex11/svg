#include <iostream>
#include <string>
#include <vector>

using namespace std;


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
	int x;
	int y;
	string value;
	string label;
	string word;
	node* nodes[10];
	int rank;
	int child_number;
	bool whether_dad;
	bool whether_leaf;
	node(const string& value, int rank) {
		int x = 0;
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

};
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
	cout << root->word << endl;
	for (int i = 0; i < root->child_number; ++i) {
		print_tree(root->nodes[i]);
	}
}
void build_vector(node* root, vector<node*>* leaves) {
	if (root->whether_leaf) leaves->push_back(root);
	for (int i = 0; i < root->child_number; ++i) {
		build_vector(root->nodes[i], leaves);
	}
}
void build_x(node* root)
{
	//build x×ø±êÓÃ
}

int main() {

	string str = "(SBARQ (WHADJP (WRB How) (JJ many)) (S (NNS customers) (VP (VBP have) (NP (DT an) (NN account)))) (. ?))";

	node root(str, 1);
	build_tree(&root);
	build_vector(&root, &node::leaves);
	for (node* nodee : node::leaves) {
		cout << nodee->label << endl;
	}
	system("pause");
	return 0;
}