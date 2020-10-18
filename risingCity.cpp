#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <string>
#include <queue>
using namespace std;
#define MaxHp 2000	//Maximum size of the heap 

int min=0;
int gc=0;	//Global Counter that increments 
bool color;	//color is defined as a boolean with red=1 and black =0
int sz=0;	//Size of the heap
int ttl=0;	//Total time for the city
int tempsz=0;	//Size of temp Heap
ofstream outFile;
vector<int> bNum_vector;
vector<int> eTime_vector;
vector<int> tTime_vector;

//struct of RBT node
struct RBNode	
{	
	bool color;
	int bNum, eTime, tTime;
	struct RBNode *lft, *rght, *parent;
	RBNode(int b, int t) 	//constructor for RBT Node
{	
	bNum=b;
	eTime=0; //Initial eTime is set to 0
	tTime=t;
	color=1; //Initial default color is set to red(1)
	parent=NULL;
	lft=NULL;
	rght=NULL; 
}
};

//Prototype for RBNode functions
RBNode* sblngOf(struct RBNode *x);
RBNode* uncleOf(struct RBNode *x);
RBNode* gParent(struct RBNode *x);
bool withRdChild(struct RBNode *x);
bool isLChild(struct RBNode *x);
void goDwn(struct RBNode *newNode, struct RBNode *x);

//struct for RB Tree
struct RBTree
{
	struct RBNode *root;

	RBTree()
	{ 
		root=NULL; 
	} 	
};

//Prototype for RBTree functions
RBTree tree;
void swapClr(RBNode *x, RBNode *y);
void swapVlue(RBNode *x, RBNode *y);
void RBUpdate(int n);
void RBInsrtNode(int n, int m);
void RBDlt(int n);
void inOrdr(RBNode *x, int n);
void RBPrnt(int a);
void RBPrnt(int a,int b);
void LvlOrderPrnt();
void RRrotate(RBNode *x);
void LLrotate(RBNode *x);
void RRcorrect(RBNode *x);
RBNode *lftMost(RBNode *x);
RBNode *dltSubs(RBNode *x);
void BBcorrect(RBNode *x);
void dltNode(RBNode *x);
void lvlOrdr(RBNode *x);
RBNode* rtrnRoot();
RBNode* search(int n);

//struct for the minHeap
struct minHp
{
	int eTime;
	int bNum;
	int tTime;
};

//Prototypes for minHeap functions
struct minHp hp[MaxHp]; //array of struct type MinHeap 
void hpify(minHp *h, int i, int n); 
int hParent(int i);
struct minHp tempHp[10];

bool isLChild(struct RBNode *x) //To check if the node is Left Child of parent
{
	if(x==x->parent->lft)
		return true;
	else
		return false;
}

bool withRdChild(struct RBNode *x)	//To check if node has a Red Child
{
	 if((x->rght!=NULL and x->rght->color==1)or(x->lft!=NULL and x->lft->color==1))
	 	return true;
	else
	{
		return false;	
	}
}

RBNode* gParent(struct RBNode *x) //To find the GrandParent of a node
{
		return x->parent->parent; 
}

RBNode* sblngOf(struct RBNode *x) //To return the sibling of a node
{
	if (x->parent==NULL) 
		return NULL;
	else if (isLChild(x)==1) 
		return x->parent->rght;
	else
		return x->parent->lft;
}

RBNode* uncleOf(struct RBNode *x) //To return the uncle of a node
{
	if ((x->parent==NULL)or(gParent(x)==NULL)) 
		return NULL;
	else if (isLChild(x->parent)==1) 
		return gParent(x)->rght; 
	else
		return gParent(x)->lft;
}

RBNode* rtrnRoot() //Returns the root
{
	return tree.root;
}

void goDwn(struct RBNode *newNode, struct RBNode *x) // Goes down the RBT to put the node in the right position
{
	if (x->parent!=NULL)
	{
		if (isLChild(x)==0) 
			x->parent->rght=newNode;  
		else
			x->parent->lft=newNode;
	}
	newNode->parent=x->parent;
	x->parent=newNode;
}

void LLrotate(struct RBNode *x) //Rotates the the tree with the node in a LL position
{
	RBNode *newParent=x->lft; 
	if (x==tree.root) 
		tree.root=newParent;
	goDwn(newParent,x);  
	x->lft=newParent->rght; 
	if (newParent->rght != NULL) 
		newParent->rght->parent=x;
	newParent->rght=x; 
}

void RRrotate(struct RBNode *x) //Rotates the the tree with the node in a RR position
{
	RBNode *newParent=x->rght; 
	if (x==tree.root)
		tree.root=newParent; 
	goDwn(newParent,x); 
	x->rght=newParent->lft; 
	if (newParent->lft!=NULL) 
		newParent->lft->parent=x;
	newParent->lft = x; 
}

void RRcorrect(RBNode *x) //To correct a Red-Red condition
{
	if (x==tree.root)
	{
		x->color= 0;
		return;
	}
	RBNode *parent = x->parent;
	RBNode *grandparent = gParent(x);
	RBNode *uncle =uncleOf(x); 
	if (parent->color!=0)
	{
		if (uncle!=NULL&&uncle->color==1) 
		{
			parent->color = 0; 
			uncle->color= 0; 
			grandparent->color = 1; 
			RRcorrect(grandparent); 
		}
		else 
		{
			if (isLChild(parent)) 
			{
				if (isLChild(x)) 
					swapClr(parent, grandparent);
				else 
				{
					RRrotate(parent);
					swapClr(x, grandparent);
				}
				LLrotate(grandparent);
			}
			else 
			{
					if (isLChild(x)) 
					{
						LLrotate(parent);
						swapClr(x, grandparent);
					}
					else 
						swapClr(parent, grandparent);
					RRrotate(grandparent);
			}
		}
	}
}

void swapClr(struct RBNode *x, struct RBNode *y) //To swap the colors of two nodes
{	bool temp;	//creates temporary color
	temp=x->color;	
	x->color=y->color;
	y->color=temp;
}

void swapVlue(struct RBNode *x, struct RBNode *y) //To swap the data values in two nodes
{
	RBNode *temp=new RBNode(0,0);	//creates temporary node
	temp->bNum=x->bNum;
	temp->tTime=x->tTime;
	temp->eTime=x->eTime;
	x->bNum=y->bNum;
	x->tTime=y->tTime;
	x->eTime=y->eTime;
	y->bNum=temp->bNum;
	y->tTime=temp->tTime;
	y->eTime=temp->eTime;
}

RBNode* lftMost(RBNode *x) //Returns the leftmost node in the tree
{
	RBNode *temp=x;
	while(temp->lft!=NULL)
		temp=temp->lft;
	return temp; 
}

RBNode* dltSubs(RBNode *x) //Finds a substitute node for the node that is deleted
{
	if (x->lft!=NULL and x->rght!=NULL) 
		return lftMost(x->rght); 
	if (x->lft==NULL and x->rght==NULL) 
		return NULL;
	if (x->lft!=NULL) 
		return x->lft;
	else			
		return x->rght;
}

void BBcorrect(RBNode *x) 
{
	if (x==tree.root)
		return; 
	RBNode *sibling=sblngOf(x), *parent=x->parent;
	if (sibling==NULL)
		BBcorrect(parent); 
	else
	{
		if (sibling->color==1)
		{
			parent->color= 1;
			sibling->color = 0;
			if (isLChild(sibling)) 
				LLrotate(parent);
			else 
				RRrotate(parent);
			BBcorrect(x);
		}
		else
		{
			if (withRdChild(sibling))
			{
				if (sibling->lft != NULL and sibling->lft->color==1) 
				{
					if (isLChild(sibling)) 	
					{
						sibling->lft->color=sibling->color;
						sibling->color=parent->color;
						LLrotate(parent);
					}
					else 	
					{
						sibling->lft->color = parent->color;
						LLrotate(sibling);
						RRrotate(parent);
					}
				}
				else
				{
					if (isLChild(sibling)) 
					{
						sibling->rght->color=parent->color;
						RRrotate(sibling);
						LLrotate(parent);
					}
					else 
					{
						sibling->rght->color=sibling->color;
						sibling->color=parent->color;
						RRrotate(parent);
					}
				}
				parent->color = 0;
			}
			else 
			{
				sibling->color=1;
				if (parent->color==0)
					BBcorrect(parent);
				else
					parent->color=0;
			}
		}
	}
}

void dltNode(RBNode *x) //To Delete a node in the RBT
{
   	RBNode *u=dltSubs(x); //Node to be used for substitution
   	bool bothBlack=((u==NULL or u->color==0) and (x->color==0)); 
   	RBNode *parent=x->parent;
    if (u==NULL) 
	{
   		if (x==tree.root) 
       		tree.root=NULL;
		else
		{
       		if (bothBlack) 
       			BBcorrect(x); 
			else 
			{
       			if (sblngOf(x)!= NULL) 
           			sblngOf(x)->color=1;
       		}
       		if (isLChild(x))
       			parent->lft = NULL; 
			else
       			parent->rght = NULL; 
    	}
      	delete x; 
      	return;
    }
  	if (x->lft==NULL or x->rght==NULL) 
	{
    	if (x==tree.root) 
		{
       		x->bNum=u->bNum;
			x->tTime=u->tTime;
			x->eTime=u->eTime;
       		x->lft=x->rght=NULL;
       		delete u; 
    	}
		else
		{
       		
       		if (isLChild(x)) 
       			parent->lft = u; 
			else 
      			parent->rght = u; 
       		delete x;
       		u->parent = parent; 
       		if (bothBlack)
       			BBcorrect(u); 
			else
       			u->color= 0; 	
    	}
    	return;
    }
    swapVlue(u, x); 
    dltNode(u);
}


RBNode* search(int n) //Finds the node for the next insertion, deletion or Print based on bNum.
{
	RBNode *temp=tree.root;	//Creation of a temporary node
	while (temp!=NULL)
	{
  		if (n==temp->bNum) 
    		break;
		else if (n<temp->bNum) 
		{
    		if (temp->lft==NULL)  
      			break;
    		else
      			temp=temp->lft;
  		}

		else 
		{
    		if (temp->rght==NULL) 
      			break;
    		else
      			temp=temp->rght;
  		}
	}
	return temp; //Returns search result
}

void RBInsrtNode(int n, int m)	//Inserts a node with bNum and tTiem values
{
	RBNode *newNode=new RBNode(n,m); 
	if (tree.root==NULL)
	{
  		newNode->color=0; 
  		tree.root=newNode;  //Node is inserted as root 
	}
	else
	{
	  	RBNode *t=search(n); 
  		if (t->bNum==n)
		{
			cout<<"\nBuilding is already present!\n";
    		return;
  		}
  		newNode->parent=t; 
  		if(n<t->bNum) //Added as left child
    		t->lft=newNode;
  		else //Added as right child 
    		t->rght=newNode;
  		RRcorrect(newNode); 
	}
}

void RBDlt(int n) //Deletes node by a particular bNum
{
	if (tree.root==NULL) //Tree is empty
		return;
	RBNode *v=search(n);
	if (v->bNum!=n)
	{
		cout << "Unable to find node with bNum:" << n << endl;
		return;
	}
	dltNode(v);
}
void RBPrnt(int a)	//Prints RBT with one bNum
{
	RBNode *sn=search(a);
	if(sn->bNum==a)
		outFile<<"("<<sn->bNum<<","<<sn->eTime<<","<<sn->tTime<<")"<<endl;
	else
		outFile<<"(0,0,0)"<<endl;
}

void RBPrnt(int a, int b)	//
{	if(sz==0)
		outFile<<"(0,0,0)"<<endl;
	int FirstB=1;
	int Bpresent=0;
	for (int i=a;i<=b;i++)
	{
	RBNode *sn =search(i);
	
	if (sn!=NULL && sn->bNum == i && FirstB==1)
	{

		outFile<< "(" << sn->bNum << "," << sn->eTime << "," << sn->tTime << ")";
		Bpresent = 1;
		FirstB = 0;
	}

 	else if (sn!= NULL && sn->bNum == i && FirstB==0 )
	{
						
		outFile<< ",(" << sn->bNum << "," << sn->eTime << "," << sn->tTime << ")";
		Bpresent = 1;
	}		
	}
	outFile<<endl;	
}

void RBUpdate(int n)//Updates the eTime in the RBT when it changes in the heap
{
	//int a=x->bNum;
	RBNode *x=search(n);
	x->eTime=x->eTime+1;	
}

bool procd=true;	//Tells when to proceed with the execution
void Insert(int bnum, int ttime)	//Inserts into the heap with bNum and tTime
{
    if (sz>MaxHp) 
    {
        cout <<"Size has exceeded maximum size!\n";
        return;
    }
    if(procd==true)
    {
    
	sz=sz+1;
    int i=sz-1;
    hp[i].tTime=ttime;	
	hp[i].bNum=bnum;
	hp[i].eTime=0;
	int j=hParent(i);
    	
		while (i!=0 and hp[i].eTime<=hp[j].eTime)
    	{	
			if(hp[j].eTime==hp[i].eTime)
			{
			 	if(hp[j].bNum>hp[i].bNum)
    			{
    				swap(hp[i], hp[j]);
       				i=j;
					j=hParent(i);
				}
				else
    			{
       				i=j;
       				j=hParent(i);
				}
			}
			else if(hp[j].eTime>hp[i].eTime)
			{
				swap(hp[i], hp[j]);
       			i =j;
				j=hParent(i);
			}
    	}
	}
	else
	{
	
		tempHp[tempsz].bNum=bnum;
		tempHp[tempsz].tTime=ttime;
  		tempHp[tempsz].eTime=0;
  		tempsz++;
	}
}

int hParent(int i) //Returns the parent of the heap
{
	return floor((i-1)/2);
}

void pushTmp()	//Pushes from the temp heao to the main 
{
	int i=0;
			while(i<tempsz)
			{
				Insert(tempHp[i].bNum, tempHp[i].tTime);
				tempsz--;
				i++;
			}
			tempsz=0;
}

void constrFin()	//Finishes the last part of Building construction
{

	hpify(hp,sz,0);
    	procd=true;
    	if(tempsz>0)
		{	
			pushTmp();
		}
}

void construct()	//Constructs a building 
{
	
	if(sz<=0)
		return;
	hp[0].eTime=hp[0].eTime+1;
	RBUpdate(hp[0].bNum);
	if(hp[0].eTime==hp[0].tTime)
	{
		if (sz==1)
        	ttl=gc; // Sets the total time when it is the last node
		outFile<<"("<<hp[0].bNum<<","<<gc<<")"<<endl;
		int q=hp[0].bNum;
		RBDlt(q);
    	hp[0]=hp[sz-1];
    	sz=sz-1;
    	constrFin();
	}
	else if(hp[0].eTime%5==0)
	{
		constrFin();
	}
	else
		procd=false;
	}
	void hpify(minHp *hp, int n, int i)	//Restores the minHeap property 
{
	int small=i;
	int l=2*i+1; //Left Sibling 
	int r=2*i+2; //Right Sibling 
	if((r<n)&&(hp[r].eTime==hp[i].eTime))
	{	if(hp[r].eTime==hp[l].eTime)
		{	
			if(hp[r].bNum<hp[l].bNum&&hp[r].bNum<hp[i].bNum)
				small=r;
			else if(hp[l].bNum<hp[r].bNum&&hp[l].bNum<hp[i].bNum)
				small=l;
			else
				small=i;
		}
		else
		{
			if(hp[r].bNum<hp[i].bNum)
				small=r;
			else
				small=i;
		}
	}
	else if(l<n&&hp[l].eTime==hp[i].eTime)
	{
		if(hp[i].bNum > hp[l].bNum)
			small=l;
		else
			small=i;
	}

	if(r<n&&hp[r].eTime<hp[small].eTime&&hp[l].eTime<hp[small].eTime)
	{ 	if(hp[l].eTime==hp[r].eTime)
		{
			if(hp[l].bNum > hp[r].bNum)
				small=r;
			else
				small=l;
		}
	}
	else if(l<n&&hp[l].eTime<hp[small].eTime)
		small=l;
	else if(r<n&&hp[r].eTime<hp[small].eTime)
		small=r;
	if(small!=i)
	{
		swap(hp[small],hp[i]);
		hpify(hp,n,small);
	}
}

void swap(minHp a, minHp b)	//Swaps two MinHeap nodes
{
	struct minHp temp;
	temp=a;
	a=b;
	b=temp;
}

int main (int argc, char** argv)
{	outFile.open("output_file.txt",ios::out|ios::trunc);	//writes to output file 
	int ctr=0;	//Counter
	int prm1=0;	//First paramter 
	int prm2=0;	//Second paramter
	int psn=0;	//Position
	int flag=0;	
	bool b=0;
	string func,gcString,prm;	//Declaration for function, Globalcounter String and paramter 
  	ifstream inFile;	//Input File 
    
	inFile.open(argv[1]);
  	if (!inFile) {
    cout << "Failed to open Input File!";

    }
  	else if (inFile)
	{
    	while (getline(inFile, gcString, ':'))	//Returns the Paramter passed by the Input
    	{
			istringstream(gcString)>>ctr;
			getline(inFile,func, '(');
			getline(inFile,prm, ')');
    		while(gc<ctr)
			{
				gc=gc+1;
				construct();
			}
			int InsCmpr=func.compare("Insert");
			int InsCmpr2=func.compare(" Insert");
			int PrntCmpr=func.compare(" PrintBuilding");
			int PrntCmpr2=func.compare("PrintBuilding");
			
			if(InsCmpr==0||InsCmpr2==0) 	//Checks if paramater is compare
			{
				psn=prm.find(',');
				int prmL=prm.length();
				istringstream(prm.substr(0,prm.find(',')))>>prm1;
				istringstream(prm.substr(psn+1,prmL-1))>>prm2;
				RBInsrtNode(prm1,prm2);
				Insert(prm1,prm2);
				
			}
			else if(PrntCmpr==0||PrntCmpr2==0)	////Checks if paramater is Print
			{
				psn=prm.find(',');
				int prmL=prm.length();
				if (psn==std::string::npos)
				{
					istringstream(prm)>>prm1;
					RBPrnt(prm1);
				}
				else
				{
					istringstream(prm.substr(0,psn))>>prm1;
					istringstream(prm.substr(psn+1,prmL-1))>>prm2;
					RBPrnt(prm1,prm2);
				}
			}
		}
		while(sz>0)
  		{
  			gc=gc+1;	//Increases Global counter
  			construct();	
		}
    	inFile.close();
  	}

	// cout<<"\nTime taken to for the city to be constructed "<<ttl<<endl;
	outFile.close();
	return 0;
}
