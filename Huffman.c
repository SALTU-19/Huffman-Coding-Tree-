#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct tree{
	int freq;
	char letter;
	struct tree *left;
	struct tree *right;
}Tree;
typedef struct counting{
	char password[50];
	char letter;
}Password;
Password passwords[256];
int count=0;
void reading(char *filename,int arr[256]){
	char c;
	FILE *fp;
	fp = fopen(filename,"r");

	while(!feof(fp)){
		c = getc(fp);
		if(c == EOF){
			break;
		}
		arr[c]++;		
	}
}
void find_smallest(Tree **arr,int size){
	int i,j;
	Tree *temp;
	for(i=0;i<size;i++){
		for(j=1;j<size-i;j++){
			if(arr[j-1]->freq > arr[j]->freq){
				temp = arr[j-1];
				arr[j-1] = arr[j];
				arr[j] = temp; 
		     }		
		}
	}
}
Tree* newnode(char letter, int freq){ 
    Tree* temp = (Tree*)malloc(sizeof(Tree)); 
    temp->left = temp->right = NULL; 
    temp->letter = letter; 
    temp->freq = freq;
    return temp; 
}
Tree* merge(Tree **tree,int size){
	Tree *node,*temp;
	node = (Tree*)malloc(sizeof(Tree));
	node->left = tree[1];
	node->right = tree[0];
	node->freq = tree[0]->freq + tree[1]->freq;
	node->letter = '&';
	
	tree[0] = node;
	temp = tree[size-1];
	tree[size-1] = tree[1];
	tree[1] = temp;

	tree[size-1] = NULL;
	free(tree[size-1]);
 
	return node;
} 
void enCodes(Tree *root, int *arr, int size){ 
    if (root->left != NULL) { 
  
        arr[size] = 0; 
        enCodes(root->left, arr, size + 1); 
    } 
    if (root->right != NULL) { 
  
        arr[size] = 1; 
        enCodes(root->right, arr, size + 1); 
    } 
    if (!(root->left) && !(root->right) == 1) { 
        passwords[count].letter = root->letter;
        for (int i = 0; i < size; ++i){ 
       		passwords[count].password[i] =  arr[i] + '0';
    	}
    	count++;   
    } 
} 
char *read(char *filename,int *size){
	FILE *fp;
	fp = fopen(filename,"r");
	char *arr,c;
	int i=2;
	arr = (char*)malloc(sizeof(char));
	while(!feof(fp)){
		c = fgetc(fp);
		if(c == EOF){
			break;
		}
		if(c!='\n'){
			arr[i-2] = c;
			arr = realloc(arr,i*sizeof(char));
			i++;
		}
	}
	*size=i-2;
	return arr;
}
int main(){
	FILE *fp,*fp1,*fp2;
	char reference[15],message[15],encoded[15],decoded[15];
	printf("Please enter referencing txt: \n");
	scanf("%s",reference);
	printf("Please enter message txt: \n");
	scanf("%s",message);
	printf("Please enter where do you want to encode (.dat): \n");
	scanf("%s",encoded);
	printf("Please enter where do you want to decode (.txt): \n");
	scanf("%s",decoded);
	fp = fopen(encoded,"wb");
	int i,j=0,size,freq[256] = {0},counter=2,num[100],sizes[200],counter2=0,k,counter3=0,arr_size;
	char *arr,arrr[256];
	Tree **nodes,*tree;
	nodes = (Tree**)malloc(sizeof(Tree*));
	reading(reference,freq);
	arr = read(message,&arr_size);
    for(i = 0;i<256;i++){
    	if(freq[i] != 0){
    		nodes[counter-2] = newnode(i,freq[i]);
    		nodes = realloc(nodes,counter*sizeof(Tree*));
    		counter++;
    	}
    }
   	size = counter-2;
    while(size>1){
	    find_smallest(nodes,size);
	    tree = merge(nodes,size);
	    size = size-1;
	}
	enCodes(tree,num,0);
	for(i=0;arr[i]!='\0';i++){
		for(j=0;j<count;j++){
			if(arr[i] == passwords[j].letter){
				fwrite(passwords[j].password,sizeof(passwords[j].password), 1, fp);
				for(k=0;passwords[j].password[k];k++){
					counter3++;
				}
			}
		}
	}
	fclose(fp);
	fp1=fopen(encoded,"rb");
	fp2 = fopen(decoded,"w");
	while(!feof(fp1)){
		fread(arrr,sizeof(passwords[j].password), 1, fp1);
		for(i=0;i<256;i++){
			if(strcmp(arrr,passwords[i].password) == 0){
				if(counter2 != arr_size){
					fprintf(fp2,"%c",passwords[i].letter);
					counter2++;
				}
			}
		}				
	}
	printf("The size of %s = %.2f byte\nThe size of %s = %d byte\n",encoded,(float)((float)counter3/8.0),decoded,counter2);
	fclose(fp1);
	fclose(fp2);

}