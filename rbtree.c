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
	rbtree_node* nil;  //�սڵ㣬������Ҷ�ӽڵ㣬Ĭ��Ϊ��ɫ
};

void rbtree_left_rotate(rbtree* T, rbtree_node* x)  //x�����ӽڵ����
{
	rbtree_node* y = x->right;

	y->parent = x->parent;
	if (x->parent == T->nil)  //xû�и��ڵ㣬xΪ���ڵ�
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

void rbtree_right_rotate(rbtree* T, rbtree_node* y)  //x�����ӽڵ����
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
		if (node->parent == node->parent->parent->left)   //���ڵ����游�ڵ��������
		{
			rbtree_node* y = node->parent->parent->right;
			if (y->color == RED)
			{
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;   //�游�ڵ㱻�ı�Ϊ��ɫ�ˣ�
			}
			else
			{//�常�ڵ��Ǻ�ɫ
				if (node == node->parent->right)   //���ýڵ��븸�ڵ㡢�游�ڵ������ͬһ������
				{
					node = node->parent;
					rbtree_left_rotate(T, node);
				}

				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rbtree_right_rotate(T, node->parent->parent);
			}
		}
		else//���ڵ����游�ڵ��������
		{
			rbtree_node* y = node->parent->parent->left;
			if (y->color == RED)
			{
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;   //�游�ڵ㱻�ı�Ϊ��ɫ�ˣ�
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
		else   //�ڵ����
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


int Bn(rbtree* T,rbtree_node* node)  //�ж�node�����ǲ��Ǻ�ɫ�ڵ�ƽ���Һ�ɫ�ڵ㲻��������С����1������0��ʾ��ƽ��
{
	if (node == T->nil)
		return 1;
	if (node->color == RED)
		if (node->parent->color == RED)
			return 0;  //��ɫ�ڵ�����
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
	if (T->root == T->nil) //��Ϊ��
		return true;
	if (T->root->color == RED)
		return false;
	else
	{//����������ǰ����
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
	//ǰ�����
	if (node != T->nil)
	{
		rbtree_traversal(T, node->left);
		cout << "Key:" << node->key << " Color:" << node->color << endl;
		rbtree_traversal(T, node->right);
	}
}

rbtree_node* rbtree_maxi(rbtree* T, rbtree_node* node) //����node���������key�ڵ�
{
	while (node->right != T->nil)
		node = node->right;

	return node;
}
rbtree_node* rbtree_mini(rbtree* T, rbtree_node* node) //����node��������Сkey�ڵ�
{
	while (node->left != T->nil)
		node = node->left;

	return node;
}
rbtree_node* rbtree_successor(rbtree* T, rbtree_node* node)  //��������������£�node�ڵ�ĺ�̽ڵ�
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

void rbtree_delete_fixup(rbtree* T, rbtree_node* node)  //node�����֧�ڸ߶���1
{//node���ֵܽڵ�һ�����ӽڵ�
	while (node != T->root && node->color == BLACK) //��������ڵ��Ǹ��ڵ���ɫ�ڵ㣬��ֱ�ӽ��ڵ��Ϊ��ɫ����
	{
		if (node == node->parent->left)  //�����ڵ������ӽڵ�
		{
			rbtree_node* bro = node->parent->right; //�ֵܽڵ�
			if (bro->color == RED)  //�ֵܽڵ��Ǻ�ɫ�����ڵ��Ǻ�ɫ
			{
				rbtree_left_rotate(T, node->parent);
				bro->color = BLACK;
				node->parent->color = RED;
			}
			//else// �ֵܽڵ��Ǻ�ɫ���˴��ɼ�else��Ҳ�ɲ���,��Ϊ��һ�������ɺ�node����������ұض�����������һ����ε����
			{
				if (bro->left->color == BLACK && bro->right->color == BLACK) //ֶ�ӽڵ㶼Ϊ��ɫ
				{
					bro->color = RED;
					node = node->parent;
				}
				else if(bro->right->color == RED)  //�ֵܽڵ�����ӽڵ�Ϊ��ɫ�����ӽڵ���ɫ����
				{
					bro->color = node->color;
					node->parent->color = BLACK;
					rbtree_left_rotate(T, node->parent);
					bro->right->color = BLACK;
					return;  //�������
				}
				else  ////�ֵܽڵ�����ӽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ
				{
					bro->color = RED;
					bro->left->color = BLACK;
					rbtree_right_rotate(T, bro);
				}
			}
		}
		else  //�����ڵ������ӽڵ㣬������Ĵ��뾵��
		{
			rbtree_node* bro = node->parent->left;
			if (bro->color == RED)  //�ֵܽڵ��Ǻ�ɫ�����ڵ��Ǻ�ɫ
			{
				rbtree_right_rotate(T, node->parent);
				bro->color = BLACK;
				node->parent->color = RED;
			}
			{
				if (bro->left->color == BLACK && bro->right->color == BLACK) //ֶ�ӽڵ㶼Ϊ��ɫ
				{
					bro->color = RED;
					node = node->parent;
				}
				else if (bro->left->color == RED)  //Զֶ�ӽڵ�Ϊ��ɫ����ֶ�ӽڵ���ɫ����
				{
					bro->color = node->color;
					node->parent->color = BLACK;
					rbtree_right_rotate(T, node->parent);
					bro->left->color = BLACK;
					return;  //�������
				}
				else  ////�ֵܽڵ�Ľ�ֶ�ӽڵ�Ϊ��ɫ��Զֶ�ӽڵ�Ϊ��ɫ
				{
					bro->color = RED;
					bro->right->color = BLACK;
					rbtree_left_rotate(T, bro);
				}
			}
		}
	}
	if(node->color == RED)  //��������ڵ��Ǻ�ɫ��ֱ�ӱ�Ϊ��ɫ����
		node->color = BLACK;

	if (T->root->color == RED)
		T->root->color = BLACK;
}

rbtree_node* rbtree_delete(rbtree* T, rbtree_node* node)
{
	rbtree_node* y = T->nil;//Ҫɾ���Ľڵ�
	rbtree_node* x = T->nil;//Ҫɾ���ڵ����һ���ڵ�//ͨ������Ĳ�����ʹɾ���ڵ����ֻ��һ���ӽڵ�
	if (node->left == T->nil || node->right == T->nil)
		y = node;
	else
		y = rbtree_successor(T, node); //�ҵ���̽ڵ㣬��Ϊnode���Ǹ��ڵ㣬����һ���к�̽ڵ�,����Ϊ������������̽ڵ�һ��û�����ӽڵ�

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
		cout << (isRBT(&tree) ? "��" : "����") << endl;
	}

	if (tree.root->left->color == RED)
		tree.root->left->color = BLACK;
	else
		tree.root->left->color = RED;
	cout << (isRBT(&tree) ? "��" : "����") << endl;
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
	cout << (isRBT(&tree) ? "��" : "����") << endl;
	rbtree_delete(&tree, tree.root);
	cout << (isRBT(&tree) ? "��" : "����") << endl;

}

int main()
{
	test03();
}
