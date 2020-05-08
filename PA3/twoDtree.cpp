
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

// Node constructor, given.
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

// twoDtree destructor, given.
twoDtree::~twoDtree(){
	clear();
}

// twoDtree copy constructor, given.
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


// twoDtree assignment operator, given.
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ 
	width = imIn.width();
	height = imIn.height();
	stats* stat = new stats(imIn);
	root = buildTree(*stat,make_pair(0,0),make_pair(width - 1,height - 1),true);
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {
	Node* node = new Node(ul,lr,s.getAvg(ul,lr));
	// if (vert) {
	// 	if (lr.first - ul.first >0) {
	// 		node->left = buildTree(s,ul,make_pair((ul.first + lr.first)/2,lr.second),false);
	// 		node->right = buildTree(s,make_pair((ul.first + lr.first)/2+1,ul.second),lr,false);
	// 	} else if (lr.second - ul.second > 0) {
	// 		node->left = buildTree(s,ul,make_pair(lr.first,(lr.second + ul.second)/2),false);
	// 		node->right = buildTree(s,make_pair(ul.first,(lr.second + ul.second)/2+1),lr,false);
	// 	} 
	// } else {
	// 	if (lr.second - ul.second > 0) {
	// 		node->left = buildTree(s,ul,make_pair(lr.first,(lr.second + ul.second)/2),true);
	// 		node->right = buildTree(s,make_pair(ul.first,(lr.second + ul.second)/2+1),lr,true);
	// 	}  else if (lr.first - ul.first >0) {
	// 		node->left = buildTree(s,ul,make_pair((ul.first + lr.first)/2,lr.second),true);
	// 		node->right = buildTree(s,make_pair((ul.first + lr.first)/2+1,ul.second),lr,true);
	// 	}
	// }
	// return node;
	if (vert && (lr.first - ul.first > 0)) {
		int currLeft = ul.first;
		long minScore = s.getScore(ul,make_pair(ul.first,lr.second)) + s.getScore(make_pair(ul.first+1,ul.second),lr);
		for (int i = ul.first; i < lr.first; i++) {
			long currScore = s.getScore(ul,make_pair(i,lr.second)) + s.getScore(make_pair(i+1,ul.second),lr);
			if (currScore <= minScore) {
				minScore = currScore;
				currLeft = i;
			}
		}
		node->left = buildTree(s,ul,make_pair(currLeft,lr.second), false);
		node->right = buildTree(s,make_pair(currLeft+1,ul.second), lr, false);
	} else if (!vert && (lr.second - ul.second >0)) {
		int currTop = ul.second;
		long minScore = s.getScore(ul,make_pair(lr.first, ul.second)) + s.getScore(make_pair(ul.first,ul.second+1),lr);
		for (int i = ul.second; i < lr.second; i++) {
			long currScore = s.getScore(ul,make_pair(lr.first, i)) + s.getScore(make_pair(ul.first,i+1),lr);
			if (currScore <= minScore) {
				minScore = currScore;
				currTop = i;
			}
		}
		node->left = buildTree(s,ul,make_pair(lr.first,currTop),true);
		node->right = buildTree(s,make_pair(ul.first,currTop+1),lr,true);
	} else if ((lr.first == ul.first) && (lr.second - ul.second > 0)) {
		int currTop = ul.second;
		long minScore = s.getScore(ul,ul) + s.getScore(make_pair(ul.first,ul.second+1),lr);
		for (int i = ul.second; i < lr.second; i++) {
			long currScore = s.getScore(ul,ul) + s.getScore(make_pair(ul.first,i+1),lr);
			if (currScore <= minScore) {
				minScore = currScore;
				currTop = i;
			}
		}
		node->left = buildTree(s,ul,make_pair(lr.first,currTop),false);
		node->right = buildTree(s,make_pair(ul.first,currTop+1),lr,false);
	} else if ((lr.second == ul.second) && (lr.first - ul.first > 0)) {
		int currLeft = ul.first;
		long minScore = s.getScore(ul,ul) + s.getScore(make_pair(ul.first+1,ul.second),lr);
		for (int i = ul.first; i < lr.first; i++) {
			long currScore = s.getScore(ul,ul) + s.getScore(make_pair(i+1,ul.second),lr);
			if (currScore <= minScore) {
				minScore = currScore;
				currLeft = i;
			}
		}
		node->left = buildTree(s,ul,make_pair(currLeft,ul.second),true);
		node->right = buildTree(s,make_pair(currLeft+1,lr.second),lr,true);
	}
	return node;
}

PNG twoDtree::render(){
	PNG* png = new PNG(width,height);
	renderTree(*png,root);
	return *png;
}

void twoDtree::renderTree(PNG & im, Node* node) {
	if (node != NULL) {
		if (node->left == NULL && node->right == NULL) {
			for (int j = node->upLeft.first; j < node->lowRight.first + 1; j++){

			for (int i = node->upLeft.second; i < node->lowRight.second + 1; i++){

				*(im.getPixel(j, i)) = node->avg;

			}

		}
		} else {
			renderTree(im,node->left);
			renderTree(im,node->right);
		}
	}
}

int twoDtree::idealPrune(int leaves){
}

int twoDtree::pruneSize(int tol){
	return pruneSizeHelper(root, tol);
}

int twoDtree::pruneSizeHelper(Node* node, int tol) {
	if (node == NULL) {
		return 0;
	} else if (check_tol(node,tol,node->avg)) {
		return 1;
	} else if (!node->left && !node->right) {
		return 1;
	} else {
		return pruneSizeHelper(node->left,tol) + pruneSizeHelper(node->right,tol);
	}
}

void twoDtree::prune(int tol){
	pruneHelper(root, tol);
}

void twoDtree::pruneHelper(Node* node, int tol) {
	if (check_tol(node,tol,node->avg)) {
		clearHelper(node->left);
		node->left = NULL;
		clearHelper(node->right);
		node->right = NULL;
	} 
	if (node->left) {
		pruneHelper(node->left, tol);
	}
	if (node->right) {
		pruneHelper(node->right,tol);
	}
}

bool twoDtree::check_tol(Node* node, int tol, RGBAPixel avg) {
	if (node == NULL) {
		return true;
	}
	int dist = (avg.r - node->avg.r)*(avg.r - node->avg.r) + (avg.g - node->avg.g)*(avg.g - node->avg.g) +
		(avg.b - node->avg.b)*(avg.b - node->avg.b);
	if (dist <= tol) {
		return check_tol(node->left,tol,avg) && check_tol(node->right,tol,avg);
	} else {
		return false;
	}
}

void twoDtree::clear() {
	clearHelper(root);
}

void twoDtree::clearHelper(Node* node) {
	if (node) {
		clearHelper(node->right);
		clearHelper(node->left);
		delete node;
		node = NULL;
	}
}

twoDtree::Node* twoDtree::copyHelper(twoDtree::Node* head) {
	if (head != NULL) {
		Node* n = new Node(head->upLeft,head->lowRight,head->avg);
		n->left = copyHelper(head->left);
		n->right = copyHelper(head->right);
		return n;
	}
	return NULL;
}

void twoDtree::copy(const twoDtree & orig){
	width = orig.width;
	height = orig.height;
	root = copyHelper(orig.root);
}



