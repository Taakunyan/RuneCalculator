#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSV_FILE "rune.csv"
#define BUFFER 1024

void errorArgument(char *commandName){
    fprintf(stderr, "Error: Argument error.\nUsage : %s [Current Power] [Target Power] (Max Num of Runes)\n", commandName);
    exit(-1);
}
void errorCannotAllocateMemory(size_t size){
    fprintf(stderr, "Error: Can't allocate memory %zu bytes.\n", size);
    exit(-2);
}
void errorCannotOpenFile(char *fileName){
    fprintf(stderr, "Error: Can't open file '%s'.\n", fileName);
    exit(-3);
}

struct Rune{char *name; char *type; unsigned int level; unsigned int power;};
struct Node{struct Rune *rune; struct Node *left; struct Node *center; struct Node *right;};
struct Node *mallocNode(struct Rune *rune){
    struct Node *node;
    if((node = (struct Node*)malloc(sizeof(struct Node))) == NULL)errorCannotAllocateMemory(sizeof(struct Node));
    node->rune = rune;
    node->left = NULL;
    node->center = NULL;
    node->right = NULL;
    return node;
}
struct Node *addNode(struct Node *root, struct Rune *rune){
    if(root == NULL)return mallocNode(rune);
    else if(rune->power < root->rune->power)root->left = addNode(root->left, rune);
    else if(rune->power > root->rune->power)root->right = addNode(root->right, rune);
    else if(rune->power == root->rune->power)root->center = addNode(root->center, rune);
    return root;
}
struct Node *searchNode(struct Node *root, unsigned int search){
    while(root != NULL){
        if(root->rune->power == search)return root;
        else if(root->rune->power > search)root = root->left;
        else if(root->rune->power < search)root = root->right;
    }
    return (struct Node*)NULL;
}
unsigned int countNode(struct Node *root, unsigned int duplicate){
    if(root == NULL)return 0;
    if(duplicate)return countNode(root->left, duplicate) + countNode(root->center, duplicate) + countNode(root->right, duplicate) + 1;
    return countNode(root->left, duplicate) + countNode(root->right, duplicate) + 1;
}
unsigned int tree2intArray(struct Node *root, unsigned int array[], unsigned int index, unsigned int duplicate){
    if(root == NULL)return index;
    index = tree2intArray(root->right, array, index, duplicate);
    array[index++] = root->rune->power;
    if(duplicate)index = tree2intArray(root->center, array, index, duplicate);
    index = tree2intArray(root->left, array, index, duplicate);
    return index;
}
void freeTree(struct Node *root){
    if(root == NULL)return;
    freeTree(root->right);
    freeTree(root->center);
    freeTree(root->left);
    free(root->rune->name);
    free(root->rune->type);
    free(root->rune);
    free(root);
}
struct Rune *mallocRune(char *name_ptr, char *type_ptr, unsigned int level, unsigned int power){
    char *name, *type;
    if((name = (char*)malloc(sizeof(char) * (strlen(name_ptr) + 1))) == NULL)errorCannotAllocateMemory(sizeof(char) * (strlen(name_ptr) + 1));
    if((type = (char*)malloc(sizeof(char) * (strlen(type_ptr) + 1))) == NULL)errorCannotAllocateMemory(sizeof(char) * (strlen(type_ptr) + 1));
    strcpy(name, name_ptr);
    strcpy(type, type_ptr);
    struct Rune *rune;
    if((rune = (struct Rune*)malloc(sizeof(struct Rune))) == NULL)errorCannotAllocateMemory(sizeof(struct Rune));
    rune->name = name;
    rune->type = type;
    rune->level = level;
    rune->power = power;
    return rune;
}

struct Data{unsigned int *pair; unsigned int ptr; unsigned int sum; unsigned int current;};
struct Queue{struct Data *data; struct Queue *next;};
struct Queue *mallocQueue(struct Data *data){
    struct Queue *queue;
    if((queue = (struct Queue*)malloc(sizeof(struct Queue))) == NULL)errorCannotAllocateMemory(sizeof(struct Queue));
    queue->data = data;
    queue->next = NULL;
    return queue;
}
void enqueue(struct Queue **head, struct Queue **tail, struct Data *data){
    if(*head == NULL)*head = *tail = mallocQueue(data);
    else *tail = (*tail)->next = mallocQueue(data);
}
struct Data *dequeue(struct Queue **head){
    if(*head == NULL)return (struct Data*)NULL;
    struct Queue *tmp = *head;
    struct Data *data = (*head)->data;
    *head = (*head)->next;
    free(tmp);
    return data;
}
struct Data *mallocData(unsigned int *pair, unsigned int ptr, unsigned int sum, unsigned int current){
    struct Data *data;
    if((data = (struct Data*)malloc(sizeof(struct Data))) == NULL)errorCannotAllocateMemory(sizeof(struct Data));
    data->pair = pair;
    data->ptr = ptr;
    data->sum = sum;
    data->current = current;
    return data;
}

struct Node *readCSV(char *fileName){
    char buf[BUFFER], *ptr, *name, *type, *token = " ,\n";
    unsigned int power;
    struct Node *root = NULL;
    FILE *fp;
    if((fp = fopen(fileName, "r")) == NULL)errorCannotOpenFile(fileName);
    while(fgets(buf, BUFFER, fp) != NULL){
        if(buf[0] == '#')continue;
        name = strtok(buf, token);
        type = ptr = strtok(NULL, token);
        for(unsigned int lv = 1; ptr != NULL; lv++){
            if((ptr = strtok(NULL, token)) == NULL)break;
            if(!(power = atoi(ptr)))continue;
            root = addNode(root, mallocRune(name, type, lv, power));
        }
    }
    fclose(fp);
    return root;
}

struct Queue *searchPair(unsigned int array[], unsigned int array_size, unsigned int target, unsigned int max){
    struct Queue *head = NULL, *tail = NULL, *ans_head = NULL, *ans_tail = NULL;
    struct Data *data;
    unsigned int *pair = NULL, sum;
    enqueue(&head, &tail, mallocData(pair, 0, 0, 0));
    while((data = dequeue(&head)) != NULL){
        for(unsigned int index = data->current; index < array_size; index++){
            if((sum = data->sum + array[index]) > target)continue;
            if((pair = (unsigned int*)malloc(sizeof(unsigned int) * (data->ptr + 1))) == NULL)errorCannotAllocateMemory(sizeof(unsigned int) * (data->ptr + 1));
            for(unsigned int i = 0; i < data->ptr; i++)pair[i] = data->pair[i];
            pair[data->ptr] = array[index];
            if(sum == target)enqueue(&ans_head, &ans_tail, mallocData(pair, data->ptr + 1, sum, index));
            else if(data->ptr + 1 == max)free(pair);
            else enqueue(&head, &tail, mallocData(pair, data->ptr + 1, sum, index));
        }
        free(data->pair);
        free(data);
    }
    return ans_head;
}

void consoleWrite(struct Queue *ans, struct Node *root){
    unsigned int flag = 0;
    struct Node **tmp;
    struct Data *data;
    while((data = dequeue(&ans)) != NULL){
        for(unsigned int i = 0; i < 23 * data->ptr; i++)printf("-");
        printf("\n");
        if((tmp = (struct Node**)malloc(sizeof(struct Node*) * data->ptr)) == NULL)errorCannotAllocateMemory(sizeof(struct Node) * data->ptr);
        for(unsigned int i = 0; i < data->ptr; i++){
            printf("\x1b[33m%-23d\x1b[39m",data->pair[i]);
            tmp[i] = searchNode(root, data->pair[i]);
        }
        printf("\n");
        do{
            flag = 0;
            for(unsigned int i = 0; i < data->ptr; i++){
                if(tmp[i] == NULL){
                    printf("                       ");
                    continue;
                }
                printf("\x1b[39m%2d\x1b[%dm %-20s\x1b[39m", tmp[i]->rune->level, !strcmp(tmp[i]->rune->type, "UŒ‚") ? 31 : !strcmp(tmp[i]->rune->type, "–hŒä")? 32 : 39, tmp[i]->rune->name);
                tmp[i] = tmp[i]->center;
                flag = 1;
            }
            printf("\n");
        }while(flag);
        flag = data->ptr;
        free(data->pair);
        free(data);
        free(tmp);
    }
    if(flag)for(unsigned int i = 0; i < 23 * flag; i++)printf("-");
    printf("\n");
}

int main(int argc, char *argv[]){
    unsigned int currentPower, targetPower, diffPower, runeNum;
    if(argc == 1){
        printf("Current Power : ");
        scanf("%d", &currentPower);
        printf("Target Power  : ");
        scanf("%d", &targetPower);
        printf("Number of runes : ");
        scanf("%d", &runeNum);
        diffPower = targetPower - currentPower;
    }
    else{
        if(argc < 3)errorArgument(argv[0]);
        diffPower = atoi(argv[2]) - atoi(argv[1]), runeNum = 6;
        if(argc > 3)runeNum = atoi(argv[3]);
    }
    struct Node *root;
    struct Queue *ans;
    unsigned int *array, array_size;
    root = readCSV(CSV_FILE);
    if((array = (unsigned int*)malloc(sizeof(unsigned int) * (array_size = countNode(root, 0)))) == NULL)errorCannotAllocateMemory(sizeof(unsigned int) * array_size);
    tree2intArray(root, array, 0, 0);
    ans = searchPair(array, array_size, diffPower, runeNum);
    consoleWrite(ans, root);
    free(array);
    freeTree(root);
    system("PAUSE");
    return 0;
}
