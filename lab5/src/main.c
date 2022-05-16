#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define IsNotSymbol 777

typedef struct TTree TTree;
typedef struct TElementAlTree TElementAlTree;
typedef struct TMainQueue TMainQueue;
typedef struct TQueueElement TQueueElement;

struct TTree {
    TTree* Left;
    TTree* Right;
    short Symbol;
    char Used;
    int Count;
};

struct TElementAlTree {
    char Code[1000];
    int LenCode;
};

struct TMainQueue {
    struct TQueueElement {
        TTree* Root;
        TQueueElement* Next;
    }*Head;
    TQueueElement* Tail;
};

int LetterCode (int symbol) {
    if (symbol < 0) {
        symbol += 256;
    }

    return symbol;
}

void PushQueue (TMainQueue* MainQueue, TTree* root) {
    TQueueElement* newElement = calloc (1, sizeof (TQueueElement));

    newElement->Root = root;
    newElement->Next = NULL;

    if (MainQueue->Head) {
        if (!MainQueue->Tail) {
            TQueueElement* temp;
            temp = MainQueue->Head;

            while (temp->Next) {
                temp = temp->Next;
            }
        }
        MainQueue->Tail->Next = newElement;
    } else {
        MainQueue->Head = newElement;
    }

    MainQueue->Tail = newElement;
}

void PopQueue (TMainQueue* MainQueue, TTree** root) {
    if (MainQueue->Head == NULL) {
        return;
    }

    TQueueElement* element = MainQueue->Head->Next;
    *root = MainQueue->Head->Root;
    free (MainQueue->Head);
    MainQueue->Head = element;
}

void ClearingQueue (TMainQueue MainQueue) {
    while (MainQueue.Head) {
        TTree* RootNow;
        PopQueue (&MainQueue, &RootNow);
    }
}

void FreeTree (TTree* FullTree) {
    if (FullTree->Left) {
        FreeTree (FullTree->Left);
    }

    if (FullTree->Right) {
        FreeTree (FullTree->Right);
    }
    free (FullTree);
}

void PrintCodeTree (TTree FullTree, int sizeTree) { //BFS
    TMainQueue MainQueue;
    fprintf (stdout, "%c", sizeTree);

    MainQueue.Head = MainQueue.Tail = NULL;
    PushQueue (&MainQueue, &FullTree);

    if (FullTree.Symbol == IsNotSymbol) {
        int lenOutput = 0;
        fprintf(stdout, "0");

        while (lenOutput < sizeTree) {
            TTree* RootNow;
            PopQueue (&MainQueue, &RootNow);

            if (RootNow != NULL) {
                if (RootNow->Left != NULL) {
                    if (RootNow->Left->Symbol < IsNotSymbol) {
                        fprintf (stdout, "1");
                        fprintf (stdout, "%c", RootNow->Left->Symbol);
                        ++lenOutput;
                    } else {
                        fprintf (stdout, "0");
                    }
                } else {
                    fprintf (stdout, "0");
                }
                PushQueue (&MainQueue, RootNow->Left);
            }
            
            if (RootNow != NULL) {
                if (RootNow->Right != NULL && lenOutput < sizeTree) {
                    if (RootNow->Right->Symbol < IsNotSymbol) {
                        fprintf (stdout, "1");
                        fprintf (stdout, "%c", RootNow->Right->Symbol);
                        ++lenOutput;
                    } else {
                        fprintf (stdout, "0");
                    }
                } else {
                    fprintf (stdout, "0");
                }
                PushQueue (&MainQueue, RootNow->Right);
            }
        }
    } else {
        fprintf (stdout, "1");
        fprintf (stdout, "%c", FullTree.Symbol);
    }

    ClearingQueue (MainQueue);
}

void SearchMin (TTree** FullTree, int sizeTree, int* min1I, int* min2I) {
    int min1C = 100000;
    int min2C = 100000;

    for (int i = 0; i < sizeTree; ++i) {
        if (!FullTree[i]->Used) {
            if (FullTree[i]->Count < min1C) {
                min2C = min1C;
                *min2I = *min1I;
                min1C = FullTree[i]->Count;
                *min1I = i;
            } else if (FullTree[i]->Count < min2C) {
                min2C = FullTree[i]->Count;
                *min2I = i;
            }
        }
    }
}

TTree* CombiningTrees (TTree* First, TTree* Second) {
    TTree* NewTree = calloc (1, sizeof (TTree));

    NewTree->Count = First->Count + Second->Count;
    NewTree->Left = Second;
    NewTree->Right = First;
    NewTree->Used = 0;
    NewTree->Symbol = IsNotSymbol;

    return NewTree;
}

TTree* AlgorithmHuffman (TTree** FullTree, int sizeTree) {
    int min1Index = 0;
    int min2Index = 0;

    for (int i = 0, N = sizeTree; i < N - 2; ++i) {
        SearchMin (FullTree, sizeTree, &min1Index, &min2Index);

        FullTree[sizeTree] = CombiningTrees (FullTree[min1Index], FullTree[min2Index]);
        FullTree[min1Index]->Used = FullTree[min2Index]->Used = 1;

        sizeTree += 1;
    }

    if (sizeTree > 1) {
        FullTree[sizeTree] = CombiningTrees (FullTree[sizeTree - 1], FullTree[sizeTree - 2]);
    } else {
        TTree* Temp = FullTree[0];

        FullTree[sizeTree] = calloc (1, sizeof (TTree));
        FullTree[sizeTree]->Count = Temp->Count;
        FullTree[sizeTree]->Left = NULL;
        FullTree[sizeTree]->Right = Temp;
        FullTree[sizeTree]->Symbol = IsNotSymbol;
    }

    return FullTree[sizeTree];
}

TTree** CreateAllTree (int sizeTree, int* alphabet) {
    TTree** AlphabetTree = calloc (2 * sizeTree, sizeof (TTree*));

    for (int i = 0, N = 0; i < 256; ++i) {
        if (alphabet[i]) {
            AlphabetTree[N] = calloc (1, sizeof (TTree));
            AlphabetTree[N]->Count = alphabet[i];
            AlphabetTree[N]->Symbol = i;
            AlphabetTree[N]->Left = AlphabetTree[N]->Right = NULL;
            AlphabetTree[N]->Used = 0;
            ++N;
        }
    }

    return AlphabetTree;
}

TTree* BuildingTree (unsigned char* codeTree) {
    TTree* FullTree = calloc (1, sizeof (TTree));
    FullTree->Count = FullTree->Used = 0;
    FullTree->Left = FullTree->Right = NULL;
    FullTree->Symbol = IsNotSymbol;

    TMainQueue MainQueue;
    MainQueue.Head = MainQueue.Tail = NULL;
    PushQueue (&MainQueue, FullTree);

    for (int i = 0; codeTree[i] != '\0'; ++i) {
        TTree* RootNow = NULL;
        PopQueue (&MainQueue, &RootNow);

        if (RootNow != NULL) {
            RootNow->Count = RootNow->Used = 0;
            RootNow->Left = RootNow->Right = NULL;
            RootNow->Symbol = IsNotSymbol;

            if (codeTree[i] == '0') {
                RootNow->Left = calloc (1, sizeof (TTree));
                RootNow->Right = calloc (1, sizeof (TTree));
            } else {
                ++i;
                RootNow->Symbol = LetterCode (codeTree[i]);
            }

            PushQueue (&MainQueue, RootNow->Left);
            PushQueue (&MainQueue, RootNow->Right);
        }
    }

    ClearingQueue (MainQueue);

    return FullTree;
}

void CalculatingTheCode (TTree TreeNow, TElementAlTree* Alphabet, TElementAlTree ElementNow) {
    ++ElementNow.LenCode;

    if (TreeNow.Left != NULL) {
        ElementNow.Code[ElementNow.LenCode - 1] = '0';
        CalculatingTheCode (*TreeNow.Left, Alphabet, ElementNow);
    }

    if (TreeNow.Right != NULL) {
        ElementNow.Code[ElementNow.LenCode - 1] = '1';
        CalculatingTheCode (*TreeNow.Right, Alphabet, ElementNow);
    }

    if (TreeNow.Symbol < IsNotSymbol) {

        ElementNow.Code[ElementNow.LenCode - 1] = '\0';
        Alphabet[TreeNow.Symbol] = ElementNow;

        return;
    }
}


void FindAndPrintCodeSymbols (TTree* FullTree, unsigned char* input, int lenText) {
    TTree* TreeNow = FullTree;
    unsigned char countBitInLast = input[lenText - 1];
    int twoInDegree;

    for (int i = 0; i < lenText - 2; ++i) {
        twoInDegree = 7;

        while (twoInDegree >= 0) {
            if (TreeNow->Symbol < IsNotSymbol) {
                printf ("%c", TreeNow->Symbol);
                TreeNow = FullTree;
            }

            if ((input[i] & 1 << (twoInDegree)) == 0) {
                TreeNow = TreeNow->Left;
            } else {
                TreeNow = TreeNow->Right;
            }

            --twoInDegree;
        }
    }

    twoInDegree = 7;

    while (twoInDegree >= 8 - countBitInLast) {
        if (TreeNow->Symbol < IsNotSymbol) {
            printf ("%c", TreeNow->Symbol);
            TreeNow = FullTree;
        }

        if ((input[lenText - 2] & 1 << (twoInDegree)) == 0) {
            TreeNow = TreeNow->Left;
        } else {
            TreeNow = TreeNow->Right;
        }

        --twoInDegree;
    }

    if (TreeNow->Symbol < IsNotSymbol) {
        printf ("%c", TreeNow->Symbol);
    }
}

int main (void) {
    unsigned char str[100000] = {'\0'};
    unsigned char task = 0;

    //freopen ("in.txt", "rb", stdin); freopen ("out.txt", "wb", stdout);
    //char a = 0, b = 1; fprintf (stdout, "c"); fprintf (stdout, "%c", a); fprintf (stdout, "%c", b); fprintf (stdout, "%c", a); fprintf (stdout, "%c", b); return 0;

    if (fscanf (stdin, "%c", &task) != 1) {
        return 1;
    }

    if (task == 'c') {
        int alphabet[256] = {0};
        int lenText = 0;

        while (fscanf (stdin,  "%c", &str[lenText]) == 1) {
            ++lenText;
        }

        if (lenText == 0) {
            return 0;
        }

        for (int i = 0; i < lenText; ++i) {
            ++alphabet[LetterCode (str[i])];
        }

        int sizeTree = 0;
        TElementAlTree* AlphabetCodes = calloc (256, sizeof (TElementAlTree));
        TElementAlTree ElementNow;
        ElementNow.LenCode = 0;

        for (int i = 0; i < 256; ++i) {
            if (alphabet[i]) {
                ++sizeTree;
            }
        }

        TTree** AlphabetTree = CreateAllTree (sizeTree, alphabet);

        if (sizeTree < 2) {
            printf ("%c 1%c%d", 1, AlphabetTree[0]->Symbol, lenText);
        } else {
            TTree* FullTree = AlgorithmHuffman (AlphabetTree, sizeTree);

            PrintCodeTree (*FullTree, sizeTree);

            CalculatingTheCode (*FullTree, AlphabetCodes, ElementNow);

            unsigned char code = 0;
            char j = 0;
            for (int i = 0; i < lenText; ++i) {
                char* strCodeNow = AlphabetCodes[(int)str[i]].Code;
                int z = 0;

                while (strCodeNow[z] != '\0') {
                    if (strCodeNow[z] == '1') {
                        code |= 128 >> j;
                    }

                    ++j;
                    ++z;

                    if (j > 7) {

                        printf ("%c", code);

                        j = 0;
                        code = 0;
                    }
                }
            }

            printf ("%c%c", code, j);

            FreeTree (FullTree);
        }

        free (AlphabetTree);
        free (AlphabetCodes);
    } else {
        int lenText = 0;
        int sizeTreeNow = 0;
        unsigned char sizeTree = 0;

        if (fscanf (stdin, "%c ", &sizeTree) != 1) {
            return 0;
        }

        while (lenText < sizeTree) {
            if (fscanf (stdin, "%c", &str[sizeTreeNow]) != 1) {
                return 0;
            }
            ++sizeTreeNow;
            if (str[sizeTreeNow - 1] == '1') {
                if (fscanf (stdin, "%c", &str[sizeTreeNow]) != 1) {
                    return 0;
                }
                ++sizeTreeNow;
                ++lenText;
            }
        }

        if (str[0] == '1') {
            int countPrint = 0;
            if (fscanf (stdin, "%d", &countPrint) != 1) {
                return 0;
            }

            for (int i = 0; i < countPrint; ++i) {
                printf ("%c", str[1]);
            }
        } else {
            TTree* FullTree;

            FullTree = BuildingTree (str);

            lenText = 0;

            while (fscanf (stdin, "%c", &str[lenText]) == 1) {
                ++lenText;
            }

            if (lenText == 0) {
                return 0;
            }

            str[lenText] = '\0';

            FindAndPrintCodeSymbols (FullTree, str, lenText);

            FreeTree (FullTree);
        }
    }

    return 0;
}
