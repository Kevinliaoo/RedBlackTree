#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    int data;
    Node *parent;
    Node *left;
    Node *right;
    int color; // 1 --> Red    0 --> Black
};

typedef Node *NodePtr;

class RedBlackTree
{
private:
    NodePtr root;
    NodePtr TNULL;

    void printHelper(NodePtr root, string indent, bool last)
    {
        if (root != TNULL)
        {
            cout << indent;

            if (last)
            {
                cout << "R----";
                indent += "   ";
            }
            else
            {
                cout << "L----";
                indent += "|  ";
            }

            string sColor = root->color ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")" << endl;

            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

    void printPath(vector<NodePtr> &path)
    {
        for (int i = 0; i < path.size(); i++)
            cout << path[i]->data << " --> ";

        cout << endl;
    }

    void twoRedChildren(vector<NodePtr> &path)
    {
        // Search for nodes with two red children
        for (int i = 0; i < path.size(); i++)
        {
            if (path[i]->left->color == 1 && path[i]->right->color == 1)
            {
                if (path[i]->left->data != NULL && path[i]->right->data != NULL)
                {
                    // cout << "[Log]: " << path[i]->data << " has two red children\n";
                    path[i]->right->color = 0;
                    path[i]->left->color = 0;
                    path[i]->color = 1;

                    // cout << "Pintando: " << path[i]->data << " de rojo\n";
                    // cout << "Pintando: " << path[i]->right->data << " de negro\n";
                    // cout << "Pintando: " << path[i]->left->data << " de negro\n";

                    if (path[i] == this->root)
                        path[i]->color = 0;
                }
            }
        }
    }

    bool twoConsecutiveRed(vector<NodePtr> &path, bool colorChange = true)
    {
        bool changesMade = false;
        for (int i = 0; i < path.size(); i++)
        {
            if (path[i]->color == 1 && i != path.size() - 1)
            {
                // Two consecutive red Nodes were spotted
                if (path[i + 1]->color == 1)
                {
                    if (path[i] == path[i]->parent->right)
                    {
                        if (path[i]->right == path[i + 1]) // 1
                        {
                            leftRotate(path[i]->parent);
                            changesMade = true;
                        }
                        else if (path[i]->left == path[i + 1]) // 2
                        {
                            NodePtr temp = path[i]->parent;
                            rightRotate(path[i]);
                            leftRotate(temp);
                            changesMade = true;
                        }
                    }
                    else if (path[i] == path[i]->parent->left)
                    {
                        if (path[i]->left == path[i + 1]) // 3
                        {
                            rightRotate(path[i]->parent);
                            changesMade = true;
                        }
                        else if (path[i]->right == path[i + 1]) // 4
                        {
                            NodePtr temp = path[i]->parent;
                            leftRotate(path[i]);
                            rightRotate(temp);
                            changesMade = true;
                        }
                    }
                }
            }
        }

        return changesMade;
    }

    vector<NodePtr> createPath(int key)
    {
        NodePtr temp = this->root;
        vector<NodePtr> path;

        if (temp == TNULL)
            return path;

        while (temp != nullptr)
        {
            if (temp != TNULL)
                path.push_back(temp);

            if (key >= temp->data)
                temp = temp->right;
            else
                temp = temp->left;
        }

        return path;
    }

    void insertPrev(int key)
    {
        // cout << "[Log]: Previous insertion: \n";
        // printTree();
        vector<NodePtr> path = createPath(key);

        twoRedChildren(path);
        // cout << "[Log]: Two red children:\n";
        // printTree();
        twoConsecutiveRed(path);
    }

    void insertFix(int key)
    {
        vector<NodePtr> path = createPath(key);

        bool changesMade = twoConsecutiveRed(path, false);
        while (changesMade)
        {
            path = createPath(key);
            changesMade = twoConsecutiveRed(path, false);
        }
    }

    NodePtr searchHelper(NodePtr node, int key)
    {
        if (node == TNULL || key == node->data)
        {
            return node;
        }

        if (key < node->data)
            return searchHelper(node->left, key);

        return searchHelper(node->right, key);
    }

public:
    RedBlackTree()
    {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->right = nullptr;
        TNULL->left = nullptr;
        root = TNULL;
    }

    void printTree()
    {
        if (root)
            printHelper(this->root, "", true);
    }

    void leftRotate(NodePtr x)
    {
        // y will be the new root
        NodePtr y = x->right;
        // Swap children
        x->right = y->left;

        if (y->left != TNULL)
            y->left->parent = x;

        y->parent = x->parent;

        // If x is the root
        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;

        // if (y == this->root)
        //     y->color = 0;
        // else
        //     y->color = 1;

        y->color = 0;

        if (y->right != nullptr)
            y->right->color = 1;
        if (y->left != nullptr)
            y->left->color = 1;
    }

    void rightRotate(NodePtr x)
    {
        NodePtr y = x->left;
        x->left = y->right;

        if (y->right != TNULL)
            y->right->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;

        // if (y == this->root)
        //     y->color = 0;
        // else
        //     y->color = 1;

        y->color = 0;
        if (y->right != nullptr)
            y->right->color = 1;
        if (y->left != nullptr)
            y->left->color = 1;
    }

    NodePtr search(int key)
    {
        return searchHelper(this->root, key);
    }

    NodePtr getRoot()
    {
        return this->root;
    }

    void insertNode(int key)
    {
        insertPrev(key);

        NodePtr node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1; // Default red

        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != TNULL)
        {
            y = x;
            if (node->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;

        if (y == nullptr)
            this->root = node;
        else if (node->data < y->data)
            y->left = node;
        else
            y->right = node;

        // New node inserted is the first one (root)
        if (node->parent == nullptr)
        {
            node->color = 0;
            return;
        }

        // New node's parent is the root
        if (node->parent->parent == nullptr)
            return;

        insertFix(key);
    }

    int getHeight(NodePtr node)
    {
        if (node == TNULL)
            return 0;

        int lheight = getHeight(node->left);
        int rheight = getHeight(node->right);

        if (lheight > rheight)
            return lheight + 1;
        else
            return rheight + 1;
    }

    void inOrder(vector<NodePtr> &vect, NodePtr node)
    {
        if (node == TNULL)
            return;

        inOrder(vect, node->left);
        vect.push_back(node);
        inOrder(vect, node->right);
    }

    void getCurrentLevel(NodePtr node, int level, vector<NodePtr> &vect)
    {
        if (node == TNULL)
            return;
        if (level == 1)
            vect.push_back(node);
        else if (level > 1)
        {
            getCurrentLevel(node->left, level - 1, vect);
            getCurrentLevel(node->right, level - 1, vect);
        }
    }

    void levelOrder(vector<NodePtr> &vect, NodePtr node)
    {
        int h = getHeight(this->root);

        for (int i = 1; i <= h; i++)
            getCurrentLevel(this->root, i, vect);
    }
};

vector<int> readInput()
{
    vector<int> res;
    int num;

    while (cin >> num)
        res.push_back(num);

    return res;
}

void printVector(const vector<int> &vect)
{
    for (int i = 0; i < vect.size(); i++)
        cout << vect[i] << " ";
    cout << endl;
}

vector<NodePtr> getTeam(vector<NodePtr> &vect, int team)
{
    vector<NodePtr> tList;

    for (int i = 0; i < vect.size(); i++)
    {
        if (vect[i]->color == team)
            tList.push_back(vect[i]);
    }

    return tList;
}

void printTeam(vector<NodePtr> &team, vector<NodePtr> &teamInOrder, string color)
{
    cout << color << " team: ";
    for (int i = 0; i < teamInOrder.size(); i++)
    {
        if (i == teamInOrder.size() - 1)
            cout << teamInOrder[i]->data << endl;
        else
            cout << teamInOrder[i]->data << ",";
    }

    cout << "outfield: ";

    for (int i = team.size() - 3; i < team.size(); i++)
    {
        if (i == team.size() - 1)
            cout << team[i]->data << endl;
        else
            cout << team[i]->data << ",";
    }

    cout << "jump ball: " << team[0]->data << endl;
}

int main()
{
    RedBlackTree rbt;

    vector<int> input = readInput();

    for (int i = 0; i < input.size(); i++)
    {
        // cout << "[Log]: Inserting " << input[i] << endl;
        rbt.insertNode(input[i]);
        // cout << "[Log]: Finished inserting: " << input[i] << endl;
        // rbt.printTree();
    }

    // rbt.printTree();

    vector<NodePtr> inOrderRes;
    rbt.inOrder(inOrderRes, rbt.getRoot());

    vector<NodePtr> levelRes;
    rbt.levelOrder(levelRes, rbt.getRoot());

    vector<NodePtr> redTeam = getTeam(levelRes, 1);   // Get red team members
    vector<NodePtr> blackTeam = getTeam(levelRes, 0); // Get black team members

    vector<NodePtr> redTeamOrder = getTeam(inOrderRes, 1);
    vector<NodePtr> blackTeamOrder = getTeam(inOrderRes, 0);

    if (redTeam.size() < 4 || blackTeam.size() < 4)
        cout << "No game" << endl;
    else
    {
        printTeam(redTeam, redTeamOrder, "Red");
        printTeam(blackTeam, blackTeamOrder, "Black");
    }
}