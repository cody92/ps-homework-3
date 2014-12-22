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
        std::cout << ((indexedGraph[iterator])->symbol + (indexedGraph[iterator])->symbolExtended) << " " << (indexedGraph[iterator])->index << " -> " << iterator + 1 << " " << (indexedGraph[iterator])->nrOfApp << "\n";
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

void Huffman::balanceGraph(GraphTree *node) {
    GraphTree *aux, *changeAux, *parent;
    GraphIndex::size_type iterator, auxIndex;

    for (iterator = node->index - 1; iterator > 0; iterator--) {
        if ((indexedGraph[iterator - 1])->nrOfApp >= node->nrOfApp) {
            break;
        }
    }
    if (iterator == node->index - 1) {
        node->parent->nrOfApp++;
        if (node->parent->parent != NULL) {
            balanceGraph(node->parent);
        }
        return void();
    }

    if (iterator == node->index - 2) {
        (indexedGraph[iterator - 1])->right = indexedGraph[iterator];
        (indexedGraph[iterator - 1])->left = node;
        indexedGraph[iterator]->index++;
        node->index--;
        indexedGraph[iterator] = node;
        indexedGraph[iterator + 1] = (indexedGraph[iterator - 1])->right;
        balanceGraph(node);
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
        balanceGraph(indexedGraph[iterator]);
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

std::string Huffman::encodeSymbol(unsigned char symbol) {
    GraphSearch result;

    result = findSymbol(symbol);
    if (result.type == true) {
        updateSymbol(result);
    } else {
        addSymbol(result, symbol);
    }

    balanceGraph(result.reference);
    updateMap();
}