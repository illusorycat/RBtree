#include <iostream>
using namespace std;

enum Color
{
	RED = 0,
	BLACK = 1
};

typedef int KEY_TYPE;

int Key_Compare(const KEY_TYPE& k1, const KEY_TYPE& k2)
{
	if (k1 == k2)
		return 0;
	if (k1 < k2)
		return -1;
	else
		return 1;
}

struct rbtree_node
{
	Color color;
	KEY_TYPE key;
	rbtree_node* right;
	rbtree_node* left;
	rbtree_node* parent;
	rbtree_node(KEY_TYPE _key, Color _color):key(_key), color(_color)
	{
		right = left = parent = nullptr;
	}
};

struct rbtree
{
	rbtree_node* root;
	rbtree_node* nil;  //空节点，用于做叶子节点，默认为黑色
};

void rbtree_left_rotate(rbtree* T, rbtree_node* x)  //x的右子节点存在
{
	rbtree_node* y = x->right;

	y->parent = x->parent;
	if (x->parent == T->nil)  //x没有父节点，x为根节点
		T->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	x->right = y->left;
	if (y->left != T->nil)
		y->left->parent = x;

	x->parent = y;
	y->left = x;

}

void rbtree_right_rotate(rbtree* T, rbtree_node* y)  //x的左子节点存在
{
	rbtree_node* x = y->left;

	x->parent = y->parent;
	if (y->parent == T->nil)
		T->root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	y->left = x->right;
	if (x->right != T->nil)
		x->right->parent = y;

	y->parent = x;
	x->right = y;
}

void rbtree_insert_fixup(rbtree* T, rbtree_node* node)
{
	while (node->parent->color == RED)
	{
		if (node->parent == node->parent->parent->left)   //父节点是祖父节点的左子树
		{
			rbtree_node* y = node->parent->parent->right;
			if (y->color == RED)
			{
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;   //祖父节点被改变为红色了，
			}
			else
			{//叔父节点是黑色
				if (node == node->parent->right)   //将该节点与父节点、祖父节点调整在同一方向上
				{
					node = node->parent;
					rbtree_left_rotate(T, node);
				}

				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rbtree_right_rotate(T, node->parent->parent);
			}
		}
		else//父节点是祖父节点的右子树
		{
			rbtree_node* y = node->parent->parent->left;
			if (y->color == RED)
			{
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;   //祖父节点被改变为红色了，
			}
			else
			{
				if (node == node->parent->left)
				{
					node = node->parent;
					rbtree_right_rotate(T, node);
				}

				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rbtree_left_rotate(T, node->parent->parent);
			}
		}
	}

	//
	if (T->root->color == RED)
		T->root->color = BLACK;

}
void rbtree_insert(rbtree* T, rbtree_node* node)
{
	rbtree_node* p = T->nil;
	rbtree_node* c = T->root;  

	while (c != T->nil)
	{
		p = c;    
		if (Key_Compare(c->key, node->key) < 0)
		{
			c = c->right;
		}
		else if (Key_Compare(c->key, node->key) > 0)
		{
			c = c->left;
		}
		else   //节点存在
		{
			return;
		}
	}

	node->parent = p;
	if (p == T->nil)
		T->root = node;
	else if (Key_Compare(p->key, node->key) < 0)
		p->right = node;
	else
		p->left = node;

	node->color = RED;
	node->left = T->nil;
	node->right = T->nil;

	rbtree_insert_fixup(T, node);
}


int Bn(rbtree* T,rbtree_node* node)  //判断node子树是不是黑色节点平衡且红色节点不连续，最小返回1，返回0表示不平衡
{
	if (node == T->nil)
		return 1;
	if (node->color == RED)
		if (node->parent->color == RED)
			return 0;  //红色节点连续
	int BlackOfL = Bn(T, node->left);
	if (BlackOfL == 0)
		return 0;
	int BlackOfR = Bn(T, node->right);
	if (BlackOfR == 0)
		return 0;

	if (BlackOfL == BlackOfR)
	{
		if (node->color == RED)
			return BlackOfL;
		else
			return ++BlackOfL;
	}
	else
		return 0;
}
bool isRBT(rbtree* T)
{
	if (T->root == T->nil) //树为空
		return true;
	if (T->root->color == RED)
		return false;
	else
	{//在搜索树的前提下
		return (Bn(T, T->root) == 0) ? false : true;
	}
}

rbtree_node* rbtree_search(rbtree* T, KEY_TYPE key)
{
	rbtree_node* node = T->root;
	while (node != T->nil)
	{
		if (Key_Compare(node->key, key) == 0)
			return node;
		else if (Key_Compare(node->key, key) > 0)
			node = node->left;
		else
			node = node->right;
	}
	return T->nil;
}

void rbtree_traversal(rbtree* T, rbtree_node* node)
{
	//前序遍历
	if (node != T->nil)
	{
		rbtree_traversal(T, node->left);
		cout << "Key:" << node->key << " Color:" << node->color << endl;
		rbtree_traversal(T, node->right);
	}
}

rbtree_node* rbtree_maxi(rbtree* T, rbtree_node* node) //查找node子树的最大key节点
{
	while (node->right != T->nil)
		node = node->right;

	return node;
}
rbtree_node* rbtree_mini(rbtree* T, rbtree_node* node) //查找node子树的最小key节点
{
	while (node->left != T->nil)
		node = node->left;

	return node;
}
rbtree_node* rbtree_successor(rbtree* T, rbtree_node* node)  //查找在中序遍历下，node节点的后继节点
{
	if (node->right != T->nil)
		return rbtree_mini(T, node->right);

	rbtree_node* p = node->parent;
	while ((p != T->nil) && (node == p->right))
	{
		node = p;
		p = p->parent;
	}
	return p;
}

void rbtree_delete_fixup(rbtree* T, rbtree_node* node)  //node这个分支黑高度少1
{//node的兄弟节点一定有子节点
	while (node != T->root && node->color == BLACK) //如果调整节点是根节点或红色节点，则直接将节点变为黑色即可
	{
		if (node == node->parent->left)  //调整节点是左子节点
		{
			rbtree_node* bro = node->parent->right; //兄弟节点
			if (bro->color == RED)  //兄弟节点是红色，父节点是黑色
			{
				rbtree_left_rotate(T, node->parent);
				bro->color = BLACK;
				node->parent->color = RED;
			}
			//else// 兄弟节点是黑色，此处可加else，也可不加,因为上一代码段完成后，node仍需调整，且必定是在下面这一代码段的情况
			{
				if (bro->left->color == BLACK && bro->right->color == BLACK) //侄子节点都为黑色
				{
					bro->color = RED;
					node = node->parent;
				}
				else if(bro->right->color == RED)  //兄弟节点的右子节点为红色，左子节点颜色任意
				{
					bro->color = node->color;
					node->parent->color = BLACK;
					rbtree_left_rotate(T, node->parent);
					bro->right->color = BLACK;
					return;  //调整完成
				}
				else  ////兄弟节点的左子节点为红色，右子节点为黑色
				{
					bro->color = RED;
					bro->left->color = BLACK;
					rbtree_right_rotate(T, bro);
				}
			}
		}
		else  //调整节点是右子节点，与上面的代码镜像
		{
			rbtree_node* bro = node->parent->left;
			if (bro->color == RED)  //兄弟节点是红色，父节点是黑色
			{
				rbtree_right_rotate(T, node->parent);
				bro->color = BLACK;
				node->parent->color = RED;
			}
			{
				if (bro->left->color == BLACK && bro->right->color == BLACK) //侄子节点都为黑色
				{
					bro->color = RED;
					node = node->parent;
				}
				else if (bro->left->color == RED)  //远侄子节点为红色，近侄子节点颜色任意
				{
					bro->color = node->color;
					node->parent->color = BLACK;
					rbtree_right_rotate(T, node->parent);
					bro->left->color = BLACK;
					return;  //调整完成
				}
				else  ////兄弟节点的近侄子节点为红色，远侄子节点为黑色
				{
					bro->color = RED;
					bro->right->color = BLACK;
					rbtree_left_rotate(T, bro);
				}
			}
		}
	}
	if(node->color == RED)  //如果调整节点是红色，直接变为黑色即可
		node->color = BLACK;

	if (T->root->color == RED)
		T->root->color = BLACK;
}

rbtree_node* rbtree_delete(rbtree* T, rbtree_node* node)
{
	rbtree_node* y = T->nil;//要删除的节点
	rbtree_node* x = T->nil;//要删除节点的下一个节点//通过下面的操作，使删除节点最多只有一个子节点
	if (node->left == T->nil || node->right == T->nil)
		y = node;
	else
		y = rbtree_successor(T, node); //找到后继节点，因为node不是根节点，所以一定有后继节点,且因为有右子树，后继节点一定没有左子节点

	if (y->left != T->nil)
		x = y->left;
	else if (y->right != T->nil)
		x = y->right;

	x->parent = y->parent;
	if (y->parent == T->nil)
		T->root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	if (y != node) // 
	{
		node->key = y->key;
	}

	if (y->color == BLACK)
	{
		rbtree_delete_fixup(T, x);
	}

	return y;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <vector>
void test01()
{
	rbtree tree;
	rbtree_node node(0, BLACK);
	tree.nil = &node;
	tree.root = tree.nil;

	vector<rbtree_node> vecn;

	int i = 0;
	while (i < 100)
		vecn.push_back(rbtree_node(i++, RED));
	while (i > 0)
	{
		rbtree_insert(&tree, &(vecn[--i]));
		cout << "----------------------------------" << endl;
		rbtree_traversal(&tree, tree.root);
		cout << "----------------------------------" << endl;
	}

}

void test02()
{
	rbtree tree;
	rbtree_node node(0, BLACK);
	tree.nil = &node;
	tree.root = tree.nil;

	vector<rbtree_node> vecn;

	int i = 0;
	while (i < 100)
		vecn.push_back(rbtree_node(i++, RED));
	while (i > 0)
	{
		rbtree_insert(&tree, &(vecn[--i]));
		cout << (isRBT(&tree) ? "是" : "不是") << endl;
	}

	if (tree.root->left->color == RED)
		tree.root->left->color = BLACK;
	else
		tree.root->left->color = RED;
	cout << (isRBT(&tree) ? "是" : "不是") << endl;
}

void test03()
{
	rbtree tree;
	rbtree_node node(0, BLACK);
	tree.nil = &node;
	tree.root = tree.nil;

	vector<rbtree_node> vecn;

	int i = 0;
	while (i < 100)
		vecn.push_back(rbtree_node(i++, RED));
	while (i > 0)
	{
		rbtree_insert(&tree, &(vecn[--i]));
	}

	rbtree_delete(&tree, &(vecn[88]));
	cout << (isRBT(&tree) ? "是" : "不是") << endl;
	rbtree_delete(&tree, tree.root);
	cout << (isRBT(&tree) ? "是" : "不是") << endl;

}

int main()
{
	test03();
}
