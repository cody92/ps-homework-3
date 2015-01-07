#include "Huffman.h"

Huffman::Huffman() {
    this->initGraph();
}

Huffman::~Huffman() {
}

void Huffman::initGraph() {

    GraphTree *parent, *left, *right;
    parent = new GraphTree;
    left = new GraphTree;
    right = new GraphTree;
    parent->symbol = 0;
    parent->symbolExtended = 0;
    parent->nrOfApp = 2;


    parent->parent = NULL;
    parent->left = left;
    parent->right = right;
    parent->index = 1;

    indexedGraph.push_back(parent);

    left->parent = parent;
    right->parent = parent;

    left->left = NULL;
    left->right = NULL;

    right->left = NULL;
    right->right = NULL;

    left->symbol = 255;
    left->symbolExtended = 1;

    right->symbol = 255;
    right->symbolExtended = 2;

    left->nrOfApp = 1;
    right->nrOfApp = 1;

    left->index = 2;
    right->index = 3;

    indexedGraph.push_back(left);
    indexedGraph.push_back(right);

    symbolMap[right->symbol + right->symbolExtended] = right;

    symbolMap[left->symbol + left->symbolExtended] = left;


    this->rootNode = parent;

}

GraphSearch Huffman::findSymbol(unsigned char symbol) {
    GraphSearch result;
    if (symbolMap.count(symbol) > 0) {
        result.type = true;
        result.reference = symbolMap[symbol];
    } else {
        result.type = false;
        result.reference = symbolMap[this->ESC_VALUE];
    }
    return result;
}

void Huffman::updateSymbol(GraphSearch result) {
    (result.reference)->nrOfApp = (result.reference)->nrOfApp + 1;
}

void Huffman::addSymbol(GraphSearch result, unsigned char symbol) {
    GraphTree *left, *right;


    left = new GraphTree;
    left->nrOfApp = 1;
    left->right = NULL;
    left->left = NULL;
    left->symbol = symbol;
    left->symbolExtended = 0;
    left->parent = result.reference;
    left->index = (result.reference)->index + 1;

    right = new GraphTree;
    right->nrOfApp = 1;
    right->right = NULL;
    right->left = NULL;
    right->symbol = 255;
    right->symbolExtended = 2;
    right->parent = result.reference;
    right->index = (result.reference)->index + 2;


    (result.reference)->left = left;
    (result.reference)->right = right;
    (result.reference)->nrOfApp = 2;
    (result.reference)->symbol = 0;
    (result.reference)->symbolExtended = 0;

    indexedGraph.push_back(left);
    indexedGraph.push_back(right);


    symbolMap[this->ESC_VALUE] = right;

}

void Huffman::displayIndexedGraph() {
    GraphIndex::size_type iterator;
    for (iterator = 0; iterator < indexedGraph.size(); ++iterator) {
        std::cout << (unsigned char) ((indexedGraph[iterator])->symbol + (indexedGraph[iterator])->symbolExtended) << " " << (indexedGraph[iterator])->index << " -> " << iterator + 1 << " " << (indexedGraph[iterator])->nrOfApp << "\n";
    }
    std::cout << "\n\n";
}

void Huffman::displayGraph(GraphTree *root) {
    if ((root->symbol + root->symbolExtended) == 0) {
        std::cout << "Pondere: " << root->nrOfApp << "\n";
        displayGraph(root->left);
        displayGraph(root->right);
    } else {
        std::cout << "Simbol: " << (root->symbol + root->symbolExtended) << "\n";
    }


}

void Huffman::balanceGraph(GraphTree *node, bool init) {
    GraphTree *aux, *changeAux, *parent;
    GraphIndex::size_type iterator, auxIndex;

//    this->displayIndexedGraph();
    for (iterator = node->index - 1; iterator > 0; iterator--) {
        if ((indexedGraph[iterator - 1])->nrOfApp > node->nrOfApp) {
            break;
        } else if ((indexedGraph[iterator - 1])->nrOfApp == node->nrOfApp) {
            if(init == true) {
                break;
            }
            if ((indexedGraph[iterator - 1])->getSymbol() <= node->getSymbol()) {
                break;
            }
        }
    }
    if (iterator == node->index - 1) {
        node->parent->nrOfApp++;
        if (node->parent->parent != NULL) {
            balanceGraph(node->parent, false);
        }
        return void();
    }

    if (iterator == node->index - 2) {
        aux = indexedGraph[iterator];
        auxIndex = node->index;
        node->index = aux->index;
        aux->index = auxIndex;
        indexedGraph[iterator] = node;

        parent = aux->parent;
        changeAux = node->parent;
        if (iterator % 2 == 1) {
            parent->left = node;
        } else {
            parent->right = node;
        }
        node->parent = parent;
        aux->parent = changeAux;

        indexedGraph[auxIndex - 1] = aux;
        if (auxIndex % 2 == 0) {
            changeAux->left = aux;
        } else {
            changeAux->right = aux;
        }

        balanceGraph(node, false);
        return void();
    }

    if ((iterator - (node->index - 2)) > 0 && node->index - 2 >= 0) {
        aux = indexedGraph[iterator];
        auxIndex = node->index;
        node->index = aux->index;
        aux->index = auxIndex;
        indexedGraph[iterator] = node;

        parent = aux->parent;
        changeAux = node->parent;
        if (iterator % 2 == 1) {
            parent->left = node;
        } else {
            parent->right = node;
        }
        node->parent = parent;
        aux->parent = changeAux;

        indexedGraph[auxIndex - 1] = aux;
        if (auxIndex % 2 == 0) {
            changeAux->left = aux;
        } else {
            changeAux->right = aux;
        }

        //updateChildrens(aux->index - 1, node->index - 1);

        balanceGraph(indexedGraph[iterator], false);
    }
}

void Huffman::updateMap() {
    int symbol;
    GraphIndex::size_type iterator;
    for (iterator = 0; iterator < indexedGraph.size(); iterator++) {
        symbol = indexedGraph[iterator]->symbolExtended + indexedGraph[iterator]->symbol;
        if (symbol > 0) {
            symbolMap[symbol] = indexedGraph[iterator];
        }
    }
}

void Huffman::encodeSymbol(unsigned char symbol) {
    GraphSearch result;
    //cout << "Sy " << symbol << " ---------------------------------------------------------------------------------\n";
    result = findSymbol(symbol);
    if (result.type == true) {
        updateSymbol(result);
    } else {
        addSymbol(result, symbol);
    }

    balanceGraph(result.reference, true);
    updateMap();
    //this->displayIndexedGraph();
}

void Huffman::updateChildrens(int n, int m) {
    int iterator;
    GraphTree *backLeft, *backRight, *auxLeft, *auxRight;

    auxLeft = this->indexedGraph[n]->left;
    auxRight = this->indexedGraph[n]->right;

    iterator = m + 1;

    while (iterator <= n) {
        backLeft = auxLeft;
        backRight = auxRight;
        if (!this->isLeaf(this->indexedGraph[iterator])) {
            auxLeft = this->indexedGraph[iterator]->left;
            auxRight = this->indexedGraph[iterator]->right;
            backLeft->parent = this->indexedGraph[iterator];
            backRight->parent = this->indexedGraph[iterator];
            this->indexedGraph[iterator]->left = backLeft;
            this->indexedGraph[iterator]->right = backRight;
        }
        iterator++;
    }



}


bool Huffman::isLeaf(GraphTree *node) {
    int sum;
    bool isLeaf;
    sum = node->symbol + node->symbolExtended;
    if (sum > 0) {
        isLeaf = true;
    } else {
        isLeaf = false;
    }

    return isLeaf;
}
