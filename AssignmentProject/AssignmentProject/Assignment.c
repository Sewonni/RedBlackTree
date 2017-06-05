#include <stdio.h>
#include <malloc.h>
#define RED 0
#define BLACK 1

typedef struct Node* NodePtr;
struct Node {
	int val, color;
	NodePtr left, right, parent;
}Node;
typedef struct BST* BSTPtr;
struct BST {
	NodePtr root;
	NodePtr nil;
};

NodePtr node_alloc(int newval);
BSTPtr bst_alloc();
BSTPtr create_nilnode(BSTPtr bst);
void bst_insert(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n);
void rb_insert(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n);
NodePtr tree_search(BSTPtr self, NodePtr where, int find);
NodePtr tree_minimum(BSTPtr self, NodePtr x);
void transplant(BSTPtr self, NodePtr origin, NodePtr n);
void tree_delete(BSTPtr self, NodePtr tree, NodePtr x);
void left_rotate(BSTPtr self, NodePtr n);
void right_rotate(BSTPtr self, NodePtr n);

NodePtr node_alloc(int newval) {
	NodePtr self = (NodePtr)malloc(sizeof(struct Node));
	self->val = newval;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
	return self;
}

BSTPtr bst_alloc() {
	BSTPtr self = (BSTPtr)malloc(sizeof(struct BST));
	self->root = NULL;
	return self;
}

BSTPtr create_nilnode(BSTPtr bst) {
	bst->nil = (NodePtr)malloc(sizeof(Node));
	bst->nil->val = NULL;
	bst->nil->left = NULL;
	bst->nil->right = NULL;
	bst->nil->color = BLACK;
	bst->root = bst->nil;
	return bst;
}

void bst_insert(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n) {
	if (tree == nil) {
		self->root = n;
		n->parent = nil;
	}
	else if (n->val < tree->val) {
		n->parent = tree;
		if (tree->left == nil) {
			tree->left = n;
			n->parent = tree;
		}
		else
			bst_insert(self, tree->left, nil, n);
	}
	else {
		n->parent = tree;
		if (tree->right == nil) {
			tree->right = n;
			n->parent = tree;
		}
		else
			bst_insert(self, tree->right, nil, n);
	}
	n->left = nil;
	n->right = nil;
}

void rb_insert(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n) {
	bst_insert(self, tree, nil, n);
	n->color = RED;
	NodePtr temp; //temp = node_alloc(0);
	while ((n != tree) && n->parent->color == RED) {
		if (n->parent == n->parent->parent->left) {
			temp = n->parent->parent->right;
			if (temp->color == RED) {
				n->parent->color = BLACK; temp->color = BLACK;
				n->parent->parent->color = RED;
				n = n->parent->parent;
			}
			else {
				if (n == n->parent->right) {
					n = n->parent;
					left_rotate(self, n);
				}
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				right_rotate(self, n->parent->parent);
			}
		}
		else {
			temp = n->parent->parent->left;
			if (temp->color == RED) {
				n->parent->color = BLACK;
				temp->color = BLACK;
				n->parent->parent->color = RED;
				n = n->parent->parent;
			}
			else {
				if (n == n->parent->left) {
					n = n->parent;
					right_rotate(self, n);
				}
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				left_rotate(self, n->parent->parent);
			}
		}
	}
	self->root->color = BLACK;
}

NodePtr tree_search(BSTPtr self, NodePtr where, int find) {
	if (where == NULL || find == where->val)
		return where;
	if (find < where->val)
		return tree_search(self, where->left, find);
	else
		return tree_search(self, where->right, find);
}

NodePtr tree_minimum(BSTPtr self, NodePtr x) {
	while (x->left != NULL) {
		x = x->left;
	}
	return x;
}

void transplant(BSTPtr self, NodePtr origin, NodePtr n) {
	if (origin->parent == NULL)
		self->root = n;
	else if (origin == origin->parent->left)
		origin->parent->left = n;
	else
		origin->parent->right = n;
	if (n != NULL)
		n->parent = origin->parent;
}

void tree_delete(BSTPtr self, NodePtr tree, NodePtr x) {
	if (x->left == NULL) {
		transplant(self, x, x->right);
	}
	else if (x->right == NULL)
		transplant(self, x, x->left);
	else {
		NodePtr y = tree_minimum(self, x->right);
		if (y->parent != x) {
			transplant(self, y, y->right);
			y->right = x->right;
			y->right->parent = y;
		}
		transplant(self, x, y);
		y->left = x->left;
		y->left->parent = y;
	}
}

void left_rotate(BSTPtr self, NodePtr n) {
	NodePtr y = n->right;
	n->right = y->left;
	if (y->left != self->nil)
		y->left->parent = n;
	y->parent = n->parent;
	if (n->parent == self->nil)
		self->root = y;
	else if (n == n->parent->left)
		n->parent->left = y;
	else n->parent->right = y;
	y->left = n;
	n->parent = y;
}

void right_rotate(BSTPtr self, NodePtr n) {
	NodePtr y = n->left;
	n->left = y->right;
	if (y->right != self->nil)
		y->right->parent = n;
	y->parent = n->parent;
	if (n->parent == self->nil)
		self->root = y;
	else if (n == n->parent->right)
		n->parent->right = y;
	else n->parent->left = y;
	y->right = n;
	n->parent = y;
}

void bst_print(BSTPtr self, NodePtr tree, int level) {
	if (tree->right != self->nil)
		bst_print(self, tree->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("    ");
	printf("%d[%d]\n", tree->val, tree->color);
	if (tree->left != self->nil)
		bst_print(self, tree->left, level + 1);
}

void bst_inorder(BSTPtr self, NodePtr tree) {
	if (tree == self->nil)
		return;
	else {
		bst_inorder(self, tree->left);
		printf("%d ", tree->val);
		bst_inorder(self, tree->right);
	}
}

void main() {
	BSTPtr bst = bst_alloc();
	create_nilnode(bst);
	rb_insert(bst, bst->root, bst->nil, node_alloc(11));
	rb_insert(bst, bst->root, bst->nil, node_alloc(6));
	rb_insert(bst, bst->root, bst->nil, node_alloc(8));
	rb_insert(bst, bst->root, bst->nil, node_alloc(10));
	rb_insert(bst, bst->root, bst->nil, node_alloc(17));
	rb_insert(bst, bst->root, bst->nil, node_alloc(12));
	bst_print(bst, bst->root, 0);
	bst_inorder(bst, bst->root);
	printf("\n");
}
