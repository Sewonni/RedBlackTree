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
void rbt_insert(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n);
void rbt_insert_fixup(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n);
NodePtr tree_search(BSTPtr self, NodePtr where, int find);
void transplant(BSTPtr self, NodePtr origin, NodePtr n);
void rb_delete(BSTPtr self, NodePtr tree, NodePtr z);
void rb_delete_fixup(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr x);
void left_rotate(BSTPtr self, NodePtr n);
void right_rotate(BSTPtr self, NodePtr n);
void bst_print(BSTPtr self, NodePtr tree, int level);
NodePtr tree_maximum(BSTPtr bst, NodePtr n);
NodePtr tree_minimum(BSTPtr bst, NodePtr n);
NodePtr predecessor(BSTPtr bst, NodePtr n);
NodePtr successor(BSTPtr bst, NodePtr n);



void main() {

	BSTPtr bst = bst_alloc();
	create_nilnode(bst);
	FILE *fp, *fp2, *fp3;
	int file, file2, file3;
	int x = 0, y = 0;

	//rb_insert(bst, bst->root, bst->nil, node_alloc(1));
	//rb_insert(bst, bst->root, bst->nil, node_alloc(2));
	//rb_insert(bst, bst->root, bst->nil, node_alloc(3));
	//rb_insert(bst, bst->root, bst->nil, node_alloc(4));
	//rb_insert(bst, bst->root, bst->nil, node_alloc(5));
	//bst_print(bst, bst->root, 0);
	//rb_delete(bst, bst->root, tree_search(bst, bst->root, 2));
	//bst_print(bst, bst->root, 0);
	//printf("%d\n", predecessor(bst, tree_search(bst, bst->root, 4))->val);

	fopen_s(&fp, "C:\\zebra\\test01.txt", "rt");
	while (fscanf_s(fp, "%d", &file, sizeof(file)) != EOF) {
		if (file > 0) {
			rb_insert(bst, bst->root, bst->nil, node_alloc(file));
		}
		else if (file < 0) {
			if (tree_search(bst, bst->root, -(file)) == bst->nil) {
			}
			else {
				rb_delete(bst, bst->root, tree_search(bst, bst->root, -(file)));
			}
		}
		else {
			break;
		}
	}
	fclose(fp);
	bst_print(bst, bst->root, 0);
	printf("%d\n", tree_maximum(bst, bst->root)->val);
	fopen_s(&fp2, "C:\\zebra\\search01.txt", "rt");
	fopen_s(&fp3, "C:\\zebra\\output00.txt", "at");
	while (fscanf_s(fp2, "%d", &file2, sizeof(file)) != EOF) {
		/*printf("%d\n", tree_search(bst, bst->root, file2)==bst->nil);*/
		if (file2 == 0)
			break;
		else if (tree_search(bst, bst->root, file2) == bst->nil) {
			if (file2 < tree_minimum(bst, bst->root)->val)
				fprintf(fp3, "NIL NIL %d\n", tree_minimum(bst, bst->root)->val);
			else if (file2 > tree_maximum(bst, bst->root)->val)
				fprintf(fp3, "%d NIL NIL\n", tree_maximum(bst, bst->root)->val);
			else {
				if (tree_search(bst, bst->root, file2) == tree_search(bst, bst->root, file2)->parent->left) {
					x = predecessor(bst, tree_search(bst, bst->root, file2)->parent)->val;
					y = tree_search(bst, bst->root, file2)->parent->val;
				}
				else {
					x = tree_search(bst, bst->root, file2)->parent->val;
					y = successor(bst, tree_search(bst, bst->root, file2)->parent)->val;
				}
				fprintf(fp3, "%d NIL %d\n", x, y);
			}
/*

				fprintf(fp3,"%da ", predecessor(bst, tree_search(bst, bst->root, file2))->val);

			fprintf(fp3, "NIL ");
			
			if (file2 > tree_maximum(bst, bst->root)->val)
				fprintf(fp3, "NIL\n");
			else
				fprintf(fp3, "%db\n", successor(bst, tree_search(bst, bst->root, file2))->val);*/
		}
		else {
			if (tree_search(bst, bst->root, file2) == tree_minimum(bst, bst->root))
				fprintf(fp3, "NIL ");
			else
				fprintf(fp3, "%d ", predecessor(bst, tree_search(bst, bst->root, file2))->val);
			
			fprintf(fp3, "%d ", file2);
			
			if (tree_search(bst, bst->root, file2) == tree_maximum(bst, bst->root))
				fprintf(fp3, "NIL\n");
			else
				fprintf(fp3, "%d\n", successor(bst, tree_search(bst, bst->root, file2))->val);
		}
	}
	fclose(fp2); fclose(fp3);
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
	bst->nil->left = bst->nil;
	bst->nil->right = bst->nil;
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
				break;
			}
			else {
				if (n == n->parent->right) {
					n = n->parent;
					left_rotate(self, n);
				}
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				right_rotate(self, n->parent->parent);
				break;
			}
		}
		else {
			temp = n->parent->parent->left;
			if (temp->color == RED) {
				n->parent->color = BLACK;
				temp->color = BLACK;
				n->parent->parent->color = RED;
				n = n->parent->parent;
				break;
			}
			else {
				if (n == n->parent->left) {
					n = n->parent;
					right_rotate(self, n);
				}
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				left_rotate(self, n->parent->parent);
				break;
			}
		}
	}
	self->root->color = BLACK;
}

void rbt_insert(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n) {
	NodePtr y = nil;
	while (tree != nil) {
		y = tree;
		if (n->val < tree->val) {
			tree = tree->left;
		}
		else
			tree = tree->right;
	}
	n->parent = y;
	if (y == nil) {
		self->root = n;
	}
	else if (n->val < y->val) {
		y->left = n;
	}
	else {
		y->right = n;
	}
	n->left = nil;
	n->right = nil;
	n->color = RED;
	rbt_insert_fixup(self, tree, nil, n);
}

void rbt_insert_fixup(BSTPtr self, NodePtr tree, NodePtr nil, NodePtr n) {
	NodePtr y;
	while (n->parent->color == RED) {
		if (n->parent == n->parent->parent->left) {
			y = n->parent->parent->right;
			if (y->color == RED) {
				n->parent->color = BLACK;
				y->color = BLACK;
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
			y = n->parent->parent->left;
			if (y->color == RED) {
				n->parent->color = BLACK;
				y->color = BLACK;
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
				right_rotate(self, n->parent->parent);
			}
		}
	}
	self->root->color = BLACK;
}

NodePtr tree_search(BSTPtr self, NodePtr x, int find) {

	while (x != self->nil && find != x->val) {
		if (find < x->val)
			x = x->left;
		else x = x->right;
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
	//if (n!= self->nil)
	n->parent = origin->parent;
}

void rb_delete(BSTPtr self, NodePtr tree, NodePtr z) {
	NodePtr x;
	NodePtr y = z;

	int y_original_color = y->color;
	if (z->left == self->nil) {
		x = z->right;
		//x->parent = z;
		transplant(self, z, z->right);
	}
	else if (z->right == self->nil) {
		x = z->left;
		//x->parent = z;
		transplant(self, z, z->left);
	}
	else {
		y = tree_minimum(self, z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z) {
			x->parent = z->right;
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

NodePtr tree_maximum(BSTPtr bst, NodePtr n) {
	while (n->right != bst->nil)
		n = n->right;
	return n;
}

NodePtr tree_minimum(BSTPtr bst, NodePtr n) {
	while (n->left != bst->nil)
		n = n->left;
	return n;
}

NodePtr predecessor(BSTPtr bst, NodePtr n) {
	if (n == bst->nil) {
		if (n == n->parent->left)
			return predecessor(bst, n->parent);
		else
			return n->parent;
	}
	else {
		if (n->left != bst->nil)
			return tree_maximum(bst, n->left);
		NodePtr y = n->parent;
		while (y != bst->nil && n == y->left) {
			n = y;
			y = y->parent;
		}
		return y;
	}
}

NodePtr successor(BSTPtr bst, NodePtr n) {
	if (n == bst->nil) {
		if (n == n->parent->left)
			return n->parent;
		else
			return successor(bst, n->parent);
	}
	else {
		if (n->right != bst->nil)
			return tree_minimum(bst, n->right);
		NodePtr y = n->parent;
		while (y != bst->nil && n == y->right) {
			n = y;
			y = y->parent;
		}
		return y;
	}
}