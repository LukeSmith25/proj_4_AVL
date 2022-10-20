#ifndef AVL_TREE_STUDENT_PROJ4 
#define AVL_TREE_STUDENT_PROJ4 

#include "avl-tree-prof-proj4.h"
#include <queue>

/* Place your implementation of the AVLNode, AVLNode, and EncryptionTree classes
 * here.
 *
 * Your driver should #include this file, which (as you can see above) will also
 * #include the professor file.
 */

template<class Base>
AVLNode<Base>::~AVLNode() {
    if (getLeft()) {
        delete left;
    }
    if (getRight()) {
        delete right;
    }
}

template<class Base>
void AVLNode<Base>::printPreorder(ostream &os, string indent) const {
    if (this) {
        os << indent << getData() << endl;
        indent += "  ";
        getLeft()->printPreorder(os, indent);
        getRight()->printPreorder(os, indent);
    } else {
        os << indent << "NULL" << endl;
        indent.pop_back();
    }
}

template<class Base>
const AVLNode<Base> *AVLNode<Base>::minNode() const {
    if (getLeft() != NULL) {
        getLeft()->minNode();
    } else {
        return this;
    }
}

template<class Base>
const AVLNode<Base> *AVLNode<Base>::maxNode() const {
    if (getRight() != NULL) {
        getRight()->maxNode();
    } else {
        return this;
    }
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::singleRotateLeft() {
    AVLNode<Base> *leftMost = this->right->left;
    AVLNode<Base> *child = this->right;

    // If root = currNode
    if (this->root == this) {
        this->root = child;
    }

    // Assign leftMost to node's right ptr
    this->right = leftMost;

    // Move child to where parent was and set curNode as left child
    child->left = this;

    this->updateHeight();
    child->updateHeight();

    return child;
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::singleRotateRight() {
    AVLNode<Base> *rightMost = this->left->right;
    AVLNode<Base> *child = this->left;

    // If root = currNode
    if (this->root == this) {
        this->root = child;
    }

    // Assign rightmost to node's left ptr
    this->left = rightMost;

    // Move child to where parent was and set curNode as left child
    child->right = this;

    this->updateHeight();
    child->updateHeight();

    return child;
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::doubleRotateLeftRight() {
    this->left->singleRotateLeft();
    return singleRotateRight();
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::doubleRotateRightLeft() {
    this->right->singleRotateRight();
    return singleRotateLeft();;
}

template<class Base>
void AVLTree<Base>::insert(const Base &item) {
    // Declare new node to be inserted
    AVLNode<Base> *newNode = new AVLNode<Base>(item);

    // Check if root is NULL
    if (this->root == NULL) {
        this->root = newNode;
        newNode->updateHeight();
        return;
    }

    AVLNode<Base> *parNode = NULL;
    AVLNode<Base> *childNode = this->root;

    int depth = 0;
    AVLNode<Base> *path[32];

    while (childNode) {
        // Assign parNode to childNode
        parNode = childNode;
        // Add child node to path
        *path[depth] = *childNode;

        // If newNode = currNode, insert as currNode's right child
        if (!(parNode->getData() < item) && !(item < parNode->getData())) {
            return;
        }
        // If newNode < currNode
        else if (item < parNode->getData()) {
            childNode = childNode->left;
            depth++;
        }
        // If newNode > currNode
        else {
            childNode = childNode->right;
            depth++;
        }
    }

    // Insertion
    if (item < parNode->getData()) {
        parNode->left = newNode;
    }
    else {
        parNode->right = newNode;
    }

    // Update parent height
    newNode->updateHeight();
    parNode->updateHeight();

    // Rebalance and pass depth as numOnPath
    rebalancePathToRoot(path, depth);
}

template<class Base>
void AVLTree<Base>::remove(const Base &item) {
    AVLNode<Base> *toRemove = this->root;

    // Check for empty tree
    if (!toRemove) {
        return;
    }
    // Parent pointer used for rebalancing
    AVLNode<Base> *parent = NULL;

    // Path For Rebalancing
    int depth = 0;
    AVLNode<Base> *path[32];

    while (!(toRemove->getData() < item && item < toRemove->getData())) {
        // Move Parent
        parent = toRemove;
        // Add toRemove to path
       *path[depth] = *toRemove;
        // If item is less than toRemove, search left
        if (item < toRemove->getData()) {
            toRemove = toRemove->left;
        }
        // If item is greater than toRemove, search right
        if (toRemove->getData() < item) {
            toRemove = toRemove->right;
        }
        depth++;
    }

    // CASE 1: Internal Node With 2 Children
    if (toRemove->left && toRemove->right) {
        AVLNode<Base> *successor = toRemove->right;
        while (successor->left) {
            successor = successor->left;
        }

        // Copy data from successor
        toRemove->data = successor->getData();

        // Recursively delete successor
        remove(successor->getData());
    }
    // CASE 2: Root Node With 1 or 2 Children
    else if (toRemove == this->root) {
        if (toRemove->left) {
            this->root = toRemove->left;
        }
        else {
            this->root = toRemove->right;
        }
        delete toRemove;
    }
    // CASE 3: Internal Node With Left Child Only
    else if (toRemove->left) {
        parent->left = toRemove->left;
    }
    // CASE 4: Internal Node With Right Child Only OR Leaf
    else {
        parent->right = toRemove->right;
    }

    delete toRemove;

    // Rebalance path to root
    rebalancePathToRoot(path, depth);
}

template<class Base>
void AVLTree<Base>::printLevelOrder(ostream &os) const {
    // If the tree is empty, return null
    if (!root) {
        os << NULL;
        return;
    }

    // Traverse tree and print level order
    queue<AVLNode<Base>> levelOrder;
    levelOrder.push(this->root);

    while (!levelOrder.empty()) {
        AVLNode<Base> *temp = levelOrder.front();
        os << temp->getData() << " ";
        levelOrder.pop();

        // Push node's left child to the queue
        if (temp->left != NULL) {
            levelOrder.push(temp->left);
        }
        // Push node's right child to the queue
        if (temp->right != NULL) {
            levelOrder.push(temp->right);
        }
    }
}

template<class Base>
void AVLTree<Base>::rebalancePathToRoot(AVLNode<Base> **path, int numOnPath) {
    for ( ; numOnPath-1 >= 0; --numOnPath) {
        path[numOnPath]->updateHeight();

        // Check left height, -1 if null
        int leftHeight = -1;
        if (path[numOnPath]->left) {
            leftHeight = AVLNode<Base>::getHeight(path[numOnPath]->left);
        }

        // Check right height, -1 if null
        int rightHeight = -1;
        if (path[numOnPath]->right) {
            rightHeight = &path[numOnPath]->right->getHeight();
        }

        // Calculate balance
        int balance = leftHeight - rightHeight;

        // Check for right heavy imbalance
        if (balance == -2) {
            if (path[numOnPath]->right->left->getHeight() - path[numOnPath]->right->left->getHeight() == 1) {
                // Double rotate right then left
                path[numOnPath]->singleRotateRight();
            }
            // Single rotate left
            path[numOnPath]->singleRotateLeft();
        }

        // Check for left heavy imbalance
        else if (balance == 2) {
            if (path[numOnPath]->left->left->getHeight() - *path[numOnPath]->left->right->getHeight() == -1) {
                // Double rotate left then right
                path[numOnPath]->singleRotateLeft();
            }
            // Single rotate right
            path[numOnPath]->singleRotateRight();
        }

    }
}

/**
 * encrypt
 *
 * This function encrypts a cleartext message
 *
 * Parameters:
 *      item: encrypts item passed in based on location in BST.
 *
 * Return value:
 *      string: String that is modified to be encrypted.
 */
template<class Base>
string EncryptionTree<Base>::encrypt(const Base &item) const {
    const AVLNode<Base> *cur = this->root;
    string path = "r";

    while (cur != NULL) {

        // Equivalent comparison that returns path
        if (!(cur->getData() < item) && !(item < cur->getData())) {
            return path;
        }

            // Traverses left and adds 0
        else if (item < cur->getData()) {
            cur = cur->getLeft();
            path += "0";
        }

            // Traverses right and adds 1
        else if (cur->getData() < item) {
            cur = cur->getRight();
            path += "1";
        }

    }

    // If not found, returns ? for invalid word
    return "?";
}

/**
 * decrypt
 *
 * This function decrypts a BST with encrypted string passed in.
 *
 * Parameters:
 *      path: string representing encrypted path in BST
 *
 * Return value:
 *      Base: templated data of node
 */
template<class Base>
const Base *EncryptionTree<Base>::decrypt(const string &path) const {
    const AVLNode<Base> *cur = this->root;

    for (int i = 0; i < path.size(); i++) {
        if (path.at(i) == 'r') {
            cur = this->root;
        } else if (path.at(i) == '0') {
            cur = cur->getLeft();
        } else if (path.at(i) == '1') {
            cur = cur->getRight();
        } else {
            cur = NULL;
        }
        if (cur == NULL) {
            return NULL;
        }
    }
    return &(cur->getData());
}

#endif

