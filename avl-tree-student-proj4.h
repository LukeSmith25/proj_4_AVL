/**
 * file: avl-tree-student-proj4.h
 * author: Luke Smith
 * course: CSI 3334
 * assignment: project 4
 * due date: October 24, 2022
 *
 * Implementation for the AVL tree.
 */
#ifndef AVL_TREE_STUDENT_PROJ4
#define AVL_TREE_STUDENT_PROJ4 

#include "avl-tree-prof-proj4.h"
#include <queue>

/**
 * AVLNode Destructor
 *
 * This function is the default destructor for AVLNode.
 *
 * Parameters: None.
 *
 * Return value: None.
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

/**
 * printPreorder
 *
 * This function traverses the tree and prints it in preorder.
 *
 * Parameters:
 *      -os: output stream containing preorder traversal
 *      -indent: indentation representing depth of node in tree
 *
 * Return value: None.
 */
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

/**
 * minNode
 *
 * This function returns the leftmost node.
 *
 * Parameters: None.
 *
 * Return value: Leftmost AVLNode.
 */
template<class Base>
const AVLNode<Base> *AVLNode<Base>::minNode() const {
    if (getLeft() != NULL) {
        getLeft()->minNode();
    } else {
        return this;
    }
}

/**
 * maxNode
 *
 * This function returns the rightmost node.
 *
 * Parameters: None.
 *
 * Return value: Rightmost AVLNode.
 */
template<class Base>
const AVLNode<Base> *AVLNode<Base>::maxNode() const {
    if (getRight() != NULL) {
        getRight()->maxNode();
    } else {
        return this;
    }
}

/**
 * singleRotateLeft
 *
 * This function rotates left at node called on.
 *
 * Parameters: None.
 *
 * Return value: Parent pointer.
 */
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

/**
 * singleRotateRight
 *
 * This function rotates right at node called on.
 *
 * Parameters: None.
 *
 * Return value: Parent pointer.
 */
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

/**
 * doubleRotateLeftRight
 *
 * This function rotates left at node called on, then right.
 *
 * Parameters: None.
 *
 * Return value: Parent pointer.
 */
template<class Base>
AVLNode<Base> *AVLNode<Base>::doubleRotateLeftRight() {
    this->left = this->left->singleRotateLeft();
    return this->singleRotateRight();
}

/**
 * doubleRotateRightLeft
 *
 * This function rotates right at node called on, then left.
 *
 * Parameters: None.
 *
 * Return value: Parent pointer.
 */
template<class Base>
AVLNode<Base> *AVLNode<Base>::doubleRotateRightLeft() {
    this->right = this->right->singleRotateRight();
    return this->singleRotateLeft();
}

/**
 * insert
 *
 * Inserts node into AVL tree.
 *
 * Parameters:
 *      item: item to be inserted into tree
 *
 * Return value: None.
 */
template<class Base>
void AVLTree<Base>::insert(const Base &item) {
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

    // Declare new node to be inserted
    AVLNode<Base> *newNode = new AVLNode<Base>(item);

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

/**
 * remove
 *
 * This function removes a node from the AVL tree.
 *
 * Parameters:
 *      item: item to be removed
 *
 * Return value: None.
 */
template<class Base>
void AVLTree<Base>::remove(const Base &item) {
    // Declare ptr to node to remove
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
            parent->left = toRemove->right;
        } else {
            parent->right = toRemove->right;
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

/**
 * printLevelOrder
 *
 * This function traverses the tree and prints it in level order.
 *
 * Parameters:
 *      -os: output stream containing preorder traversal
 *
 * Return value: None.
 */
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
/**
 * rebalancePathToRoot
 *
 * This function traverses the path from bottom to top and rebalances nodes.
 *
 * Parameters:
 *      -path: array of pointers for nodes
 *      -numOnPath: number of nodes to traverse
 *
 * Return value: None.
 */
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

