#include "risingcity.hpp"

// ---------- tree initialization ---------- //
RedBlackTree::RedBlackTree(){
	root = nullptr;
}

// ---------- node initialization ---------- //
RedBlackNode::RedBlackNode(Building * bd){
	building = bd;
	color = red;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}
RedBlackNode * RedBlackNode::sibling(){
	if (parent==nullptr) return nullptr;
	if (this == parent->left) return parent->right;
	else return parent->left;
}
Building * nullBuilding = new Building(0, 0);
RedBlackNode * nullNode = new RedBlackNode(nullBuilding);

// ---------- search key ---------- //
RedBlackNode * RedBlackTree::get(int buildingNum){
	RedBlackNode * thisnode = root;
	while (1){
		if (thisnode == nullptr){ // not found
			return nullNode;
		}
		if (buildingNum < thisnode->building->buildingNum) // go to left
			thisnode = thisnode->left;
		else if (buildingNum > thisnode->building->buildingNum) // go to right
			thisnode = thisnode->right;
		else // found
			return thisnode;
	}
}

// ---------- search range ---------- //
void getrange_subtree(RedBlackNode * root, int buildingNum1, int buildingNum2, std::vector<Building *> & buildings){
	/* append all elements in [buildingNum1, buildingNum2] in the subtree rooted at root to buildings */
	if (root == nullptr) return;
	if (buildingNum1 < root->building->buildingNum) // search left subtree
		getrange_subtree(root->left, buildingNum1, buildingNum2, buildings);
	if (buildingNum1 <= root->building->buildingNum
	 && buildingNum2 >= root->building->buildingNum) // output root
		buildings.push_back(root->building);
	if (buildingNum2 > root->building->buildingNum) // search right subtree
		getrange_subtree(root->right, buildingNum1, buildingNum2, buildings);
}
std::vector<Building *> RedBlackTree::getrange(int buildingNum1, int buildingNum2){
	std::vector<Building *> buildings;
	getrange_subtree(root, buildingNum1, buildingNum2, buildings);
	if (buildings.size() == 0) buildings.push_back(nullBuilding);
	return buildings;
}

// ---------- insert/delete utils ---------- //
RedBlackNode * removeDegree1(RedBlackNode * toremove, RedBlackNode * toraise, RedBlackTree * tree){
	RedBlackNode * v;
	if (toremove->parent != nullptr){ // toremove is not root
		if (toremove->parent->left == toremove){
			toremove->parent->left = toraise;
			v = toremove->parent->right;
		}
		else {
			toremove->parent->right = toraise;
			v = toremove->parent->left;
		}
	}
	else {
		tree->root = toraise; // toremove is root
		v = nullptr;
	}
	if (toraise != nullptr){
		toraise->parent = toremove->parent;
		toraise->color = black;
	}
	return v;
}
void raiseup(RedBlackNode * toraise, RedBlackNode * gp, RedBlackTree * tree){
	/* put toraise to gp's position */
	RedBlackNode * sib = removeDegree1(gp, toraise, tree);
	gp->parent = toraise;
	gp->color = red;
}
void LLb_rotation(RedBlackNode * pp, RedBlackNode * gp, RedBlackTree * tree){
	raiseup(pp, gp, tree);
	RedBlackNode * c = pp->right;
	gp->left = c;
	pp->right = gp;
	if (c != nullptr) c->parent = gp;
}
void RRb_rotation(RedBlackNode * pp, RedBlackNode * gp, RedBlackTree * tree){
	raiseup(pp, gp, tree);
	RedBlackNode * c = pp->left;
	gp->right = c;
	pp->left = gp;
	if (c != nullptr) c->parent = gp;
}
void LRb_rotation(RedBlackNode * p, RedBlackNode * pp, RedBlackNode * gp, RedBlackTree * tree){
	raiseup(p, gp, tree);
	pp->parent = p;
	RedBlackNode * b = p->left;
	RedBlackNode * c = p->right;
	pp->right = b;
	gp->left = c;
	p->left = pp;
	p->right = gp;
	if (b != nullptr) b->parent = pp;
	if (c != nullptr) c->parent = gp;
}
void RLb_rotation(RedBlackNode * p, RedBlackNode * pp, RedBlackNode * gp, RedBlackTree * tree){
	raiseup(p, gp, tree);
	pp->parent = p;
	RedBlackNode * b = p->right;
	RedBlackNode * c = p->left;
	pp->left = b;
	gp->right = c;
	p->right = pp;
	p->left = gp;
	if (b != nullptr) b->parent = pp;
	if (c != nullptr) c->parent = gp;
}

// ---------- insert ---------- //
void insertBinarySearchTree(RedBlackNode *& root, RedBlackNode * newnode){
	/* insert a newnode to a binary search tree rooted at root */
	if (root == nullptr){ // do insertion
		root = newnode;
		return;
	}
	if (newnode->building->buildingNum == root->building->buildingNum) // if buildingNum is a duplicate output an error and stop
		throw std::invalid_argument("The inserted building has a duplicate building number: "+std::to_string(newnode->building->buildingNum));
	newnode->parent = root;
	if (newnode->building->buildingNum < root->building->buildingNum) // insert to left
		insertBinarySearchTree(root->left, newnode);
	if (newnode->building->buildingNum > root->building->buildingNum) // insert to right
		insertBinarySearchTree(root->right, newnode);
}
void twored_rotation(RedBlackNode * p, RedBlackNode * pp, RedBlackNode * gp, RedBlackTree * tree){
	if (pp == gp->left && p == pp->left) LLb_rotation(pp, gp, tree);
	if (pp == gp->right && p == pp->right) RRb_rotation(pp, gp, tree);
	if (pp == gp->left && p == pp->right) LRb_rotation(p, pp, gp, tree);
	if (pp == gp->right && p == pp->left) RLb_rotation(p, pp, gp, tree);
}
void adjust_twored(RedBlackNode * p, RedBlackTree * tree){
	/* adjust if p and pp are both red */
	RedBlackNode * pp = p->parent;
	if (pp == nullptr) { // p is root
		p->color = black;
		return;
	}
	RedBlackNode * gp = pp->parent;
	if (gp == nullptr) { // pp is root
		pp->color = black;
		return;
	}
	if (p->color == red && pp->color == red){
		if (pp->sibling() != nullptr){
			if (pp->sibling()->color == red){ // color flip
				pp->color = black;
				gp->color = red;
				pp->sibling()->color = black;
				adjust_twored(gp, tree);
			}
			else twored_rotation(p, pp, gp, tree);
		}
		else twored_rotation(p, pp, gp, tree);
	}
}
void RedBlackTree::insert(Building * toinsert){
	RedBlackNode * newnode = new RedBlackNode(toinsert);
	insertBinarySearchTree(root, newnode);
	adjust_twored(newnode, this);
}

// ---------- remove ---------- //
void adjust_deficient(RedBlackNode * py, RedBlackNode * v, RedBlackTree * tree){
	/* adjust if py is the parent of a deficient node
	 * v is the sibling of the deficient node */
	if (py == nullptr) return; // y is root
	RedBlackNode *w, *a, *b, *c, *x;
	enum colors pycolor = py->color;
	bool Rxx = (v == py->left); // 1 for RXx, 0 for LXx
	w = Rxx? v->right : v->left;
	if (v->color == black){
		a = Rxx? v->left : v->right;
		if (w != nullptr && w->color == red){ // Lb2, Rb2 or Lb1, Rb1 case 2
			if (Rxx) LRb_rotation(w, v, py, tree);
			else RLb_rotation(w, v, py, tree);
			w->color = pycolor;
			py->color = black;
			return;
		}
		else {
			if (a != nullptr && a->color == red){ // Lb1, Rb1 case 1
				if (Rxx) LLb_rotation(v, py, tree);
				else RRb_rotation(v, py, tree);
				v->color = pycolor;
				py->color = black;
				a->color = black;
				return;
			}
			else { // Lb0, Rb0
				if (pycolor == black){ // Lb0, Rb0 case 1
					v->color = red;
					if (py->parent == nullptr) return;
					else adjust_deficient(py->parent, py->sibling(), tree);
				}
				else { // Lb0, Rb0 case 2
					v->color = red;
					py->color = black;
					return;
				}
			}
		}
	}
	else { // v red
		a = Rxx? w->left : w->right;
		x = Rxx? w->right : w->left;
		if (x != nullptr && x->color == red){ // Lr(2), Rr(2) or Lr(1), Rr(1) case 2
			if (Rxx) LRb_rotation(x, v, py, tree);
			else RLb_rotation(x, v, py, tree);
			if (Rxx) {
				w->right = v->right;
				if (v->right != nullptr) v->right->parent = w;
				v->right = w;
			}
			else {
				w->left = v->left;
				if (v->left != nullptr) v->left->parent = w;
				v->left = w;
			}
			w->parent = v;
			py->color = black;
			return;
		}
		else {
			if (a != nullptr && a->color == red){ // Lr(1), Rr(1) case 1
				if (Rxx) LRb_rotation(w, v, py, tree);
				else RLb_rotation(w, v, py, tree);
				py->color = black;
				a->color = black;
				return;
			}
			else { // Lr(0), Rr(0)
				if (Rxx) LLb_rotation(v, py, tree);
				else RRb_rotation(v, py, tree);
				py->color = black;
				w->color = red;
				return;
			}
		}

	}
}
RedBlackNode * replace(RedBlackNode * toreplace){
	/* find the rightmost leaf in the left subtree and replace
	 * assuming toreplace has both left and right child 
	 * return the leaf node */
	RedBlackNode * thisnode = toreplace->left;
	while (1){
		if (thisnode->right == nullptr){ // swap thisnode and toreplace
			// plan A
			std::swap(toreplace->building, thisnode->building);
			return thisnode;
		}
		else thisnode = thisnode->right;
	}
}
void RedBlackTree::remove(Building * toremove){
	RedBlackNode * removenode = get(toremove->buildingNum);
	RedBlackNode * toraise;
	bool adjust = false; // whether need to continue adjusting
	if ((removenode->left == nullptr) ^ (removenode->right == nullptr)){ // degree 1 node
		if (removenode->left != nullptr) toraise = removenode->left; // degree 1 node with left child
		else if (removenode->right != nullptr) toraise = removenode->right; // degree 1 node with right child
	}
	else { // leaf or degree 2
		if (removenode->left != nullptr) removenode = replace(removenode); // degree 2 node
		toraise = removenode->left; // can be null
	}
	if (removenode->color == black){ // might neet to rebalance
		if (toraise == nullptr) adjust = true;
		else if (toraise->color == black) adjust = true;
	}
	RedBlackNode * v = removeDegree1(removenode, toraise, this);
	if (adjust) adjust_deficient(removenode->parent, v, this);
}
