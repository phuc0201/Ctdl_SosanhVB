#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include<time.h>

using namespace std;

struct Node 
{
    string data;
    Node* left;
    Node* right;
    int sml;
    int height;
};

int _max(int a, int b);

// Calculate height
int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int _max(int a, int b) {
    return (a > b) ? a : b;
}

double Jaro(string s1, string s2)
{
    if (s1 == s2)
        return 1.0;

    int len1 = s1.length(),
        len2 = s2.length();

    int max_dist = floor(max(len1, len2) / 2) - 1;
    int match = 0;
    int* hash_s1 = new int[s1.length()]{ 0 };
    int* hash_s2 = new int[s2.length()]{ 0 };
    for (int i = 0; i < len1; i++) {
        for (int j = max(0, i - max_dist);
            j < min(len2, i + max_dist + 1); j++)
            if (s1[i] == s2[j] && hash_s2[j] == 0) {
                hash_s1[i] = 1;
                hash_s2[j] = 1;
                match++;
                break;
            }
    }
    if (match == 0)
        return 0.0;
    double t = 0;

    int point = 0;
    for (int i = 0; i < len1; i++)
        if (hash_s1[i]) {

            while (hash_s2[point] == 0)
                point++;

            if (s1[i] != s2[point++])
                t++;
        }

    t /= 2;
    return (((double)match) / ((double)len1)
        + ((double)match) / ((double)len2)
        + ((double)match - t) / ((double)match))
        / 3.0;
}
double JaroWinkler(string s1, string s2)
{
    double JaroDist = Jaro(s1, s2);

    if (JaroDist > 0.7)
    {
        int prefix = 0;
        for (int i = 0; i < min(s1.length(), s2.length()); i++)
        {
            if (s1[i] == s2[i])
                prefix++;
            else
                break;
        }
        prefix = min(4, prefix);

        JaroDist += 0.1 * prefix * (1 - JaroDist);
    }
    return JaroDist;
}
// New node creation
Node* newNode(string data) {
    Node* node = new Node();
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->sml = 1;
    node->height = 1;
    return (node);
}
// Rotate right
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = _max(height(y->left),
        height(y->right)) +
        1;
    x->height = _max(height(x->left),
        height(x->right)) +
        1;
    return x;
}

// Rotate left
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = _max(height(x->left),
        height(x->right)) +
        1;
    y->height = _max(height(y->left),
        height(y->right)) +
        1;
    return y;
}

// Get the balance factor of each node
int getBalanceFactor(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) -
        height(N->right);
}


Node* insertNode(Node* node, string data)
{
    // Find the correct postion and insert the node
    if (node == NULL)
        return newNode(data);
    if (JaroWinkler(node->data, data) >= 0.9)
    {
        node->sml++;
    }
    else if (data < node->data)
        node->left = insertNode(node->left, data);
    else if (data > node->data)
        node->right = insertNode(node->right, data);
    // Update the balance factor of each node and
    // balance the tree
    node->height = 1 + _max(height(node->left),
        height(node->right));
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor > 1) {
        if (data < node->left->data) {
            return rightRotate(node);
        }
        else if (data > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }
    if (balanceFactor < -1) {
        if (data > node->right->data) {
            return leftRotate(node);
        }
        else if (data < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }
    return node;
}

int FindNode(Node* root, string value)
{
    int count = 0;
    Node* temp = root;
    while (temp != nullptr)
    {
        if (JaroWinkler(value, temp->data) >= 0.9)
        { 
            break;
        }
        if (temp->data < value)
            temp = temp->right;
        else if (temp->data > value)
            temp = temp->left;
    }
    if (temp == nullptr)
        return 0;
    else
        return temp->sml;

}
int CheckIfExist(string str, vector<string> _vct)
{
    for (int i = 0; i < _vct.size(); i++)
    {
        if (JaroWinkler(str, _vct[i]) >= 0.9)
            return 1;
    }
    return 0;
}
double Result(Node* Parent1, Node* Parent2, vector<string> vct)
{
    int sum = 0;
    int u = 0, v = 0;
    vector<int> vct_f1;
    vector<int> vct_f2;
    for (int i = 0; i < vct.size(); i++)
    {
        vct_f1.push_back(FindNode(Parent1, vct[i]));
        vct_f2.push_back(FindNode(Parent2, vct[i]));
    }
    for (int i = 0; i < vct_f1.size(); i++)
    {
        sum += (vct_f1[i] * vct_f2[i]);
        u += (vct_f1[i] * vct_f1[i]);
        v += (vct_f2[i] * vct_f2[i]);
    }
    cout << "\n";
    /*
    for (int i = 0; i < vct_f1.size(); i++)
        cout << vct_f1[i] << " ";
    cout << "\n";
    for (int i = 0; i < vct_f1.size(); i++)
        cout << vct_f2[i] << " ";
    cout << "\n";*/
    double T = (double)sum / (sqrt(u) * sqrt(v));
    return T;
}
void DocFile(Node*& root, vector<string>& vct, string name)
{
    int count = 0;
    string f;
    ifstream file;
  //  ifstream file2;
    file.open(name, ios::in);
   // file2.open(name_f2, ios::in);
    string str1 = "";
    string str2 = "";
    while (!(file.eof()))
    {
        file >> f;
        str1 += f;
        count++;
        if (f[f.size() - 1] == '.')
        {
            if (CheckIfExist(str1, vct) == 0 && count != 1)
            {
               vct.push_back(str1);
            }
            root = insertNode(root, str1);
            str1 = "";
            count = 0;
        }
    }
    count = 0;
    if (str1 != "")
    {
        if (CheckIfExist(str1, vct) == 0)
            vct.push_back(str1);
        root = insertNode(root, str1);
    }
    if (vct[vct.size() - 1] == "")
        vct.pop_back();
}
int check_rong(string namefile)
{
    ifstream file;
    file.open(namefile, ios::in);
    string f;
    string str = "";
    while (!file.eof())
    {
        file >> f;
        str += f;
        if (str != "")
            return 1;
        str = "";
    }
    return 0;
}
void NhapTenFile(string& NameFile)
{
    ifstream file;
    do
    {
        cout << "\nNhap ten file ban muon so sanh: \n";
        cin >> NameFile;
        file.open(NameFile, ios::in);
        if (!file.is_open() || check_rong(NameFile) == 0)
            cout << "\nKhong hop le!\nVUI LONG NHAP LAI!\n";
    }while(!file.is_open() || check_rong(NameFile)==0);
}
void sosanhTungfile()
{
    clock_t start, end;
    string namef;
    Node* root_f = NULL;
    vector<string> vct;
    int n;
    NhapTenFile(namef);
    cout << "\n";
    //cin >> n;
    cout << "Ban muon so sanh voi bao nhieu file: ";
    while (!(cin >> n)) {
        cerr << "Vui long nhap so nguyen: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }
    string* MangFile = new string[n];
    for (int i = 0; i < n; i++)
    {
        cout << "\nFile thu " << i + 1;
        NhapTenFile(MangFile[i]);
    }
    start = clock();
    cout << "\n-----------MUC DO TUONG TU----------\n";
    cout << "\n------------------------------------";
    for (int i = 0; i < n; i++)
    {
        root_f = NULL;
        Node* root1 = NULL;
        DocFile(root_f, vct, namef);
        DocFile(root1, vct, MangFile[i]);
        cout << "|          File " << i + 1 << ": " << Result(root_f, root1, vct) << "        |";
        vct.clear();
    }
    cout << "\n------------------------------------\n";
    end = clock();
    cout << "Thoi gian thuc thi: " << (double)(end - start)/CLOCKS_PER_SEC;
}
void sosanhvoiNhieuFile()
{
    clock_t start, end;
    Node* root_f = NULL;
    string namef;
    vector<string> vct;
    Node* root2 = NULL;
    int n;
    NhapTenFile(namef);
    cout << "\n";
    cout << "Ban muon so sanh voi bao nhieu file: ";
    while (!(cin >> n)) {
        cerr << "Vui long nhap so nguyen: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    string* MangFile = new string[n];
    for (int i = 0; i < n; i++)
    {
        cout << "\nFile thu " << i + 1;
        NhapTenFile(MangFile[i]);
    }
    start = clock();
    cout << "\n----------MUC DO TUONG TU---------\n";
    cout << "\n----------------------------------";
    DocFile(root_f, vct, namef);
    for (int i = 0; i < n; i++)
    {
        DocFile(root2, vct, MangFile[i]);
    }
    cout << "|          File: " << Result(root_f, root2, vct) << "        |";
    vct.clear();
    cout << "\n----------------------------------\n";
    end = clock();
    cout << "Thoi gian thuc thi: " << (double)(end - start) / CLOCKS_PER_SEC;
}
int main()
{
    int m;
    do
    {
        cout << "BAN MUON SO SANH";
        cout << "\n1.Voi tung file";
        cout << "\n2.Voi nhieu file";
        cout << "\n3.Thoat chuong trinh";
        cout << "\n\nSu lua chon cua ban: ";
        while (!(cin >> m)) { 
            cerr << "Vui long nhap nhu huong dan: ";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
        system("cls");
        switch (m)
        {
        case 1:
            sosanhTungfile();
            break;

        case 2:
            sosanhvoiNhieuFile();
            break;
        }
        cout << "\n\n";
    } while (m == 1 ||m == 2);
   // for (int i = 0; i < vct.size(); i++)
   //     cout << vct[i] <<"\n";
    return 0;
}