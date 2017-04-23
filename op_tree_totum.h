#include<iostream>
using namespace std;

class node{
public:
	node();
	node(int value);
	~node();
	friend void gothrough(node *p);

private:
	node *left, *right; // the left child and the right child pointer
	int number; // record the node's number
	int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

	friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

public:
	op_tree_totum();
	~op_tree_totum();
	void insertion(int s);
	void deletion(int s);
	void inorder_run();
	void reverseorder_run();
	int size();

private:
	node *root, *head, *tail;
	int num;//caculate how many nodes in the totum
};

//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->left = head;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if(p!=NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl==0 && p->left!= NULL) gothrough(p->left);
	if (p->is_threadr==0 && p->right!= NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
	//TODO: fill in the code to do the insertion of the node with number s
    num++;
    if(num == 1)
    {
        root = new node(s);
        root->is_threadl=0;
        root->is_threadr=0;
        root->right = NULL;
	root->left = NULL;
	head = root;
        tail = root;
    }else
    {
        node* tmp_node = root;
        node* tmp_subnode;
        int is_thread;
        int dir; // 0 left 1 right
        if(root->number>s)
        {
            is_thread=root->is_threadl;
            tmp_subnode = root->left;
            dir = 0;
        }
        else if(root->number<s)
        {
            is_thread=root->is_threadr;
            tmp_subnode = root->right;
            dir = 1;
        }
	else return;
        while(is_thread == 0 && tmp_subnode != NULL)
        {
                if(tmp_subnode->number > s)
                {
                    tmp_node = tmp_subnode;
                    is_thread=tmp_subnode->is_threadl;
                    tmp_subnode = tmp_subnode->left;
                    dir = 0;
                }
                else if(tmp_subnode->number < s)
                {
                    tmp_node = tmp_subnode;
                    is_thread=tmp_subnode->is_threadr;
                    tmp_subnode = tmp_subnode->right;
                    dir = 1;
                }
		else return;
        }
        if(is_thread)
        {
            if(dir)
            {
		//node* tmp = tmp_node->right;
                 tmp_node->right = new node(s);
		 tmp_node->is_threadr=0;
                (tmp_node->right)->right = tmp_subnode;
                (tmp_node->right)->left = tmp_node;
            }
            else
            {
		//node* tmp = tmp_node->left;
		tmp_node->is_threadl=0;
                tmp_node->left = new node(s);
                (tmp_node->left)->left = tmp_subnode;
                (tmp_node->left)->right = tmp_node;
            }
        }
        else
        {
            if(dir)  //right end (tail)
            {
                tmp_node->right = new node(s);
                tail = tmp_node->right;
                tail->is_threadr=0;
                tail->left = tmp_node;
            }else
            {
                tmp_node->left = new node(s);
                head = tmp_node->left;
                head->is_threadl=0;
                head->right = tmp_node;
            }
        }
    }



}

void op_tree_totum::deletion(int s){
	//TODO: fill in the code to do the deletion of the node with number s
//%%%%%
	    node* tmp = root;
    node* tmp_father;
    int dir;
    int is_find=1;
    while(tmp->number != s)
    {
        if(tmp->number > s )
        {
            if(tmp->left == NULL || tmp->is_threadl == 1){is_find=0;break;}
            else {tmp_father=tmp;tmp = tmp->left;dir=0;}
        }else
        {
            if(tmp->right == NULL || tmp->is_threadr == 1){is_find=0;break;}
            else {tmp_father=tmp;tmp = tmp->right;dir=1;}
        }
    }
    //沒找到 OR 找到=>1.邊界2.有小孩
    if(is_find){
	num-- ;
        if(tmp == root) // root
        {
	  if(tmp->right == NULL)
            {
                if(tmp->left == NULL) //only root
                {
                    root = NULL;
                    tail = root;
		    head = root;
		    delete tmp;
                }
                else //right empty
                {
                    root = tmp->left;
                    tail = root;
                    if(tail->is_threadr)
                    {
                        tail->is_threadr = 0;
                        tail->right = NULL;
                    }
                    delete tmp;
                }
            }
            else // root but both subtree
            {
                tmp_father=tmp;
                tmp=tmp->right;
                while(tmp->is_threadl == 0)
                {
                    tmp = tmp->left;
                }
		if(root->right == tmp)
		{
  		    tmp_father->number = tmp->number;
		    if(tmp == tail)
		    {
			tail = root ;
			root->right = NULL;
			delete tmp;
		    }
		    else
		    {
		    tmp_father->number = tmp->number;
		    tmp_father->right = tmp->right;
		    node* delete_node = tmp;
		    tmp = tmp->right;
	     	    while(tmp->is_threadl == 0)tmp = tmp->left;
   		    tmp->left = root;
		    delete delete_node;
		    
		    }
		}
                else if(tmp->is_threadr) //right no subtree
                {
                    tmp_father->number = tmp->number;
                    tmp->right->left = tmp->left;
                    tmp->right->is_threadl = 1;
                    delete tmp;
                }
                else   // right subtree
                {
                    node* tmp_tmp=tmp;
                    tmp_father->number = tmp->number;
                    while(tmp->is_threadr == 0)tmp = tmp->right;
                    tmp->right->left = tmp_tmp->right;
                    tmp = tmp_tmp->right;
                    while(tmp->is_threadl == 0)tmp = tmp->left;
                    tmp->left = tmp_tmp->left;
                    delete tmp;
                }

            }
        }
        else if(tmp == tail ) //delete tail
        {
	  if(tmp->is_threadl){
            tail = tmp->left;
            tail->right = NULL;
            delete tmp;
	  }
	  else{
	    tail = tmp->left;
            tmp_father->right = tail;
	    while(tail->is_threadr == 0)tail=tail->right;
            tail->right = NULL;
	    tail->is_threadr = 0;
	    delete tmp;
	  }
    
        }else if(tmp == head) // delete head
        {
	  if(tmp->is_threadr)
	   {
            head = tmp->right;
	//    while(head->is_threadl == 0)head = head->left;
            head->left = NULL;
	    head->is_threadl = 0;
	//    tmp_father->left = tmp->right;
           }
	   else
	   {
	     head = tmp->right;
	     while(head->is_threadl == 0)head = head->left;
            head->left = NULL;
            head->is_threadl = 0;
            tmp_father->left = tmp->right;
	   }   
	 delete tmp;
        }else if(tmp->is_threadl==0 && tmp->is_threadr == 1)// left subtree right empty
        {
            if(dir)//right
            {
                tmp_father->right = tmp->left;
                node* tmpp=tmp->left;
                while(tmpp->is_threadr != 1)tmpp = tmpp->right;
                tmpp->right = tmp->right;
                delete tmp;
            }else
            {
                tmp_father->left = tmp->left;
                node* tmpp=tmp->left;
                while(tmpp->is_threadr != 1)tmpp = tmpp->right;
                tmpp->right = tmp->right;
                delete tmp;
            }
        }
        else if(tmp->is_threadl==1 && tmp->is_threadr == 0)// right subtree left empty
        {
            if(dir)//right
            {
                tmp_father->right = tmp->right;
                node* tmpp=tmp->right;
                while(tmpp->is_threadl != 1)tmpp = tmpp->left;
                tmpp->left = tmp->left;
                delete tmp;
            }else
            {
                tmp_father->left = tmp->right;
                node* tmpp=tmp->right;
                while(tmpp->is_threadl != 1)tmpp = tmpp->left;
                tmpp->left = tmp->left;;
                delete tmp;
            }
        }
        else if(tmp->is_threadl ==1 && tmp->is_threadr == 1) // endpoint but not head or tail
        {
            if(dir)
            {
                tmp_father->right = tmp->right;
		tmp_father->is_threadr = 1;
                delete tmp;
            }
            else
            {
                tmp_father->left = tmp->left;
		 tmp_father->is_threadl = 1;
                delete tmp;
            }

        }
        else if(tmp->is_threadl ==0 && tmp->is_threadr == 0) //not root but both subtree
        {
            tmp_father = tmp;
            tmp = tmp->right;
            while(tmp->is_threadl == 0)tmp = tmp->left;
            tmp_father->number = tmp->number;
            if(tmp->is_threadr ==1 && tmp->is_threadl==1)
            {
	      if(tmp_father->right == tmp)
                  {
                    tmp_father->number = tmp->number;
                    tmp_father->right = tmp->right;
                    tmp_father->is_threadr = 1;
                        delete tmp;
                  }
		else{
                (tmp->right)->left = tmp_father;
                tmp->right->is_threadl = 1;
		delete tmp;}
            }
            else if(tmp->right == NULL) // tail
            {
                tail = tmp->left;
                tail->right = NULL;
                delete tmp;
            }
            else //right subtree
            {
               if(tmp->is_threadl)
		{
		    node* tmp_tmp=tmp;
		    tmp_father->number = tmp->number;
		    tmp_father->right = tmp->right;
		    tmp = tmp->right;
		    while(tmp->is_threadl ==0)tmp=tmp->left;
		    tmp->left = tmp_father;
		    tmp->is_threadl = 1;
		    delete tmp_tmp;
       		}
		else
		{
                    tmp_father->number = tmp->number;
		    tmp->right->left=tmp->left;
		    tmp->right->is_threadl = 1;
                    /*
		    tmp = tmp->right;
                    while(tmp->is_threadr == 0)tmp = tmp->right;
                    tmp->right->left = tmp_tmp->right;
                    tmp = tmp_tmp->right;
                    while(tmp->is_threadl == 0)tmp = tmp->left;
                    tmp->left = tmp_tmp->left;
		    */
                    delete tmp;
		}
            }
      }
    }
  }

void op_tree_totum::inorder_run(){

	//TODO: travel the tree from the head node to the tail node and output the values
	//You should NOT use stack or recurtion
	node* tmp = head;
	node* tmp_father=tmp;
        if(root == NULL){cout<<"NO node"<<endl;return;} //resolve empty
       	cout<<tmp->number<<" ";
	while(tmp->right != NULL) //cout until fight is NULL (tail)
     {
//        tmp = tmp->right;
//        cout<<tmp->number<<" ";
//	if(tmp->right == NULL)break;
        tmp_father=tmp;
        tmp = tmp->right;
        if(tmp->is_threadl == 0 && tmp_father->is_threadr == 0)while(tmp->is_threadl == 0)
        {
            tmp = tmp->left;
        }
        cout<<tmp->number<<" ";
        while(tmp->is_threadr)
        {
            tmp = tmp->right;
            cout<<tmp->number<<" ";
        }
   }

}

void op_tree_totum::reverseorder_run(){
      //TODO: travel the tree from the tail node to the head node and output the values
	//You should NOT use stack or recurtion
        node* tmp = tail;
	node* tmp_father=tmp;
         if(root == NULL){cout<<"NO node"<<endl;return;}
	cout<<tmp->number<<" ";
	while(tmp->left != NULL)
     {
           tmp_father=tmp;
        tmp = tmp->left;
        if(tmp->is_threadr == 0 && tmp_father->is_threadl == 0)while(tmp->is_threadr == 0)
        {
            tmp = tmp->right;
        }
        cout<<tmp->number<<" ";
        while(tmp->is_threadl)
        {
            tmp = tmp->left;
            cout<<tmp->number<<" ";
        }
   }
}

int op_tree_totum::size(){
	return num;
}

