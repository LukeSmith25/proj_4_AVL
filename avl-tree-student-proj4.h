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
    AVLNode<Base>* child = this->right;

    // If root = currNode
    if (this->root == this) {
        this->root = child;
    }

    // Move child to where parent was and set curNode as left child
    child->left = this;

    this->updateHeight();
    child->updateHeight();

    return child;
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::singleRotateRight() {
    AVLNode<Base>* child = this->left;

    // If root = currNode
    if (this->root == this) {
        this->root = child;
    }

    // Move child to where parent was and set curNode as left child
    child->right = this;

    this->updateHeight();
    child->updateHeight();

    return child;
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::doubleRotateLeftRight() {
    this->left->singleRotateLeft();
    this->singleRotateRight();
    return this;
}

template<class Base>
AVLNode<Base> *AVLNode<Base>::doubleRotateRightLeft() {
    this->right->singleRotateRight();
    this->singleRotateLeft();
    return this;
}

template<class Base>
void AVLTree<Base>::insert(const Base &item) {
    // Declare new node to be inserted
    AVLNode<Base> *node = new AVLNode<Base>(item);

    // Create a previous and temporary node for iteration
    AVLNode<Base> *par = NULL;
    AVLNode<Base> *cur = this->root;

    // While temp is not null iterate through tree
    while (cur) {
        // If item is the same, return
        if (!(item < cur->getData()) && !(cur->getData() < item)) {
            return;
        }
        par = cur;
        // If item < temp, temp = left node
        if (item < cur->data) {
            cur = cur->left;
        }
            // If item > temp, temp = right node
        else if (cur->data < item) {
            cur = cur->right;
        }
    }

    // If the root is null then assign root to item
    if (!root) {
        this->root = node;
    }
        // Insert new node at location left
    else if (item < par->getData()) {
        par->left = node;
    }
        // Insert new node at location right
    else {
        par->right = node;
    }
}

template<class Base>
void AVLTree<Base>::remove(const Base &item) {
    AVLNode<Base> *parent = this->root;
    AVLNode<Base> *toRemove = this->root;
    bool complete = false;

    if (!toRemove) {
        complete = true;
    }

    while (!complete) {
        // Key is found
        if (!(toRemove->data < item) && !(item < toRemove->data)) {
            // Remove leaf node
            if (toRemove->left == NULL && toRemove->right == NULL) {
                if (parent->left == toRemove) {
                    parent->left = NULL;
                }
                else {
                    parent->right = NULL;
                }

                toRemove = NULL;
                delete toRemove;
            }

                // Remove node with one child
            else if (toRemove->left == NULL || toRemove->right == NULL) {
                AVLNode<Base> *grandChild;
                if (toRemove->left != NULL) {
                    grandChild = toRemove->left;
                    toRemove->left = NULL;
                }
                else {
                    grandChild = toRemove->right;
                    toRemove->right = NULL;
                }
                if (parent->left == toRemove) {
                    parent->left = grandChild;
                }
                else {
                    parent->right = grandChild;
                }

                toRemove = NULL;
                delete toRemove;
            }

                // Remove node with two children
            else {
                AVLNode<Base> *leftMost = toRemove->right;
                AVLNode<Base> *leftMostParent = toRemove;

                if (leftMost->left != NULL) {
                    while (leftMost->left != NULL) {
                        leftMostParent = leftMost;
                        leftMost = leftMost->left;
                    }
                    leftMostParent->left = leftMost->right;
                    leftMost->right = toRemove->right;
                }
                leftMost->left = toRemove->left;

                if (parent->left == toRemove) {
                    parent->left = leftMost;
                }
                else {
                    parent->right = leftMost;
                }

                toRemove->left = toRemove->right = NULL;

                toRemove = NULL;
                delete toRemove;
            }
        }

            // Search Right
        else if (toRemove->data < item) {
            parent = toRemove;
            toRemove = toRemove->right;
        }

            // Search Left
        else {
            parent = toRemove;
            toRemove = toRemove->left;
        }

        if (toRemove == NULL) {
            complete = true;
        }
    }

}


#endif

