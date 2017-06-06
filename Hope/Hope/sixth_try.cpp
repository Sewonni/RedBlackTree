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
void rb_delete(BSTPtr self, NodePtr tree, NodePtr z);
void rb_delete_fixup(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr x);
void left_rotate(BSTPtr self, NodePtr n);
void right_rotate(BSTPtr self, NodePtr n);
void bst_print(BSTPtr self, NodePtr tree, int level);
void bst_inorder(BSTPtr self, NodePtr tree);
int total_node(BSTPtr self, NodePtr tree, int total);
int black_node(BSTPtr self, NodePtr tree, int nb);
int black_height(BSTPtr self, NodePtr tree, int bh);


void main() {
	BSTPtr bst = bst_alloc();
	create_nilnode(bst);
	FILE *fp, *fp2;
	int index;
	int file;
	int total = 0;
	int nb = 0;
	int bh = 0;
	fopen_s(&fp, "C:\\zebra\\test2.txt", "rt");
	while (fscanf_s(fp, "%d", &file, sizeof(file)) != EOF) {
		if (file > 0) {
			rb_insert(bst, bst->root, bst->nil, node_alloc(file));
		}
		else if (file < 0) {
			if(tree_search(bst, bst->root, -(file)) == bst->nil){
				fopen_s(&fp2, "C:\\zebra\\input.txt", "wt");
				fprintf(fp2, "%d\n", -(file));
				fclose(fp2);
			}
			else {
				rb_delete(bst, bst->root, tree_search(bst, bst->root, -(file)));
			}
		}
		else {
			bst_print(bst, bst->root, 0);
			printf("total = %d\n", total_node(bst, bst->root, total));
			printf("nb = %d\n", black_node(bst, bst->root, nb));
			printf("bh = %d\n", black_height(bst, bst->root, bh));
			bst_inorder(bst, bst->root);
		}

	}
	fclose(fp);

	/*fopen_s(&fp, "C:\\zebra\\input.txt", "wt");
	for (index = 0; index < 10; index++) {
		fprintf(fp, "%d\n", index);

	}
	fclose(fp);

	*/
}

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
	if (where == self->nil || find == where->val)
		return where;
	if (find < where->val) {
		if (where->left == self->nil)
			return self->nil;
		else 
			return tree_search(self, where->left, find);
	}
	else {
		if (where->right == self->nil)
			return self->nil;
		else
			return tree_search(self, where->right, find);
	}
}

NodePtr tree_minimum(BSTPtr self, NodePtr x) {
	while (x->left != self->nil) {
		x = x->left;
	}
	return x;
}

void transplant(BSTPtr self, NodePtr origin, NodePtr n) {
	if (origin->parent == self->nil)
		self->root = n;
	else if (origin == origin->parent->left) {
		origin->parent->left = n;
	}
	else
		origin->parent->right = n;
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

void rb_delete(BSTPtr self, NodePtr tree, NodePtr z) {
	NodePtr x;
	NodePtr y = z;
	int y_original_color = y->color;
	if (z->left == self->nil) {
		x = z->right; x->parent = z;
		transplant(self, z, z->right);
	}
	else if (z->right == self->nil) {
		x = z->left; x->parent = z;
		transplant(self, z, z->left);
	}
	else {
		y = tree_minimum(self, z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z) {
			x->parent = y;
		}
		else {
			transplant(self, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(self, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}

	if (y_original_color == BLACK)
		rb_delete_fixup(self, tree, self->nil, x);
}

void rb_delete_fixup(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr x) {
	NodePtr w;
	while (x != self->root && x->color == BLACK) {
		if (x == x->parent->left) {
			w = x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				left_rotate(self, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					right_rotate(self, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				left_rotate(self, x->parent);
				x = self->root;
			}
		}
		else {
			w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				right_rotate(self, x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color = BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					left_rotate(self, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				right_rotate(self, x->parent);
				x = self->root;
			}
		}
	}
	x->color = BLACK;
}

void left_rotate(BSTPtr self, NodePtr n) {
	NodePtr y = n->right;
	n->right = y->left;
	if (y->left != self->nil && y-> left != NULL)
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
	if (y->right != self->nil && y->right != NULL)
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
		printf("%d\n ", tree->val);
		bst_inorder(self, tree->right);
	}
}

int total_node(BSTPtr self, NodePtr tree, int total) {
	if (tree == self->nil) {
		return 0;
	}
	else{
		return 	total_node(self, tree->left, total) + total_node(self, tree->right, total) +1;
	}
}

int black_node(BSTPtr self, NodePtr tree, int nb) {
	if (tree == self->nil)
		return 0;

	if (tree->color == BLACK) {
		nb += tree->color;
	}

	nb += black_node(self, tree->left, nb);
	nb += black_node(self, tree->right, nb);


	return nb;
}

int black_height(BSTPtr self, NodePtr tree, int bh) {
	while (tree != self->nil) {
		if (tree->color = BLACK) {
			bh++;
		}
		tree = tree->left;
	}
	bh--; //nil Á¦¿Ü
	return bh;
}