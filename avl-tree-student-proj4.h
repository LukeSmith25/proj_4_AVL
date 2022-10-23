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
    this->left = this->left->singleRotateLeft();
    return this->singleRotateRight();
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::doubleRotateRightLeft() {
    this->right = this->right->singleRotateRight();
    return this->singleRotateLeft();
}

template<class Base>
void AVLTree<Base>::insert(const Base &item) {
    // Declare new node to be inserted
    AVLNode<Base> *newNode = new AVLNode<Base>(item);

    // Create parent and child pointers
    AVLNode<Base> *parNode = NULL;
    AVLNode<Base> *childNode = this->root;

    // Set depth and path of pointers
    int depth = -1;
    AVLNode<Base> *path[32];

    while (childNode) {
        // Assign parNode to childNode
        parNode = childNode;
        // Add child node to path
        path[++depth] = childNode;

        // If newNode = currNode, insert as currNode's right child
        if (!(childNode->getData() < item) && !(item < childNode->getData())) {
            return;
        }
        // If newNode < currNode
        else if (item < childNode->getData()) {
            childNode = childNode->left;
        }
        // If newNode > currNode
        else {
            childNode = childNode->right;
        }
    }

    // Insertion
    if (!root) {
        this->root = newNode;
    }
    else if (item < parNode->getData()) {
        parNode->left = newNode;
    }
    else {
        parNode->right = newNode;
    }

    // Update parent height
    newNode->updateHeight();
    if (parNode) {
        parNode->updateHeight();
    }

    // Rebalance and pass depth as numOnPath
    if (depth >= 0) {
        rebalancePathToRoot(path, depth);
    }

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
    int depth = -1;
    AVLNode<Base> *path[32];
    path[++depth] = root;

    while (toRemove) {

        // If item is found
        if (!(toRemove->getData() < item) && !(item < toRemove->getData())) {
            break;
        }

        // Move Parent
        parent = toRemove;
        // Add toRemove to path
        path[++depth] = toRemove;

        // If item is less than toRemove, search left
        if (item < toRemove->getData()) {
            toRemove = toRemove->left;
        }
        // If item is greater than toRemove, search right
        else if (toRemove->getData() < item) {
            toRemove = toRemove->right;
        }
        // If item is not found
        else {
            return;
        }
        if(!toRemove){
            return;
        }
    }

    // CASE 1: Internal Node With 2 Children
    if (toRemove->left && toRemove->right) {
        AVLNode<Base> *successor = toRemove->right;
        path[++depth] = successor;
        while (successor->left) {
            successor = successor->left;
            path[++depth] = successor;
        }

        // Copy data from successor
        toRemove->data = successor->getData();

        if(toRemove->right == successor){
            path[++depth] = toRemove;
            toRemove->right = successor->right;
            successor->right = NULL;
        } else{
            // Recursively delete successor
            path[depth - 1]->left = path[depth]->right;
            path[depth--]->right = NULL;
        }

        delete successor;
    }
    // CASE 2: Root Node With 1 or 2 Children
    else if (toRemove == this->root) {
        if (toRemove->left) {
            this->root = toRemove->left;
            toRemove->left = NULL;
        }
        else {
            this->root = toRemove->right;
            toRemove->right = NULL;
        }
        delete toRemove;
    }
    // CASE 3: Internal Node With Left Child Only
    else if (toRemove->left) {
        if(parent->right == toRemove){
            parent->right = toRemove->left;
        } else{
            parent->left = toRemove->left;
        }
        toRemove->left = nullptr;
        delete toRemove;
    }
    // CASE 4: Internal Node With Right Child Only
    else if (toRemove->right) {
        if(parent->left == toRemove){
            parent->left = toRemove->left;
        } else{
            parent->right = toRemove->left;
        }
        toRemove->right = nullptr;
        delete toRemove;
    }
    // CASE 5: Remove Leaf
    else if (toRemove->left == NULL && toRemove->right == NULL) {
        if (parent->left == toRemove) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
        delete toRemove;
    }


    // Rebalance path to root
    if (depth >= 0) {
        rebalancePathToRoot(path, depth);
    }
}

template<class Base>
void AVLTree<Base>::printLevelOrder(ostream &os) const {
    // If the tree is empty, return null
    if (!root) {
        os << "NULL" << endl;
        return;
    }

    // Traverse tree and print level order
    queue<AVLNode<Base>*> levelOrder;
    levelOrder.push(this->root);

    bool last = false;
    int count = 0;

    while (!levelOrder.empty()) {
        AVLNode<Base> *temp = levelOrder.front();
        levelOrder.pop();

        // Push node's left child to the queue
        if (temp) {
            levelOrder.push(temp->left);
            levelOrder.push(temp->right);
        }

        // Check for last word
        if (levelOrder.size() == 0) {
            last = true;
        }
        if (temp) {
            os << temp->getData();
        }
        else {
            os << "NULL";
        }
        count++;

        if (!last && count != 20) {
            os << " ";
        }
        else {
            count = 0;
            os << endl;
        }
    }
    if (!last) {
        os << endl;
    }
}

template<class Base>
void AVLTree<Base>::rebalancePathToRoot(AVLNode<Base> **path, int numOnPath) {
    for ( ; numOnPath >= 0; numOnPath--) {
        path[numOnPath]->updateHeight();

        // Check left height, -1 if null
        int lh = AVLNode<Base>::getHeight(path[numOnPath]->left);

        // Check right height, -1 if null
        int rh = AVLNode<Base>::getHeight(path[numOnPath]->right);

        AVLNode<Base> *replacement = NULL;
        // Check for right heavy imbalance
        if (lh - rh == -2) {
            int rlh = AVLNode<Base>::getHeight(path[numOnPath]->right->left);
            int rrh = AVLNode<Base>::getHeight(path[numOnPath]->right->right);
            if (rlh - rrh == 1) {
                replacement = path[numOnPath]->doubleRotateRightLeft();
            } else {
                replacement = path[numOnPath]->singleRotateLeft();
            }
        }

        // Check for left heavy imbalance
        else if (lh > rh + 1) {
            int llh = AVLNode<Base>::getHeight(path[numOnPath]->left->left);
            int lrh = AVLNode<Base>::getHeight(path[numOnPath]->left->right);
            if (lrh > llh) {
                replacement = path[numOnPath]->doubleRotateLeftRight();
            } else {
                replacement = path[numOnPath]->singleRotateRight();
            }
        }
        // Update parent right ptr
        if (replacement && path[numOnPath] == this->root) {
            this->root = replacement;
        }
        else if (replacement && path[numOnPath-1]->right == path[numOnPath]) {
            path[numOnPath-1]->right = replacement;
        }
        // Update parent left ptr
        else if (replacement && path[numOnPath-1]->left == path[numOnPath]) {
            path[numOnPath-1]->left = replacement;
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

    for (unsigned int i = 0; i < path.size(); i++) {
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

