#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int getN() { // getting keyAmount from the user
    int input = 0;
    std::cout << "Please define n.\nn = " << std::endl;
    std::cin >> input;
    if(input < 400) { // setting keyAmount to 400 if keyAmount is less than 400
        return 400;
    }
    return input;
}

struct node { // the basic node structure needs an array of keys and an array of child nodes
    std::vector<int> keys;
    std::vector<node*> childArray;
    int keyAmount;
    bool isLeaf;
};

node* createNode(bool isLeaf) {
    node* n = NULL;
    n = new node;
    n->keys = std::vector<int>(5); // keys list
    n->childArray = std::vector<node*>(6); // child nodes list
    n->keyAmount = 0;
    n->isLeaf = isLeaf;
    return n;
}

void split(int cKey, node *splittingNode, node* parent) {
    int mid = splittingNode->keys[2]; // Pull the middle key out of the node being split, so it can be put in the parent node
    node *newNode = createNode(splittingNode->isLeaf); // create a new node

    newNode->keys[0] = splittingNode->keys[3]; // the new node will take the larger half of the numbers from the splitting node
    newNode->keys[1] = splittingNode->keys[4];
    newNode->keyAmount = 2;

    splittingNode->keys[2] = 0; // remove the keys that the new node took
    splittingNode->keys[3] = 0;
    splittingNode->keys[4] = 0;
    splittingNode->keyAmount -= 3;

    if(splittingNode->childArray[0] != NULL){ // move the child nodes over if necessary
        newNode->childArray[0] = splittingNode->childArray[3];
        newNode->childArray[1] = splittingNode->childArray[4];
        newNode->childArray[2] = splittingNode->childArray[5];

        splittingNode->childArray[3] = NULL;
        splittingNode->childArray[4] = NULL;
        splittingNode->childArray[5] = NULL;
    }

    cKey = parent->keyAmount - 1; // current key will let the loop start at the end of n->keys

    while (cKey >= 0 && parent->keys[cKey] > mid) { // then we have to move all the values in the keys array over
        parent->keys[cKey+1] = parent->keys[cKey]; // to make room for the key that's supposed to be inserted
        parent->childArray[cKey+2] = parent->childArray[cKey+1];
        cKey -= 1;
    }

    cKey += 1; // increasing cKey to make sure val gets put in the right
    parent->keys[cKey] = mid; // inserting the val at cKey
    parent->childArray[cKey+1] = newNode;
    parent->keyAmount += 1; // Updating the number of keys in the node

}
void print2n(node *node, int n) { // same as treeTraversal function, but only prints if the key is between 2 and 2n
    int child = 0;
    for(int i = 0; i < node->keyAmount; i++) {
        if(node->childArray[0] != NULL) {
            print2n(node->childArray[child], n);
        }
        if(node->keys[i] >= n && node->keys[i] <= 2*n){
            std::cout << " " << node->keys[i];
        }
        child++;
    }
    if(node->childArray[0] != NULL) {
        print2n(node->childArray[child], n);
    }
}

void treeTraversal(node *n) {
    int child = 0;
    for(int i = 0; i < n->keyAmount; i++) { // checks for children, if true, go into the child node
        if(n->childArray[0] != NULL) {
            treeTraversal(n->childArray[child]);
        }
        std::cout << " " << n->keys[i]; // print the key value
        child++;
    }
    if(n->childArray[0] != NULL) { // if the node has children, check the last one since it can't be reached in the loop
        treeTraversal(n->childArray[child]);
    }
}

void insertToNode(int val, node *n) {
    int cKey = n->keyAmount - 1; // current key will let the loop start at the end of n->keys
    if(n->isLeaf == false){ // if this node is a leaf
        while(cKey >= 0 && n->keys[cKey] > val) {
            cKey -= 1;
        }
        if(n->childArray[0] != NULL) {
            if (n->childArray[cKey + 1]->keyAmount == 5) { // check if the child is full
                split(cKey + 1, n->childArray[cKey + 1], n); // if so, split it
                if (n->keys[cKey + 1] < val) { // once split, check which one gets the new val
                    cKey += 1;
                }
            }
        }
        insertToNode(val, n->childArray[cKey + 1]); // call insert on the node found

    } else { // if it isn't a leaf node, we need to find which leaf to put the val in
        while (cKey >= 0 && n->keys[cKey] > val) { // then we have to move all the values in the keys array over
            n->keys[cKey+1] = n->keys[cKey]; // to make room for the key that's supposed to be inserted
            cKey -= 1;
        }
        cKey += 1; // increasing cKey to make sure val gets put in the right place
        n->keys[cKey] = val; // inserting the val at cKey
        n-> keyAmount += 1; // Updating the number of keys in the node
    }
}

std::vector<int> generateNums(int n) { // generate the random numbers to be inserted
    std::vector<int> numVec;
    bool done = false;
    srand((unsigned)time(0)); // set the seed

    while(!done) {
        int random = rand() % ((n-1)*3); // in the range (0, keyAmount*3)
        numVec.push_back(random); // push the value into the vector

        for(int i = 0; i < numVec.size(); i++) { // loop through the vector and make sure
            if(random == numVec[i] && i != numVec.size()-1){ // that there isn't another of the same value
                numVec.pop_back(); // if there is, pop the last one added
            }
        }

        int counter = 0;
        if(numVec.size() == n-1){ // when the vector is filled with n numbers
            for(int j = 0; j < numVec.size(); j++) { // loop through and make sure that
                for(int k = 0; k < numVec.size(); k++) { // there aren't any repeating values
                    if(j != k && numVec[j] == numVec[k]){ // it's basically one final security check
                        counter+=1;
                        std::cout << counter;
                    }
                }
            }
            if(counter == 0){ // if the final check doesn't see any duplicates, it's good to go.
                done = true;
            }
        }

    }

    return numVec; // return the vector
}

node* insert(int val, node* root) {
    if(root->keyAmount == 5) { // check if the root needs to be split or not
        node *newRoot = createNode(false); // if it does, create a new root
        newRoot->childArray[0] = root;
        newRoot->isLeaf = false;
        split(0, root, newRoot); // pass the new root through the split function as the parent node.

        root = newRoot;
        root->isLeaf = false; // make sure the new root is not a leaf node

        insertToNode(val, root); // insert the value into the tree
    } else {
        insertToNode(val, root);
    }
    return root;
}

int main() {
    int n = getN(); // getting keyAmount from user

    node *root = createNode(true);
    root->isLeaf = true;


    std::vector<int> nums = generateNums(n+1); // creating the random numbers
    for(int i = 0; i < nums.size(); i++) { // showing the user what the random nums are
        std::cout << nums[i] << " ";
        root = insert(nums[i], root);
    }
    std::cout << "\n^^ These numbers were inserted in this order ^^\n" << std::endl;

    treeTraversal(root);
    std::cout << "\n^^ By going through the tree, these are the elements in numeric order ^^\n" << std::endl;

    print2n(root, n);
    std::cout << "\n^^ These are the numbers in the tree from " << n << " (n) to " << n*2 << " (2*n) ^^" << std::endl;

    return 0;
}